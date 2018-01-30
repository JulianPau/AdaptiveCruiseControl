#include <avr/io.h>

uint8_t x;
uint8_t toggle;

int main(void)
{
	x = 0;
	toggle = 0;
    //PWM
	DDRB |= (1 << DDB3);					// PB3 as pwm output
    TCCR2 |= (1 << COM21);					// Set none-inverting mode
	TCCR2 |= (1 << WGM21) | (1 << WGM20);	// Fast PWM Mode
	TCCR2 |= (1 << CS21);					// Prescaler=8 & start PWM


	while(1) {
    	
		// Multiplex between ADCs
		if(toggle){
        	
			// ADC1
			// Measure potentiometer in one cycle
			ADMUX = (1<<REFS0);               		// Set Reference to AVCC and input to ADC0
    		ADCSRA = (1<<ADEN)|(1<<ADPS2);    		// Enable ADC, set prescaler to 16
			ADCSRA |= (1<<ADSC);					// Start the first conversion
			
			while(ADCSRA &(1<<ADSC));     			// Wait until conversion is finished
        	ADCSRA |= (1<<ADSC);          			// Start the next conversion					
			x = (ADC / 1024.0) * 100 ;				// max 1024 oder 0
			OCR2 = (255.0/100.0) * x;				// set dutycycle
			toggle = 0;
			
		}else{

			// ADC2
			// Measure motorcurrent in other cycle
			
			//TODO adc channel auf anderen eingang schalten und auslesen
			
			toggle = 1;
		}  	
	}

}











/*int main(void)
{
    DDRB |= (1 << DDB3);					// PB3 is now an output
    TCCR2 |= (1 << COM21);					// set none-inverting mode
	TCCR2 |= (1 << WGM21) | (1 << WGM20);	// set fast PWM Mode
	TCCR2 |= (1 << CS21);					// set prescaler to 8 and starts PWM

	x = 0;


	ADMUX = (1<<REFS0);               		// Set Reference to AVCC and input to ADC0
    ADCSRA = (1<<ADEN)|(1<<ADPS2);    		// Enable ADC, set prescaler to 16

	ADCSRA |= (1<<ADSC);					// Start the first conversion
	
	while( 1 ) {
    	
		
        while(ADCSRA &(1<<ADSC));     		// Wait until conversion is finished

        ADCSRA |= (1<<ADSC);          		// Start the next conversion
		
		
		x = (ADC / 1024.0) * 100 ;			// max 1024 oder 0
		
		OCR2 = (255.0/100.0) * x;
	
  	
	
	}

}*/











