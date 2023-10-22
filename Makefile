# Compile for attiny85 - optimize for size
compile:
	avr-gcc -Wall  -mmcu=attiny85 main.c

# Get the assembly
assembly:
	avr-gcc -S  -mmcu=attiny85 main.c -o assembly.asm
preprocessor:
	avr-gcc -E  -mmcu=attiny85 main.c -o preprocessor.c
compileWithNoLink: 
	avr-gcc -c  -mmcu=attiny85 main.c -o mainNoLink.o
# Dump hex from out file
dump: compile
	hexdump a.out

# Create Intel hex from out file
hex: compile
	avr-objcopy -O ihex -j .text -j .data a.out a.hex

# Flash ATtiny85 with hex through usbtiny
flash: hex
	avrdude  -v -p t85 -c usbtiny -U flash:w:a.hex:i

clean:
	rm -f a.out assembly.asm a.hex preprocessor.c
