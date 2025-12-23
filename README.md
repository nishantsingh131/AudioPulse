# 🎧 PulseAudio Audio Player for Embedded Linux (Yocto)

##  Overview

This project is a **custom audio playback application written in C using PulseAudio APIs**.  
It is designed for **Embedded Linux systems**, especially **Yocto Project–based images** such as `core-image-sato`, and targets ARM boards like **BeagleBone Black**.

The application demonstrates how an embedded user-space program plays audio through **PulseAudio**, without directly accessing ALSA hardware devices.

---

##  Purpose of This Project

- Learn **PulseAudio audio flow**
- Understand **ALSA vs PulseAudio**
- Enable audio playback on embedded hardware
- Integrate a custom application into **Yocto**
- Understand routing, mixing, and resampling

---

##  Audio Architecture Flow

```
Application (PulseAudio API)
        ↓
PulseAudio Sound Server
        ↓
ALSA (Kernel Audio Layer)
        ↓
McASP / I2S Controller
        ↓
External Audio Codec
        ↓
Speaker / Headphone
```

---

##  Features Automatically Enabled

- Audio routing
- Software mixing (multiple apps)
- Per-application volume control
- Mono to Stereo duplication
- Resampling & format conversion
- Device abstraction
- Basic latency handling

---

## 📂 Project Structure

```
pulse-audio-player/
├── pa_play_wav.c
├── pa-player_1.0.bb
└── README.md
```

---

## 📦 Dependencies

### Runtime
- pulseaudio
- libpulse
- libpulse-simple
- alsa-lib

### Yocto
```
DEPENDS = "pulseaudio alsa-lib"
```

---

##  Understanding "default" Audio Device

```
"default"
   ↓
PulseAudio Sink
   ↓
ALSA hw:0,0
   ↓
McASP / I2S
   ↓
Codec → Speaker
```

---

## ⚙️ Yocto Integration

### Add to local.conf
```
IMAGE_INSTALL:append = " nisplay pulseaudio"
```

### Build
```
bitbake core-image-sato
```

---

## ▶️ Run on Target

```
nisplay test.wav
```

---

## 👤 Author

Nishant – Senior Embedded Software Engineer  
Embedded Linux | Yocto | Audio Systems

---

## 📄 License

CLOSED – Educational Use
