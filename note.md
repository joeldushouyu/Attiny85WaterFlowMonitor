## avrdude  format
```bash
avrdude
-C path of /avrdude.conf  (has a default already)
-p controller type
-c programmer
-U user command
```


## bitwise operation
PORTB |= (1<<5) turn on
PORTB &= ~(1<<5) turn off
PORTB ^= (1<<5)  toggle

## REFERENCE:ca
https://medium.com/@bradford_hamilton/bare-metal-programming-attiny85-22be36f4e9ca

## 1. change fuse bit for clock (optional)

change CKDIV8 to 1 in Table 20-5: to use the full 8MHZ clock


Change lfuse from 0x62 -> 0xE2
```bash

avrdude -p t85 -c usbtiny -U lfuse:w:0xE2:m
```

<!-- 
# 2. change by disable reset pin
change HFUSe from 0xDF -> 0x5F
```bash
avrdude -p t85 -c usbtiny -U hfuse:w:0x5F:m
``` -->

#compile our code

```bash
avr-gcc -Os -mmcu=attiny85 main.c

```
# convert assembly to hex
```bash
avr-objcopy -O ihex -j .text -j .data a.out a.hex
```

# load program to microcontroller
### write to flash memory
```bash
avrdude -p t85 -c usbtiny -U flash:w:a.hex:i

```