/******************************************************************************
  OLED 모듈에 그림그리기(Ver.2.x)
  
  동작 : OLED모듈에 BMP파일을 출력한다.
  
  포트 연결 :
     1) Cortex-M3모듈의 포트A 0~7은 8핀 케이블로 OLED모듈의 D0~D7핀에 연결한다.
        PA0~7 -> D0~7
    2) Cortex-M3모듈의 포트C 0~4은 4핀케이블과 2핀 케이블로 
       OLED모듈의 CS,RST,DC,WN,E핀에 연결한다.
        PC0 -> CS
        PC1 -> RST
        PC2 -> DC
        PC3 -> WN
        PC4 -> E
        
  라이브러리 설명 :
    1)  initOLED()
        OLED 초기화 함수, 128x128 16비트로 설정한다.
    2)  void Draw_Rectanle(unsigned int outColor,unsigned int inColor)
        OLED에 inColor 내부 바탕색에, outColor색의 태두리색을가지는 사각형을
        그린다.
    3)  void OLED_DrawImage(const u16 *Image)
        Image배열에 저장된 RGB 데이타를 OLED에 출력한다.
        (한백전자의 마이크로 프로세서 다운로드 페이지의 bmp2c프로그램으로 변환된 
          이미지 파일)
  소스설명 :            
    1)  프로그램에 필요한 헤더파일을선언한다.
    2)  RGB타입으로 저장된 BMP파일의 배열의 시작주소를 Image로 복사한다.
    3)  IMG 배열에 저장된 RGB 데이타를 배열에 출력한다.
    4)  OLED를 초기화 한다.
    5)  빨간바탕의 녹색테두리의 사각형을 그린다.
    6)  사용자 이미지파일을 OLED에 출력한다.
******************************************************************************/
  //  1)
#include "stm32f10x_lib.h"
#include "System_func.h"
#include "oled.h"
#include "lstar.h" //사용자가 변환한 BMP 배열 
#include "user_delay.h"

  //  2)
const u16 *Image0 = lstar; //사용자가 만든 파일명이 exam.c, exam.h 인경우 
//옆의 lstar0, lstar1 대신 exam 로 바꾸게 되면 사용자가 만든 그림이 나오게 된다.

  //  3) 
void OLED_DrawImage(const u16 *IMG);

int main(void){

  Init_STM32F103();  

  Timer3_Delay_init();

  //  4)
  initOLED();
  //  5)  
  Draw_Rectanle(0,0,127,127,RGB565(0,255,0),RGB565(255,0,0));
  delay_ms(1000);
  
  //  6)
  OLED_DrawImage(Image0);
 
  while (1);
  
}//end main


void OLED_DrawImage(const u16 *IMG){
	
  u8 x,y,ye,xe; 	// x, y 값 저장 
  u8 width,height;	// 그림 높이, 넓이 

  u16 buf,i=0;

  width = 128; 
  height= 128;

  x = 0;
  xe = width;
  y = 0;
  ye = height; 	
        
  while(y<ye){
       
      while(x<xe){
          buf = (IMG[i]);
          i++;
          PutPixel(x++,ye-1,buf); 			
        }//end while x<xe
      
    x=0;
    ye--;
  }//end while  

}
