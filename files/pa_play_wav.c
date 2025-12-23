#include <pulse/simple.h>
#include <pulse/error.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Simple WAV header (PCM only) */
struct wav_header {
    char riff[4];              // "RIFF"
    uint32_t size;
    char wave[4];              // "WAVE"
    char fmt[4];               // "fmt "
    uint32_t fmt_size;
    uint16_t format;           // PCM = 1
    uint16_t channels;         // 1 = mono, 2 = stereo
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;  // 8 / 16
    char data[4];              // "data"
    uint32_t data_size;
};

#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    FILE *fp;
    struct wav_header header;
    pa_simple *pa = NULL;
    pa_sample_spec ss;
    int error;
    uint8_t buffer[BUFFER_SIZE];

    if (argc < 2) {
        printf("Usage: %s <wav file>\n", argv[0]);
        return -1;
    }

    /* Open WAV file */
    fp = fopen(argv[1], "rb");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    /* Read WAV header */
    if (fread(&header, sizeof(header), 1, fp) != 1) {
        printf("Failed to read WAV header\n");
        fclose(fp);
        return -1;
    }

    /* Validate PCM format */
    if (header.format != 1) {
        printf("Unsupported WAV format (only PCM supported)\n");
        fclose(fp);
        return -1;
    }

    /* PulseAudio sample specification */
    ss.rate = header.sample_rate;
    ss.channels = header.channels;

    if (header.bits_per_sample == 16)
        ss.format = PA_SAMPLE_S16LE;
    else if (header.bits_per_sample == 8)
        ss.format = PA_SAMPLE_U8;
    else {
        printf("Unsupported bit depth: %d\n",
               header.bits_per_sample);
        fclose(fp);
        return -1;
    }

    /* Create PulseAudio playback stream */
    pa = pa_simple_new(
        NULL,                   // Default PulseAudio server
        "PulseAudio Player",    // Application name
        PA_STREAM_PLAYBACK,
        NULL,                   // Default sink
        "playback",             // Stream description
        &ss,
        NULL,                   // Default channel map
        NULL,                   // Default buffering
        &error
    );

    if (!pa) {
        printf("pa_simple_new() failed: %s\n",
               pa_strerror(error));
        fclose(fp);
        return -1;
    }

    /* Playback loop */
    while (!feof(fp)) {
        size_t bytes = fread(buffer, 1, BUFFER_SIZE, fp);
        if (bytes > 0) {
            if (pa_simple_write(pa, buffer, bytes, &error) < 0) {
                printf("pa_simple_write() failed: %s\n",
                       pa_strerror(error));
                break;
            }
        }
    }

    /* Drain and cleanup */
    pa_simple_drain(pa, &error);
    pa_simple_free(pa);
    fclose(fp);

    return 0;
}
