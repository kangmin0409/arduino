#include <Arduino.h>
#include <TM1637Display.h>
#include <Wire.h>
#include <RTClib.h>
#include <OzOLED.h>
#define CLK 2
#define DIO 3
#include <DHT11.h>
int pin=8;    // 연결한 아두이노 디지털 핀 번호
const int buz=9;
const int relay=11;
RTC_DS1307 rtc;//RTC_DS1307 rtc;
int trigPin = 6;
int echoPin = 7;

DHT11 dht11(pin);
TM1637Display display(CLK, DIO);


const static unsigned char kkm[] PROGMEM = {
0x00
};


void setup()
{
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));
  pinMode(buz,OUTPUT);
  pinMode(relay,OUTPUT);
  display.setBrightness(15); //0~15
  display.clear();
  Serial.begin(9600);
  OzOled.init();
  pinMode(echoPin, INPUT);   // echoPin 입력    
  pinMode(trigPin, OUTPUT);  // trigPin 출력
  OzOled.clearDisplay();
  OzOled.drawBitmap(kkm,0,0,16,8);
  delay(5000);
  OzOled.clearDisplay();
}

void loop()
{
  DateTime now = rtc.now();
  long duration, distance;
  digitalWrite(trigPin, HIGH);  // trigPin에서 초음파 발생(echoPin도 HIGH)        
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);    // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance = ((float)(340 * duration) / 1000) / 2;
  
  if(distance>750){
    OzOled.setBrightness(10);
    //digitalWrite(relay, LOW);
    //tone(buz,4500,200);
  }

  else{
    OzOled.setBrightness(255);
    //digitalWrite(relay, HIGH);
    //delay(5000);
  }
  
  int err,bbb;
  float temp, humi;
  humi = humi * 1.77;
  if((err=dht11.read(humi, temp))==0)
  {
    bbb=0.72*(temp+humi)+40.6;
    //Serial.println(bbb); --
    //display.showNumberDec(humi*1.77, 0,2,2);
    
    OzOled.setCursorXY(0, 0);
    OzOled.printString("Temp: ");
    OzOled.printString(String(temp).c_str());
    OzOled.printString(" C");
    
    OzOled.setCursorXY(0, 2);
    OzOled.printString("Humi: ");
    OzOled.printString(String(humi*1.5).c_str());
    OzOled.printString(" %");
    
    OzOled.setCursorXY(0, 4);
    OzOled.printString("Distance: ");
    OzOled.printString(String(distance/10).c_str());
    OzOled.printString("cm ");
    
    OzOled.setCursorXY(0, 7);
    OzOled.printString(String(now.hour(), DEC).c_str());
    OzOled.printString(":");
    OzOled.printString(String(now.minute(), DEC).c_str());
    OzOled.printString(":");
    OzOled.printString(String(now.second(), DEC).c_str());
    OzOled.printString("    ");
    
    OzOled.setCursorXY(0, 6);
    OzOled.printString("B.C: ");
    OzOled.printString(String(bbb).c_str());
    OzOled.printString("    ");

    Serial.print(now.year(), DEC);                  // 시리얼모니터에 출력
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    //delay(1000);

    
    if(bbb>=95){
      tone(buz,4500,200);
      tone(buz,4500,200);
      tone(buz,4500,200);
      tone(buz,4500,200);
      digitalWrite(relay,1);
      delay(1000);
    }
    else{
      digitalWrite(relay,0);
    }
  }
  
  else
  {
    OzOled.clearDisplay();
    OzOled.setCursorXY(0, 1);
    OzOled.printString("Error No: ");
    OzOled.printString(String(err).c_str());
    OzOled.setCursorXY(0,3);
    OzOled.printString("(Use Reset Button)");
  }
  
  delay(DHT11_RETRY_DELAY); //delay for reread
}
