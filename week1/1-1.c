/*
 * week1-1.c
 *
 * Created: 2020-09-15 오후 2:15:19
 * Author : lobgd
 */ 

#define F_CPU 16000000
#include<avr/io.h>
#include<util/delay.h>
unsigned char led_on;

void PB_LEDOnOff(void) // LED ON/OFF
{
	if (led_on) {
		PORTB=0b10000000;// LED OFF
		led_on = 0;
	}
	else {
		PORTB=0b00000000;// LED ON
		led_on = 1;
	}
}

int main(void){
	unsigned char key;
	DDRB = 0b10000000; // 입출력 방향 설정
	DDRE = 0b00000000; // 입출력 방향 설정
	PORTE = 0b10000000; // 풀업저항 설정
	PORTB = 0b00000000;
	led_on = 1;
	while(1)
	{
		key = (PINE & 0b10000000); // 버튼 스위치 값을 읽기
		switch(key) // key와 값이 일치하면 해당 case 실행문을 실행
		{

			case 0b00000000: // 스위치가 눌렸음을 확인
			PB_LEDOnOff();
			_delay_ms(500); // 0.5초 시간 지연 발생
			break;
			default: // 변수와 값이 불일치하면 default문 실행
			break;
		}
	}
}
