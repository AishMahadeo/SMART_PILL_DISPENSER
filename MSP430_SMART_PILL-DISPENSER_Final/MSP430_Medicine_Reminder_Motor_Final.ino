
#include <LiquidCrystal.h>

#include <Servo.h> 
Servo myservo;
int pos = 0;

LiquidCrystal lcd(P2_7, P3_2, P6_6, P1_6, P3_3, P3_4);

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;


const int Buzzer = P1_2; 

//const int Servo_M = P1_0; 
const int IR_Sensor = P1_3; 

const int IN1 = P2_5; 
const int IN2 = P2_4; 
const int IN3 = P1_4;
const int IN4 = P1_5;  

const int Sw_1 = P6_0; 
const int Sw_2 = P6_1; 
const int Sw_3 = P6_2; 
const int Sw_4 = P6_3; 

int Current_Hour=0;
int Current_minute=0;
int Current_Second=0;

int Medicine_A_HH_1=18;
int Medicine_A_MM_1=48;

int Medicine_B_HH_1=18;
int Medicine_B_MM_1=49;


int Medicine_A_HH_2=18;
int Medicine_A_MM_2=50;

int Medicine_B_HH_2=18;
int Medicine_B_MM_2=51;

int Take_Medicine_A=0;
int Take_Medicine_B=0;

int Motor_A_Process=0;
int Motor_B_Process=0;
void setup() 
{

    Serial.begin(9600);
    Serial.println("ShriZero@3327...");
    pinMode(Buzzer, OUTPUT); 
//    pinMode(Sw_1, INPUT);
//    pinMode(Sw_2, INPUT); 
//    pinMode(Sw_3, INPUT); 
//    pinMode(Sw_4, INPUT); 
    
  pinMode(Sw_1,  INPUT_PULLUP);
  pinMode(Sw_2, INPUT_PULLUP);
  pinMode(Sw_3, INPUT_PULLUP);
  pinMode(Sw_4, INPUT_PULLUP);
    digitalWrite(Buzzer, 1);  
    digitalWrite(Sw_1, 1);  
    digitalWrite(Sw_2, 1);  
    digitalWrite(Sw_3, 1);  
    digitalWrite(Sw_4, 1);
    
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
    myservo.attach(P2_0);
    delay(1000);        
    myservo.write(0);        
    digitalWrite(Buzzer, 0);   
    lcd.begin(20, 4);
    
    lcd.print("Medicine Reminder");
    
  Wire.begin();       //I2C
  
  rtc.begin();                       
     if (! rtc.begin())
     {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
     //rtc.adjust(DateTime(2021, 10, 19, 0, 6, 0));
  }
   
    delay(2000);  
  lcd.clear();
 // while(1)stepper_motor();
}


void loop() 
{
  if (digitalRead(Sw_4) == 0)Set_time();
    RTC_Time_Read();
     lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Time: ");
    lcd.print(Current_Hour);
    lcd.print(":");
    lcd.print(Current_minute);
    lcd.print(":");
    lcd.print(Current_Second); 
    lcd.setCursor(0, 2); 
    lcd.print("M A 1>");  
    lcd.print(Medicine_A_HH_1);
    lcd.print(":");
    lcd.print(Medicine_A_MM_1);
    lcd.setCursor(13, 2); 
    lcd.print("2>");
    lcd.print(Medicine_A_HH_2);
    lcd.print(":");
    lcd.print(Medicine_A_MM_2);
    lcd.setCursor(0, 3); 
    lcd.print("M B 1>");  
    lcd.print(Medicine_B_HH_1);
    lcd.print(":");
    lcd.print(Medicine_B_MM_1);
    lcd.setCursor(13, 3); 
    lcd.print("2>");
    lcd.print(Medicine_B_HH_2);
    lcd.print(":");
    lcd.print(Medicine_B_MM_2);
    
    Check_Time();  
    
    if((Take_Medicine_A==1 && Motor_A_Process==0) ||
       (Take_Medicine_B==1 && Motor_B_Process==0))
      Motor_Process(); 
    
    delay(500);     
    
}

void Set_time()
{
  int Exit=0;

   Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_4) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Medicine_A_HH_1++;
     if (digitalRead(Sw_2) == 0)Medicine_A_HH_1--;
     if(Medicine_A_HH_1>=24)Medicine_A_HH_1=0;
     if(Medicine_A_HH_1<0)Medicine_A_HH_1=23;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Medicine A T1:HH ");    
      lcd.setCursor(8, 2);
      lcd.print(Medicine_A_HH_1);
      lcd.print("  ");
     delay(200);
  }

  
   Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_4) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Medicine_A_MM_1++;
     if (digitalRead(Sw_2) == 0)Medicine_A_MM_1--;
     if(Medicine_A_MM_1>=60)Medicine_A_MM_1=0;
     if(Medicine_A_MM_1<0)Medicine_A_MM_1=59;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Medicine A T1:MM ");    
      lcd.setCursor(8, 2);
      lcd.print(Medicine_A_MM_1);
      lcd.print("  ");
     delay(200);
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Time Set For ");
  lcd.setCursor(0, 1);
  lcd.print("  Medicine A: T1");
  lcd.setCursor(6, 2);
  lcd.print(Medicine_A_HH_1);
  lcd.print(':');
  lcd.print(Medicine_A_MM_1);
  delay(2000);
   Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_4) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Medicine_A_HH_2++;
     if (digitalRead(Sw_2) == 0)Medicine_A_HH_2--;
     if(Medicine_A_HH_2>=24)Medicine_A_HH_2=0;
     if(Medicine_A_HH_2<0)Medicine_A_HH_2=23;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Medicine A T2:HH ");    
      lcd.setCursor(8, 2);
      lcd.print(Medicine_A_HH_2);
      lcd.print("  ");
     delay(200);
  }

  
   Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_4) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Medicine_A_MM_2++;
     if (digitalRead(Sw_2) == 0)Medicine_A_MM_2--;
     if(Medicine_A_MM_2>=60)Medicine_A_MM_2=0;
     if(Medicine_A_MM_2<0)Medicine_A_MM_2=59;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Medicine A T2:MM ");    
      lcd.setCursor(8, 2);
      lcd.print(Medicine_A_MM_2);
      lcd.print("  ");
     delay(200);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Time Set For ");
  lcd.setCursor(0, 1);
  lcd.print("  Medicine A: T2");
  lcd.setCursor(6, 2);
  lcd.print(Medicine_A_HH_2);
  lcd.print(':');
  lcd.print(Medicine_A_MM_2);
  delay(2000);
  ///////////////


   Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_4) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Medicine_B_HH_1++;
     if (digitalRead(Sw_2) == 0)Medicine_B_HH_1--;
     if(Medicine_B_HH_1>=24)Medicine_B_HH_1=0;
     if(Medicine_B_HH_1<0)Medicine_B_HH_1=23;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Medicine B T1:HH ");    
      lcd.setCursor(8, 2);
      lcd.print(Medicine_B_HH_1);
      lcd.print("  ");
     delay(200);
  }

  
   Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_4) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Medicine_B_MM_1++;
     if (digitalRead(Sw_2) == 0)Medicine_B_MM_1--;
     if(Medicine_B_MM_1>=60)Medicine_B_MM_1=0;
     if(Medicine_B_MM_1<0)Medicine_B_MM_1=59;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Medicine B T1:MM ");    
      lcd.setCursor(8, 2);
      lcd.print(Medicine_B_MM_1);
      lcd.print("  ");
     delay(200);
  }



  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Time Set For ");
  lcd.setCursor(0, 1);
  lcd.print("  Medicine B: T1");
  lcd.setCursor(6, 2);
  lcd.print(Medicine_B_HH_1);
  lcd.print(':');
  lcd.print(Medicine_B_MM_1);
  delay(2000);
   Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_4) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Medicine_B_HH_2++;
     if (digitalRead(Sw_2) == 0)Medicine_B_HH_2--;
     if(Medicine_B_HH_2>=24)Medicine_B_HH_2=0;
     if(Medicine_B_HH_2<0)Medicine_B_HH_2=23;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Medicine B T2:HH ");    
      lcd.setCursor(8, 2);
      lcd.print(Medicine_B_HH_2);
      lcd.print("  ");
     delay(200);
  }

  
   Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_4) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Medicine_B_MM_2++;
     if (digitalRead(Sw_2) == 0)Medicine_B_MM_2--;
     if(Medicine_B_MM_2>=60)Medicine_B_MM_2=0;
     if(Medicine_B_MM_2<0)Medicine_B_MM_2=59;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Medicine B T2:MM ");    
      lcd.setCursor(8, 2);
      lcd.print(Medicine_B_MM_2);
      lcd.print("  ");
     delay(200);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Time Set For ");
  lcd.setCursor(0, 1);
  lcd.print("  Medicine B: T2");
  lcd.setCursor(6, 2);
  lcd.print(Medicine_B_HH_2);
  lcd.print(':');
  lcd.print(Medicine_B_MM_2);
  delay(2000);
   ////////////
   
  Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_3) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Current_Hour++;
     if (digitalRead(Sw_2) == 0)Current_Hour--;
     if(Current_Hour>=24)Current_Hour=0;
     if(Current_Hour<0)Current_Hour=23;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Current Hour: ");
      lcd.print(Current_Hour);
      lcd.print("  ");
     delay(200);
  }

  
  Exit=0;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Seting Mode");
  while(digitalRead(Sw_3) == 0);
  while(Exit==0)
  {
     if (digitalRead(Sw_1) == 0)Current_minute++;
     if (digitalRead(Sw_2) == 0)Current_minute--;
     if(Current_minute>=60)Current_minute=0;
     if(Current_minute<0)Current_minute=59;
     if (digitalRead(Sw_3) == 0)Exit=1;     
      lcd.setCursor(0, 1);
      lcd.print("Set Current Min: ");
      lcd.print(Current_minute);
      lcd.print("  ");
     delay(200);
  }
  rtc.adjust(DateTime(2021, 10,21, Current_Hour, Current_minute, 0));
         
  
}

void RTC_Time_Read()
{
    DateTime now = rtc.now();
    Current_Hour=(now.hour());
    Current_minute=(now.minute());
    Current_Second=(now.second());
          
}


void Check_Time()
{
  if(Current_Hour==Medicine_A_HH_1 && Current_minute==Medicine_A_MM_1)Take_Medicine_A=1;
  else if(Current_Hour==Medicine_A_HH_2 && Current_minute==Medicine_A_MM_2)Take_Medicine_A=1;
  else {Take_Medicine_A=0;Motor_A_Process=0;}
  
  if(Current_Hour==Medicine_B_HH_1 && Current_minute==Medicine_B_MM_1)Take_Medicine_B=1;
  else if(Current_Hour==Medicine_B_HH_2 && Current_minute==Medicine_B_MM_2)Take_Medicine_B=1;
  else {Take_Medicine_B=0;Motor_B_Process=0;}



  if(Take_Medicine_A==1 || Take_Medicine_B==1 )
  {
    lcd.setCursor(0, 1);
    lcd.print("Medicine: ");
    
        if(Take_Medicine_A==1 && Motor_A_Process==0)
        {
          lcd.setCursor(9, 1);
          lcd.print("A");
    digitalWrite(Buzzer, 1);
        }
        else 
        {
          lcd.setCursor(9, 1);
          lcd.print("-");
        }  
  
        if(Take_Medicine_B==1 && Motor_B_Process==0)
        {
          lcd.setCursor(11, 1);
          lcd.print("B");
    digitalWrite(Buzzer, 1);
        }
        else 
        {
          lcd.setCursor(11, 1);
          lcd.print("-");
        }  
  
        delay(100); 
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("********************");
  }

digitalWrite(Buzzer, 0);  
}


void Motor_Process()
{
   if(digitalRead(IR_Sensor) == 0)
   {
    lcd.setCursor(19, 0);
    lcd.print("*");
    stepper_motor();
   }
   else
   {

    lcd.setCursor(19, 0);
    lcd.print("-");
   }
}

void stepper_motor()
{
  int count=0;
  
    lcd.setCursor(0, 1);
    lcd.print("Please Wait       ");
  if(Take_Medicine_A==1)count=40;
  if(Take_Medicine_B==1)count=40;
 while(count!=0)
 { 
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 1);
    delay(10); 
  
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
    delay(10); 
  
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 1);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
    delay(10); 
  
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 1);
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
    delay(10);
    count--;     
 }

 for(pos = 0; pos < 90; pos += 1)  
  {                                 
    myservo.write(pos);              
    delay(15);                        
  } 
  
    lcd.setCursor(0, 1);
    lcd.print("Collect Medicine    ");
  delay(2000);
  
  for(pos = 90; pos>=1; pos-=1)      
  {                                
    myservo.write(pos);               
    delay(15);                        
  } 
    
  lcd.setCursor(0, 1);
  lcd.print("Thank You          ");
  if(Take_Medicine_A==1){count=111;Motor_A_Process=1;}
  if(Take_Medicine_B==1){count=148;Motor_B_Process=1;}

// while(count!=0)
// { 
//    digitalWrite(IN1, 1);
//    digitalWrite(IN2, 1);
//    digitalWrite(IN3, 1);
//    digitalWrite(IN4, 0);
//    delay(10); 
//  
//    digitalWrite(IN1, 1);
//    digitalWrite(IN2, 1);
//    digitalWrite(IN3, 0);
//    digitalWrite(IN4, 1);
//    delay(10); 
//  
//    digitalWrite(IN1, 1);
//    digitalWrite(IN2, 0);
//    digitalWrite(IN3, 1);
//    digitalWrite(IN4, 1);
//    delay(10); 
//  
//    digitalWrite(IN1, 0);
//    digitalWrite(IN2, 1);
//    digitalWrite(IN3, 1);
//    digitalWrite(IN4, 1);
//    delay(10);
//    count--;     
// }
}
