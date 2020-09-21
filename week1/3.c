/*
 * week1-3.c
 *
 * Created: 2020-09-15 오후 2:44:36
 * Author : lobgd
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include<util/delay.h>

int num =0;
unsigned char segment[16] ={0b00111111,0b00000110,0b01011011,0b01001111,
							0b01100110,0b01101101,0b01111101,0b00100111,
							0b01111111,0b01100111,0b01110111,0b01111100,
							0b01011000,0b01011110,0b01111001,0b01110001};
int main(void)
{
   DDRB = 0b01111111;
   DDRE = 0b00000000;
   PORTB =0b00000000;
   PORTE = 0b11110000;
   
    while (1) 
    {	
		unsigned char first;
		unsigned char seconde;
		unsigned char third;
		unsigned char forth;
		first = (PINE & 0b00010000);
		seconde= (PINE & 0b00100000);
		third = (PINE & 0b01000000);
		forth = (PINE & 0b10000000);
		// 버튼이 닫히면 1 스위치가 열리면 0 
		if(first == 0b00000000){ //닫히면
			num+=1;
		}
		if(seconde == 0b00000000){ //닫히면
			num+=2;
		}
		if(third == 0b00000000){ //닫히면
			num+=4;
		}
		if(forth == 0b00000000){ //닫히면
			num+=8;
		}
		PORTB = segment[num];
		_delay_ms(500);
		num =0;
}

}
