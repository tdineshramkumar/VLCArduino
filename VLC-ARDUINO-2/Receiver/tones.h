
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


int decodeDataFrameNote(char data){
  char res = (data & 0b01111000) >> 3;
  return getNoteDecode(res);
}
int decodeDataFrameDur(char data){
   char res = (data & 0b00000011) ;
   return getDurDecode(res);
}

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

