if avr-gcc -mmcu=attiny85 main.c; then
avr-objcopy -O ihex -j .text -j .data a.out a.hex
avrdude -p t85 -c usbtiny -U flash:w:a.hex:i
else
echo "compile error"
fi