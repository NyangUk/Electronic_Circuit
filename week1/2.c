#define F_CPU 16000000
#define __DELAY_BACKWARD_COMPATIBLE__ 

#include<avr/io.h>
#include<util/delay.h>

unsigned char led_on;
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

void PB_LEDOnOff(void) // LED ON/OFF
{	if(led_on){
	PORTB =ledArray[i];
	_delay_ms(interval);
	PORTB =0b00000000;
	i++;
		if(i>8){
			i=0;	
		}
	}
	
}
void change(){
	if (led_on) {	// LED OFF
		led_on = 0;
	}
	else {			// LED ON
		led_on = 1;
	}
}
int main(void){
	unsigned char key;
	unsigned char up;
	unsigned char down;
	DDRB = 0b11111111; // 입출력 방향 설정
	DDRE = 0b00000000; // 입출력 방향 설정
	PORTE = 0b11100000; // 풀업저항 설정
	PORTB = 0b00000000;
	led_on = 1;
	
	while(1)
	{
		key = (PINE & 0b10000000); // 버튼 스위치 값을 읽기
		up =(PINE & 0b01000000); 
		down =(PINE & 0b00100000); 
		
		PB_LEDOnOff();
		
		if(key==0b00000000){
			change();
		}
		if(up==0b00000000){
			interval+=500;
		}
		if (down==0b00000000){
			if(interval>0){
				interval-=500;	
			}
		}
	}
}
