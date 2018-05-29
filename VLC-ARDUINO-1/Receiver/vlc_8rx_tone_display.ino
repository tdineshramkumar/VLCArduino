
/*
TEST FRAME 
|         |  DATA                                 |       |
|START        |  LSB                                      MSB |  END  |
| S1 S2 S3 S4 | D0  | D1  | D2  | D3  | D4  | D5  | D6  | D7  | E1 E2 |
| 0  0  0  1  | x x | x x | x x | x x | x x | x x | x x | x x | 0  1  |
INDEX -->
  0  1  2  3    4 5   6 7   8 9  10 11  12 13 14 15 16 17 18 19 20 21

NOTICE FRAME ALWAYS ENDS WITH A LOW TO HIGH TRANSITION
AND BEGINS WITH LOW TO HIGH TRANSITION
*/
//NOTE : EVERYTHING IS INCVERTED
#define FRAME_DURATION 5
#define FRAME_SIZE 22
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int din = 2 ;
const int buzzer = 9 ;
volatile unsigned long prev = 0;
volatile unsigned long curr = 0;
volatile int frame[FRAME_SIZE];
volatile int index ;
volatile boolean started = false ; // indicates if start of frame found 

// TONE DEFINIOTS GO HERE
#define NOTE_C4  262   //Defining note frequency
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349

#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784

#define NOTE_A5  880
#define NOTE_B5  988

int decodeDataFrameNote(char data){
  char res = (data & 0b01111000) >> 3;
  return getNoteDecode(res);
}
int decodeDataFrameDur(char data){
   char res = (data & 0b00000011) ;
   return getDurDecode(res);
}
int getDurDecode(char val){
  int res = 0;
  switch(val){
    case 0b00:
    res = 125;
    break;
    case 0b01:
    res = 250;
    break;
    case 0b10:
    res = 375;
    break;
    case 0b11:
    res = 500;
    break;    
  }
  return res;
}
int getNoteDecode(char note){
  int res = 0;
  switch(note){
    case   0b0000:
      res = NOTE_C4;
      break;
    case   0b0001:
      res = NOTE_D4;
      break;
    case   0b0010:
      res = NOTE_E4;
      break;
    case   0b0011:
      res = NOTE_F4;
      break;
    case   0b0100:
      res = NOTE_G4;
      break;
    case   0b0101:
      res = NOTE_A4;
      break;
    case   0b0110:
      res = NOTE_B4;
      break;
    case   0b0111:
      res = NOTE_C5;
      break;
    case   0b1000:
      res = NOTE_D5;
      break;
    case   0b1001:
      res = NOTE_E5;
      break;
    case   0b1010:
      res = NOTE_F5;
      break;
    case   0b1011:
      res = NOTE_G5;
      break;
    case   0b1100:
      res = NOTE_A5;
      break;
    case   0b1101:
      res = NOTE_B5;
      break;
    default:
      res = 0;
    
  }
  return res;
}

/// END OF TONE


byte armsDown[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte armsUp[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

void extractData(){
  char data = 0 ;
  for (int i = 4 ; i <= 18 ; i = i + 2 ){
    if(frame[i] == frame[i+1]){
      Serial.print("INVALID");
      return ;
    }
  } 
  for (int i = 4 ; i <= 18 ; i = i + 2 ){
    //MODIFIED
    bitWrite(data,(i-4)/2,frame[i+1]) ;
  }
  Serial.print(data);
  // Updated here
  int note1 = decodeDataFrameNote(data);
  int dur1 = decodeDataFrameDur(data);
  tone(buzzer,note1,dur1);          //tone(pin,frequency,duration)
  lcd.setCursor(0, 1);
  for ( int i = 0 ; i < 16 ; i++){
    lcd.write(byte((note1 >> (i%10) )%2 ))  ;
  }
      //FOR PRINTING THE FRAME
      for (int i = 0 ; i < FRAME_SIZE; i ++ ){
        Serial.print(frame[i]);
      }
    //Serial.println("DONE");
}
void isr(){
//        char buffer[100];
        
  curr = millis();
       //Serial.print(digitalRead(din));
      //  sprintf (buffer, "diff:%ul",curr - prev);
       // Serial.println(buffer);
         if(curr - prev < 1.25 * FRAME_DURATION){
           //Serial.print('Q');
           //Serial.print(digitalRead(din)) ;
         }
         else if(curr - prev < 2.25 * FRAME_DURATION){
          //Serial.print('W'); 
          // Serial.print((digitalRead(din)+1)%2) ;
            //Serial.print(digitalRead(din)) ;
         
         }else if(curr - prev < 3.25 * FRAME_DURATION){
           //Serial.print('E');
           //Serial.print((digitalRead(din)+1)%2) ;
           //Serial.print((digitalRead(din)+1)%2) ;
            //Serial.print(digitalRead(din)) ;
         }else if(curr - prev < 4.25 * FRAME_DURATION){
           //Serial.print('R');
           //Serial.print((digitalRead(din)+1)%2) ;
           //Serial.print((digitalRead(din)+1)%2) ;
           //Serial.print((digitalRead(din)+1)%2) ;
            //Serial.print(digitalRead(din)) ;
         }
         else{
         //Serial.print('T');
         }
  if(started){
            //Serial.println("STARTED");
           //MODIFIED
    if(digitalRead(din) == HIGH){
      // NOTICE FRAME CANT END
      if(curr - prev < 1.25 * FRAME_DURATION){
        // ONE BIT DURATION
        frame[index++] = 0 ;
        
      }
      else{
        // TWO BIT DURATION
        frame[index++] = 1 ;
        frame[index++] = 0 ;

      }
    }
    else{
      //  digitalRead(din) == HIGH
      if(curr - prev < 1.25 * FRAME_DURATION){
        // ONE BIT DURATION
        frame[index++] = 1 ;
        
      }
      else{
        // TWO BIT DURATION
        frame[index++] = 0 ;
        frame[index++] = 1 ;

      }
      
      // IF FRAME ENDED
      if(index == FRAME_SIZE){
        //do something ..
        extractData();
        
        started = false;
      }
    }
        
  }
  else{
          //  Serial.println("NOT STARTED");sprintf (buffer, "diff:%d",(int) (2.75 * FRAME_DURATION));
       // Serial.println(buffer);
    // if not started
//MODIFIED
    if(digitalRead(din) == LOW && curr - prev > 2.75 * FRAME_DURATION && curr - prev < 3.75 * FRAME_DURATION){
      // THE FRAME STATED
      
                        frame[0] = 0;
      frame[1] = 0;
      frame[2] = 0;
      frame[3] = 1;
      started = true ;
      index = 4 ; // begining of data
        
    }
  }
  prev = curr ;
}

void setup(){
  Serial.begin(9600);
  pinMode(din,INPUT_PULLUP);
  lcd.begin(16, 2);
    // create a new character
  lcd.createChar(0, armsDown);
  // create a new character
  lcd.createChar(1, armsUp);
   // set the cursor to the top left
  lcd.setCursor(0, 0);

  // Print a message to the lcd.
  //lcd.print("I ");
  //lcd.write(byte(0)); // when calling lcd.write() '0' must be cast as a byte
  //lcd.print(" Arduino! ");
  //lcd.write((byte)1);
  lcd.print("Prates Cariben");
  attachInterrupt(0,isr,CHANGE);
}
void loop(){
  //doing nothing
}
