/*
 * week3-3.c
 *
 * Created: 2020-10-02 오후 10:03:16
 * Author : lobgd
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
volatile int count =1;
volatile int row =0;
volatile int column =0;
volatile char str[10][40] = {{"Hels forgotten all about the people in c"},
	{"locks until he passed a group of them ne"},{"xt to the bakerls. He eyed them angrily "},
	{"as he passed. He didlt know why. but th"},{"Hels forgotten all about the people in c"},
	{"locks until he passed a group of them ne"},{"xt to the bakerls. He eyed them angrily "},
	{"as he passed. He didlt know why. but th"},{"Hels forgotten all about the people in c"},
	{"locks until he passed a group of them ne"}}; // Initial LCD display

void cmd(int command){
	PORTE = command;
	PORTB = 0x04; // 데이터 통신 enable, lcd 위치 설정 명령
	_delay_ms(1);
	PORTB = 0x00; // 데이터 통신 disable
}
void data(char str){
	PORTE = str;
	PORTB = 0x05; // 데이터 통신 enable, 레지스터에 저장해놨던 위치에 값을 쓰는 명령
	_delay_ms(1);
	PORTB = 0x01; // 데이터 통신 disable
}
void display1(char*str){
	cmd(0x80); // lcd 첫 위치 (0,0)
	int i = column;
	while (1)
	{
		data(str[i++]);
		count++;
		if((count%17)==0){ //lcd 1 line에 문자가 꽉차면 line의 처음으로 돌아감
			count=0;
			break;
		}
	}
}
void display2(char*str){
	cmd(0x80 | 0x40); //두번째줄 시작
	int i=column;
	while (1)
	{
		data(str[i++]);
		count++;
		if((count%17)==0){ //lcd 1 line에 문자가 꽉차면 line의 처음으로 돌아감
			count=0;
			break;
		}
	}
}
int main(){
	unsigned char up;
	unsigned char down;
	unsigned char right;
	unsigned char left;
	DDRB=0xff;
	DDRE=0xff;
	DDRD=0b00000000; //하우좌상(4321)
	PORTD=0x00; //풀업저항 설정
	
	cmd(0x38); // Set 8bit 2Line 5x7 dots
	cmd(0x01); // Display clear
	cmd(0x80); // Set DDRAM address or cursor position on display
	cmd(0x0c); // Display on Cursor Off
	cmd(0x06); // Entry Mode
	while(1){
		up =(PIND&0b0000001);
		left =(PIND&0b00000010);
		right =(PIND&0b00000100);
		down =(PIND&0b00001000);
		if((up==0b00000001) && (row>0)){
			row-=1;
		}
		if((down==0b00001000) && (row<10)){
			row+=1;
		}
		if((left==0b00000010) && (column>0)){
			column-=1;
		}
		if((right==0b00000100) && (column<31)){
			column+=1;
		}
		display1(str[row]);
		display2(str[row+1]);
		_delay_ms(1000);
	}
}


