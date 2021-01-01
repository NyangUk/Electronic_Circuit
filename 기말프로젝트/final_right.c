/*
 * final.c
 *
 * Created: 2020-12-17 오전 4:44:41
 * Author : lobgd
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define delay 150
int index =15;
int dd[2][16]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}; //lcd에 표시될 자리의 수를 설정하기 위함
volatile char str1[2][16] = {{"000000000000000"},{"000000000000000"}}; // 자리마다 디스플레이되는 초기값
volatile char str6[2][16] = {{"Woman          "},{"man            "}};  // 자리 반납 성공시 출력되는 안내문구
volatile char str2[1][16] = {{"     good      "}}; // 문제가 없을 때 출력되는 안내문구
volatile char str3[1][16] = {{"     bad       "}};  // 문제가 있을 때 출력되는 안내문구
volatile char str4[1][16] = {{"  be returned  "}};  // 자리 반납 성공시 출력되는 안내문구

volatile char str7[2][16] = {{" Please choose "},{"  your gender  "}};  // 자리 반납 성공시 출력되는 안내문구
int mode =1 ;
int gender = 0; //여성이 0 남성이 1
   

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
void display1(char*str){
   cmd(0x80); // lcd 첫 위치 (0,0)
   int i = 0;
   while (str[i] != '\0' )
   {
      data(str[i++]);
   }
   display2(str1[gender]);
}
void display2(char*str){
   cmd(0x80|0x40); // lcd 첫 위치 (1,0)
   int i = 0;
   while (str[i] != '\0' )
   {
      data(str[i++]);
   }
   
}


void selectGenderPage1(char*str){
   if (mode==1){
      cmd(0x80); // lcd 첫 위치 (0,0)
      int i = 0;
      while (str[i] != '\0' )
      {
         data(str[i++]);
      }
      display2(str7[1]);
   }
   else{
      display1(str1[0]);
   }
}
void selectGenderPage2(char*str){
   cmd(0x80|0x40); // lcd 첫 위치 (1,0)
   int i = 0;
   while (str[i] != '\0' )
   {
      data(str[i++]);
   }
   
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
   if(str1[gender][index]=='0' && dd[gender][index]==0){ //해당 값이 0이고 lcd에서 선택된 출력값도 0이면 실행
      str1[gender][index] ='1'; // lcd에 디스플레이 되는 값을 1로 변경
      dd[gender][index-1] -= 1;
      dd[gender][index] -= 1;
      dd[gender][index+1] -= 1;
      PORTE=0b00000010; //초록불
	  display1(str2[0]);
      //딩동댕
      //도
      PORTG = 0b10000;
      _delay_us(1908*4);
      PORTG = 0b00000;
      _delay_us(1908*4);
      // 미
      PORTG = 0b10000;
      _delay_us(1515*4);
      PORTG = 0b00000;
      _delay_us(1515*4);
      //솔
      PORTG = 0b10000;
      _delay_us(1275*4);
      PORTG = 0b00000;
      _delay_us(1275*4);

   }
   else{
      display1(str3[0]); //이미 선택된 자리 거나 그 주변에 앉지 못할때 bad출력

      PORTE=0b00000001; //빨간불
     //땡
     PORTG = 0b10000;
     _delay_us(954*4);
     PORTG = 0b00000;
     _delay_us(954*4);

   }
   _delay_ms(delay);
   index =15;  // 바뀐 자리 번호때문에 다시 lcd에 출력하게 되면 커서가 리셋됨으로 index도 리셋
}
ISR(INT4_vect){ // 여성
   gender =0;
   mode =2;
   _delay_ms(delay);
   display1(str6[gender]);
}

ISR(INT5_vect){ // 남성
   gender =1;
   mode =2;
   _delay_ms(delay);
   display1(str6[gender]);
}

void returnSit(){
   if(str1[gender][index]=='1'&& dd[gender][index]==-1){//해당 값이 -1이고 lcd에 선택된 출력값도 1이면 실행
      str1[gender][index] ='0';
      dd[gender][index-1] += 1;
      dd[gender][index] += 1;
      dd[gender][index+1] += 1;
      display1(str4[0]);
      PORTE=0b00000010; //초록불
      _delay_ms(delay);
     //딩동댕
     //도
     PORTG = 0b10000;
     _delay_us(1908*4);
     PORTG = 0b00000;
     _delay_us(1908*4);
     // 미
     PORTG = 0b10000;
     _delay_us(1515*4);
     PORTG = 0b00000;
     _delay_us(1515*4);
     //솔
     PORTG = 0b10000;
     _delay_us(1275*4);
     PORTG = 0b00000;
     _delay_us(1275*4);

      index=15;
   }
   else{ // 반납이 불가능한 자리 선택 시
      PORTE=0b00000001; //빨간불
	  display1(str3[0]);
      _delay_ms(delay);
     //땡
     PORTG = 0b10000;
     _delay_us(954*4);
     PORTG = 0b00000;
     _delay_us(954*4);

      index=15; // 바뀐 자리 번호때문에 다시 lcd에 출력하게 되면 커서가 리셋됨으로 index도 리셋
   }
}
ISR(INT3_vect){// 반납
   returnSit();
}




int main(){
   DDRB=0xff;
   DDRF=0xff;
   DDRD=0b00000000;
   DDRE=0b00000011;
   PORTD =0xff;
   DDRG=0xff;

   
   EICRA = 0b10000000; //인터럽트 트리거 방식 설정
   EIMSK = 0b00111111; //인터럽트 허용 설정
   SREG |= 0x80; //전체 인트럽트 허가

   
   cmd(0x38); // 8bit 데이터 버스 사용
   cmd(0x0D); // Display clear
   cmd(0x0e); // Display on Cursor on
   cmd(0x06); // Entry Mode

   selectGenderPage1(str7[0]);
   while(1){

   }
}