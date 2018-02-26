#include <avr/io.h>

//adc variables
volatile uint8_t WadcPer, XadcPer, MadcPer;
volatile int16_t WadcNum, XadcNum, MadcNum;

//control variables
volatile int16_t W, X, M; 
volatile int16_t E; 
volatile int16_t Y;

//program variables
volatile float  Um, Uemk, U, Rv, Rm, Kp, Umot;



int main(void)
{
      WadcPer = 0;
      WadcNum = 0;
      XadcNum = 0;
      XadcPer = 0;
      W = 0;
      X = 0;
      E = 0;
      Um = 0;
      U = 5.0;
      Rv = 2.0;
      Rm = 5.1;
      Uemk = 0;
      //Kp regulation of 2.5 experimental
      Kp = 2.5;
      MadcNum = 0;
      MadcPer = 0;
      M = 0;
      Umot = 0;
      
      //PWM
      DDRB |= (1 << DDB3);                            // PB3 is pwm output
      TCCR2 |= (1 << COM21);                                // none-inverting mode
      TCCR2 |= (1 << WGM21) | (1 << WGM20);                 // Fast PWM Mode
      TCCR2 |= (1 << CS21);                           // Prescaler=8 & start PWM

        //Indicate LED
        DDRD |= (1 << PD7);
            

      while(1) {
                
            // ADC0
            // Measure potentiometer in one cycle
            ADMUX = 0x40;                                   // Set Reference to AVCC and input to ADC0
            ADCSRA = (1<<ADEN)|(1<<ADPS2);                  // Enable ADC, set prescaler to 16
            ADCSRA |= (1<<ADSC);                            // Start the first conversion     
            while(ADCSRA &(1<<ADSC));                       // Wait until conversion is finished   
            ADCSRA |= (1<<ADSC);                            // Start the next conversion
            WadcNum = ADC;                                                         
            //WadcPer = (WadcNum *  100.0) / 1024.0;        // max 1023 or 0
            WadcPer = (WadcNum *  50.0) / 512.0;            // max 1023 or 0
            W = (255.0 * WadcPer) / 100.0;                  // set dutycycle
                  
            // ohne regelung
            // OCR2 = W;                

                  
            // ADC2
            // Measure motorvoltage: is it running?
            ADMUX = 0x42;                                  // Set Reference to AVCC and input to ADC2
            ADCSRA = (1<<ADEN)|(1<<ADPS2);                 // Enable ADC, set prescaler to 16
            ADCSRA |= (1<<ADSC);                           // Start the first conversion
            while(ADCSRA &(1<<ADSC));                      // Wait until conversion is finished
            ADCSRA |= (1<<ADSC);                           // Start the next conversion
            MadcNum = ADC;
            Umot = (MadcNum * 5.0) / 1024.0;
            MadcPer = (MadcNum *  50.0) / 512.0;      // max 1023 or 0
            M = (255.0 * MadcPer) / 100.0;                               

            // ADC1
            // Measure motorcurrent with Rm
            ADMUX = 0x41;                                  // Set Reference to AVCC and input to ADC1                                   
            ADCSRA = (1<<ADEN)|(1<<ADPS2);                 // Enable ADC, set prescaler to 16
            ADCSRA |= (1<<ADSC);                           // Start the first conversion
            while(ADCSRA &(1<<ADSC));                      // Wait until conversion is finished
            ADCSRA |= (1<<ADSC);                           // Start the next conversion
                  
            // voltage on Rm numeric value
            XadcNum = ADC;
                  
            XadcPer = (XadcNum *  50.0) / 512.0;     // max 1023 or 0
            X = (255.0 * XadcPer) / 100.0;                               
 
            // calculate to voltage ->         
            Um = (XadcNum * 5.0) / 1024.0;       

            //Uemk = U - (Umotor * (Rv/Rm * Um);
            Uemk = 5.0 - Umot - (2.0/5.0 * Um); 
            
            XadcNum = ((Uemk / 5.0) * 1024.0);

            XadcPer = (XadcNum * 50) / 512;           // max 1023 or 0
            X = (255.0 * XadcPer) / 100.0; 
                   
            E = W - X;

            Y = Kp * E; 
                  
            //Limitation of Y
            if ( Y < 0) {
                  Y = 0;
                  //Indicate LED
                  PORTD &= ~(1 << PD7);
            } 
            if ( Y > 255) {
                  Y = 255;
                  PORTD |=  (1 << PD7);
            }     
            OCR2 = Y;              
      }

}




