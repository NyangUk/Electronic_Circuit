#define F_CPU 16000000
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include<avr/io.h>
#include<util/delay.h>

unsigned char led_on =1;
int i =0;
double interval =500.0;
unsigned char ledArray[8] = {0b10000000,
	0b01000000,
	0b00100000,
	0b00010000,
	0b00001000,
	0b00000100,
	0b00000010,
0b00000001};

void play(){
	if(led_on){
		PORTB =ledArray[i];
		_delay_ms(interval);
		PORTB =0b00000000;
		i++;
		if(i>8){
			i=0;
		}
	}
	else{
		PORTB =0b00000000;
	}
}
ISR(INT5_vect){
	interval+=200;
}
ISR(INT6_vect){
	if (interval>0){
		interval-=200;	
	}
}
ISR(INT7_vect){
	if (led_on) {	// LED OFF
		led_on = 0;
	}
	else {			// LED ON
		led_on = 1;
	}
}

int main(void){
	// led
	DDRB = 0b11111111; // 입출력 방향 설정 
	PORTB = 0b00000000;
	// 버튼
	DDRE = 0b00000000; // 입출력 방향 설정 인터럽트 사용
	PORTE = 0b11100000; // 풀업저항 설정
	EICRB = 0b10000000; //인터럽트 트리거 방식 설정
	EIMSK = 0b11100000; //인터럽트 허용 설정
	SREG |= 0x80; //전체 인트럽트 허가
	while(1)
	{	
		play();
	}
}