#include  <SoftwareSerial.h>  
SoftwareSerial BTSerial(10, 11); // RX | TX  
int i = 0;  
void setup()  
{  
  Serial.begin(9600);  
  Serial.println("Enter AT commands:");  
  BTSerial.begin(38400);  // HC-05 default speed in AT command more  
}  
void loop()  
{  
  // Keep reading from HC-05 and send to Arduino Serial Monitor  
if (BTSerial.available())  
  {  
    Serial.write(BTSerial.read());  
  }  
  // Keep reading from Arduino Serial Monitor and send to HC-05  
  if (Serial.available())  
  {  
    BTSerial.write(Serial.read());  
  }  
}  
/*查詢HC-05 address: 「AT+ADDR?」
 *改名字為CAVEDU: 「AT+NAME=CAVEDU」
 *查名字 : 「AT+NAME?」
 *改鮑率為9600 : 「AT+UART=9600,0,0」
 *查鮑率: 「AT+UART? 」
 *設定配對密碼為1234: 「AT+ PSWD=1234」
 *預設出廠模式: 「AT+ORGL」
 */
