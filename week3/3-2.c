/*
 * week3-2.c
 *
 * Created: 2020-10-02 오후 9:09:35
 * Author : lobgd
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
volatile int count =0;
volatile char str[1][30] = {{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"}}; // Initial LCD display

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
	while (1)
	{	
		if(count==16){ //lcd 1 line에 문자가 꽉차면 line의 처음으로 돌아감
			cmd(0x80);
			count=0;
		}
		if(i==26){ //모든 알파벳이 출력되면 다시 처음부터 출력
			i=0;
		}
		data(str[i++]);
		_delay_ms(300);

		count++;
	}
}

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
