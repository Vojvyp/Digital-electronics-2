/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library
#include "gen_functions.h"  // Own library for generating functions

#ifndef F_CPU
#define F_CPU 160000000
#endif

// look- up talbe for sin
uint16_t table[] = {127,130,133,136,140,143,146,149,152,155,158,161,164,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,247,248,249,250,251,252,252,253,253,254,254,254,254,254,254,254,254,254,254,253,253,252,252,251,250,249,248,247,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,164,161,158,155,152,149,146,143,140,136,133,130,127,124,121,118,114,111,108,105,102,99,96,93,90,87,84,81,78,75,72,69,66,64,61,58,56,53,51,48,46,43,41,39,36,34,32,30,28,26,24,22,20,19,17,16,14,13,11,10,9,8,7,6,5,4,3,2,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,2,3,4,5,6,7,8,9,10,11,13,14,16,17,19,20,22,24,26,28,30,32,34,36,39,41,43,46,48,51,53,56,58,61,64,66,69,72,75,78,81,84,87,90,93,96,99,102,105,108,111,114,118,121,124,127};

uint16_t Frequency = 10; // stored frequency
char gen = 'S'; // generator mode: S/R/Q/D (sin, ramp, square, DTMF)
char button = '1'; // last button pressed


/* Function definitions ----------------------------------------------*/

int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(0, 0); lcd_puts("GENERATOR:");
    lcd_gotoxy(0, 1); lcd_puts("FREQ:");
    lcd_gotoxy(11, 0);
    lcd_puts("SIN ");
    
    // Initialize R2R output 
    output_init();

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 <<REFS1);

    // Set input channel to ADC0
    ADMUX &= ~(15);

    // Enable ADC module
    ADCSRA |= (1 << ADEN);

    // Enable conversion complete interrupt
    ADCSRA |= (1 << ADIE);

    // Set clock prescaler to 128
    ADCSRA |= 7;


    // Configuration 16-bit Timer/Counter1  and Timer/Counter0 
    TIM1_overflow_interrupt_enable();
    TIM1_overflow_262ms();

    TIM0_overflow_interrupt_enable();
    TIM0_overflow_16us();

    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));


    // Enables interrupts by setting the global interrupt mask
    sei();
    
    // Initialization ADC
    ADCSRA |= (1 << ADSC);

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
ISR(TIMER1_OVF_vect)
{   
    // setting variables
    uint16_t value = 0;
    char lcd_string[4] = "0000";
    
    static uint8_t position = 0;        // position of digits
    static char frq_string[10] = "10"; 
       
    value = ADC;    // Copy ADC result to 16-bit variable    
    itoa(value, lcd_string, 10);    // Convert to string in decimal
     
        
    
    lcd_gotoxy(8, 1);           // Clearing freq positions
    lcd_puts("           ");
    
    lcd_gotoxy(7, 1);           // Printing frequency
    lcd_puts(frq_string);
    
    lcd_gotoxy(12,1);          
    lcd_puts("Hz");

    

    if (value < 50)
    {
        //lcd_gotoxy(8, 1);
        //lcd_puts("None");
    } 
    else
    {
        if (value > 600)
        {
            button = '7';                       // updating button variable storing last pressed key
            frq_string[position] = '7';         // Adding 7 to frequency string
            if (position < 3) {position++;}     // incrementing position
        } 
        else
        {
            if (value > 400)
            {
                button = '8';
                frq_string[position] = '8';
                if (position < 3) {position++;}
            } 
            else
            {
                if (value > 280)
                {
                    button = '9';
                    frq_string[position] = '9';
                    if (position < 3) {position++;}
                } 
                else
                {
                    if (value > 240)
                    {
                        lcd_gotoxy(11, 0);
                        lcd_puts("SIN   ");     // Printing sin
                        gen = 'S';              // updating gen mode
                    } 
                    else
                    {
                        if (value > 200)
                        {
                          button = '4';
                          frq_string[position] = '4';
                          if (position < 3) {position++;}  
                        } 
                        else
                        {
                            if (value > 170)
                            {
                                button = '5';
                                frq_string[position] = '5';
                                if (position < 3) {position++;}
                            } 
                            else
                            {
                                if (value > 145)
                                {
                                   button = '6';
                                   frq_string[position] = '6';
                                   if (position < 3) {position++;}
                                } 
                                else
                                {
                                    if (value > 131)
                                    {
                                        lcd_gotoxy(11, 0);
                                        lcd_puts("RAMP  ");     // Printing RAMP
                                        gen = 'R';              // Updating gen mode
                                    } 
                                    else
                                    {
                                        if (value > 120)
                                        {
                                            button = '1';
                                            frq_string[position] = '1';
                                            if (position < 3) {position++;}
                                        } 
                                        else
                                        {
                                            if (value > 108)
                                            {
                                               button = '2';
                                               frq_string[position] = '2';
                                               if (position < 3) {position++;}
                                            } 
                                            else
                                            {
                                                if (value > 97)
                                                {
                                                    button = '3';
                                                    frq_string[position] = '3';
                                                    if (position < 3) {position++;}
                                                } 
                                                else
                                                {
                                                    if (value > 90)
                                                    {
                                                        lcd_gotoxy(11, 0);      // Printing SQR
                                                        lcd_puts("SQR   ");
                                                        gen = 'Q';              // updating gen mode
                                                    } 
                                                    else
                                                    {
                                                        if (value > 85)
                                                        {                                                           
                                                            button = '*';
                                                                                        // used for clearing freq string to default (10 Hz)
                                                            
                                                            lcd_gotoxy(5, 1);           // Clearing freq positions
                                                            lcd_puts("           ");
                                                            frq_string[0] = '1';        // Seting freq string to "10        "
                                                            frq_string[1] = '0';
                                                            frq_string[2] = ' ';
                                                            frq_string[3] = ' ';
                                                            frq_string[4] = ' ';
                                                            frq_string[5] = ' ';
                                                            frq_string[6] = ' ';
                                                            frq_string[7] = ' ';
                                                            frq_string[8] = ' ';
                                                            frq_string[9] = ' ';
                                                            
                                                            
                                                            position = 0;           // reseting position
                                                        } 
                                                        else
                                                        {
                                                            if (value > 77)
                                                            {
                                                               button = '0';
                                                               frq_string[position] = '0';
                                                               if (position < 3) {position++;}
                                                            } 
                                                            else
                                                            {
                                                                if (value > 73)
                                                                {
                                                                   button = '#';
                                                                   position = 0;    // reseting positon
                                                                   if ((atoi(frq_string) <= 5000) & (atoi(frq_string) > 9) )
                                                                   {
                                                                       Frequency =  atoi(frq_string);   // storing frequency from string
                                                                   }
                                                                   else
                                                                   {
                                                                      lcd_gotoxy(7, 1);
                                                                      lcd_puts("error");        //  printing error if frequenci is not in the boundries (10 < Freq < 5k)
                                                                   }

                                                                } 
                                                                else
                                                                {
                                                                    lcd_gotoxy(11, 0);
                                                                    lcd_puts("DTMF");       //Printing DTMF
                                                                    gen = 'D';              // updating den mode
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    ADCSRA |= (1 << ADSC);  // starting AD conversion
}


ISR(TIMER0_OVF_vect)
{
    static uint8_t bitstream = 0b00000000;

  

    uint16_t steps = 100000/(1.6*Frequency);  // number of steps in 1 period
        
    float step_size = 0;
    uint8_t step_size_int =0;
    step_size = (float)255/steps;       // calculation size of step
    
    static float cumulative_step_size = 0;
        
    if (cumulative_step_size < 1)
    {
        cumulative_step_size += step_size ;
            
    }
    else
    {
        cumulative_step_size = 0;
    }
        
    step_size_int = (int)cumulative_step_size;      // taking integer value from float
 
 
    // function for SIN
    static uint8_t position = 0;  
    if (gen == 'S')
    {
        bitstream = table[position];        // taking values from look-up table
        output_bitstream(bitstream);
            if (position  < sizeof(table))      //updating position
            {
                position+= step_size_int;
            }
            else
            {
                position = 0;
            }
    } 
     
    
    //Function for RAMP
    if (gen == 'R')
    {   
        if ((bitstream +step_size_int) <= 0xff) 
        {    
            bitstream += step_size_int;        // increasing output value until
            output_bitstream(bitstream);       
        } 
        else
        {
            bitstream = 0xff;               //reseting output value
            output_bitstream(bitstream);
            bitstream = 0x00;
            output_bitstream(bitstream);
        }
    }
    
    // funktion for square
    if (gen == 'Q')
    {
    static uint16_t counter = 0;  
    uint16_t steps = 100000/(1.6*Frequency);  
    
    if (counter < steps/2)      // half of the steps low value
    {
        counter++;
        bitstream = 0xff;
        output_bitstream(bitstream);

    }
    else if (counter < steps)   //other half high value
        {
            counter++;
            bitstream = 0x00;
            output_bitstream(bitstream);
        }
        else {counter = 0;}     //reseting counter
    }
        

    //function for DTMF
    if (gen == 'D')
    {
        lcd_gotoxy(15, 0);
        lcd_putc(button);       // Printing selected button
        uint16_t freqC  = 0;
        uint16_t freqR = 0;
        
        // assigning frequencies (in row)
        if ((button == '1') | (button == '2') | (button == '3'))
        {
            freqR = 697;
        }
        else 
        {

            if ((button == '4') | (button == '5') | (button == '6'))
            {
                freqR = 770;
            } 
            else
            {
                if ((button == '7') | (button == '8') | (button == '9'))
                {
                    freqR = 852;
                } 
                else
                {
                  if ((button == '*') | (button == '0') | (button == '#'))
                  {
                      freqR = 941;
                  }  
                }
            }                  
        }
        
        
        // assigning frequencies (in columns)
        if ((button == '1') | (button == '4') | (button == '7') | (button == '*'))
        {
            freqC = 1209;
        } 
        else
        {
            if ((button == '2') | (button == '5') | (button == '8') | (button == '0'))
            {
                freqC = 1336;
            } 
            else
            {
                if ((button == '3') | (button == '6') | (button == '9') | (button == '#'))
                {
                    freqC = 1477;
                }
            }
        }

        //calculationg step sizes
        uint8_t step_size_R = freq_to_stepsize(freqR);
        uint8_t step_size_C = freq_to_stepsize(freqC);
        
        
        static uint8_t positionR = 0;
        static uint8_t positionC = 0;

        bitstream = (table[positionR] + table[positionC] /2);   // calculating output value
        output_bitstream(bitstream);
        
        //updating positions
        if ((positionR + step_size_R) < sizeof(table))
        {
            positionR += step_size_R;
        }
        else
        {
            positionR = 0;
        }

        if ((positionC + step_size_C) < sizeof(table))
        {
            positionC += step_size_C;
        }
        else
        {
            positionC = 0;
        }                          
    }


    //UART - sending shown info to uart
    //unfortunately the uart part makes the whole app not work
        
    /*
    uart_puts("Generator: ");       // printing gen mode
    uart_putc(gen);
    uart_puts("\r\n");
    if (gen == 'D')
    { 
        uart_puts("Button presed: ");       //printing button
        uart_putc(button);          
        uart_puts("\r\n");
    }
    else
    {
        uart_puts("Frequency: ");       //printing frequency
        char lcd_string[6] = "";
        itoa(Frequency, lcd_string, 10);        
        uart_puts(lcd_string);
        uart_puts("\r\n");       
    }
    */   
    
}
