/*
 * final_right.c
 *
 * Created: 2020-12-18 오전 8:43:44
 * Author : lobgd
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// Normal Mode
unsigned int count =0;
unsigned char hour=0,min=0 ,sec=0;
unsigned int on =1;

ISR(TIMER2_OVF_vect){
	if(on){
		count++;
		if(count==60){
			sec++;
			count=0;
			if(sec==60){
				min++;
				sec=0;
				if(min==60){
					hour++;
					min=0;
					
				}
			}
		}
	}
	
	
}





void si(unsigned char hour)
{
	static unsigned char hour2[] = "000";
	hour2[0]='0'+(hour/10);
	hour2[1]='0'+(hour%10);
	hour2[2]=':';
	display4(hour2);
}
void boon(unsigned char min)
{
	static unsigned char min2[] = "000";
	min2[0] ='0'+(min/10);
	min2[1] ='0'+(min%10);
	min2[2] =':';
	display3(min2); //
}
void cho(unsigned char sec)
{
	static unsigned char sec2[] = "00";
	sec2[0]='0'+(sec/10);
	sec2[1]='0'+(sec%10);
	display2(sec2);
}

void cmd (int command){
	PORTE = command;
	PORTB = 0x04;
	_delay_ms(1);
	PORTB = 0x00;
}

void data (char str){
	PORTE = str;
	PORTB = 0x05;
	_delay_ms(1);
	PORTB = 0x01;
}

void display1 (char*str){
	cmd(0x80);
	int i= 0;
	while(str[i] != '\0')
	{
		data(str[i++]);
	}
}

void display2 (char*str){
	cmd(0xc6);
	int i= 0;
	while(str[i] != '\0')
	{
		data(str[i++]);
	}
}
void display3 (char*str){
	cmd(0xc3);
	int i= 0;
	while(str[i] != '\0')
	{
		data(str[i++]);
	}
}
void display4 (char*str){
	cmd(0xc0);
	int i= 0;
	while(str[i] != '\0')
	{
		data(str[i++]);
	}
}




volatile char str[1][12] = {{"usage time"}};

int main()
{
	DDRE=0xff;
	DDRB=0xff;
	DDRF = 0b10000000;
	
	EICRA = 0b00101010;
	EIMSK = 0b00000111;
	


	cmd(0x38); // Set 8bit 2Line 5x7 dots
	cmd(0x01); // Display clear
	cmd(0x80); // Set DDRAM address or cursor position on display
	cmd(0x0c); // Display on Cursor Off
	cmd(0x06); // Entry Mode
	
	
	TCCR2 = 0b00000101 ; //프리스케일러 분주비 1024
	TIMSK = 0b01000000 ; //오버플로우 인터럽트 허용
	TCNT2 = 0; //타이머카운터0 초기화
	SREG= 0x80 ; //전체 인터럽트 허가
	
	display1(str[0]);
	
	while(1){
		si(hour);
		boon(min);
		cho(sec);
		
		
	}
	

	return 0;

}
