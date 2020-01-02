/*
    Experimenting around fault injection and how instructions are
    altered by it.
    Copyright (C) 2019 Gianluca Pacchiella <gp@ktln2.org>.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hal.h"
#include "uart.h"
#include <stdint.h>
#include <stdlib.h>

#define USART USARTC0

#include "simpleserial.h"

#define CODE __asm__("mov r0, r0")

#define ten(a)     a;a;a;a;a;a;a;a;a;a
#define hundred(a) ten(ten(a))

/*
 74a:   80 91 a1 08     lds     r24, 0x08A1     ; 0x8008a1 <__TEXT_REGION_LENGTH__+0x7008a1>
 74e:   85 ff           sbrs    r24, 5
 750:   fc cf           rjmp    .-8             ; 0x74a <main+0xfa>
 752:   81 e4           ldi     r24, 0x41       ; 65
 754:   80 93 a0 08     sts     0x08A0, r24
*/
/* r24 is used to store the byte to send outside */
#define putch_r(r) __asm__( \
    "lds r24, 0x08a1\n \
    sbrs r24, 5\n      \
    rjmp .-8\n         \
    mov r24, r" #r "\n     \
    sts 0x08a0, r24"    \
    )

/* so obviously we need to save r24 */
#define save_r24() __asm__("push r24")
#define putch_r24() __asm__( \
    "lds r24, 0x08a1\n \
    sbrs r24, 5\n      \
    rjmp .-8\n         \
    pop r24\n          \
    sts 0x08a0, r24"   \
    )

/* nice: ldi can be used only with registers r16-r31 */
#define set_r(r,value) __asm__( \
    "ldi r31, " #value "\n\
     mov r" #r ", r31 "   \
    )

int main(void)
{
    platform_init();
	init_uart();
	trigger_setup();

    trigger_high();
    trigger_low();
	putch('h');
	putch('e');
	putch('l');
	putch('l');
	putch('o');
    set_r(0, 0x80);
    set_r(1, 0x01);
    set_r(2, 0x02);
    set_r(3, 0x03);
    set_r(4, 0x04);
    set_r(5, 0x05);
    set_r(6, 0x06);
    set_r(7, 0x07);
    set_r(8, 0x08);
    set_r(9, 0x09);
    set_r(10, 0x0a);
    set_r(11, 0x0b);
    set_r(12, 0x0c);
    set_r(13, 0x0d);
    set_r(14, 0x0e);
    set_r(15, 0x0f);
    set_r(16, 0x10);
    set_r(17, 0x11);
    set_r(18, 0x12);
    set_r(19, 0x13);
    set_r(20, 0x14);
    set_r(21, 0x15);
    set_r(22, 0x16);
    set_r(23, 0x17);
    set_r(24, 24);
    set_r(25, 0x19);
    set_r(26, 0x1a);
    set_r(27, 0x1b);
    set_r(28, 0x1c);
    set_r(29, 0x1d);
    set_r(30, 0x1e);
    set_r(31, 0x1f);

    hundred(CODE);

    save_r24();
    putch_r(0);
    putch_r(1);
    putch_r(2);
    putch_r(3);
    putch_r(4);
    putch_r(5);
    putch_r(6);
    putch_r(7);
    putch_r(8);
    putch_r(9);
    putch_r(10);
    putch_r(11);
    putch_r(12);
    putch_r(13);
    putch_r(14);
    putch_r(15);
    putch_r(16);
    putch_r(17);
    putch_r(18);
    putch_r(19);
    putch_r(20);
    putch_r(21);
    putch_r(22);
    putch_r(23);
    putch_r24();
    putch_r(25);
    putch_r(26);
    putch_r(27);
    putch_r(28);
    putch_r(29);
    putch_r(30);
    putch_r(31);
}
