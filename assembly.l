# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"
# 22 "main.c"
# 1 "/usr/lib/avr/include/avr/interrupt.h" 1 3
# 38 "/usr/lib/avr/include/avr/interrupt.h" 3
# 1 "/usr/lib/avr/include/avr/io.h" 1 3
# 99 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/sfr_defs.h" 1 3
# 126 "/usr/lib/avr/include/avr/sfr_defs.h" 3
# 1 "/usr/lib/avr/include/inttypes.h" 1 3
# 37 "/usr/lib/avr/include/inttypes.h" 3
# 1 "/usr/lib/gcc/avr/5.4.0/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/avr/5.4.0/include/stdint.h" 3 4
# 1 "/usr/lib/avr/include/stdint.h" 1 3 4
# 125 "/usr/lib/avr/include/stdint.h" 3 4

# 125 "/usr/lib/avr/include/stdint.h" 3 4
typedef signed int int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
typedef signed int int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__)));
typedef signed int int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__)));

typedef signed int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));
# 146 "/usr/lib/avr/include/stdint.h" 3 4
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 163 "/usr/lib/avr/include/stdint.h" 3 4
typedef int8_t int_least8_t;




typedef uint8_t uint_least8_t;




typedef int16_t int_least16_t;




typedef uint16_t uint_least16_t;




typedef int32_t int_least32_t;




typedef uint32_t uint_least32_t;







typedef int64_t int_least64_t;






typedef uint64_t uint_least64_t;
# 217 "/usr/lib/avr/include/stdint.h" 3 4
typedef int8_t int_fast8_t;




typedef uint8_t uint_fast8_t;




typedef int16_t int_fast16_t;




typedef uint16_t uint_fast16_t;




typedef int32_t int_fast32_t;




typedef uint32_t uint_fast32_t;







typedef int64_t int_fast64_t;






typedef uint64_t uint_fast64_t;
# 277 "/usr/lib/avr/include/stdint.h" 3 4
typedef int64_t intmax_t;




typedef uint64_t uintmax_t;
# 10 "/usr/lib/gcc/avr/5.4.0/include/stdint.h" 2 3 4
# 38 "/usr/lib/avr/include/inttypes.h" 2 3
# 77 "/usr/lib/avr/include/inttypes.h" 3
typedef int32_t int_farptr_t;



typedef uint32_t uint_farptr_t;
# 127 "/usr/lib/avr/include/avr/sfr_defs.h" 2 3
# 100 "/usr/lib/avr/include/avr/io.h" 2 3
# 428 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/iotn85.h" 1 3
# 38 "/usr/lib/avr/include/avr/iotn85.h" 3
# 1 "/usr/lib/avr/include/avr/iotnx5.h" 1 3
# 39 "/usr/lib/avr/include/avr/iotn85.h" 2 3
# 429 "/usr/lib/avr/include/avr/io.h" 2 3
# 627 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/portpins.h" 1 3
# 628 "/usr/lib/avr/include/avr/io.h" 2 3

# 1 "/usr/lib/avr/include/avr/common.h" 1 3
# 630 "/usr/lib/avr/include/avr/io.h" 2 3

# 1 "/usr/lib/avr/include/avr/version.h" 1 3
# 632 "/usr/lib/avr/include/avr/io.h" 2 3






# 1 "/usr/lib/avr/include/avr/fuse.h" 1 3
# 239 "/usr/lib/avr/include/avr/fuse.h" 3
typedef struct
{
    unsigned char low;
    unsigned char high;
    unsigned char extended;
} __fuse_t;
# 639 "/usr/lib/avr/include/avr/io.h" 2 3


# 1 "/usr/lib/avr/include/avr/lock.h" 1 3
# 642 "/usr/lib/avr/include/avr/io.h" 2 3
# 39 "/usr/lib/avr/include/avr/interrupt.h" 2 3
# 23 "main.c" 2

# 23 "main.c"
unsigned long count = 0;



void __vector_10(void) __attribute__ ((signal));
void __vector_10(void) { 
# 28 "main.c" 3
                        (*(volatile uint8_t *)((0x18) + 0x20)) 
# 28 "main.c"
                              ^= (1 << (0)); }






void delayMillisecond(unsigned long time){
    
# 36 "main.c" 3
   (*(volatile uint8_t *)((0x2C) + 0x20)) 
# 36 "main.c"
         = (1 << (7));
    
# 37 "main.c" 3
   (*(volatile uint8_t *)((0x2A) + 0x20)) 
# 37 "main.c"
          |= (1 << (1));
    
# 38 "main.c" 3
   (*(volatile uint8_t *)((0x33) + 0x20)) 
# 38 "main.c"
          |= (1 << (1));

    
# 40 "main.c" 3
   (*(volatile uint8_t *)((0x29) + 0x20)) 
# 40 "main.c"
         = 0x7D;
    
# 41 "main.c" 3
   (*(volatile uint8_t *)((0x39) + 0x20)) 
# 41 "main.c"
         |= (1 << (4));;




    
# 46 "main.c" 3
   (*(volatile uint8_t *)((0x2C) + 0x20)) 
# 46 "main.c"
         = 0x00;
    count = 0;

    while(count < time){


        if( (
# 52 "main.c" 3
            (*(volatile uint8_t *)((0x38) + 0x20)) 
# 52 "main.c"
                 & (1 << (4))) != 0){
            count ++;
            
# 54 "main.c" 3
           (*(volatile uint8_t *)((0x38) + 0x20)) 
# 54 "main.c"
                |=0<<4;
        }

    }
    
# 58 "main.c" 3
   (*(volatile uint8_t *)((0x39) + 0x20)) 
# 58 "main.c"
         =0;

}




int main(){
    *((volatile unsigned char*)(0x1F +0x20)) |= (1 << (7));

    
# 68 "main.c" 3
   (*(volatile uint8_t *)((0x17) + 0x20)) 
# 68 "main.c"
        |= (1<<0);
    while(1){

        delayMillisecond(100);

    }


    return 0;
}
