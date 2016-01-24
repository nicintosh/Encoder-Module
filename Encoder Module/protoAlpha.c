//
//  protoAlpha.c
//  Encoder Module
//
//  Created by Nicholas Burgel on 2016-01-23.
//  Copyright © 2016 JIN Advanced Systems. All rights reserved.
//

//Macros
#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (0x01 << (m)))
#define bit_clear(p,m) ((p) &= ~(0x01 << (m)))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

//Stepper Driver
#define STEPPER PORTD
#define SD_EN PORTD0
#define SD_MS1 PORTD1
#define SD_MS2 PORTD2
#define SD_MS3 PORTD3
#define SD_STEP PORTD4
#define SD_DIR PORTD5

//Encoder
#define ENCODER PORTC
#define ENC_SEL1 PORTC0
#define ENC_SEL2 PORTC1
#define ENC_OE PORTC2
#define ENC_RST PORTC3

//Data IN
#define DATA PORTA
#define D0 PORTA0
#define D1 PORTA1
#define D2 PORTA2
#define D3 PORTA3
#define D4 PORTA4
#define D5 PORTA5
#define D6 PORTA6
#define D7 PORTA7

//PORT B
#define IO1 PORTB0
#define CLK PORTB1
#define STEP_IN PORTB2
#define DIR_IN PORTB3
#define MOSI PORTB4
#define MISO PORTB5
#define SCK PORTB6
#define RESET PORTB7
