# Prophet Program Switcher

Teensy 4.1 based USB Host to send MIDI ProgramChange messages to attached device.

When connecting a device, the device must initially send a ProgramChange message itself before the Prophet Program Switcher starts working.
This is due to the fact that MIDI ProgramChange messages are always absolute, and the Program Switcher must first be informed about the currently active program of the attached device.
