//Christmas themed clock with Christmas countdown
//Spoken countdown via TMRPCM library https://github.com/TMRh20/TMRpcm/wiki
//Samples are in wav format on SD card (22kHz, 8bit mono)

//pin defines for display
#define ST7920RS 5
#define ST7920RW 6
#define ST7920E 7
#define ST7920CMD LOW
#define ST7920DATA HIGH
#include <avr/pgmspace.h>
//RTC library for RTC Shield    

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
//set to one for 12 hour clock, zero for 24 hour clock
#define TWELVEHOURMODE 1

#include <SD.h>               // need to include the SD library
#define SD_ChipSelectPin 10   //using digital pin 10 on XC4536
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

char days[]="SunMonTueWedThuFriSat";
char months[]="JanFebMarAprMayJunJulAugSepOctNovDec";
byte const PROGMEM font[]={
  0x07,0xe0,0x1f,0xf8,0x3f,0xfc,0x3f,0xfc,0x7e,0x7e,0x7c,0x3e,0x78,0x3e,0xf8,0x1e,0xf8,0x1f,0xf8,0x1f,0xf8,0x1f,0xf8,0x1f,0xf8,0x1f,0xf8,0x1f,0xf8,0x1f,0xf8,0x1e,0x78,0x3e,0x7c,0x3e,0x7e,0x7e,0x3f,0xfc,0x3f,0xfc,0x1f,0xf8,0x07,0xe0,0x00,0x00,
  0x00,0xf0,0x01,0xf0,0x03,0xf0,0x07,0xf0,0x0f,0xf0,0x1f,0xf0,0x7f,0xf0,0x7f,0xf0,0x79,0xf0,0x61,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x01,0xf0,0x00,0x00,
  0x07,0xf0,0x1f,0xf8,0x3f,0xfc,0x7f,0xfe,0x7c,0x3e,0xf8,0x3f,0xf8,0x1f,0x00,0x1f,0x00,0x3e,0x00,0x3e,0x00,0x7e,0x00,0xfc,0x01,0xf8,0x03,0xf0,0x07,0xe0,0x0f,0xc0,0x1f,0x00,0x3e,0x00,0x7e,0x00,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
  0x07,0xe0,0x1f,0xf8,0x3f,0xfc,0x7f,0xfc,0x7c,0x7e,0x7c,0x3e,0x38,0x3e,0x00,0x3c,0x00,0x7c,0x03,0xf8,0x03,0xf0,0x03,0xfc,0x00,0x7e,0x00,0x3e,0x00,0x1f,0x00,0x1f,0xf8,0x1f,0xfc,0x3f,0x7c,0x7e,0x7f,0xfe,0x3f,0xfc,0x1f,0xf8,0x07,0xe0,0x00,0x00,
  0x00,0x7c,0x00,0xfc,0x01,0xfc,0x01,0xfc,0x03,0xfc,0x07,0xfc,0x07,0xfc,0x0f,0xfc,0x1f,0x7c,0x1e,0x7c,0x3c,0x7c,0x7c,0x7c,0xf8,0x7c,0xf0,0x7c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x7c,0x00,0x7c,0x00,0x7c,0x00,0x7c,0x00,0x7c,0x00,0x00,
  0x1f,0xfe,0x1f,0xfe,0x3f,0xfe,0x3f,0xfe,0x3e,0x00,0x3c,0x00,0x3c,0x00,0x7f,0xf0,0x7f,0xf8,0x7f,0xfc,0x7f,0xfe,0x7c,0x3f,0x78,0x1f,0x00,0x1f,0x00,0x1f,0x00,0x1f,0xf8,0x1f,0xfc,0x1f,0x7e,0x3e,0x7f,0xfe,0x3f,0xfc,0x1f,0xf8,0x07,0xe0,0x00,0x00,
  0x03,0xf0,0x0f,0xf8,0x1f,0xfc,0x3f,0xfe,0x7e,0x3e,0x7c,0x3e,0x7c,0x1e,0xfc,0x00,0xf8,0x00,0xf9,0xf0,0xff,0xfc,0xff,0xfe,0xff,0xfe,0xfe,0x3f,0xfc,0x1f,0xfc,0x1f,0x7c,0x1f,0x7c,0x1f,0x7e,0x3f,0x3f,0xfe,0x1f,0xfe,0x0f,0xf8,0x07,0xf0,0x00,0x00,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x00,0x3c,0x00,0x7c,0x00,0xf8,0x00,0xf0,0x01,0xf0,0x01,0xe0,0x03,0xe0,0x03,0xc0,0x07,0xc0,0x07,0xc0,0x07,0x80,0x0f,0x80,0x0f,0x80,0x0f,0x80,0x1f,0x00,0x1f,0x00,0x1f,0x00,0x1f,0x00,0x1f,0x00,0x00,0x00,
  0x0f,0xe0,0x1f,0xf8,0x3f,0xfc,0x7e,0x7c,0x7c,0x3e,0x78,0x3e,0x78,0x3e,0x78,0x3e,0x7c,0x3e,0x3c,0x7c,0x3f,0xf8,0x0f,0xf0,0x3f,0xfc,0x7c,0x7e,0x7c,0x3e,0xf8,0x1f,0xf8,0x1f,0xf8,0x1f,0xfc,0x3e,0x7c,0x7e,0x7f,0xfc,0x1f,0xf8,0x07,0xe0,0x00,0x00,
  0x0f,0xe0,0x3f,0xf0,0x7f,0xf8,0x7f,0xfc,0xfc,0x7e,0xf8,0x3e,0xf8,0x3e,0xf8,0x3e,0xf8,0x3f,0xfc,0x7f,0x7f,0xff,0x7f,0xff,0x3f,0xff,0x0f,0x9f,0x00,0x3f,0x00,0x3e,0x78,0x3e,0xfc,0x3e,0x7c,0x7c,0x7f,0xfc,0x3f,0xf8,0x1f,0xf0,0x0f,0xc0,0x00,0x00
};

void setup(){
  tmrpcm.speakerPin = 9;        //can't be changed from this on Uno
  SD.begin(SD_ChipSelectPin);   //start card
  rtc.begin();                  //RTC begin
  ST7920setup();                //start display
  ST7920setTextmode();          //clear display
  ST7920setGraphicsmode();
  ST7920GraphicsClear();
  doclock();
  
}

void loop(){  
  static unsigned long ctmout=millis();
  static unsigned long stmout=millis();  
  if(millis()-ctmout>3000){               //update clock about every 3s
    doclock();
    ctmout=millis();
  }  
  if(millis()-stmout>5000){               //don't talk unless it's been more than 5s
    if(digitalRead(A3)){                  //and sensor is triggered
      delay(500);                         //wait a bit so the trigger sound is finished
      docountdown();
      stmout=millis();    
    }
  }
}

void doclock(){
  DateTime now = rtc.now();
  unsigned long un,uc,ud;
  un=now.unixtime();
  DateTime c=DateTime(now.year(), 12, 25, 0, 0, 0);                       //this christmas
  uc=c.unixtime();
  if(uc<un){c=DateTime(now.year()+1, 12, 25, 0, 0, 0);uc=c.unixtime();}   //or next christmas if it's past
  ud=(uc-un)/60;                                                          //seconds=>minutes
  int d=(ud/1440)+1;                                                      //add a day so we're counting sleeps.
  int th,tm;                                                              //for displaying current time
  unsigned int c1,c2,c3,c4,c5;                                            //for number graphics
  th=now.hour();
  tm=now.minute();
  if(TWELVEHOURMODE){th=((th+11)%12)+1;}                                  //convert to 12 hour mode if necessary
  for(int y=0;y<24;y++){
    c1=0;c2=0;c3=0;c4=0;c5=0;                                             //4 digits to display plus colon
    if((y&4)&&!(y&16)){c5=240;}
    if((th/10)||(!TWELVEHOURMODE)){c1=pgm_read_byte_near(&font[(th/10)*48+y*2])*256+pgm_read_byte_near(&font[(th/10)*48+y*2+1]);}         //tens of hours (if needed)      
    c2=pgm_read_byte_near(&font[(th%10)*48+y*2])*256+pgm_read_byte_near(&font[(th%10)*48+y*2+1]);               //units of hours
    c3=pgm_read_byte_near(&font[(tm/10)*48+y*2])*256+pgm_read_byte_near(&font[(tm/10)*48+y*2+1]);               //units of minutes
    c4=pgm_read_byte_near(&font[(tm%10)*48+y*2])*256+pgm_read_byte_near(&font[(tm%10)*48+y*2+1]);               //units of minutes
      ST7920GraphicsWriteremap(3,y+4,c1>>4);                              //fit graphics into columns
      ST7920GraphicsWriteremap(4,y+4,c1<<12|c2>>6);    
      ST7920GraphicsWriteremap(5,y+4,c2<<10|c5|c3>>14);    
      ST7920GraphicsWriteremap(6,y+4,c3<<2);    
      ST7920GraphicsWriteremap(7,y+4,c4);    
  }
  
  ST7920setTextmode();
  ST7920SetCursor(1,0);
  ST7920send(days[now.dayOfTheWeek()*3],ST7920DATA);                    //day of the week
  ST7920send(days[now.dayOfTheWeek()*3+1],ST7920DATA);
  ST7920send(days[now.dayOfTheWeek()*3+2],ST7920DATA);
  ST7920send(' ',ST7920DATA);
  ST7920SetCursor(0,1);
  if(now.day()<10){ST7920send(' ',ST7920DATA);}else{ST7920send('0'+now.day()/10,ST7920DATA);}         //day of the month
  ST7920send('0'+now.day()%10,ST7920DATA);
  ST7920send(' ',ST7920DATA);
  ST7920send(months[now.month()*3-3],ST7920DATA);                       //month
  ST7920send(months[now.month()*3-2],ST7920DATA);
  ST7920send(months[now.month()*3-1],ST7920DATA);
  ST7920send(' ',ST7920DATA);

  ST7920SetCursor(0,2);
  ST7920string("    ");                                               
  if(d>99){ST7920send('0'+(d/100)%10,ST7920DATA);}                      //# of days, remove leading zeros
  if(d>9){ST7920send('0'+(d/10)%10,ST7920DATA);}
  ST7920send('0'+d%10,ST7920DATA);
  ST7920string(" day");                                                 ///day
  if(d!=1){ST7920send('s',ST7920DATA);}                                 //only pluralize if not 1
  ST7920SetCursor(0,3);
  ST7920string(" 'til Christmas.");                                     //end of sentence
  ST7920setGraphicsmode();
}

void docountdown(){
  DateTime now = rtc.now();
  unsigned long un,uc,ud;
  un=now.unixtime();
  DateTime c=DateTime(now.year(), 12, 25, 0, 0, 0);                       //this christmas
  uc=c.unixtime();
  if(uc<un){c=DateTime(now.year()+1, 12, 25, 0, 0, 0);uc=c.unixtime();}   //next christmas
  ud=(uc-un)/60;                                                          //seconds=>minutes
  int d,w;
  d=(ud/1440)+1;                                                          //add a day so we're counting sleeps.
  w=d/7;
  d=d%7;
  if(w==52){w=0;d=0;}                                                     //suppress countdown on Christmas day
  readtillxmas(w,d);                                                      //read out countdown
  playphrase(millis()%19+1);                                              //play random phrase
}

void readbyte(byte i){
  int zerosuppress=0;    //so we don't say three hundred and thirty zero
  if(i>99){
    readbyte(i/100);
    playb("100.wav");
    i=i%100;
    zerosuppress=1;
  }
  if((zerosuppress)&&(i)){playb("and.wav");}  //delete for American pronuncation
  if(i>19){
    switch(i/10){
    case 2:    playb("20.wav");    break;
    case 3:    playb("30.wav");    break;
    case 4:    playb("40.wav");    break;
    case 5:    playb("50.wav");    break;
    case 6:    playb("60.wav");    break;
    case 7:    playb("70.wav");    break;
    case 8:    playb("80.wav");    break;
    case 9:    playb("90.wav");    break;
    }
    i=i%10;
    zerosuppress=1;
  }    
  switch(i){
    case 0:    if(!zerosuppress){playb("0.wav");}    break;
    case 1:    playb("1.wav");    break;
    case 2:    playb("2.wav");    break;
    case 3:    playb("3.wav");    break;
    case 4:    playb("4.wav");    break;
    case 5:    playb("5.wav");    break;
    case 6:    playb("6.wav");    break;
    case 7:    playb("7.wav");    break;
    case 8:    playb("8.wav");    break;
    case 9:    playb("9.wav");    break;
    case 10:    playb("10.wav");    break;
    case 11:    playb("11.wav");    break;
    case 12:    playb("12.wav");    break;
    case 13:    playb("13.wav");    break;
    case 14:    playb("14.wav");    break;
    case 15:    playb("15.wav");    break;
    case 16:    playb("16.wav");    break;
    case 17:    playb("17.wav");    break;
    case 18:    playb("18.wav");    break;
    case 19:    playb("19.wav");    break;
  }  
}

void readtillxmas(byte w,byte d){
  if((w==0)&&(d==0)){playb("mxmas.wav");return;}      //Merry Christmas instead of saying zero days
  playb("its.wav");                                   //It's
  if(w){
    readbyte(w);                                      //number of
    if(w==1){playb("w.wav");}else{playb("ws.wav");}   //week/s
  }
  if((w)&&(d)){playb("and.wav");}                     //only need 'and' if both weeks and days are said
  if(d){
    readbyte(d);                                      //number of
    if(d==1){playb("d.wav");}else{playb("ds.wav");}   //days
  }
  playb("tilxmas.wav");                               //Until christmas
}

void playb(char f[]){           //play and wait until finished (normally plays and returns)
  tmrpcm.play(f);
  while(tmrpcm.isPlaying()){}   //wait
}

void playphrase(byte n){        //play phrase 00-99
  char phrase[]="PH00.wav";
  phrase[2]=(n/10)%10+'0';
  phrase[3]=n%10+'0';
  playb(phrase);
}

void ST7920SetCursor(int x,int y){
  x=x&15;
  y=(y&1)*16+(y&2)*4;           //lines start at 0,16,8,24 respectively
  ST7920send(128|x|y,ST7920CMD);  
}

void ST7920GraphicsClear(){      //fill screen with 0's
  ST7920setGraphicsmode();
  for(int y=0;y<64;y++){
    ST7920send(128|y,ST7920CMD); //set y
    ST7920send(128,ST7920CMD);   //set x=0
    for(int x=0;x<32;x++){       //do 16x2 byte x's      
      ST7920send(0,ST7920DATA);  //send 0's
    }
  }
}

void ST7920GraphicsWriteremap(unsigned int x,unsigned int y,unsigned int d){    //remapping done to suit XC4617
  //ST7920GraphicsWrite((x&7)|((y&32)>>2),y&31,d);
  ST7920send(128|(y&31),ST7920CMD);  //set y
  ST7920send(128|(x&7)|((y&32)>>2),ST7920CMD);  //set x
  ST7920send((d>>8)&255,ST7920DATA);  //send MSB
  ST7920send((d)&255,ST7920DATA);  //send MSB  
}

void ST7920setGraphicsmode(){
  ST7920send(36,ST7920CMD);  //extended instructions
  ST7920send(38,ST7920CMD);  //graphics mode
  //stay in extended mode for GDRAM addressing
}

void ST7920setTextmode(){
  ST7920send(36,ST7920CMD);  //extended instructions
  ST7920send(36,ST7920CMD);  //text mode
  ST7920send(32,ST7920CMD);  //normal instructions    
}

void ST7920string(char *str){
  while(*str){
  ST7920send(*str++,ST7920DATA);
  }
}

void ST7920Clear(){
  ST7920send(1,ST7920CMD);    //clear
  delay(2);                   //wait for clear
}

void ST7920setup(){
  delay(40);                  //40ms for IC to stabilise on power on
  pinMode(ST7920E,OUTPUT);
  pinMode(ST7920RW,OUTPUT);
  pinMode(ST7920RS,OUTPUT);
  ST7920send(32,ST7920CMD);   //normal instructions, graphic mode off
  ST7920send(1,ST7920CMD);    //clear
  delay(2);                   //wait for clear
  ST7920send(36,ST7920CMD);   //extended instructions
  ST7920send(2,ST7920CMD);    //extended instructions- scroll off/cgram enabled  
  ST7920send(32,ST7920CMD);   //normal instructions, graphic mode off
  ST7920send(12,ST7920CMD);   //display on
}

void ST7920send(byte b,byte dc){ //dc is high for data, low for command
  digitalWrite(ST7920E,LOW);     //make sure SCL low
  digitalWrite(ST7920RS,HIGH);   //enable CS (high)
  //first byte
  digitalWrite(ST7920RW,HIGH);   //five high bits
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920RW,LOW);    //RW low for write
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  if(dc){digitalWrite(ST7920RW,HIGH);   //DC high for data
  }else{digitalWrite(ST7920RW,LOW);}   //DC low for command
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920RW,LOW);    //one low bit
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  //second byte
  if(b&128){digitalWrite(ST7920RW,HIGH);   
  }else{digitalWrite(ST7920RW,LOW);}  
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  if(b&64){digitalWrite(ST7920RW,HIGH);   
  }else{digitalWrite(ST7920RW,LOW);}  
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  if(b&32){digitalWrite(ST7920RW,HIGH);   
  }else{digitalWrite(ST7920RW,LOW);}  
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  if(b&16){digitalWrite(ST7920RW,HIGH);   
  }else{digitalWrite(ST7920RW,LOW);}  
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920RW,LOW);   //four low bits
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  //third byte
  if(b&8){digitalWrite(ST7920RW,HIGH);   
  }else{digitalWrite(ST7920RW,LOW);}  
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  if(b&4){digitalWrite(ST7920RW,HIGH);   
  }else{digitalWrite(ST7920RW,LOW);}  
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  if(b&2){digitalWrite(ST7920RW,HIGH);   
  }else{digitalWrite(ST7920RW,LOW);}  
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  if(b&1){digitalWrite(ST7920RW,HIGH);   
  }else{digitalWrite(ST7920RW,LOW);}  
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920RW,LOW);    //four low bits
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW); 
  digitalWrite(ST7920E,HIGH);    //toggle
  digitalWrite(ST7920E,LOW);   
  digitalWrite(ST7920RS,LOW);    //disable CS 
}

