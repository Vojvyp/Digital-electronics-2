#ifndef GEN_FUNCTIONS_H_
#define GEN_FUNCTIONS_H_

#include <avr/io.h>

void output_init(void);
// funtion for setting ports to output

void output_bitstream(uint8_t bitstream);
//funtion for setting output bitsream

uint8_t freq_to_stepsize(uint16_t Frequency);
//funtion for calculatin stepsize


#endif /* GEN_FUNCTIONS_H_ */