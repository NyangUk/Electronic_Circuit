/*
 * week5-2.c
 *
 * Created: 2020-10-11 오후 9:09:35
 * Author : lobgd
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int count =0;
volatile int flag =0;
volatile int count_time =0;
volatile char str[1][30] = {{"stop watch"}}; 
volatile char ch_num[1][30] = {{"00:00:00"}}; 
volatile char reset[1][30] = {{"00:00:00"}}; 
ISR(TIMER2_OVF_vect){
	if(flag){
		count++;
		if (count == 60) { //1초
			count_time+=1;
			set();
			count =0;
		}
	}
}
void set(){
	hour_change((count_time/3600));
	min_change(count_time/60);
	sec_change(count_time%60);
}
ISR(INT5_vect){ //시작
	flag =1;
}
ISR(INT6_vect){ //정지
	flag=0;
}
ISR(INT7_vect){ //
	count_time=0;
	set();
}
void hour_change(char num){
	ch_num[0][0] = num/10+'0';
	ch_num[0][1] = num%10+'0';
}
void min_change(int num){
	ch_num[0][3] = num/10+'0';
	ch_num[0][4] = num%10+'0';
}
void sec_change(int num){
	ch_num[0][6] = num/10+'0';
	ch_num[0][7] = num%10+'0';
}

void cmd(int command){
	PORTB = command;
	PORTD = 0x04; // 데이터 통신 enable, lcd 위치 설정 명령
	_delay_ms(1);
	PORTD = 0x00; // 데이터 통신 disable
}
void data(char str){
	PORTB = str;
	PORTD = 0x05; // 데이터 통신 enable, 레지스터에 저장해놨던 위치에 값을 쓰는 명령
	_delay_ms(1);
	PORTD = 0x01; // 데이터 통신 disable
}
void display1(char*str){
	cmd(0x80); // lcd 첫 위치 (0,0)
	int i = 0;
	while (str[i]!='\0')
	{	
		data(str[i++]);
	}
}
void display2(char*str){
	cmd(0x80 | 0x40); // lcd 첫 위치 (0,0)
	int i = 0;
	while (str[i]!='\0')
	{
		data(str[i++]);
	}
}

int main(){
	DDRD=0xff;
	DDRB=0xff;
	DDRE=0x00;
	PORTE=0b11100000;
	cmd(0x38); // Set 8bit 2Line 5x7 dots
	cmd(0x01); // Display clear
	cmd(0x80); // Set DDRAM address or cursor position on display
	cmd(0x0c); // Display on Cursor Off
	cmd(0x06); // Entry Mode
	// 인터럽트
	EICRB = 0b10000000; //인터럽트 트리거 방식 설정
	EIMSK = 0b11100000; //인터럽트 허용 설정
	//타이머/카운터
	TCCR2 = 0b00000101 ; //프리스케일러 분주비 1024
	TIMSK = 0b01000000 ; //오버플로우 인터럽트 허용
	TCNT2 = 0 ; //타이머카운터0 초기화
	SREG = 0x80 ;
	
	display1(str[0]);
	while(1){
		display2(ch_num[0]);
	}
}
