
/*
 * BumbasiRylan_Lab03.c
 *
 * Created: 9/17/2021 1:28:31 PM
 * Author : Rylan Bumbasi
 * RedID: 822563190
 */ 



void transmitNote(int row, int column);
void scanKeypad();


#include <avr/io.h>


unsigned char keypadNotes[4][4] = {
	{141, 133, 126, 117},
	{112, 105, 99, 94},
	{88, 83, 79, 75},
	{70, 66, 63, 59}
};


int main(void)
{
	// Initializing Columns and Rows
	DDRD |= (1<<4|1<<5|1<<6|1<<7); // Set rows as output (PORT D 4 - 7)
	DDRB  &= ~(1<<0|1<<1|1<<2|1<<3); // Set columns as Inputs
	PORTD |= (1<<4|1<<5|1<<6|1<<7); // Set rows as to high
	PORTB |= (1<<0|1<<1|1<<2|1<<3); // Set Columns to high (to enable pull up resistors)
	
	DDRD |= (1<<3); // Set PD3 to output (For PWM generation from timer)
	
	TCCR2A |= (1 << COM2B1); // Set OC2B for Non-Inverting Mode 
	
	TCCR2A |= (1 << WGM21) | (1 << WGM20); // Set timer 2 to Fast PWM Mode (top = OCR2A)
	TCCR2B |= (1 << WGM22);
	
	
	
    
	/* Replace with your application code */
    while (1)
    {
		scanKeypad();
    }
}



void scanKeypad()	{
	for(int i = 0; i <= 3; i++) { // index through matrix rows
		PORTD &= ~(1 << (i + 4)); // set the current row value to 0
		for(int j = 0; j <= 3; j++) { // index through matrix columns
			if( !(PINB & (1<<j))) // check to see if current column and row are both 0
			{
				transmitNote(i, j);
				while(!(PINB & (1<<j)));
				TCCR2B &= ~(1 << CS22);
				TCCR2B &= ~(1 << CS21); // Turn Timer off
			}

		}
		PORTD |= (1 << (i + 4)); // set the current row value back to 1
		
	}
}

void transmitNote(int row, int column) {
	OCR2A = keypadNotes[row][column];
	OCR2B = OCR2A / 2;
	TCCR2B |= (1 << CS22) | (1 << CS21); // Turn timer on (Pre-Scale 256);
	
}

