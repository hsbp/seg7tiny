#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define NUM_DIGITS 3
#define DIGIT_PORT PORTD
#define DIGIT_DDR DDRD
#define DIGIT_OFFSET 2
#define SEGMENT_PORT PORTB

volatile uint8_t framebuf[NUM_DIGITS];

#define TIMER1_PRESCALE_1 1
#define TIMER1_PRESCALE_8 2
#define TIMER1_PRESCALE_64 3
#define TIMER1_PRESCALE_256 4
#define TIMER1_PRESCALE_1024 5

uint8_t cur_digit = 0;
uint8_t DIGIT_MASK = 0xFF;

ISR( TIMER1_COMPA_vect ) {
	if (cur_digit++ >= NUM_DIGITS) cur_digit = 0;
	PORTD &= DIGIT_MASK;
	PORTB = framebuf[cur_digit];
	PORTD |= _BV(DIGIT_OFFSET) << cur_digit;
}

int main(void) {

	DDRB = 0xFF;
	DDRD |= 4 + 8 + 16;

	for (uint8_t digit = 0; digit < NUM_DIGITS; digit++) {
		framebuf[digit] = 0xFF;
		DIGIT_MASK &= ~(_BV(DIGIT_OFFSET) << digit);
	}

	TCCR1B = (1 << WGM12) | TIMER1_PRESCALE_1;
	OCR1A = (uint16_t)5000;

	TIMSK |= 1 << OCIE1A;   // Output Compare Interrupt Enable (timer 1, OCR1A) 
	sei();                 // Set Enable Interrupts

	while (1) {
		for (uint8_t digit = 0; digit < 3; digit++) {
			for (uint8_t segment = 1; segment; segment <<= 1) {
				framebuf[digit] ^= segment;
				_delay_ms(20);
			}
		}
	}
}
