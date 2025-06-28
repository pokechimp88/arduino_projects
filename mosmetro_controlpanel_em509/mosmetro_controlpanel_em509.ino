#include <elapsedMillis.h>
#include <DMXUSB.h>
#include <Keyboard.h>
#include <KeyboardLayout.h>
int thr = 0;
int throttle;
int tflag = 0;
int speed = 0;
int vudstate = 0;
int usb_in = 0;
int usb_out = 1; 
int DS_pin = 4;
int STCP_pin = 3;
int SHCP_pin = 2 ;
int lsn = 5; // Red RP/SN Light (circuit failed to assemble)
int lsd = 6; // Doors state light (doors are open)
int lrp = 7; // Green RP light (Overload relay active)
int kru = 12; // Emergency control reverser
int rvl = 9; // Reverser shaft
int attn = 10; // Attention pedal
int horn= 11; // Horn pedal
int lhrk = 8; // RK motoion lamp
int vud = 13; // Door switch
int kv = A0; // Throttle rotary switch with resistors
int krm = A1; // Brake valve potentiometer
int rv = A2; // Reverser
int sd = 0; // Door state. 0 = Closed, 1 = VUD up (unblocked), 2 = Open (LSD turns on)
int kdl = 1; // Left doors
int kdp = 1; // Right doors
int krzd = 1; // Emergency door closing
int osv = 1; // Light on
int bell = 1; //Light off/Bell
int prog2 = 1; // Program 2 Announcement
int vrp = 1; // Reset overload relay
int vru = 1; // Backup start button (reverser must be in KRU)
int kah = 1; // Emergency drive button (no ARS)
int vzn = 1; // Pneumatic Valve No.1
int kvt = 1; // Brake perception button
int prog1 = 1; // Program 1 Announcement
int kru_in = 0; // KRU state (to prevent RCU disconnect)
int rv_in = 0; // RVL state (to prevent RCU disconnect)
int digits_r [10] {252, 96, 218, 242, 102, 182, 190, 224, 254, 246}; // digits 0-9
// to display -, send 2
// to display_, send 16


#define USB_HID_KEYS

/**
 * Modifier masks - used for the first byte in the HID report.
 * NOTE: The second byte in the report is reserved, 0x00
 */
#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LMETA  0x08
#define KEY_MOD_RCTRL  0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT   0x40
#define KEY_MOD_RMETA  0x80

/**
 * Scan codes - last N slots in the HID report (usually 6).
 * 0x00 if no key pressed.
 * 
 * If more than N keys are pressed, the HID reports 
 * KEY_ERR_OVF in all slots to indicate this condition.
 */

#define KEY_NONE 0x00 // No key pressed
#define KEY_ERR_OVF 0x01 //  Keyboard Error Roll Over - used for all slots if too many keys are pressed ("Phantom key")
// 0x02 //  Keyboard POST Fail
// 0x03 //  Keyboard Error Undefined
#define KEY_A 0x04 // Keyboard a and A
#define KEY_B 0x05 // Keyboard b and B
#define KEY_C 0x06 // Keyboard c and C
#define KEY_D 0x07 // Keyboard d and D
#define KEY_E 0x08 // Keyboard e and E
#define KEY_F 0x09 // Keyboard f and F
#define KEY_G 0x0a // Keyboard g and G
#define KEY_H 0x0b // Keyboard h and H
#define KEY_I 0x0c // Keyboard i and I
#define KEY_J 0x0d // Keyboard j and J
#define KEY_K 0x0e // Keyboard k and K
#define KEY_L 0x0f // Keyboard l and L
#define KEY_M 0x10 // Keyboard m and M
#define KEY_N 0x11 // Keyboard n and N
#define KEY_O 0x12 // Keyboard o and O
#define KEY_P 0x13 // Keyboard p and P
#define KEY_Q 0x14 // Keyboard q and Q
#define KEY_R 0x15 // Keyboard r and R
#define KEY_S 0x16 // Keyboard s and S
#define KEY_T 0x17 // Keyboard t and T
#define KEY_U 0x18 // Keyboard u and U
#define KEY_V 0x19 // Keyboard v and V
#define KEY_W 0x1a // Keyboard w and W
#define KEY_X 0x1b // Keyboard x and X
#define KEY_Y 0x1c // Keyboard y and Y
#define KEY_Z 0x1d // Keyboard z and Z

#define KEY_1 0x1e // Keyboard 1 and !
#define KEY_2 0x1f // Keyboard 2 and @
#define KEY_3 0x20 // Keyboard 3 and #
#define KEY_4 0x21 // Keyboard 4 and $
#define KEY_5 0x22 // Keyboard 5 and %
#define KEY_6 0x23 // Keyboard 6 and ^
#define KEY_7 0x24 // Keyboard 7 and &
#define KEY_8 0x25 // Keyboard 8 and *
#define KEY_9 0x26 // Keyboard 9 and (
#define KEY_0 0x27 // Keyboard 0 and )

#define KEY_ENTER 0x28 // Keyboard Return (ENTER)
#define KEY_ESC 0x29 // Keyboard ESCAPE
#define KEY_BACKSPACE 0x2a // Keyboard DELETE (Backspace)
#define KEY_TAB 0x2b // Keyboard Tab
#define KEY_SPACE 0x2c // Keyboard Spacebar
#define KEY_MINUS 0x2d // Keyboard - and _
#define KEY_EQUAL 0x2e // Keyboard = and +
#define KEY_LEFTBRACE 0x2f // Keyboard [ and {
#define KEY_RIGHTBRACE 0x30 // Keyboard ] and }
#define KEY_BACKSLASH 0x31 // Keyboard \ and |
#define KEY_HASHTILDE 0x32 // Keyboard Non-US # and ~
#define KEY_SEMICOLON 0x33 // Keyboard ; and :
#define KEY_APOSTROPHE 0x34 // Keyboard ' and "
#define KEY_GRAVE 0x35 // Keyboard ` and ~
#define KEY_COMMA 0x36 // Keyboard , and <
#define KEY_DOT 0x37 // Keyboard . and >
#define KEY_SLASH 0x38 // Keyboard / and ?
#define KEY_CAPSLOCK 0x39 // Keyboard Caps Lock

#define KEY_F1 0x3a // Keyboard F1
#define KEY_F2 0x3b // Keyboard F2
#define KEY_F3 0x3c // Keyboard F3
#define KEY_F4 0x3d // Keyboard F4
#define KEY_F5 0x3e // Keyboard F5
#define KEY_F6 0x3f // Keyboard F6
#define KEY_F7 0x40 // Keyboard F7
#define KEY_F8 0x41 // Keyboard F8
#define KEY_F9 0x42 // Keyboard F9
#define KEY_F10 0x43 // Keyboard F10
#define KEY_F11 0x44 // Keyboard F11
#define KEY_F12 0x45 // Keyboard F12

#define KEY_SYSRQ 0x46 // Keyboard Print Screen
#define KEY_SCROLLLOCK 0x47 // Keyboard Scroll Lock
#define KEY_PAUSE 0x48 // Keyboard Pause
#define KEY_INSERT 0x49 // Keyboard Insert
#define KEY_HOME 0x4a // Keyboard Home
#define KEY_PAGEUP 0x4b // Keyboard Page Up
#define KEY_DELETE 0x4c // Keyboard Delete Forward
#define KEY_END 0x4d // Keyboard End
#define KEY_PAGEDOWN 0x4e // Keyboard Page Down
#define KEY_RIGHT 0x4f // Keyboard Right Arrow
#define KEY_LEFT 0x50 // Keyboard Left Arrow
#define KEY_DOWN 0x51 // Keyboard Down Arrow
#define KEY_UP 0x52 // Keyboard Up Arrow

#define KEY_NUMLOCK 0x53 // Keyboard Num Lock and Clear
#define KEY_KPSLASH 0x54 // Keypad /
#define KEY_KPASTERISK 0x55 // Keypad *
#define KEY_KPMINUS 0x56 // Keypad -
#define KEY_KPPLUS 0x57 // Keypad +
#define KEY_KPENTER 0x58 // Keypad ENTER
#define KEY_KP1 0x59 // Keypad 1 and End
#define KEY_KP2 0x5a // Keypad 2 and Down Arrow
#define KEY_KP3 0x5b // Keypad 3 and PageDn
#define KEY_KP4 0x5c // Keypad 4 and Left Arrow
#define KEY_KP5 0x5d // Keypad 5
#define KEY_KP6 0x5e // Keypad 6 and Right Arrow
#define KEY_KP7 0x5f // Keypad 7 and Home
#define KEY_KP8 0x60 // Keypad 8 and Up Arrow
#define KEY_KP9 0x61 // Keypad 9 and Page Up
#define KEY_KP0 0x62 // Keypad 0 and Insert
#define KEY_KPDOT 0x63 // Keypad . and Delete

void setup() {
  Serial.begin(9600);
  for(int p=9; p < 13; p++) {
  pinMode(p, INPUT_PULLUP);
  }
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  pinMode(DS_pin,   OUTPUT);
  pinMode(STCP_pin, OUTPUT);
  pinMode(SHCP_pin, OUTPUT);           
}
uint8_t buf[8] ={
  0
};    /* Keyboard report buffer */

/*
Metrostroi keybinds
1,2,3,4,5,6,7 Throttle (send key press momentarily) A
Numpad1,2,3,4,5,6,7 Brake valve (on 334 6 and 7 not used, send key continuously) A
A, D Door select/open D
V Close door toggle D
G Overload relay reset D
Shift+1 Light on D
Shift+2 Light off, bell D
  Special keybind for starting train (Shift+0, then 0, Numpad 0) and turning off train (Numpad 0, 9, Shift+7)
Numpad / Backup start D
Numpad * KAH / AK button D
=,- Program 1 D
L Horn pedal D
N VZ-1 D
Space Attention pedal D
Shift+Space Brake perception button D
No keyind for: Compressor on/off, Error signal, Door interlock control, SARMAT/Vityaz/PA-M/ASNP setup to be done using mouse or numpad
Total digital pins needed: 12
Digital pins available: 8 + 4 unused analog pins

Pins 5-6: Lights: Red RP (flash briefly on throttle position key change except for throttle 0/key 4), Green RP(not simulated), Door state(if VUD open and KDL/KDP pressed)

Buttons pressed at the same time: (pins 7-13)
Alertness pedal 
VUD (pin 13)
Horn
KVT
Emergency door closing
Left doors
Right doors

A0: Throttle and KRU
A1: BrakeValve (013)
Buttons used separately (grouped) (A3-A5)
A2: Reverser position (not KRU)
Light on, Light off (Bell), Announcement program 2
OL Reset, Backup start, VZ, and KAH
Shift+7, Shift+8, Announce program 1
*/

// int state = 1;

void analogButtons(int A_PIN, int& b1, int& b2, int& b3, int& b4) {
  int pinVal = resistance(A_PIN);
  if(round(pinVal/100)*100 == round(220/100)*100){
    b1 = LOW;
  } else {
    b1 = HIGH;
  }
  if(round(pinVal/100)*100 == round(470/100)*100){
    b2 = LOW;
  } else {
    b2 = HIGH;
  }
  if(round(pinVal/100)*100 == round(3900/100)*100){
    b3 = LOW;
  } else {
    b3 = HIGH;
  }
  if(round(pinVal/100)*100 == round(10000/100)*100){
    b4 = LOW;
  } else {
    b4 = HIGH;
  }
}

void sendKey(int state, int keycode, int mod = 0)
{
  if (state == LOW)
  {
    if (mod != 0)
  {
    buf[0] = mod;     // Shift key
    Serial.write(buf, 8 );   // Send keypress
  }
    buf[2] = keycode;     // W key
    Serial.write(buf, 8 );   // Send keypress
  }
  else
  {
    releaseKey();
  }
}
void pressedKey(int keycode, int mod = 0) {
    if (mod != 0)
  {
    buf[0] = mod;     // Shift key
    Serial.write(buf, 8 );   // Send keypress
  }
    buf[2] = keycode;     // W key
    Serial.write(buf, 8 );   // Send keypress
}

void releaseKey()
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8 );   // Release key
}

void render(int r_d,int r_e,int r_ms) { 
  for(int rt = 0; rt < r_ms; rt++) {
  digitalWrite(STCP_pin,HIGH);
  shiftOut(DS_pin,SHCP_pin,LSBFIRST,r_d);
  digitalWrite(STCP_pin, LOW);
  delay(1);
  digitalWrite(STCP_pin,HIGH);
  shiftOut(DS_pin,SHCP_pin,LSBFIRST,r_e+1);
  digitalWrite(STCP_pin, LOW);

  }
}

void display(int spd, int time) {
  render(digits_r[(spd / 1U) % 10],digits_r[(spd / 10U) % 10],time);
}

double resistance(int pin) {
  auto Value= analogRead(pin);
  double buffer= Value * 5;
  double Vout= (buffer)/1024.0;
  buffer= (5/Vout) -1;
  double Rtest= 1000 * buffer;
  return Rtest;
}
/*
double voltage(int pin) {
  int Input  = analogRead(A0); // Read the analog  value 
  double voltage = Input * (5.0 / 1024.0) * 10;
  return voltage;
}
*/
void rotarySwitch(int pin, int type) {
  int pinVal = resistance(pin);
  if (type==1){
    switch(round(pinVal/100)*100) {
      case round(3900/100)*100: pressedKey(KEY_KP1);break;
      case round(2200/100)*100: pressedKey(KEY_KP2);break;
      case round(1500/100)*100: pressedKey(KEY_KP3);break;
      case round(1000/100)*100: pressedKey(KEY_KP4);break;
      case round(470/100)*100: pressedKey(KEY_KP5);break;
      case round(220/100)*100: pressedKey(KEY_KP6);break;
      case round(100/100)*100: pressedKey(KEY_KP7);break;
      default: releaseKey();
    }
  }
  if (type==0){
    if(round(pinVal/100)*100 == round(1500/100)*100){
      throttle = 1;
      pressedKey(KEY_1);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(2200/100)*100){
      throttle = 2;
      pressedKey(KEY_2);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(3900/100)*100){
      throttle = 3;
      pressedKey(KEY_3);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(1000/100)*100){
      throttle = 0;
      pressedKey(KEY_4);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(470/100)*100){
      throttle = 5;
      pressedKey(KEY_5);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(220/100)*100){
      throttle = 6;
      pressedKey(KEY_6, KEY_MOD_LSHIFT);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(100/100)*100){
      throttle = 7;
      pressedKey(KEY_7);
    } else {
      releaseKey();
    }
  }
  if (type==3){
    if(round(pinVal/100)*100 == round(1500/100)*100){
      pressedKey(KEY_1);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(2200/100)*100){
      pressedKey(KEY_2);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(3900/100)*100){
      pressedKey(KEY_3);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(10000/100)*100){
      pressedKey(KEY_4);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(1000/100)*100){
      pressedKey(KEY_5);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(470/100)*100){
      pressedKey(KEY_6);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(220/100)*100){
      pressedKey(KEY_7);
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(100/100)*100){
      pressedKey(KEY_8);
    } else {
      releaseKey();
    }
  }
  if (type==2){
    int prev = 1;
    if(round(pinVal/100)*100 == round(20000/100)*100){
      pressedKey(KEY_9);
      prev = 2;
      delay(5);
      releaseKey();
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(10000/100)*100){
      if(prev==0) pressedKey(KEY_9);
      if(prev==2) pressedKey(KEY_0);
      prev = 1;
      delay(5);
    releaseKey();
    } else {
      releaseKey();
    }
    if(round(pinVal/100)*100 == round(3900/100)*100){
      pressedKey(KEY_0);
      prev = 0;
      delay(5);
    releaseKey();
    } else {
      releaseKey();
    }
  }
}

void checkReverser() {
  if(digitalRead(kru)==LOW&& kru_in == 0) {
    pressedKey(KEY_8, KEY_MOD_LSHIFT);
    delay(5);
    releaseKey();
    kru_in = 1;
    rv_in = 0;
  }if(digitalRead(kru)==HIGH && kru_in == 1){
    pressedKey(KEY_7, KEY_MOD_LSHIFT);
    delay(5);
    releaseKey();
    kru_in = 0;
  }
  if(digitalRead(rvl)==LOW && rv_in == 0) {
    pressedKey(KEY_9, KEY_MOD_LSHIFT);
    delay(5);
    releaseKey();
    rv_in = 1;
    kru_in = 0;
  }if(digitalRead(rvl)==HIGH && rv_in == 1) {
    pressedKey(KEY_7, KEY_MOD_LSHIFT);
    delay(5);
    releaseKey();
    rv_in = 0;
  }
}

void checkSpeed() {
  if (speed<0) speed = 0;
  if (throttle != thr) digitalWrite(lsn, 1); delay(10); digitalWrite(lsn, 0); // circuit failure simulated
  if (throttle==1 && speed < 58) {speed += 1.98;}
  else if (throttle==2 && speed < 64) {speed += 3.96; tflag = 1;}
  else if (throttle==2 && speed >= 64) {speed += 1.98; tflag = 1;}
  else if (throttle==3 && speed < 92) {speed += 3.96; tflag = 1;}
  else if (throttle==3 && speed >= 92 && speed < 120) { speed += 1.23; tflag = 1;}
  else if (throttle==0) { tflag=0; speed -= 0.01; digitalWrite(lsn, 0);}
  else if (throttle==5) { speed -= 2.16;}
  else if (throttle==6 && tflag == 0) { speed -= 3.96; tflag = 1;}
  else if (throttle==6 && tflag == 1) {speed -= 1.23;}
  else if (throttle==7 && speed > 10) {speed -= 4.32; tflag = 1;}
  else if (throttle==7 && speed <= 10) {speed -= 1.98; tflag = 1;}
  else {speed-= 0.01; digitalWrite(lsn, 1);}
  if (vzn==0) {speed -= 2.16; digitalWrite(lsn, 1);}
  digitalWrite(lhrk, tflag);
  display(round(speed), 100); // display speed for certain time
  thr = throttle;
}

void checkLights() {
  int rpstate = 0;// get state of Green RP light
  int snstate = 0;//get state of Red RP light
  if (vrp==0) rpstate = 0;
  if (vudstate==1){
    if (kdl == 0 || kdp == 0){
      sd = 2; // after kdl/kdp pressed, LSD turns on
    }else{
      sd = 1; // if vud opens LSD can turn on later
    }}else{
    sd = 0; // VUD closed, LSD off
  }
  if (sd==2) {
    digitalWrite(lsd, 1);
  }else{
      digitalWrite(lsd, 0);
  }
  digitalWrite(lsn, snstate);
  digitalWrite(lrp, rpstate);
}

void toggleSwitch(int pin, int key) {
  if(pin==0&&vudstate==0) {
    pressedKey(key);
    delay(5);
    releaseKey();
    vudstate = 1; // VUD closed
  }
  if (pin==1&&vudstate==1) {
    pressedKey(key);
    delay(5);
    releaseKey();
    vudstate = 0; // VUD opened
  }
}

void loop() { 
  analogButtons(A3, osv, bell, prog2, kvt);
  analogButtons(A4, vrp, vru, kah, vzn);
  analogButtons(A5, kdl, kdp, krzd, prog1);
  sendKey(kdl, KEY_A);
  sendKey(kdp, KEY_D);
  sendKey(vrp, KEY_G);
  sendKey(krzd, KEY_V, KEY_MOD_LSHIFT);
  sendKey(digitalRead(horn), KEY_L);
  sendKey(digitalRead(attn), KEY_SPACE);
  sendKey(kvt, KEY_SPACE, KEY_MOD_LSHIFT);
  sendKey(kah, KEY_KPASTERISK);
  sendKey(vru, KEY_KPSLASH);
  sendKey(osv, KEY_1, KEY_MOD_LSHIFT);
  sendKey(bell, KEY_2, KEY_MOD_LSHIFT);
  sendKey(prog1, KEY_EQUAL);
  sendKey(prog2, KEY_MINUS);
  rotarySwitch(kv, 0);
  rotarySwitch(krm, 1);
  rotarySwitch(A2, 2);
  toggleSwitch(vud, KEY_V);
  checkReverser();
  checkSpeed();
  checkLights();
} 
