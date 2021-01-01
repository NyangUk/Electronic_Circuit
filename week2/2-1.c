/*
 * week2-1.c
 *
 * Created: 2020-09-18 오전 11:52:59
 * Author : lobgd
 */ 
// USART0, fosc=14.7456MHz, 9600bps
// 패리티 없음, 데이터 8비트, 정지비트 1비트
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
volatile unsigned char ch ;

ISR(USART0_RX_vect){ //수신이 완료되었을때 인터럽트의 함수
	ch = UDR0 ; ///usart0를 통해 데이터 읽기
	PORTB = ch ;
}
int main(void)
{
	DDRB = 0xff ;
	UCSR0A = 0x00;
	UCSR0B = 0b10011000; // RXCIE0=1, TXEN0=1, RXEN0=1 ==> 수신완료 인터럽트 허가, 수신허가.송신허가
	UCSR0C = 0b10000110; // 비동기 통신, 패리티 없음, ==>고정
	// 데이터 비트 : 8비트, 정지비트 : 1비트
	UBRR0H = 0; //보오 레이트 레지스터
	UBRR0L = 103; //보오 레이트 레지스터, fosc=14.7456MHz, BAUD=9600bps,
	SREG = 0x80;
	while(1);
}
