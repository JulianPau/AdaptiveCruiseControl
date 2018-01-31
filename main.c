#include <avr/io.h>

//adc variables
uint8_t WadcPer, XadcPer;
uint16_t WadcNum, XadcNum;

//control variables
uint8_t W, X, Prun, E, Y, Kp;

//program variables
float  Um, Uemk, U, Rv, Rm;
uint8_t toggle;


int main(void)
{
	WadcPer = 0;
	WadcNum = 0;
	XadcNum = 0;
	XadcPer = 0;
	toggle = 0;
	W = 0;
	Um = 0;
	U = 5.0;
	Rv = 4.0;
	Rm = 5.0;
	Uemk = 0;
	Kp = 4;

    //PWM
	DDRB |= (1 << DDB3);					// PB3 as pwm output
    TCCR2 |= (1 << COM21);					// Set none-inverting mode
	TCCR2 |= (1 << WGM21) | (1 << WGM20);	// Fast PWM Mode
	TCCR2 |= (1 << CS21);					// Prescaler=8 & start PWM


	while(1) {
    	
		// Multiplex between ADCs
		if(toggle){
        	
			// ADC0
			// Measure potentiometer in one cycle
			ADMUX = 0x40;           				// Set Reference to AVCC and input to ADC0
    		ADCSRA = (1<<ADEN)|(1<<ADPS2);    		// Enable ADC, set prescaler to 16
			ADCSRA |= (1<<ADSC);					// Start the first conversion
			
			while(ADCSRA &(1<<ADSC));     			// Wait until conversion is finished
        	
        	ADCSRA |= (1<<ADSC);					// Start the next conversion
			WadcNum = ADC;          								
			WadcPer = (WadcNum / 1024.0) * 100;		// max 1024 or 0
			W = (255.0/100.0) * WadcPer;			// set dutycycle
			
			//ohne regelung
			//OCR2 = W;			

			toggle = 0;
			
		}else{

			// ADC1
			// Measure motorcurrent in other cycle
			ADMUX = 0x41;							// Set Reference to AVCC and input to ADC1						
			ADCSRA = (1<<ADEN)|(1<<ADPS2);    		// Enable ADC, set prescaler to 16
			ADCSRA |= (1<<ADSC);					// Start the first conversion
			while(ADCSRA &(1<<ADSC));     			// Wait until conversion is finished
        	ADCSRA |= (1<<ADSC);          			// Start the next conversion
			
			//voltage on Rm numeric value
			XadcNum = ADC;

			//calculate to percentage
			XadcPer = (XadcNum / 1024.0) * 100;
			
			//calculate to voltage - 5V/100 = Um/XadcPer
			Um = 5.0/100.0 * XadcPer;
			
			//aktual measured percentage
			Prun = 100.0/0.62 * Um;

			//calculate PWM signal between 0-255
			X = (255.0/100.0) * Prun;

			//Motorspannung rueckrechnen
			//Uemk = U - Um * (1-Rv/Rm);

			toggle = 1;
		}

		//calculate resulting diffrence
		E = W - X;

		//P control: Y = Kp * E;
		Y = Kp * E;



		OCR2 = Y;

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











