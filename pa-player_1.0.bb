SUMMARY = "Custom PulseAudio WAV player"
LICENSE = "CLOSED"

SRC_URI = "file://pa_play_wav.c"

S = "${WORKDIR}"

# PulseAudio dependencies
DEPENDS = "libpulse libpulse-simple"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/nisplay.c -o pa_play_wav \
        -lpulse-simple -lpulse -lm
}

do_install() {
    # Install binary
    install -d ${D}/usr/bin
    install -m 0755 pa_play_wav ${D}/usr/bin/pa_play_wav
}

# Only include the binary
FILES_${PN} += "/usr/bin/pa_play_wav"
