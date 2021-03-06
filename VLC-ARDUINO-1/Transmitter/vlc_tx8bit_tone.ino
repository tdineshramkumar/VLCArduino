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
int FRAME_DURATION = 5;
#define FRAME_SIZE 22
#define DATA_SIZE 8
const int potin = A0 ; // not used 
const int dout = 10 ;
char data ;
void sendData(){
  //Start of Frame
  digitalWrite(dout,LOW);
        Serial.print("000");
  delay(3*FRAME_DURATION);
  digitalWrite(dout,HIGH);
        Serial.print("1");
  delay(FRAME_DURATION);
  //Send LSB First
  for (int i = 0 ; i < DATA_SIZE; i ++ ){
    if(bitRead(data,i) == 0){
      digitalWrite(dout,HIGH);
      delay(FRAME_DURATION);
      digitalWrite(dout,LOW);
      delay(FRAME_DURATION);
                        Serial.print("10");
    }
    else{
      digitalWrite(dout,LOW);
      delay(FRAME_DURATION);
      digitalWrite(dout,HIGH);
      delay(FRAME_DURATION);
                        Serial.print("01");
    }
  }
  //End of Frame
  digitalWrite(dout,LOW);
  delay(FRAME_DURATION);
  digitalWrite(dout,HIGH);
  delay(FRAME_DURATION);
        Serial.println("01");
}


/// ALL THE TONE INFO HERE


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

char encodeControlFrame(int potval){
  return (0b10000000) | getFrameSpeed(potval);
}
char getFrameSpeed(int potval){
  char res = 0 ;
   if ( potval < 128 ) {
    res = 0b00000000 ; 
   }
   else if ( potval < 128 ) {
    res = 0b00000001 ; 
   }
   else if ( potval < 256 ) {
    res = 0b00000010 ; 
   }
   else if ( potval < 384 ) {
    res = 0b00000011 ; 
   }
   else if ( potval < 512 ) {
    res = 0b00000100 ; 
   }
   else if ( potval < 640 ) {
    res = 0b00000101; 
   }
   else if ( potval < 768 ) {
    res = 0b00000110 ; 
   }
   else {
    res = 0b00000111 ; 
   }
   return res ;
}
char encodeDataFrame(int note, int duration){
  char dur = getDurEncode(duration);
  Serial.print("Duration:");
  Serial.print(duration);
  Serial.print("dur:");
  Serial.println(dur,BIN);
  char _note = getNoteEncode(note);
  
  Serial.print("NOTE:");
  Serial.print(note);
  Serial.print("_note:");
  Serial.println(_note,BIN);
  char res = 0b00000000 | (dur) | (_note << 3) ;
  Serial.print("RES");
  Serial.print(res,BIN);
  return res;
}

char getDurEncode(int val){
  char res = 0;
  switch(val){
    case 125:
    res = 0b00;
    break;
    case 250:
    res = 0b01;
    break;
    case 375:
    res = 0b10;
    break;
    case 500:
    res = 0b11;
    break;    
  }
  return res;
}


char getNoteEncode(int note){
  char res = 0;
  switch(note){
    case   NOTE_C4:
      res = 0b0000;
      break;
    case   NOTE_D4:
      res = 0b0001;
      break;
    case   NOTE_E4:
      res = 0b0010;
      break;
    case   NOTE_F4:
      res = 0b0011;
      break;
    case   NOTE_G4:
      res = 0b0100;
      break;
    case   NOTE_A4:
      res = 0b0101;
      break;
    case   NOTE_B4:
      res = 0b0110;
      break;
    case   NOTE_C5:
      res = 0b0111;
      break;
    case   NOTE_D5:
      res = 0b1000;
      break;
    case   NOTE_E5:
      res = 0b1001;
      break;
    case   NOTE_F5:
      res = 0b1010;
      break;
    case   NOTE_G5:
      res = 0b1011;
      break;
    case   NOTE_A5:
      res = 0b1100;
      break;
    case   NOTE_B5:
      res = 0b1101;
      break;
    default:
      res = 0b1111;
  }
  return res;
}



int notes[] = {       //Note of the song, 0 is a rest/pulse
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0, 
   NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
   NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,
   
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_D5, NOTE_E5, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
   NOTE_C5, NOTE_A4, NOTE_B4, 0,

   NOTE_A4, NOTE_A4, 
   //Repeat of first part
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,

   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0, 
   NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
   NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,
   
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_D5, NOTE_E5, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
   NOTE_C5, NOTE_A4, NOTE_B4, 0,
   //End of Repeat

   NOTE_E5, 0, 0, NOTE_F5, 0, 0,
   NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
   NOTE_D5, 0, 0, NOTE_C5, 0, 0,
   NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

   NOTE_E5, 0, 0, NOTE_F5, 0, 0,
   NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
   NOTE_D5, 0, 0, NOTE_C5, 0, 0,
   NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4
};
//*****************************************
int duration[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125, 
  250, 125, 250, 125, 
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125,
  //Rpeat of First Part
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125, 
  250, 125, 250, 125, 
  125, 125, 250, 125, 125,
  125, 125, 375, 375,
  //End of Repeat
  
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500,

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500
};
/// TONE ENDS HERE

void setup(){
  Serial.begin(9600);
  pinMode(dout,OUTPUT);
  digitalWrite(dout,HIGH);
  int wait;
  for (int i=0;i<203;i++){              //203 is the total number of music notes in the song
    data = encodeDataFrame(notes[i],duration[i]);
    Serial.print("DATA:") ;
    Serial.println(data);
    sendData();
    wait=  ( duration[i] - FRAME_SIZE * FRAME_DURATION ) + 10 ;
    
    delay(wait) ;
  }   
}

void loop(){
  //doing nothing
  /*if(Serial.available()>0){
    data = (char)Serial.read();
    sendData();
  }
        */
        
//        data = 'H' ;
//        sendData();
//        data = 'E' ;
//        sendData();
//        data = 'L' ;
//        sendData();
//        data = 'L' ;
//        sendData();
//        data = '0' ;
//        sendData();
      
}
