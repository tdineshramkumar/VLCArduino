
#include "tones.h"
const word symbolduration = 50 ;

void transmit(byte data) {
  digitalWrite(10,0);
  delayMicroseconds( 3 * symbolduration );
  for (int i = 0 ; i < 8 ; i ++) {
    digitalWrite( 10, (i+1)%2);
    delayMicroseconds( ( 1 + bitRead(data,i) ) * symbolduration );
  }
  digitalWrite(10,1);
  delayMicroseconds( symbolduration ) ;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(10,OUTPUT);
  pinMode(A0,INPUT);
  digitalWrite(10,HIGH);

}
//String message = "Hello World" ;
void loop() {
 // // put your main code here, to run repeatedly:
 // for (int i = 0 ; i < message.length() ; i ++ ) 
   // transmit(message.charAt(i)) ;
  //
  int wait;
  for (int i=0;i<203;i++){              //203 is the total number of music notes in the song
    int val = analogRead(A0); 
    val = map(val, 0, 1023, 1, 10);
    byte data = encodeDataFrame(notes[i],duration[i]);
    transmit(data);
    wait=  duration[i]/val ;
    delay(wait) ;
  }
  delay(1000);
}
