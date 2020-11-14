#include <EEPROM.h>
#include <FastLED.h>  //ws2812函式庫
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <avr/eeprom.h>
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <util/delay.h>     /* for _delay_ms() */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */
#include "io.c"             /* 所有IO接腳的設定 */
#include "SCoop.h"
#define LED_PIN 5    //ws2812控制角
#define NUM_LEDS 8   //ws2812燈數
defineTask(IOProcess)
defineTask(timer)
volatile long count;
CRGB leds[NUM_LEDS];  //建立一組8個位置的陣列ws2812
byte LEDpin = 13;
////////////////////////////////////////////////////////////////////////////////////
int nowstatusbyte = 0;
int latchPin = 10;       // 接 74HC595 的 ST_CP (pin 10,latch pin)
int clockPin = 11;       // 接 74HC595 的 SH_CP (pin 11, clock pin)
int dataPin = 9;         // 接 74HC595 的 DS (pin 9)
int total = 0;
int L1 = 0;
int L2 = 0;
int L3 = 0;
int L4 = 0;
int L5 = 0;
int L6 = 0;
int L7 = 0;
int L8 = 0;
////////////////////////////////////////////////////////////////////////////////////
int z = 0;
char ws_color;
int ws_pin = 0;
int bcr = 0, bcg = 0, bcb = 0;
int bcrd = 0, bcgd = 0, bcbd = 0;
int R = 0, G = 0, B = 0;
char str_in;          //建立一個字元型態變數，用來放收到的數據
int i = 0;
int a = 0;
int aa = 0;
int j = 0;
int jj = 0;
int t = 0;
const int ledf[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; //LED亮左至右
const int ledr[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //LED亮右至左
const int led2[] = {7, 6, 13, 12, 11, 10, 9, 8}; //針對8隻腳個別控制
unsigned int b;
int potpin = A5; //可變電阻控制
int val = 0;     //可變電阻接收的值
int dt = 0;      //值轉換0~255
int inPin1 = A0; //按鈕腳位
int inPin2 = A1;
int inPin3 = A2;
int inPin4 = A3;
int val1 = 0;   //存放按鈕讀取的變數
int val2 = 0;
int val3 = 0;
int val4 = 0;
unsigned long time_now = 0;
int times = 0;
int passwd = 0;
int opqr = 0;
void outputbyte(uchar b)  //針對匯流排一次輸出////////////////////////////////////////////
{
  // 送資料前要先把 latchPin 拉成低電位
  digitalWrite(latchPin, LOW);
  // 使用 shiftOut 函式送出資料
  shiftOut(dataPin, clockPin, MSBFIRST, b);
  // 送完資料後要把 latchPin 拉回成高電位
  digitalWrite(latchPin, HIGH);
  opqr = b;
}/////////////////////////////////////////////////////////////////////////////////////
void empty244()   //244的清零函數
{
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}
void emptyws2812() {
  for (int ws = 0; ws < 8; ws++) {
    leds[ws] = CRGB(0, 0, 0);
    FastLED.show();
  }
  bcr = 0;
  bcg = 0;
  bcb = 0;
}
void empty7seg() {
  a = 0;
  i = 0;
  digitalWrite(A4, HIGH);
  outputbyte(0);
  digitalWrite(A4, LOW);
}
/*
  void Breathe_LED() {
  while (1) {
    for (int bright = 255; bright >= 0; bright -= 25) {
      for (int i = 0; i < 8; i++)
      {
        leds[i] = CRGB(bcr, bcg, bcb);
        leds[i].fadeToBlackBy(bright);
        FastLED.show();
      }
      val = analogRead(potpin);
      dt = map(val, 0, 1023, 0, 25);
      sleep(dt);
    }
    for (int bright = 0; bright <= 255; bright += 25) {
      for (int i = 0; i < 8; i++)
      {
        leds[i] = CRGB(bcr, bcg, bcb);
        leds[i].fadeToBlackBy(bright);
        FastLED.show();
      }
      val = analogRead(potpin);
      dt = map(val, 0, 1023, 0, 25);
      sleep(dt);
    }
    if (Serial.available() > 0) {
      emptyws2812();
      break;
    }
  }
  }*/

int bright = 255;
int increment = 25;
void Breathe_LED()
{
  val = analogRead(potpin);
  dt = map(val, 0, 1023, 0, 25);
  sleep(dt);
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(bcr, bcg, bcb);
    leds[i].fadeToBlackBy(bright);
    FastLED.show();
  }
  if ((bright + 25) > 255 || (bright - 25) < 0)
  {
    increment = -increment;
  }
  bright += increment;

}
void ws2812() {//ws2812各顆亮燈函數
  while (t == 0) {
    emptyws2812();
    break;
  }
  ws_pin = ws_pin -= 1;
  if (ws_color == 'R') {
    leds[ws_pin] = CRGB(255, 0, 0);
    FastLED.show();
  }
  else if (ws_color == 'O') {
    leds[ws_pin] = CRGB(255, 128, 0);
    FastLED.show();
  }
  else if (ws_color == 'Y') {
    leds[ws_pin] = CRGB(255, 200, 20);
    FastLED.show();
  }
  else if (ws_color == 'G') {
    leds[ws_pin] = CRGB(0, 255, 0);
    FastLED.show();
  }
  else if (ws_color == 'B') {
    leds[ws_pin] = CRGB(0, 0, 255);
    FastLED.show();
  }
  else if (ws_color == 'I') {
    leds[ws_pin] = CRGB(111, 0, 255);
    FastLED.show();
  }
  else if (ws_color == 'P') {
    leds[ws_pin] = CRGB(160, 32, 240);
    FastLED.show();
  }
  else if (ws_color == 'W') {
    leds[ws_pin] = CRGB(255, 255, 255);
    FastLED.show();
  }
}
int job = 0;
void jobnumber() {
  empty7seg();
  job = 26;              //在這裡設定崗位號碼
  int t = 0;
  int dis = 0;
  job = EEPROM.read(passwd);   //密碼功能
  digitalWrite(3, HIGH); //設高電位關閉74LS244
  digitalWrite(A4, HIGH); //設低電位於74LS273
  sleep(10);
  do {
    job -= 10;
    t += 1;
  }
  while (job > 10);
  dis = t * 16 + job;
  //Serial.println(dis);
  outputbyte(dis);
  digitalWrite(3, HIGH); //設高電位關閉74LS244
  digitalWrite(A4, LOW); //74LS273輸出資料，正緣觸發

}
bool isTime;
int tIndex;
int tTemps;
bool isPwd;
int pIndex;
int pTemps;
void setup()
{
  //////////////////////////////////////////////////////////////////////
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //////////////////////////////////////////////////////////////////////
  pinMode(LEDpin, OUTPUT);
  pinMode(potpin, INPUT);
  pinMode(inPin1, INPUT_PULLUP);  //設按鈕為輸入,PULLUP為上拉電阻
  pinMode(inPin2, INPUT_PULLUP);
  pinMode(inPin3, INPUT_PULLUP);
  pinMode(inPin4, INPUT_PULLUP);
  for (a = 0; a < 8; a++) //針對匯流排做發送並設置低電位
  {
    pinMode(led2[a], OUTPUT);
    digitalWrite(led2[a], LOW);
  }
  pinMode(3, OUTPUT); //設控制74LS244的控制角為發送
  digitalWrite(3, HIGH);
  pinMode(A4, OUTPUT); //設控制74LS273的控制角為發送
  digitalWrite(A4, LOW);
  digitalWrite(LEDpin, HIGH);
  Serial.begin(38400);//藍芽鮑率
  Serial.setTimeout(1000);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  jobnumber();
  mySCoop.start();
  isTime = false;
  isPwd = false;
}
void IOProcess::setup()
{

}
char str_in_temp;
char tt;
int ggg = 0;
char oldledStatus [] = "1234567800qwerd";
char ledStatus[] = "1234567890qwerx";

void IOProcess::loop()
{
  val1 = digitalRead(inPin1);  //讀取按鈕
  val2 = digitalRead(inPin2);
  val3 = digitalRead(inPin3);
  val4 = digitalRead(inPin4);
  if (ledStatus[1] == '1' | str_in == 'Q')
  {
    Serial.println('0');
    str_in = '0';
    jobnumber();
    ggg = 0;
  } else if (ledStatus[2] == '1' | str_in == 'W')
  {
    Serial.println('6');
    str_in = '6';

  } else if (ledStatus[3] == '1' | str_in == 'E')
  {
    Serial.println('G');
    str_in = 'G';

  } else if (ledStatus[4] == '1'  | str_in == 'R')
  {
    Serial.println('@');
    str_in = '@';
  }
  if (Serial.available() > 0) //如果接收到的位元組數不為空
  {
    str_in_temp = Serial.read();   //把讀取到的字串放入變數裡
    //if(str_in == '@' && str_in_temp ==' ')return;
    str_in = str_in_temp;
    //Serial.print("key in chart is : ");
    Serial.println(str_in);
    if (str_in == 'p')
    {
      isPwd = true;
      return;
    }
    if (str_in == 't')
    {
      isTime = true;
      times = 0;
      return;
    }
    if (isPwd  == true)
    {
      if (pIndex == 0)
      {
        pTemps = atoi(&str_in) * 10;
        pIndex++;
        return;
      }
      if (pIndex == 1)
      {
        isPwd = false;
        pTemps = pTemps + atoi(&str_in);
        pIndex++;
        EEPROM.write(passwd, pTemps);
        Serial.println(passwd);
        pIndex = 0;
        pTemps = 0;
        return;
      }
      pIndex++;
    }
    if (isTime == true)
    {
      if (tIndex == 0)
      {
        tTemps = atoi(&str_in) * 10;
        tIndex++;
        return;
      }
      if (tIndex == 1)
      {
        isTime = false;
        tTemps = tTemps + atoi(&str_in);
        tIndex++;
        times = tTemps;
        Serial.println(times);
        ggg = 1;
        tIndex = 0;
        tTemps = 0;
        return;
      }
      tIndex++;
    }
  }
  ledStatus[0] = '[';
  ledStatus[1] = buttonstatus(digitalRead(inPin1));
  ledStatus[2] = buttonstatus(digitalRead(inPin2));
  ledStatus[3] = buttonstatus(digitalRead(inPin3));
  ledStatus[4] = buttonstatus(digitalRead(inPin4));
  //////////////////////////////////////////////////////////////////////
  for (int8_t i = 7; i >= 0; i-- ) {
    nowstatusbyte = bitRead( opqr, i );
    int r = 5 + i;
    ledStatus[r] = ledstatus(nowstatusbyte);
  }
  //////////////////////////////////////////////////////////////////////
  ledStatus[13] = ledstatus(digitalRead(3));
  ledStatus[14] = ']';
  bool isChange = !strcmp(oldledStatus, ledStatus);
  if (isChange)
  {

  } else if (isChange == 0)
  {
    Serial.flush();
    Serial.println(ledStatus);
    strcpy(oldledStatus, ledStatus);
  }

  //oldledStatus = ledStatus;
  //sleep(100);

}
char ledstatus(int dig)
{
  if (dig == 0)
  {
    return '0';
  } else
  {
    return '1';
  }

}
char buttonstatus(int dig)
{
  if (dig == 0)
  {
    return '1';
  } else
  {
    return '0';
  }

}
int tr = 0;
int v = 0;
int disr = 0;

void timer::setup() {

}


void timer::loop() {
  if (millis() > (time_now + 1000)) {
    time_now = millis();
    Serial.print("H");
  }
  if (ggg == 1) {
    v = times;
    digitalWrite(3, HIGH); //設高電位關閉74LS244
    digitalWrite(A4, HIGH); //設低電位於74LS273
    if (v > 10) {
      do {
        v -= 10;
        tr += 1;
      }
      while (v > 10);
    }
    disr = tr * 16 + v;
    if (v == 10 | v == 20 | v == 30 |
        v == 40 | v == 50 | v == 60 )
    {
      disr = disr + 6;
    }
    //Serial.println(disr);
    outputbyte(disr);
    digitalWrite(3, HIGH); //設高電位關閉74LS244
    digitalWrite(A4, LOW); //74LS273輸出資料，正緣觸發
    v = 0;
    tr = 0;
    times ++;
    sleep(1000);
  }
  if (times == 60) {
    times = 0;
  }

}

void loop()
{
  yield();
  if (true) //如果接收到的位元組數不為空
  {
    if (isTime == true)return;

    if (str_in == '0')
    {
      emptyws2812();
      empty7seg();
      ggg = 0;
      total = 0;
      L1 = 0;
      L2 = 0;
      L3 = 0;
      L4 = 0;
      L5 = 0;
      L6 = 0;
      L7 = 0;
      L8 = 0;
      EEPROM.write(passwd, 26);

    }
    else if (str_in == '@')
    {
      Breathe_LED();
    }
    else if (str_in == '+') //奇數
    {
      digitalWrite(3, HIGH); //設高電位關閉74LS244
      digitalWrite(A4, HIGH); //設低電位於74LS273
      outputbyte(128);
      digitalWrite(A4, LOW); //74LS273輸出資料，正緣觸發
      digitalWrite(3, HIGH);

    } else if (str_in == '-') //偶數
    {
      digitalWrite(3, HIGH); //設高電位關閉74LS244
      digitalWrite(A4, HIGH); //設低電位於74LS273
      outputbyte(0);
      digitalWrite(A4, LOW); //74LS273輸出資料，正緣觸發
      digitalWrite(3, HIGH);
    }
    else if (str_in == '1')
    {
      ws_pin = 1;
      ws2812();
    }
    else if (str_in == '2')
    {
      ws_pin = 2;
      ws2812();
    }
    else if (str_in == '3')
    {
      ws_pin = 3;
      ws2812();
    }
    else if (str_in == '4')
    {
      ws_pin = 4;
      ws2812();
    }
    else if (str_in == '5')
    {
      ws_pin = 5;
      ws2812();
    }
    else if (str_in == '6')
    {
      ws_pin = 6;
      ws2812();
    }
    else if (str_in == '7')
    {
      ws_pin = 7;
      ws2812();
    }
    else if (str_in == '8')
    {
      ws_pin = 8;
      ws2812();
    }
    else if (str_in == 'R')
    {
      ws_color = str_in;
      bcr = 255;
      bcg = 0;
      bcb = 0;
    }
    else if (str_in == 'O')
    {
      ws_color = str_in;
      bcr = 255;
      bcg = 128;
      bcb = 0;
    }
    else if (str_in == 'Y')
    {
      ws_color = str_in;
      bcr = 255;
      bcg = 200;
      bcb = 20;
    }
    else if (str_in == 'G')
    {
      ws_color = str_in;
      bcr = 0;
      bcg = 255;
      bcb = 0;
    }
    else if (str_in == 'B')
    {
      ws_color = str_in;
      bcr = 0;
      bcg = 0;
      bcb = 255;
    }
    else if (str_in == 'I')
    {
      ws_color = str_in;
      bcr = 111;
      bcg = 0;
      bcb = 255;
    }
    else if (str_in == 'P')
    {
      ws_color = str_in;
      bcr = 160;
      bcg = 32;
      bcb = 240;
    }
    else if (str_in == 'W')
    {
      ws_color = str_in;
      bcr = 255;
      bcg = 255;
      bcb = 255;
    }
    else if (str_in == '{')
    {
      t = 1;
    }
    else if (str_in == '}')
    {
      t = 0;
      emptyws2812();
    }
    ////////////////////////////////////////////////////////////////////////
    else if (str_in == 'm') {
      digitalWrite(3, LOW);
      if (L1 == 0) {
        total = total + 1;
        L1 = 1;
      }
      else if (L1 == 1) {
        total = total - 1;
        L1 = 0;
      }
      outputbyte(total);
    }
    else if (str_in == 'n') {
      digitalWrite(3, LOW);
      if (L2 == 0) {
        total = total + 2;
        L2 = 1;
      }
      else if (L2 == 1) {
        total = total - 2;
        L2 = 0;
      }
      outputbyte(total);
    }
    else if (str_in == ';') {
      digitalWrite(3, LOW);
      if (L3 == 0) {
        total = total + 4;
        L3 = 1;
      }
      else if (L3 == 1) {
        total = total - 4;
        L3 = 0;
      }
      outputbyte(total);
    }
    else if (str_in == 'v') {
      digitalWrite(3, LOW);
      if (L4 == 0) {
        total = total + 8;
        L4 = 1;
      }
      else if (L4 == 1) {
        total = total - 8;
        L4 = 0;
      }
      outputbyte(total);
    }
    else if (str_in == 'l') {
      digitalWrite(3, LOW);
      if (L5 == 0) {
        total = total + 16;
        L5 = 1;
      }
      else if (L5 == 1) {
        total = total - 16;
        L5 = 0;
      }
      outputbyte(total);
    }
    else if (str_in == 'k') {
      digitalWrite(3, LOW);
      if (L6 == 0) {
        total = total + 32;
        L6 = 1;
      }
      else if (L6 == 1) {
        total = total - 32;
        L6 = 0;
      }
      outputbyte(total);
    }
    else if (str_in == 'j') {
      digitalWrite(3, LOW);
      if (L7 == 0) {
        total = total + 64;
        L7 = 1;
      }
      else if (L7 == 1) {
        total = total - 64;
        L7 = 0;
      }
      outputbyte(total);
    }
    else if (str_in == 'h') {
      digitalWrite(3, LOW);
      if (L8 == 0) {
        total = total + 128;
        L8 = 1;
      }
      else if (L8 == 1) {
        total = total - 128;
        L8 = 0;
      }
      outputbyte(total);
    }//////////////////////////////////////////////////////////////////////////
    else if (str_in == '9')   //當按下Button時，LED全亮全滅///////////////////////
    {
      digitalWrite(3, LOW);
      outputbyte(255);
      sleep(1000);
      empty244();
      digitalWrite(3, HIGH);
    }/////////////////////////////////////////////////////////////////////////
    else if (str_in == 'a') //當按下Button時，LED由左依序亮至右
    {
      digitalWrite(3, LOW);
      empty244();
      for (a = 0; a < 15; a++)
      {
        outputbyte(ledf[a]);
        sleep(500);
      }
      digitalWrite(3, HIGH);
      empty244();
    }
    else if (str_in == 'e') //當按下Button時，LED由右依序亮至左
    {
      digitalWrite(3, LOW);
      empty244();
      for (a = 0; a < 15; a++)
      {
        outputbyte(ledr[a]);
        sleep(500);
      }
      digitalWrite(3, HIGH);
      empty244();
    }
    else if (str_in == 'f') //當按下Button時，LED由左恆亮至右//////////////////////////////
    {
      digitalWrite(3, LOW);
      empty244();
      int x = 0;
      for (int a = 0; a < 8; a++)
      {
        x = x + pow(2, a);
        if (x > 3) {
          x = x + 1;
        }
        outputbyte(x);
        delay(500);
      }
      x = 0;
      empty244();
      digitalWrite(3, HIGH);
    }/////////////////////////////////////////////////////////////////////////////////
    else if (str_in == 'b') //當按下Button時，LED由右恆亮至左/////////////////////////////
    {
      digitalWrite(3, LOW);
      empty244();
      int x = 0;
      for (int a = 7; a >= 0 ; a--)
      {
        x = 255 - pow(2, a) + 1;
        outputbyte(x);
        delay(500);
      }
      empty244();
      digitalWrite(3, HIGH);
    }//////////////////////////////////////////////////////////////////////////////
    else if (str_in == 'c')   //當按下Button時，七段顯示器顯示0~99，期間可變電阻可改變轉換速率
    {
      empty7seg();
      for (a = 0; a < 100; a++)
      {
        digitalWrite(3, HIGH); //設高電位關閉74LS244
        digitalWrite(A4, HIGH); //設低電位於74LS273
        sleep(dt);
        if (a == 10 | a == 20 | a == 30 |
            a == 40 | a == 50 | a == 60 |
            a == 70 | a == 80 | a == 90 )
          i = i + 6;
        val = analogRead(potpin);
        dt = map(val, 0, 1023, 0, 255);
        outputbyte(i);
        digitalWrite(A4, LOW); //74LS273輸出資料，正緣觸發
        digitalWrite(3, HIGH); //設高電位關閉74LS244
        sleep(dt);
        i++;
      }
      a = 0;
      i = 0;
      digitalWrite(A4, HIGH);
      outputbyte(0);
      digitalWrite(A4, LOW);
    }
    else if (str_in == 'd')   //當按下Button時，七段顯示器顯示99~0，期間可變電阻可改變轉換速率
    {
      empty7seg();
      for (a = 0; a < 100; a++) {
        digitalWrite(3, HIGH); //設高電位關閉74LS244
        digitalWrite(A4, HIGH); //設低電位於74LS273
        sleep(dt);
        if (a == 10 | a == 20 | a == 30 |
            a == 40 | a == 50 | a == 60 |
            a == 70 | a == 80 | a == 90 )
          i = i + 6;
        val = analogRead(potpin);
        dt = map(val, 0, 1023, 0, 255);
        aa = map(i, 0, 153, 153, 0);
        outputbyte(aa);
        digitalWrite(3, HIGH); //設高電位關閉74LS244
        digitalWrite(A4, LOW); //74LS273輸出資料，正緣觸發
        sleep(dt);
        i++;
      }
      a = 0;
      i = 0;
      digitalWrite(A4, HIGH);
      outputbyte(0);
      digitalWrite(A4, LOW);
    }
    if (str_in == '@')return;
    str_in = ' ';
  }
}
