#include <Wire.h>

#include <LiquidCrystal.h>

#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <Time.h>
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7 
#define IDL 0
#define MENU1 1
#define MENU2 2
#define SETTIMEONE 3
#define SETTIMETWO 4
#define SETTIMETHREE 5
#define SETALARMONE 6
#define SETALARMTWO 7
#define SETALARMTHREE 8
#define DISPLAYALARM 9
#define TOGGLEALARM 10
#define IDLWITHALARM 11
#define ALARM 12
#define ALARMTWO 13
uint8_t alarm[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00100,
  B00000,
};

int initialHours=0,initialMinutes=0,initialSeconds=0;
int initialAlarmHours=0,initialAlarmMinutes=0,initialAlarmSeconds=0;
int initial="initial";
int i=0;
long x=24*60*60L;
int state = IDL, button =0;
bool alarmToggle=0;
bool m1=0,m2=0,st1=0,st2=0,st3=0,va=0,sa=0,ta=0;
unsigned long timer1 = 0, timer2 = 0, timer3 =0, newMillis=0 ,timer4=0; 
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.setBacklight(TEAL);
  lcd.createChar(1, alarm);
}
void loop() {
  uint8_t buttons = lcd.readButtons();
  switch(state){
    case IDL:
      m1=0; m2=0;st1=0;st2=0;st3=0;va=0;sa=0;ta=0;
      lcd.setCursor(2,0);
      currentTime();
      lcd.print("SELECT FOR MENU!");
      if(buttons){
      if(buttons & BUTTON_SELECT) {state = MENU1; timer1 =millis(); lcd.clear();}
      }
      break;
    case MENU1:
      displayMenu1();
      m1=1; m2=0; st1=0;st2=0;st3=0; va=0; sa=0; ta=0;
      if(buttons){
      if(buttons & BUTTON_LEFT) {state = SETTIMEONE; timer2 =millis(); lcd.clear();}
      if(buttons & BUTTON_RIGHT) {state = SETALARMONE; timer2 =millis(); lcd.clear();}
      if(buttons & BUTTON_UP) {state = DISPLAYALARM; timer2 =millis(); lcd.clear();}
      if(buttons & BUTTON_DOWN) {state = TOGGLEALARM; timer2 =millis(); lcd.clear();}
      if(buttons & BUTTON_SELECT) {state =IDL; lcd.clear();}
      }
      if(millis()-timer1>5000){
        state =MENU2;
        lcd.clear();
      }
      break;
    case MENU2:
      displayMenu2();
      m1=0; m2=1; st1=0;st2=0;st3=0; va=0; sa=0; ta=0;
      if(buttons){
      if(buttons & BUTTON_LEFT) {state = SETTIMEONE; timer2 =millis(); lcd.clear();}
      if(buttons & BUTTON_RIGHT) {state = SETALARMONE; timer2 =millis(); lcd.clear();}
      if(buttons & BUTTON_UP) {state = DISPLAYALARM; timer2 =millis(); lcd.clear();}
      if(buttons & BUTTON_DOWN) {state = TOGGLEALARM; timer2 =millis(); lcd.clear();}
      if(buttons & BUTTON_SELECT) {state =IDL; lcd.clear();}
      }
      if(millis()-timer1>10000){
        state =IDL;
        lcd.clear();
      }
      break; 
    case SETTIMEONE:
      m1=0; m2=0; st1=1;st2=0;st3=0; va=0; sa=0; ta=0;
      setHours();
      if(millis()-timer2>60000){
        state =MENU1; 
        lcd.clear();
      }
      break;
    case SETTIMETWO:
      m1=0; m2=0; st1=0;st2=1;st3=0; va=0; sa=0; ta=0;
      setMinutes();
      if(millis()-timer3>60000){
        state =MENU1;
        lcd.clear();
      }     
      break;
    case SETTIMETHREE:
      m1=0; m2=0; st1=0;st2=0;st3=1; va=0; sa=0; ta=0;
      setSeconds();
      if(millis()-timer4>60000){
        state =MENU1;
        lcd.clear();
        timer1=0;timer2=0;timer3=0;timer4=0;
      }     
      break;
    case SETALARMONE:
      m1=0; m2=0; st1=1;st2=0;st3=0; va=0; sa=0; ta=0;
      setAlarmHours();
      if(millis()-timer2>60000){
        state =MENU1; 
        lcd.clear();
      }
      break;
    case SETALARMTWO:
      m1=0; m2=0; st1=0;st2=1;st3=0; va=0; sa=0; ta=0;
      setAlarmMinutes();
      if(millis()-timer3>60000){
        state =MENU1;
        lcd.clear();
      }     
      break;
    case SETALARMTHREE:
      m1=0; m2=0; st1=0;st2=0;st3=1; va=0; sa=0; ta=0;
      setAlarmSeconds();
      if(millis()-timer4>60000){
        state =MENU1;
        lcd.clear();
        timer1=0;timer2=0;timer3=0;timer4=0;
      }     
      break;
    case DISPLAYALARM:
      lcd.setCursor(1,1);
      lcd.print("CURRENT ALARM");
      char string_out[8];
      sprintf(string_out,"%02u:%02u:%02u",initialAlarmHours,initialAlarmMinutes,initialAlarmSeconds);
      lcd.setCursor(2,0);
      lcd.print(string_out);
      if(initialAlarmHours<12)lcd.print(" AM");
      if(initialAlarmHours==12)lcd.print(" PM");
      if(initialAlarmHours>12)lcd.print(" PM");
      if(initialAlarmHours==24)initialAlarmHours=0;
      if(buttons){
      if(buttons & BUTTON_SELECT) {state = MENU1; lcd.clear();}
      }
      if(millis()-timer2>60000){
        state =MENU1;
        lcd.clear();
        timer1=0;timer2=0;timer3=0;timer4=0;
      }     
      break;        
    case TOGGLEALARM:
      lcd.setCursor(0,0);
      lcd.print("ALARM");
      lcd.setCursor(6,0);
      if(alarmToggle==0)lcd.print("OFF");
      lcd.setCursor(9,0);
      if(alarmToggle==1)lcd.print("ON"); 
      if(buttons){
      if(buttons & BUTTON_UP) alarmToggle=1; lcd.clear();
      if(buttons & BUTTON_DOWN) alarmToggle=0; lcd.clear();
      if(buttons & BUTTON_SELECT) {state=IDL; lcd.clear();}
      }
      lcd.setCursor(0,1);
      lcd.print("UP/DOWN TO TOGGLE");
      break;
    case IDLWITHALARM:
      lcd.setCursor(2,0);
      //currentTimeWithAlarm();
      if(buttons){
      if(buttons & BUTTON_SELECT) {state = MENU1; timer3 =millis(); lcd.clear();}
      }
      break; 
    case ALARM:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("GET UP!");
      lcd.setCursor(0,1);
      lcd.print("GET UP!");
      lcd.setBacklight(RED);
      delay(100);
      lcd.setBacklight(YELLOW);
      delay(100);
      lcd.setBacklight(GREEN);
      delay(100);
      lcd.setBacklight(BLUE);
      delay(100);
      if((millis()-timer3)>30000){
        lcd.clear();timer3=0;timer4=millis();state=ALARMTWO;
      }
      if(buttons){
        if(buttons & BUTTON_DOWN){
          lcd.clear();
          if(initialAlarmMinutes>29)
            {initialAlarmMinutes=((initialAlarmMinutes+30)%60);
            initialAlarmHours=initialAlarmHours+1;
            if(initialHours==24)initialHours=0;
            state=IDL;
            lcd.setBacklight(TEAL);
            }
          else {initialAlarmMinutes=initialAlarmMinutes+30;
         state=IDL;lcd.setBacklight(TEAL);}
         }
        if(buttons & BUTTON_SELECT){lcd.clear();state=IDL;lcd.setBacklight(TEAL);}
      }
      break;
   case ALARMTWO:
      lcd.clear();   
      lcd.setCursor(0,0);
      lcd.print("DOWN TO SNOOZE");
      lcd.setCursor(0,1);
      lcd.print("SELECT TO STOP");
      lcd.setBacklight(RED);
      delay(100);
      lcd.setBacklight(YELLOW);
      delay(100);
      lcd.setBacklight(GREEN);
      delay(100);
      lcd.setBacklight(BLUE);
      delay(100);
      if((millis()-timer4)>30000){
        lcd.clear();timer3=0;timer4=0;state=IDL;lcd.setBacklight(TEAL);
      }
      if(buttons){
        if(buttons & BUTTON_DOWN){
          lcd.clear();
          if(initialAlarmMinutes>29)
            {initialAlarmMinutes=((initialAlarmMinutes+30)%60);
            initialAlarmHours=initialAlarmHours+1;
            if(initialHours==24)initialHours=0;
            state=IDL;
            lcd.setBacklight(TEAL);
            }
          else {initialAlarmMinutes=initialAlarmMinutes+30;
         state=IDL;lcd.setBacklight(TEAL);}
         }
        if(buttons & BUTTON_SELECT){lcd.clear();state=IDL;lcd.setBacklight(TEAL);}
      }
      break;
    }
  } 
void set_wash_temp(){
  uint8_t buttons = lcd.readButtons();
  int temp=40;
  char string_out[16];
  lcd.setCursor(0,1);
  sprintf(string_out,"Current temp: %02u",temp); 
  lcd.print(string_out);
  if(buttons & BUTTON_UP){
    temp=50;
  }
  
}

void displayBlink(int blinks, int duration)
{
  while(blinks--)
  {
    lcd.noDisplay();
    delay(duration);
    lcd.display();
    delay(duration);
  }
}

void currentTime(){
  char string_out[8];
  uint8_t buttons = lcd.readButtons();
      long combinedSeconds = ((initialSeconds+(initialMinutes*60L)+(initialHours*60L*60L))-(newMillis/1000L));
      lcd.setCursor(1,0);
      int seconds = ((millis()/1000)+combinedSeconds) % 60; //calculates the seconds from millis() function
      int minutes = (((millis()/1000)+combinedSeconds)/60)%60;
      int hours = ((((millis()/1000)+combinedSeconds)/60)/60)%24;
      sprintf(string_out, "%02u:%02u:%02u",hours,minutes,seconds);
      lcd.print(string_out);
      
      // setting AM/PM
      if(hours<12)lcd.print(" AM");
      if(hours==12)lcd.print(" PM");
      if(hours>12)lcd.print(" PM");
      if(hours==24)hours=0;
      lcd.setCursor(0,1);
      lcd.print("SELECT FOR MENU!   ");
      lcd.print(hours);
      if(alarmToggle==1){
        if(hours==initialAlarmHours & minutes==initialAlarmMinutes & seconds==initialAlarmSeconds){
          timer3=millis();
          state=ALARM;
        }
        lcd.setCursor(14,0);
        lcd.write(1);
        lcd.setCursor(15,0);
        lcd.write(1);
      }  
}



void displayMenu1(){
  lcd.setCursor(0,0);
      lcd.print("LEFT:Set Time     ");
      lcd.setCursor(0,1);
      lcd.print("UP:Current Alarm   ");
}

void displayMenu2(){
  lcd.setCursor(0,0);
      lcd.setCursor(0,0);
      lcd.print("RIGHT:Set Alarm  ");
      lcd.setCursor(0,1);
      lcd.print("DOWN:Alarm On   ");
}
void setHours(){
      uint8_t buttons = lcd.readButtons();
      lcd.setCursor(0,0);
      lcd.print("Enter the Time");
      lcd.setCursor(0,1);
      lcd.print("HOURS:");
      lcd.setCursor(9,1);
      lcd.print(initialHours);
      if(buttons){
      if(buttons & BUTTON_UP) {(initialHours=initialHours+1); if(initialHours==24)initialHours=0;lcd.clear();}
      if(buttons & BUTTON_DOWN) {(initialHours=initialHours-1); if(initialHours==(-1))initialHours=23; lcd.clear();}
      if(buttons & BUTTON_SELECT) {state = SETTIMETWO; timer3=millis(); lcd.clear();}
      }
}
void setMinutes(){
      uint8_t buttons = lcd.readButtons();
      lcd.setCursor(0,0);
      lcd.print("Enter the Time");
      lcd.setCursor(0,1);
      lcd.print("MINUTES:");
      lcd.setCursor(9,1);
      lcd.print(initialMinutes);
      if(buttons){
      if(buttons & BUTTON_UP) {(initialMinutes=initialMinutes+1); if(initialMinutes==60)initialMinutes=0; lcd.clear();}
      if(buttons & BUTTON_DOWN) {(initialMinutes=initialMinutes-1); if(initialMinutes==(-1))initialMinutes=59; lcd.clear();}
      if(buttons & BUTTON_SELECT) {state = SETTIMETHREE; timer4=millis();lcd.clear();}
      }
}
void setSeconds(){
      uint8_t buttons = lcd.readButtons();
      lcd.setCursor(0,0);
      lcd.print("Enter the Time");
      lcd.setCursor(0,1);
      lcd.print("SECONDS:");
      lcd.setCursor(9,1);
      lcd.print(initialSeconds);
      if(buttons){
      if(buttons & BUTTON_UP) {(initialSeconds=initialSeconds+1); if(initialSeconds==60)initialSeconds=0; lcd.clear();}
      if(buttons & BUTTON_DOWN) {(initialSeconds=initialSeconds-1); if(initialSeconds==(-1))initialSeconds=59;  lcd.clear();}
      if(buttons & BUTTON_SELECT) {state = IDL; lcd.clear();}
      }
      newMillis = millis();
}
void setAlarmHours(){
      uint8_t buttons = lcd.readButtons();
      lcd.setCursor(0,0);
      lcd.print("Enter the Alarm");
      lcd.setCursor(0,1);
      lcd.print("HOURS:");
      lcd.setCursor(9,1);
      lcd.print(initialAlarmHours);
      if(buttons){
      if(buttons & BUTTON_UP) {(initialAlarmHours=initialAlarmHours+1); if(initialAlarmHours==24)initialAlarmHours=0;lcd.clear();}
      if(buttons & BUTTON_DOWN) {(initialAlarmHours=initialAlarmHours-1); if(initialAlarmHours==(-1))initialAlarmHours=23; lcd.clear();}
      if(buttons & BUTTON_SELECT) {state = SETALARMTWO; timer3=millis(); lcd.clear();}
      }
}
void setAlarmMinutes(){
      uint8_t buttons = lcd.readButtons();
      lcd.setCursor(0,0);
      lcd.print("Enter the Alarm");
      lcd.setCursor(0,1);
      lcd.print("MINUTES:");
      lcd.setCursor(9,1);
      lcd.print(initialAlarmMinutes);
      if(buttons){
      if(buttons & BUTTON_UP) {(initialAlarmMinutes=initialAlarmMinutes+1); if(initialAlarmMinutes==60)initialAlarmMinutes=0; lcd.clear();}
      if(buttons & BUTTON_DOWN) {(initialAlarmMinutes=initialAlarmMinutes-1); if(initialAlarmMinutes==(-1))initialAlarmMinutes=59; lcd.clear();}
      if(buttons & BUTTON_SELECT) {state = SETALARMTHREE; timer4=millis();lcd.clear();}
      }
}
void setAlarmSeconds(){
      uint8_t buttons = lcd.readButtons();
      lcd.setCursor(0,0);
      lcd.print("Enter the Alarm");
      lcd.setCursor(0,1);
      lcd.print("SECONDS:");
      lcd.setCursor(9,1);
      lcd.print(initialAlarmSeconds);
      if(buttons){
      if(buttons & BUTTON_UP) {(initialAlarmSeconds=initialAlarmSeconds+1); if(initialAlarmSeconds==60)initialAlarmSeconds=0; lcd.clear();}
      if(buttons & BUTTON_DOWN) {(initialAlarmSeconds=initialAlarmSeconds-1); if(initialAlarmSeconds==(-1))initialAlarmSeconds=59;  lcd.clear();}
      if(buttons & BUTTON_SELECT) {state = TOGGLEALARM; lcd.clear();}
      }
}
