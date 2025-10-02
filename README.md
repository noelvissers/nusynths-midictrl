# NU Synths | MIDICTRL 

Firmware for the NU Synths 'MIDICTRL' eurorack module. Converts MIDI signals (5 pin MIDI or MIDI-USB) to eurorack compatible control voltage (CV) and gate signals.

## Contents

The following files and folders are in this repository:

|File / Folder       |Remarks
|--------------------|--------
|boards/             |Custom board files
|include/            |Header (.h) files
|lib/                |Custom libraries
|src/                |Source (.cpp) files
|test/               |Test files
|LICENSE             |
|README.md           |
|platformio.ini      |

## Documentation

### General settings

To access the settings, hold the rotary encoder button for 1 second.

|Setting       |Remarks
|--------------|--------
|MIDI channel  |MIDI channel to listen to (All, 1 to 16)
|Synth mode    |Monophonic or polyphonic mode (see below)
|Pitch bend    |Select the range of the pitch bend wheel (0 to 12 semitones)
|Cock division |Select the clock diveder amount (1 for 1:1, /2 to /128)

### Synth mode

#### Monophonic

In monophonic mode the pitch, velocity and gate are mapped to all outputs that are assigned this function. When two or more notes are played at the same time, the latest pressed note will be used to overwrite the previous one. When the latest pressed note is released, the CV value will **not** go back to the previous value, if that note is not released yet.

#### Polyphonic

In polyphonic mode the pitch, velocity and gate are mapped sequentially to the first available output. When two or more notes are played at the same time, the latest pressed note will **not** overwrite the previous one(s) when no outputs are available. 

### Output functions

Each output can be configured to a certain function via the configuration menu:

|Function              |Supported outputs |Remarks
|----------------------|------------------|-------
|Clock                 |SYNC              |MIDI sync (1:1 to /128)
|Pitch                 |CV                |1V/Oct (0-10V)
|Velocity              |CV                |0-10V
|CC + MIDI learn       |CV                |0-10V
|AfterTouch            |CV                |0-10V
|Gate                  |CV + GATE         |0-5V
|Trigger + MIDI learn* |CV + GATE         |0-5V, 1ms
|Reset                 |CV + GATE         |0-5V, 1ms


\* Optional

### Menu structure:

```
Menu
├─ Settings
│  ├─ Midi channel
│  │  ├─ All
│  │  ├─ 1
│  │  ├─ ...
│  │  └─ 16
│  ├─ Mode
│  │  ├─ Monophonic
│  │  └─ Polyphonic
│  ├─ Pitch bend
│  │  ├─ 0 semitones
│  │  ├─ ...
│  │  └─ 12 semitones
│  └─ Clock division
│     ├─ 1:0
│     ├─ /2
│     ├─ ...
│     └─ /128
├─ Output (CV [1..4])
│  ├─ Pitch
│  ├─ Velocity
│  ├─ CC → Learn MIDI CC [Required]
│  ├─ AfterTouch
│  ├─ Gate
│  ├─ Trigger → Learn MIDI note [Optional]
│  ├─ Reset
│  └─ Unassigned
└─ Output (Gates [5..8])
   ├─ Gate
   ├─ Trigger → Learn MIDI note [Optional]
   ├─ Reset
   └─ Unassigned
```

## Usage

Open the project in [Visual Studio Code](https://code.visualstudio.com/) with the [PlatformIO](https://platformio.org/) extension installed.

## Dependencies

[fortyseveneffects/MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) [v5.0.2]

[arduino-libraries/MIDIUSB](https://github.com/arduino-libraries/MIDIUSB) [v1.0.5]

[khoih-prog/FlashStorage_SAMD](https://github.com/khoih-prog/FlashStorage_SAMD) [v1.3.2]

## Release Notes

### [1.0.0]

* Initial release