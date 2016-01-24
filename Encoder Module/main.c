//
//  main.c
//  Encoder Module
//
//  Created by Nicholas Burgel on 2016-01-23.
//  Copyright © 2016 JIN Advanced Systems. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "protoAlpha.c"

//Global Variables
int32_t setPosition;
int32_t actualPosition;

void initIO(void) {
    
    /*
     * Port A Setup
     * 
     * Encoder input port
     */
    DDRA = 0b00000000;
    
    /*
     * Port B Setup
     *
     * Pin 0: N/A
     * Pin 1: CLK Out (set in fuse)
     * Pin 2: Step Input
     * Pin 3: Direction Input
     * Pin 4: N/A
     * Pin 5: MOSI
     * Pin 6: MISO
     * Pin 7: SCK
     */
    DDRB = 0b00000000;
    
    /*
     * Port C Setup
     *
     * Pin 0: Encoder Select 1 (OUTPUT)
     * Pin 1: Encoder Select 2 (OUTPUT)
     * Pin 2: Encoder OE (OUTPUT)
     * Pin 3: Encoder Reset (OUTPUT)
     * Pin 4: N/A
     * Pin 5: N/A
     * Pin 6: N/A
     * Pin 7: N/A
     */
    DDRC = 0b00001111;
    PORTC = 0b00000000;
    _delay_us(50);
    PORTC = 0b00001000;
    
    /*
     * Port D Setup
     *
     * Pin 0: Stepper Driver Enable (OUTPUT)
     * Pin 1: Stepper Driver Step Size 1 (OUTPUT)
     * Pin 2: Stepper Driver Step Size 2 (OUTPUT)
     * Pin 3: Stepper Driver Step Size 3 (OUTPUT)
     * Pin 4: Stepper Driver Step (OUTPUT)
     * Pin 5: Stepper Driver Direction (OUTPUT)
     * Pin 6: N/A
     * Pin 7: N/A
     */
    DDRD = 0xFF;
    PORTD = 0b00000000;
    
    setPosition = 0;
    actualPosition = 0;
}

/**
 * Name: readEncoder
 * Details: Reads the data from the encoder.
 *
 * Inputs:
 * Ouputs: position: the position of the encoder
 */
int32_t readEncoder(){
    
    int32_t position = 0;
    
    // Enable the quadrature decoder
    bit_clear(ENCODER, ENC_OE);
    
    // Select first Byte
    bit_clear(ENCODER, ENC_SEL1);
    bit_set(ENCODER, ENC_SEL2);
    
    // Read Byte
    position = PINC;
    
    // Select second Byte
    bit_set(ENCODER, ENC_SEL1);
    bit_set(ENCODER, ENC_SEL2);
    
    // Read Byte
    position = position << 8;
    position += PINC;
    
    // Select first Byte
    bit_clear(ENCODER, ENC_SEL1);
    bit_clear(ENCODER, ENC_SEL2);
    
    // Read Byte
    position = position << 8;
    position += PINC;
    
    // Select first Byte
    bit_set(ENCODER, ENC_SEL1);
    bit_clear(ENCODER, ENC_SEL2);
    
    // Read Byte
    position = position << 8;
    position += PINC;
    
    //Disable quadrature decoder
    bit_set(ENCODER, ENC_OE);
    
    // Return Position
    return position;
}

/**
 * Name: stepper
 * Details: Provides control of stepper driver
 *
 * Inputs: steps: 32 bit variable, number of steps
 *         direction: 8 bit variable, direction of motor
 *                      0xFF = backwards
 *                      0x00 = Forwards
 * Ouputs: success: Boolean success confirmation
 */
void step(int32_t numSteps, int8_t direction) {
    
    // Set direction
    if (direction == 0xff) {
        bit_set(STEPPER, SD_DIR);
    }
    else if (direction == 0x00) {
        bit_clear(STEPPER, SD_DIR);
    }

    
    // Loop till all steps complete
    for (int32_t i = 0; i <= numSteps; i++){
        //Create steps
        bit_set(STEPPER, SD_STEP);
        _delay_us(500);
        bit_clear(STEPPER, SD_STEP);
        _delay_us(500);
    }
}

int main(void) {
    initIO();
    
    while (1) {
        actualPosition = readEncoder();
       if (actualPosition > setPosition) {
            step(actualPosition-setPosition, 0xff);
        }
        else if (actualPosition < setPosition) {
            step(setPosition-actualPosition, 0x00);
        }
    }
    return 0; // never reached
}
