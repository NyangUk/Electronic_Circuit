/*
 * week3-1.c
 *
 * Created: 2020-10-02 오후 8:48:17
 * Author : lobgd
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
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
	int i = 0;
	while (str[i] != '\0' )
	{
		data(str[i++]);
		_delay_ms(1000);
	}
}

volatile char str[1][16] = {{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"}}; // Initial LCD display
int main(){
	DDRB=0xff;
	DDRE=0xff;
	cmd(0x38); // Set 8bit 2Line 5x7 dots
	cmd(0x01); // Display clear
	cmd(0x80); // Set DDRAM address or cursor position on display
	cmd(0x0c); // Display on Cursor Off
	cmd(0x06); // Entry Mode
	display1(str[0]);
	while(1){
	}
}