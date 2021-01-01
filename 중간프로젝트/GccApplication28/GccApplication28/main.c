#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define delay 150
int index =15;
int dd[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //lcd에 표시될 자리의 수를 설정하기 위함
volatile char str1[1][16] = {{"000000000000000"}}; // 자리마다 디스플레이되는 초기값
volatile char str2[1][16] = {{"good       "}}; // 문제가 없을 때 출력되는 안내문구
volatile char str3[1][16] = {{"bad        "}};  // 문제가 있을 때 출력되는 안내문구
volatile char str4[1][16] = {{"be returned"}};  // 자리 반납 성공시 출력되는 안내문구



void cmd(int command){
	PORTF = command;
	PORTB = 0x04; // 데이터 통신 enable, lcd 위치 설정 명령
	_delay_ms(1);
	PORTB = 0x00; // 데이터 통신 disable
}
void data(char str){
	PORTF = str;
	PORTB = 0x05; // 데이터 통신 enable, 레지스터에 저장해놨던 위치에 값을 쓰는 명령
	_delay_ms(1);
	PORTB = 0x01; // 데이터 통신 disable
}
void display2(char*str){
	cmd(0x80|0x40); // lcd 첫 위치 (1,0)
	int i = 0;
	while (str[i] != '\0' )
	{
		data(str[i++]);
	}
	
}
void display1(char*str){
	cmd(0x80); // lcd 첫 위치 (0,0)
	int i = 0;
	while (str[i] != '\0' )
	{
		data(str[i++]);
	}
	display2(str1[0]);
}

ISR(INT0_vect){ //좌측으로 이동 버튼
	cmd(0x10);// 커서를 왼쪽으로 움직임
	index--;
	_delay_ms(delay);
}

ISR(INT1_vect){//우측으로 이동 버튼
	cmd(0x14); //커서를 오른쪽으로 움직임
	index++;
	_delay_ms(delay);
}


ISR(INT2_vect){//선택
	if(str1[0][index]=='0' && dd[index]==0){ //해당 값이 0이고 lcd에서 선택된 출력값도 0이면 실행
		str1[0][index] ='1'; // lcd에 디스플레이 되는 값을 1로 변경
		dd[index-1] -= 1;
		dd[index] -= 1;
		dd[index+1] -= 1;
		display1(str2[0]); //굿
		PORTE=0b00000010; //초록불
	}
	else{
		display1(str3[0]); //이미 선택된 자리 거나 그 주변에 앉지 못할때 bad출력

		PORTE=0b00000001; //빨간불
	}
	_delay_ms(delay);
	index =15;  // 바뀐 자리 번호때문에 다시 lcd에 출력하게 되면 커서가 리셋됨으로 index도 리셋
}



ISR(INT3_vect){// 반납
	if(str1[0][index]=='1'&& dd[index]==-1){//해당 값이 -1이고 lcd에 선택된 출력값도 1이면 실행
		str1[0][index] ='0';
		dd[index-1] += 1;
		dd[index] += 1;
		dd[index+1] += 1;
		display1(str4[0]);
		PORTE=0b00000010; //초록불
		_delay_ms(delay);
		index=15;
	}
	else{ display1(str3[0]); // 반납이 불가능한 자리 선택 시
		PORTE=0b00000001; //빨간불
		_delay_ms(delay);
		index=15; // 바뀐 자리 번호때문에 다시 lcd에 출력하게 되면 커서가 리셋됨으로 index도 리셋
	}
}




int main(){
	DDRB=0xff;
	DDRF=0xff;
	DDRD=0b00000000;
	DDRE=0xff;
	PORTD =0xff;
	

	
	EICRA = 0b10000000; //인터럽트 트리거 방식 설정
	EIMSK = 0b00001111; //인터럽트 허용 설정
	SREG |= 0x80; //전체 인트럽트 허가

	
	cmd(0x38); // 8bit 데이터 버스 사용
	cmd(0x0D); // Display clear
	cmd(0x0e); // Display on Cursor on
	cmd(0x06); // Entry Mode
	display1(str2[0]);
	while(1){
		
	}
}
