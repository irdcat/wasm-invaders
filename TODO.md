# TODO List

- RST should push current PC onto stack before jumping to the interrupt vector (this instruction is basically CALL but to a fixed memory location)
- Implement proper interrupt handling logic
- Implement bus along with the peripherials

## Interrupt handling

- If interrupt flip-flop is set (by EI instruction) and there's interrupt pending cpu then should execute opcode provided by interrupt source. In Invaders arcade machine only two interrupts are requested: 0xCF (RST 8) when the beam is near the middle of the screen and 0xD7 (RST 10) when the bean is at the end of the screen (line 224 - VBLANK). 
- Executing EI instruction should not enable interrupt handling despite flip-flop being set immediately, but after the end of the next instruction, so delay has to be implemented.

## Bus and peripherials

Cpu communicate with a memory and the peripherials via Bus.

### Memory

Memory map looks like this:
0000 - 1FFF : 8KB ROM
2000 - 23FF : 1KB RAM
2400 - 3FFF : 7KB Video RAM

### Peripherials

```
Read
 00   INPUT0 (Mapped in hardware but never used by space invaders code)
 01   INPUT1
 02   INPUT2
 03   Read shift register
Write
 02   Shift offset (3 bits to keep offset between 0 and 7)
 03   Sound player (Bank 1)
 04   Shift data
 05   Sound player (Bank 2)
 06   Watchdog timer
```

#### Inputs

```
INPUT1 (Port 01 read)
 bit0   CREDIT (Insert coin)
 bit1   2P Start (1 = pressed)
 bit2   1P Start (1 = pressed)
 bit3   always 1
 bit4   1P Shoot (1 = pressed)
 bit5   1P Left (1 = pressed)
 bit6   1P Right (1 = pressed)
 bit7   Not connected

INPUT2 (Port 02 read)
 bit0   DIP Switch HI byte: number of ships/lives
 bit1   DIP Switch LO byte: number of ships/lives
        00 = 3 ships
        01 = 4 ships
        10 = 5 ships
        11 = 6 ships
 bit2   Tilt
 bit3   Dip Switch: extra ship rules
        0 = extra ship at 1500
        1 = extra ship at 1000
 bit4   2P Shoot (1 = pressed)
 bit5   2P Left (1 = pressed)
 bit6   2P Right (1 = pressed)
 bit7   DIP Switch: Display coin info on demo screen
        0 = Yes
        1 = No

SHFT_IN (Port 03 read)
 bit0-7 Shift register data
```

#### Outputs

```
SHFT_AMNT (Port 02 write)
 bit0-2 Shift offset

SOUND1 (Port 03 write)
 bit0   UFO (repeats)
 bit1   Shot
 bit2   Player death
 bit3   Invader death
 bit4   Extended play
 bit5   AMP enable
 bit6-7 Not connected

SHFT_DATA (Port 04 write)
 bit0-7 Shift data

SOUND2 (Port 05 write)
 bit 0  Fleet movement 1
 bit 1  Fleet movement 2
 bit 2  Fleet movement 3
 bit 3  Fleet movement 4
 bit 4  UFO Hit
 bit5-7 Not connected

WATCHDOG (Port 06 write)
 write anything to reset
```