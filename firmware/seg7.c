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

#define SERIAL_COMMAND 0xFF

uint8_t serial_digit = SERIAL_COMMAND;

ISR( USART_RX_vect  ) {
	if (serial_digit == SERIAL_COMMAND) {
		serial_digit = UDR & 0x0F;
		if (serial_digit >= NUM_DIGITS) serial_digit = 0;
	} else {
		framebuf[serial_digit] = UDR;
		serial_digit = SERIAL_COMMAND;
	}
}

int main(void) {

	DDRB = 0xFF;
	DDRD |= 4 + 8 + 16;

	for (uint8_t digit = 0; digit < NUM_DIGITS; digit++) {
		framebuf[digit] = 0xFF;
		DIGIT_MASK &= ~(_BV(DIGIT_OFFSET) << digit);
	}

	const uint16_t brr = F_CPU / 16 / 1200 - 1;
	UBRRL = brr & 0xFF;
	UBRRH = brr >> 8;
	UCSRB |= _BV(RXEN) | _BV(RXCIE);

	TCCR1B = (1 << WGM12) | TIMER1_PRESCALE_1;
	OCR1A = (uint16_t)5000;

	TIMSK |= 1 << OCIE1A;   // Output Compare Interrupt Enable (timer 1, OCR1A)
	sei();                 // Set Enable Interrupts

	while (1);
}
