// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Your name
// Last Modified: 12/31/2023

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#define ADCVREF_VDDA 0x000
#define ADCVREF_INT  0x200

unsigned short map[13][10]={
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},

};

struct shape{
    unsigned short coords[5][5];
    uint32_t xbound;
    uint32_t ybound;
    signed short xtiles[3];
    signed short ytiles[3];
};

typedef struct shape tile_t;

static tile_t shapesa[16] = {
    //L
  {{{0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 2, 1, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {0,0,1}, {-1,-2,0}},

    //T
  {{{0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 2, 0, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {-1,0,1}, {-1,-1,-1}},

    //I
  {{{0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 2, 0, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {0,0,0}, {-1,-2,-3}},

    //Z
  {{{0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 2, 0, 0, 0},
    {0, 0, 0, 0, 0},},
    3,1,
    {0,1,1}, {-1,-1,-2}},

    //L R1
  {{{0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 2, 1, 1, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},},
    2,1,
    {1,2,2}, {0,0,-1}},

    //T R1
  {{{0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 1, 0},
    {0, 0, 2, 0, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {0,0,1}, {-1,-2,-1}},

    //I R1
  {{{0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 2, 1, 1, 1},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},},
    2,1,
    {1,2,3}, {0,0,0}},

    //Z R1
  {{{0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 2, 1, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {-1,0,1}, {-1,-1,0}},

    //L R2
  {{{0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 2, 0, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {-1,0,0}, {-2,-2,-1}},

    //T R2
  {{{0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 2, 1, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {-1,0,1}, {0,-1,0}},

    //I R2
  {{{0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 2, 0, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {0,0,0}, {-1,-2,-3}},

    //Z R2
  {{{0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 2, 0, 0, 0},
    {0, 0, 0, 0, 0},},
    3,1,
    {0,1,1}, {-1,-1,-2}},

    //L R3
  {{{0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 2, 0, 0, 0},
    {0, 0, 0, 0, 0},},
    3,1,
    {0,1,2}, {-1,-1,-1}},

    //T R3
  {{{0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 2, 0, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {-1,0,0}, {-1,-1,-2}},

    //I R3
  {{{0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 2, 1, 1, 1},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},},
    2,1,
    {1,2,3}, {0,0,0},},

    //Z R3
  {{{0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 2, 1, 0},
    {0, 0, 0, 0, 0},},
    3,2,
    {-1,0,1}, {-1,-1,0}},
    };

uint32_t score=0;
uint8_t currshape;
uint32_t currx=3;
uint32_t curry=4;
uint32_t nextx=3;
uint32_t nexty=4;
uint32_t clrrowf;
uint32_t startgf;
uint32_t endgf;

uint32_t coly(void){
    uint32_t coll = 0;
    for(uint32_t i = 0; i<3; i++){
        if(map[nexty][currx] == 1){
            coll = 1;
        }
        else if(map[nexty+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] == 1){
            coll = 1;
        }
    }
    return coll;
}

uint32_t colx(void){
    uint32_t coll = 0;
    for(uint32_t i = 0; i<3; i++){
        if((nextx+shapesa[currshape].xtiles[i] >= 10) || (nextx+shapesa[currshape].xtiles[i] < 0)){
            coll = 1;
        }
        else if(map[curry][nextx] == 1){
            coll = 1;
        }
        else if(map[curry+(shapesa[currshape].ytiles[i])][nextx+(shapesa[currshape].xtiles[i])] == 1){
            coll = 1;
        }
    }
    return coll;
}

uint32_t clearrow(void){
    uint32_t ret = 0;
    uint32_t count = 0;
    uint32_t currow = 0;
    for(uint32_t i = 0; i<13; i++){
        uint32_t stat = 0;
        for(uint32_t s = 0; s<10; s++){
            if(map[i][s] == 0){
                stat = 1;
            }
        }
        if(stat == 0){
            ret = 1;
            currow = i;
            count++;
        }
    }
    if(count != 0){
        for(int i = currow; i>currow-count; i--){
            for(int s = 0; s<10; s++){
                map[i][s] = 0;
            }
        }
        for(int i=0; i<(currow-count)+1; i++){
            for(int s =0; s<10; s++){
                map[i+count][s] = map[i][s];
            }
        }
        clrrowf = 1;
        Sound_Start(7000);
        Clock_Delay(80000000);
        score += 500;
    }
    return ret;
}

void rotate(void){
    for(uint32_t i = 0; i<3; i++){
        map[curry][currx] = 0;
        map[curry+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] = 0;
    }
    uint8_t temp = currshape;
    currshape = (currshape + 4)%16;
    nextx = currx;
    nexty = curry;
    if(colx() != 1 && coly() !=1){
        for(uint32_t i = 0; i<3; i++){
            map[curry][currx] = 1;
            map[curry+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] = 1;
        }
    }
    else{
        currshape = temp;
        for(uint32_t i = 0; i<3; i++){
            map[curry][currx] = 1;
            map[curry+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] = 1;
        }
    }

}

void printsprite(uint32_t x, uint32_t y, uint32_t color, uint32_t clear){
    for(uint32_t i = 0; i<10; i++){
        for(uint32_t s = 0; s<10; s++){
            if((i == 0 || s == 0 || i == 9 || s == 9) && (clear == 0)){
                ST7735_DrawPixel(x*10 + i + 10, y*10 + s, ST7735_RED);
            }
            else{
                ST7735_DrawPixel(x*10 + i + 10, y*10 + s, color);
            }
        }
    }
}

void printmap(void){
    for(uint32_t i = 0; i<10; i++){
        for(int32_t s = 13; s>-1; s--){
            if(map[s][i] != 0){
                printsprite(i, s, ST7735_YELLOW, 0);
            }
            else{
                printsprite(i, s, ST7735_BLACK, 1);
            }
        }
    }
}

void forcedrop(void){
    uint32_t savey = curry;
    uint32_t savex = currx;
    for(uint32_t i = 13; i>curry; i--){
        nexty = i;
        nextx = currx;
        for(uint32_t s = 0; s<3; s++){
            map[curry][currx] = 0;
            map[curry+(shapesa[currshape].ytiles[s])][currx+(shapesa[currshape].xtiles[s])] = 0;
        }
        if(colx() != 1 && coly() != 1){
            curry = nexty;
            for(uint32_t s = 0; s<3; s++){
                map[curry][currx] = 1;
                map[curry+(shapesa[currshape].ytiles[s])][currx+(shapesa[currshape].xtiles[s])] = 1;
            }
        }
        else{
            for(uint32_t s = 0; s<3; s++){
                map[savey][savex] = 0;
                map[savey+(shapesa[currshape].ytiles[s])][savex+(shapesa[currshape].xtiles[s])] = 1;
            }
        }
    }

}

int endgamec(void){
    int gameover = 0;
    for(uint32_t i = 0; i<10; i++){
        if(map[0][i] == 1){
            gameover = 1;
        }
    }
    return gameover;
}
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}


uint32_t RetPos(uint32_t Position){
    if(Position <= 185){
        return 1;
    }
    if(Position > 185 && Position <= 371){
        return 2;
    }
    if(Position > 371 && Position <= 557){
        return 3;
    }
    if(Position > 557 && Position <= 743){
        return 4;
    }
    if(Position > 743 && Position <= 929){
        return 5;
    }
    if(Position > 929 && Position <= 1116){
        return 6;
    }
    if(Position > 1116 && Position <= 1303){
        return 7;
    }
    if(Position > 1303 && Position <= 1490){
        return 8;
    }
    if(Position > 1490 && Position <= 1676){
        return 9;
    }
    else{
        return 10;
    }
      // convert Data to Position
}

uint32_t newPos(uint32_t Pos){
    if(Pos < 1948){
        return -1;
    }
    if(Pos > 2148){
        return 1;
    }
    return 0;
}


uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
float FloatPosition;  // 32-bit floating-point cm
uint32_t Flag;        // semaphore
uint32_t startTime,stopTime;
uint32_t Offset,ADCtime,Converttime,FloatConverttime,OutFixtime,FloatOutFixtime; // in bus cycles
uint32_t Time;

uint32_t x;
uint32_t y;

void ADC_InitDual(ADC12_Regs *adc12, uint32_t channel1, uint32_t channel2, uint32_t reference){
    adc12->ULLMEM.GPRCM.RSTCTL= (uint32_t)0xB1000003;
    if(reference == ADCVREF_INT) {
        VREF->GPRCM.RSTCTL = (uint32_t)0xB1000003;
    }
    adc12->ULLMEM.GPRCM.PWREN = (uint32_t)0x26000001;
    if(reference == ADCVREF_INT){
        VREF->GPRCM.PWREN = (uint32_t) 0x26000001;
    }
    Clock_Delay (24);


    adc12->ULLMEM.GPRCM.CLKCFG= 0xA9000000; // ULPCLK
    adc12->ULLMEM.CLKFREQ = 7;
        // 40 to 48 MHz 52
    adc12->ULLMEM.CTL0 = 0x03010000;
    adc12->ULLMEM.CTL1 = 0x00010000;
    adc12->ULLMEM.CTL2 = 0x02010000;
    adc12->ULLMEM.MEMCTL[1] = reference + channel1;
    adc12->ULLMEM.MEMCTL[2] = reference + channel2;
    adc12->ULLMEM.SCOMP0 = 0; // 8 sample clocks 59
    adc12->ULLMEM.GEN_EVENT.IMASK = 0; // no interrupt
    if(reference == ADCVREF_INT) {
        VREF->CLKSEL = 0x00000008; // bus clock 61
        VREF->CLKDIV = 0; // divide by 1 62
        VREF->CTL0 = 0x0001;
        VREF->CTL2 = 0;
    while((VREF->CTL1&0x01)==0){};
 }
}

void ADC_InDual (ADC12_Regs *adc12, uint32_t d1,  uint32_t d2){
    adc12->ULLMEM.CTL0 |= 0x00000001; // enable conversions
    adc12->ULLMEM.CTL1 |= 0x00000100; // start ADC
    uint32_t volatile delay=adc12->ULLMEM.STATUS; // time to let ADC start
    while((adc12->ULLMEM.STATUS&0x01)==0x01){}; // wait for completion
        //test2 = adc12->ULLMEM.MEMRES[1];
        y = adc12->ULLMEM.MEMRES[1];
        x = 4095 - adc12->ULLMEM.MEMRES[2];

}
// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
      GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
      GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
      Time++;
      // sample 12-bit ADC0 channel 5, slidepot
      ADC_InDual(ADC0, x, y);
      // store data into mailbox
      // set the semaphore
      Flag = 1;
      GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= French; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  ST7735_OutString("Hola, c\xA2mo est\xA0s");
  Clock_Delay1ms(10000);
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  //ST7735_DrawBitmap(60, 90, tetI, 42, 12);
  uint16_t x = 1;
  uint16_t y = 2;
  for(int x1=0; x1<10; x1++){
          for(int y1=0; y1<10; y1++){
              if((x1 == 0 || y1 == 0 || x1 == 9 || y1 == 9)){
                  ST7735_DrawPixel(x*10+x1+10,y*10+y1,ST7735_MAGENTA);
              }
              else{
                  ST7735_DrawPixel(x*10+x1+10,y*10+y1,ST7735_BLUE);
              }
          }
      }
  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  uint32_t last = 0, now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  ST7735_FillScreen(0x0000);
  uint16_t x = 1;
  uint16_t y = 1;
  ST7735_SetCursor(x, y);
  while(1){
    // write code to test switches and LEDs
      now = Switch_In(); // Your Lab4 input
      if(now != last){ // change
          if(now == 1){
              ST7735_OutString("Right!");
              y++;
              ST7735_SetCursor(x, y);
          }
          if(now == 2){
              ST7735_OutString("Left!");
              y++;
              ST7735_SetCursor(x, y);
          }
          if(now == 4){
              ST7735_OutString("Up!");
              y++;
              ST7735_SetCursor(x, y);
          }
          if(now == 8){
              ST7735_OutString("Down!");
              y++;
              ST7735_SetCursor(x, y);
          }
      }
      last = now;
      Clock_Delay(800000); // 10ms, to debounce switch
    
  }
}
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
      now = Switch_In();

      if(last != 1){ // change
          if(now == 1){
              clrrowf = 1;
              Sound_Start(7000);
          }
      }
      if(last != 2){ // change
          if(now == 2){
              startgf = 1;
              Sound_Start(7000);
          }
      }
      if(last != 4){ // change
          if(now == 4){
              endgf = 1;
              Sound_Start(7000);
          }
      }
      last = now;
      Clock_Delay(800000); // 10ms, to debounce switch
   }


  }
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN

uint32_t languageselect(void){
    ST7735_FillScreen(0x0000);   // set screen to black
    ST7735_SetCursor(1, 1);
    ST7735_OutString("Select a language:");
    ST7735_SetCursor(1, 2);
    ST7735_OutString("English (Left)"); //2
    ST7735_SetCursor(1, 3);
    ST7735_OutString("Espa\xA4ol (Right)"); //1
    while(1){
        if(Switch_In() == 1){
            return 1; //span
        }
        if(Switch_In() == 2){
            return 2; //eng
        }
    }

}


int main(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ADCinit();     //PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  ADC_InitDual(ADC0, 0, 1, ADCVREF_VDDA);
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30,2);
  // initialize all data structures
  Flag = 0;
  uint32_t slide;
  uint32_t last = 0, now;
  __enable_irq();
  uint32_t language;
  language = languageselect();
  if(language == 1){
      ST7735_FillScreen(0x0000);   // set screen to black
      ST7735_SetCursor(1, 1);
      ST7735_OutString("Hola, presione");
      ST7735_SetCursor(1, 2);
      ST7735_OutString("'UP'");
      ST7735_SetCursor(1, 3);
      ST7735_OutString("para comenzar");
      ST7735_SetCursor(1, 5);
      ST7735_OutString("Mantenga presionado");
      ST7735_SetCursor(1, 6);
      ST7735_OutString("el bot\xA2n derecho");
      ST7735_SetCursor(1, 7);
      ST7735_OutString("para girar");
      ST7735_SetCursor(1, 9);
      ST7735_OutString("Mantenga presionado");
      ST7735_SetCursor(1, 10);
      ST7735_OutString("el bot\xA2n izquierdo");
      ST7735_SetCursor(1, 11);
      ST7735_OutString("para soltar");
  }
  else if(language == 2){
      ST7735_FillScreen(0x0000);   // set screen to black
      ST7735_SetCursor(1, 1);
      ST7735_OutString("Hello there, press");
      ST7735_SetCursor(1, 2);
      ST7735_OutString("'UP'");
      ST7735_SetCursor(1, 3);
      ST7735_OutString("to begin");
      ST7735_SetCursor(1, 5);
      ST7735_OutString("Hold right button");
      ST7735_SetCursor(1, 6);
      ST7735_OutString("to rotate");
      ST7735_SetCursor(1, 8);
      ST7735_OutString("Hold left button");
      ST7735_SetCursor(1, 9);
      ST7735_OutString("to drop");
  }
  while(Switch_In() != 4){}
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_FillRect(0,146,180,1,ST7735_BLUE);
  ST7735_SetCursor(0,15);
  if(language == 2){
      ST7735_OutString("Score = ");
  }
  if(language == 1){
      ST7735_OutString("Resultado = ");
  }
    Clock_Delay(80000000); // 100ms
    for(int i = 0; i<10; i++){
        for(int s = 13; s>-1; s--){
            map[s][i] = 0;
        }
    }
  currshape = (Random32()%15);
  startgf = 1;
  Sound_Start(7000);
  Clock_Delay(80000000); // 100ms
  while(1){
      __enable_irq();
      __disable_irq();
      printmap();
      if(language == 2){
          ST7735_SetCursor(8,15);
          printf("%d", score);
      }
      if(language == 1){
          ST7735_SetCursor(12,15);
          printf("%d", score);
      }
      if(curry<13){
          ADC_InDual(ADC0, x, y);
          nextx += newPos(x);
          for(uint32_t i = 0; i<3; i++){
              map[curry][currx] = 0;
              map[curry+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] = 0;
          }
          nexty++;
          if(coly() == 1 && colx() == 1){
              map[curry][currx] = 1;
              for(uint32_t i = 0; i<3; i++){
                  map[curry+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] = 1;
              }
              curry = 3;
              nexty = 3;
              nextx = 3;
              currx = 3;
              if(coly() == 1 || colx() == 1){
                  goto DONE;
              }
              score += 50;
              currshape = (Random32()%15);
          }
          else if(coly() == 1 && colx() != 1){
              map[curry][currx] = 1;
              for(uint32_t i = 0; i<3; i++){
                  map[curry+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] = 1;
              }
              curry = 3;
              nexty = 3;
              nextx = 3;
              currx = 3;
              if(coly() == 1 || colx() == 1){
                  goto DONE;
              }
              score += 50;
              currshape = (Random32()%15);
          }
          else if(coly() != 1 && colx() == 1){
              curry = nexty;
              currx = currx;
              nextx = currx;
              map[curry][currx] = 1;
              for(uint32_t i = 0; i<3; i++){
                  map[curry+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] = 1;
              }
          }
          else if(coly() != 1 && colx() != 1){
              curry++;
              currx = nextx;
              map[curry][currx] = 1;
              for(uint32_t i = 0; i<3; i++){
                  map[curry+(shapesa[currshape].ytiles[i])][currx+(shapesa[currshape].xtiles[i])] = 1;
              }
          }
      }
      else if(curry>=13){
          curry = 3;
          nexty = 3;
          nextx = 3;
          currx = 3;
          score += 50;
          currshape = (Random32()%15);
      }
      if(clearrow() == 1){
                printmap();
      }
      now = Switch_In();
      if(now != last){
          if(now == 1){
              rotate();
              int i = 4000000;
              while(i>0){
                  i--;
              }
          }
          if(now == 2){
              forcedrop();
              int i = 7000000;
              while(i>0){
                  i--;
              }
          }
      }
      last = now;
      if(endgamec() == 1){
          DONE:
          ST7735_FillScreen(0x0000);
          ST7735_SetCursor(0,0);
          if(language == 2){
              ST7735_OutString("Game Over");
              ST7735_SetCursor(0,1);
              printf("Score = %d", score);
          }
          if(language == 1){
              ST7735_OutString("Juego Terminado");
              ST7735_SetCursor(0,1);
              printf("Resultado = %d", score);
          }
          __enable_irq();
          endgf = 1;
          Sound_Start(7000);
          Clock_Delay(80000000); // 100ms
          __disable_irq();
          goto GAMEOVER;
      }
      __enable_irq();
  }
  GAMEOVER:
  for(int i = 0; i<10; i++){
      for(int s = 13; s>-1; s--){
          map[s][i] = 0;
      }
      score = 0;
      nexty = 4;
      nextx = 3;
  }
  main();
}
