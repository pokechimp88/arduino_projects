#include <SoftwareSerial.h>
#include "RedMP3.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
#define MP3_RX 5//RX of Serial MP3 module connect to D7 of Arduino
#define MP3_TX 6//TX to D8, note that D8 can not be used as RX on Mega2560, you should modify this if you donot use Arduino UNO
MP3 mp3(MP3_RX, MP3_TX);

char* sep = ' - ';
char* stations[24]  = {
  "Snowdon arr.",
  "Snowdon dep.",
  "Cote-Neiges arr.",
  "Cote-Neiges dep.",
  "Uni-Montreal arr",
  "Uni-Montreal dep",
  "Ed-Montpetit arr",
  "Ed-Montpetit dep",
  "Outremont arr.",
  "Outremont dep.",
  "Acadie arr.",
  "Acadie dep.",
  "Parc arr.",
  "Parc dep.",
  "De Castelnau arr",
  "De Castelnau dep",
  "Jean-Talon arr.",
  "Jean-Talon dep.",
  "Fabre arr.",
  "Fabre dep.",
  "D'Iberville arr.",
  "D'Iberville dep.",
  "Saint-Michel arr",
  "Saint-Michel dep",
};

char* stn[12]  = {
  "Snowdon",
  "Cote-des-Neiges",
  "Univ...Montreal",
  "Ed...-Montpetit",
  "Outremont",
  "Acadie",
  "Parc",
  "De Castelnau",
  "Jean-Talon",
  "Fabre",
  "D'Iberville",
  "Saint-Michel",
};

int8_t index  = 1;//the first station
int8_t id = 1;
int8_t newIndex  = 0;//alt station announcements
int8_t endIndex  = 23;//the last station
int8_t reverse = 0; //0=forward, 1=reverse
int8_t volume = 0x05;//0~0x1e (30 adjustable level)
int8_t annPlay = 1; //announcer play button state
int8_t DOWN = 1; //announcer button down state
int8_t UP = 1; //announcer button up state
int8_t state = 0; //announcer state
void setup()
{
  pinMode(11, INPUT_PULLUP); //+
  pinMode(12, INPUT_PULLUP); //-
  pinMode(13, INPUT_PULLUP); //enter/play
  Serial.begin(9600);
  lcd.init();
  // Make sure backlight is on       
  lcd.backlight();
  lcd.setContrast(100);
  // Clears the LCD screen
  lcd.clear();
  // Print a message to the lcd.
  lcd.print("Ligne Bleue STM");
  lcd.setCursor(0,1);
  lcd.print("Afficheur v1.0.0");
  lcd.setCursor(0,0);
  delay(1000);//Requires 500ms to wait for the MP3 module to initialize  
  annSetup(); //selectionner les stations
  mp3.playWithVolume(28,volume); //beep
  delay(1000);
}

void nextIndex() {
  if(newIndex>0) newIndex=0;

  if (index<endIndex && reverse==0) {
    index++; //standard forward operation
    id++;
  }
  else if (index==endIndex) { //last station
    index-=3; //go back
    id-=3;
    reverse = 1;
  }
  else if (reverse==1&&index<23) { //reverse direction
    if(index%2==0) {
      index-=3;
      id++;
    }
    else {
      id-=3;
      index++;
    }
    generateNewIndex();
  }
}

void prevIndex() {
  if(newIndex>0) newIndex=0;

  if (index>0 && reverse==0) {
    id--;
    index--; //standard forward operation
  }
  else if (index==0) { //last station
    index--; //go back
    id+=3;
    reverse = 1;
  }
  else if (reverse==1&&index<23) { //reverse direction
    if(index%2==0) {
      id+=3;
      index--;
    }else {
      id--;
      index+=3;
    }
    generateNewIndex();
  }
}

void generateNewIndex() {
  if(index==15) newIndex=2; //jean talon correction
  if(index==-1) newIndex=3; //next snowdon correction
  if(index==0) newIndex=1; //snowdon correction
  if(index==-3) {
    index=1; reverse=0; 
    }//reset
  if(id==23) {
    index=23; reverse=0; 
    }//reset
}

void annSetup() {
  lcd.clear();
  while(true) {
    if(state==0) {
      lcd.print("Direction:");
      lcd.setCursor(0,1);
      if(reverse) lcd.print("-> Snowdon");
      if(!reverse) lcd.print("-> Saint-Michel");
      lcd.setCursor(0,0);
      if(digitalRead(12)==HIGH&&DOWN==LOW) {
        lcd.clear();
        id=23;
        index=23;
        reverse=1;
      }
      if(digitalRead(11)==HIGH&&UP==LOW) {
        lcd.clear();
        id=1;
        index=1;
        reverse=0;
      }
    }else if(state==1) {
      lcd.print("Stn. de depart:");
      lcd.setCursor(0,1);
      lcd.print(stations[id]);
      lcd.setCursor(0,0);
      if(id>23) id=0;
      if(id<0) id=23;
      if(digitalRead(12)==HIGH&&DOWN==LOW) {
        lcd.clear();
        prevIndex();
      }
      if(digitalRead(11)==HIGH&&UP==LOW) {
        lcd.clear();
        nextIndex();
      }
    }else if(state==2) {
      lcd.print("Volume:");
      lcd.setCursor(0,1);
      lcd.print(volume);
      lcd.setCursor(0,0);
      if(digitalRead(12)==HIGH&&DOWN==LOW&&volume>0) {
          lcd.clear();
          volume--;
        }
        if(digitalRead(11)==HIGH&&UP==LOW&&volume<31) {
          lcd.clear();
          volume++;
        }
    }else if(state==3) {
      //setup est fini         
      break;
    }
    if(digitalRead(13)==HIGH&&annPlay==LOW) {
      lcd.clear();
      state++;
    }
    UP = digitalRead(11);
    DOWN = digitalRead(12);
    annPlay = digitalRead(13);
  }
}

void announce() {
    Serial.println(index);
  Serial.println(newIndex);
  Serial.println(reverse);
  if(index==1&&reverse==0) newIndex=25; //cote des neiges correction
  if(index==22&&reverse==0) newIndex=26; //st michel correction
  if(index==23) newIndex=27; //st michel correction
  if(index%2==1) {
    mp3.playWithVolume(29,volume); //dou dou dou
    delay(5000);
    }
  if(newIndex>0) {
    mp3.playWithVolume(newIndex,volume);
  }
  else mp3.playWithVolume(index+3,volume);
  delay(5000);
  nextIndex();
  lcd.clear();
}

void loop() //announcement system control
{
  lcd.print(stations[id]);
  lcd.setCursor(0,1);
  if(reverse) lcd.print("-> Snowdon");
  if(!reverse) lcd.print("-> Saint-Michel");
  lcd.setCursor(0,1);
  if(digitalRead(12)==HIGH&&DOWN==LOW) {
    lcd.clear();
    prevIndex();
  }
  if(digitalRead(11)==HIGH&&UP==LOW) {
    lcd.clear();
    nextIndex();
  }
  if(digitalRead(13)==HIGH&&annPlay==LOW) {
    lcd.print("<<< ANNONCE >>>");
    announce();
  }
  lcd.setCursor(0,0);
  UP = digitalRead(11);
  DOWN = digitalRead(12);
  annPlay = digitalRead(13);
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/