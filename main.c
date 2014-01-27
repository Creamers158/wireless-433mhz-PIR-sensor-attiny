#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#define TRANS_PIN PB3
#include "rc.h"
#define INPUT_PIN PB4
#define SWITCH_ID "100010100111100010011010" 
unsigned n;

volatile char state;
static void config_hardware(void) {
        // disable interrupts globally 
        cli();
	//power reduction
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        // turn off peripherals not in use
	PRR |= _BV(PRUSI) | _BV(PRADC); 
	// Configure TRANS_PIN as output
	DDRB |= (1 << TRANS_PIN);
	// Set INPUT_PIN as Input
	DDRB &= ~(1 << INPUT_PIN);
        // Enable PCINT0 (PB0) and PCINT1 (PB1) interrupt / triggers PCINT0_vect
        PCMSK |= (1<<PCINT4);
	// Set TRANS_PIN low
	PORTB &= ~(1 << TRANS_PIN);

        /* crude delay to allow PIR sensor to initialize */
        n = 0;
        while (n <= 9)
        {
          _delay_ms(5000);
          n++;
        }

	// pin change interrupt
        GIMSK |= (1<<PCIE);
	// turn on interrupts / GLORY time!!
        sei();
}

int main(void)
{
 	config_hardware();
         while (1){
                sleep_mode();
        }
}

ISR (PCINT0_vect)
{
	//if( !(PINB & (1 << INPUT_PIN)) ) {
	//	state = 1;
	//}
	//} else if ( !(PINB & (1 << INPUT_PIN1)) ) {
	//	state = 0;
	//}
        //byte counter =0x00;
	//byte data = 0x10 | (++counter & 0x0F);
	cli();
	n = 0;
        while (n <= 9)
        {
//              _delay_ms(1000);
              sendStream();
              n++;
        }
	sei();
}

