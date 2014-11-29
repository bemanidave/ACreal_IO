ACreal IO
========
ACreal IO is a firmware code for Arduino to build Konami ACIO compatible boards.

Konami ACIO is a protocol used for communication between arcade games and harware components such as e-Amusement readers.
Its uses rs232 and allows several "nodes" to be connected a single serial port.

Purpose of this fork
====================
This (bemanidave/ACreal_IO) is a fork from the upstream ACreal IO code. Most changes are specific to the RR10 reader, as that's the device I have. It has the following changes:

Support for ISO14443A cards
---------------------------

This code uses the [SipHash Arduino library](http://www.forward.com.au/pfod/SipHashLibrary/) to allow scanning of arbitrary ISO14443A cards, as long as they have a unique ID. It does this by reading the card ID, generating a hash with SipHash, then replacing the first 2 bytes with 0xE0 0x04. Most RFID cards you get will be of this type, e.g. public transport cards, gym membership cards, etc, although do note that some don't have a useful ID number (passports, for example, give a random ID each scan). If you plan on using this, it may be a good idea to change the following key in RR10.cpp:

``` c
        unsigned char key[] = {0xe8, 0x0b, 0x6e, 0x3a, 0x12, 0x11, 0x40, 0x57,
                               0x7c, 0x7b, 0xea, 0x17, 0x64, 0x08, 0xe8, 0x6e};
```

This will make card IDs non-portable between installations, but should help to make people more comfortable with scanning some ID card they have.

Support for LCD output
----------------------
For people who don't know their card ID, or for those who use ISO14443A, this code supports outputting to a 16x4 (default) or 16x2 LCD. Note that by default the pins overlap with Sound Voltex; there aren't enough pins on the board to not clash with something, so rearrange pins as needed.

Various bug fixes
-----------------
There are a few things that have been fixed in this codebase; this is mainly around making the card reading code more robust.


Games support
=============
Currently this project supports the following games :

pop'n music 15-18 (one old slotted reader)

pop'n music 19-21 (one new wavepass reader with encrypted communication (only game I know which uses this encryption) + optional card dispenser)

iidx 13-18 (2 old slotted readers)

iidx 19-20 (2 new wavepass readers)

jubeat 1 - knit (one old slotted reader (no keypad) + one led board (game won't boot if the led board is not present) 

jubeat copious - saucer (one new reader (no keypad) + one led board (game won't boot if the led board is not present) 

DDR SN-SN2 (2 old slotted readers)

DDR X-X3 (2 readers (old or new) in sd, 2 readers (old or new) + led board ? in HD)

drum mania (1 old reader)

guitar freaks (2 old readers)

Sound voltex (1 new wavepass reader + 1 IoBoard )
