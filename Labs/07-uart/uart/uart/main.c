/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library

#ifndef F_CPU
#define F_CPU 160000000
#endif



/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");    // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");    // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");    // Put button name here

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


    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms
    TIM1_overflow_interrupt_enable();
    TIM1_overflow_262ms();

    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));


    // Enables interrupts by setting the global interrupt mask
    sei();

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
/**
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

}

/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. Display value on LCD
 * and send it to UART.
 */
ISR(ADC_vect)
{
    // WRITE YOUR CODE HERE
    uint16_t value = ADC;
    char lcd_string[8] = "        ";

   
     //calculation  parity
     uint8_t parBit = 0;
     uint8_t parVal = 0;
     
     for (uint8_t i = 0; i < 16; i++) // passing through the whole variable: value
     {
	     parBit^= (value & 1);        // XOR 
	     value>>= 1;				  // bit shift 
     }
	 
	 if (parBit==1)  
	 {
		 parVal = 1;
	 } 
	 else
	 {
		 parVal = 0;
	 }
     
     
	   
    
    // Clear dec and hex position
    lcd_gotoxy(8, 0);           
    lcd_puts("        ");   
    
	// Print ADC value on LCD in dec
    itoa(value, lcd_string, 10);    
    lcd_gotoxy(8, 0); 
    lcd_puts(lcd_string);    
      
    // Print ADC value on LCD in hex
    itoa(value, lcd_string, 16);    
    lcd_gotoxy(13,0); 
    lcd_puts(lcd_string);    
    
    //Clear key position
    lcd_gotoxy(8, 1);           
    lcd_puts("        ");
    lcd_gotoxy(8, 1);
    
	// Print key	
    if (value == 1022)
    {
        lcd_puts("None");
    } 
    else
    {
		if (value == 650)
		{
			lcd_puts("Select");
		}
		else
		{
			if (value == 402)
			{
				lcd_puts("Left");
			}
			else
			{
				if (value == 245)
				{
					lcd_puts("Down");
				}
				else
				{
					if (value == 101)
					{
						lcd_puts("Up");
					}
					else
					{
						lcd_puts("Right");
				 }
			 }
		 }
	 }
	 
	 
	 
 }	
		
		
		
  
}	