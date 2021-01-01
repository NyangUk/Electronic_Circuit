/*
 * week4-1.c
 *
 * Created: 2020-10-03 오전 12:00:21
 * Author : lobgd
 */ 
#define F_CPU 16000000
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
unsigned char led_on;
ISR(INT3_vect){
	if (led_on) {
		PORTB=0b10000000; // LED OFF
		led_on = 0;
	}
	else {
		PORTB=0b00000000; // LED ON
		led_on = 1;
	}
}
int main(void){
	DDRB = 0b10000111; // 출력 설정
	DDRD = 0b00000000; // 입력 설정
	PORTD = 0b00001000; // 풀업저항 설정
	EICRA = 0b10000000; //인터럽트 트리거 방식 설정
	EIMSK = 0b00001000; //인터럽트 허용 설정
	SREG |= 0x80; //전체 인트럽트 허가
	led_on = 1;
	while(1) {
		PORTB=0b10000111; // LED OFF
		_delay_ms(2000);
		PORTB=0b00000000; // 모든 LED on
		_delay_ms(500);
	}
}
