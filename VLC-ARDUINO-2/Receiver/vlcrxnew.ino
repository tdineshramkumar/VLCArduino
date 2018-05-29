#include <LiquidCrystal.h>
#include "tones.h"
const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int buzzer = 9 ;

const word symbolduration = 50 ;
float errormargin = 0.4f ; // keep it below 0.5f
word duration(unsigned long curr, unsigned long prev) {
  unsigned long period = curr - prev ;
  if (period >  (1 - errormargin) * symbolduration && period <  (1 + errormargin) * symbolduration)
    return 1 ;
  if (period >  (2 - errormargin) * symbolduration && period <  (2 + errormargin) * symbolduration)
    return 2 ;
  if (period >  (3 - errormargin) * symbolduration && period <  (3 + errormargin) * symbolduration)
    return 3 ;
  if (period >  (4 - errormargin) * symbolduration && period <  (4 + errormargin) * symbolduration)
    return 4 ;
  return 0 ; // other wise
}

unsigned long prev = 0, curr = 0;
int curbit = 0 ; // not started
char data ;
bool started = false ;
void noteplay(char data){
  int note1 = decodeDataFrameNote(data);
  int dur1 = decodeDataFrameDur(data);  
  if (note1 != 0)
    tone(buzzer,note1,dur1);          //tone(pin,frequency,duration)
  else 
    noTone(buzzer);
  lcd.setCursor(0, 1);
  for ( int i = 0 ; i < 16 ; i++){
    lcd.write(byte((note1 >> (i%10) )%2 ))  ;
  }
}
void isr() {
  // Serial.print('S');
  //Serial.print(curbit);
  curr = micros();
  int x = duration(curr, prev);
  if ( x == 3 ) {
    started = true;
    curbit = 0 ;
    data = 0;
  }
  else if ( started  ) {
    if (x == 1) {
      bitClear(data, curbit);
      curbit ++ ;
      if (curbit == 8) {
        started = false ;
        curbit = 0 ;
        Serial.println(data);
        noteplay(data);
      }
    }
    else if (x == 2) {
      bitSet(data, curbit);
      curbit ++ ;
      if (curbit == 8) {
        started = false ;
        curbit = 0 ;
        Serial.println(data);
        noteplay(data);
      }
    }
    else {
      Serial.println("ERROR");
      started = false;
      curbit = 0 ;
    }
  }
  prev = curr ;
}
const int interruptPin = 2 ;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(interruptPin, INPUT);
  pinMode(buzzer,OUTPUT);
  lcd.begin(16, 2);
  lcd.createChar(0, armsDown);
  lcd.createChar(1, armsUp);
  lcd.setCursor(0, 0);
  lcd.print("Prates Cariben");
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

}
