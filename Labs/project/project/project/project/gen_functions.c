#include "gpio.h"

#define BIT0 PB2
#define BIT1 PB3
#define BIT2 PB4
#define BIT3 PB5
#define BIT4 PC4
#define BIT5 PC5
#define BIT6 PD2
#define BIT7 PD3

//defining pins



void output_init(void)
{
    //setting pins to output
    GPIO_config_output(&DDRD, BIT7);
    GPIO_config_output(&DDRD, BIT6);
    GPIO_config_output(&DDRC, BIT5);
    GPIO_config_output(&DDRC, BIT4);
    GPIO_config_output(&DDRB, BIT3);
    GPIO_config_output(&DDRB, BIT2);
    GPIO_config_output(&DDRB, BIT1);
    GPIO_config_output(&DDRB, BIT0);    
}

void output_bitstream(uint8_t bitstream)
{
    //setting pins to bitsream value
    if ((bitstream >> 0) & 1)   // checking BIT0 - B2
    {
        GPIO_write_high(&PORTB, BIT0);
    } 
    else
    {
        GPIO_write_low(&PORTB, BIT0);
    }
    
    if ((bitstream >> 1) & 1)   // checking BIT1 - B3
    {
        GPIO_write_high(&PORTB, BIT1);
    }
    else
    {
        GPIO_write_low(&PORTB, BIT1);
    }
    
    if ((bitstream >> 2) & 1)   // checking BIT2 - B4
    {
        GPIO_write_high(&PORTB, BIT2);
    }
    else
    {
        GPIO_write_low(&PORTB, BIT2);
    }
    
    if ((bitstream >> 3) & 1)   // checking BIT3 - B5
    {
        GPIO_write_high(&PORTB, BIT3);
    }
    else
    {
        GPIO_write_low(&PORTB, BIT3);
    }
        
    if ((bitstream >> 4) & 1)   // checking BIT4 - C4
    {
        GPIO_write_high(&PORTC, BIT4);
    }
    else
    {
        GPIO_write_low(&PORTC, BIT4);
    }
    
    if ((bitstream >> 5) & 1)   // checking BIT5 - C5
    {
        GPIO_write_high(&PORTC, BIT5);
    }
    else
    {
        GPIO_write_low(&PORTC, BIT5);
    }
    
    if ((bitstream >> 6) & 1)    // checking BIT6 - D2
    {
        GPIO_write_high(&PORTD, BIT6);
    }
    else
    {
        GPIO_write_low(&PORTD, BIT6);
    }
    
    if ((bitstream >> 7) & 1)    // checking BIT7 - D3
    {
        GPIO_write_high(&PORTD, BIT7);
    }
    else
    {
        GPIO_write_low(&PORTD, BIT7);
    }
    
}

uint8_t freq_to_stepsize(uint16_t Frequency)
{
    //calculationg step size
    uint16_t steps = 100000/(1.6*Frequency);    // number of steps in 1 period

    float step_size = 0;
    uint8_t step_size_int =0;
    step_size = (float)255/steps;   // calculation size of step

    float cumulative_step_size = 0;
    

    if (cumulative_step_size < 1)
    {
    cumulative_step_size += step_size ;
    
    }
    else
    {
    cumulative_step_size = 0;
    }

    // taking integer value from float
    step_size_int = (int)cumulative_step_size;
    
    return step_size_int;
}


