//Coded by Alestor Aldous/Alestor-Tech-Industries
//You are free to use and modify this code


#include <SoftReset.h>
#include "U8glib.h"
#include <SoftwareSerial.h>
#include <MemoryFree.h>
//#include "string.h"
//#include <stdlib.h>
//#include <avr/wdt.h>


#define nextButton 7      //define the pin numbers for the buttons
#define selectButton 6
#define previousButton 5
#define menuButton 4

SoftwareSerial bluetooth(2,3);    //Use software serial for bluetooth



const unsigned char PROGMEM ICON_BITMAP_1[] = {     //Android Logo
0x00,0x00,
0x00,0x00,
0x04,0x40,
0x07,0xc0,
0x0f,0xe0,
0x0b,0xa0,
0x1f,0xf0,
0x00,0x00,
0x5f,0xf4,
0x5f,0xf4,
0x5f,0xf4,
0x5f,0xf4,
0x1f,0xf0,
0x0c,0x60,
0x0c,0x60,
0x00,0x00
};


const unsigned char PROGMEM ICON_BITMAP_20[] = {    //chat
0x00,0x00,
0x00,0x00,
0x3f,0xf8,
0x40,0x04,
0x40,0x04,
0x40,0x04,
0x40,0x04,
0x49,0x24,
0x40,0x04,
0x40,0x04,
0x40,0x04,
0x3f,0xc8,
0x00,0x28,
0x00,0x18,
0x00,0x08,
0x00,0x00
}; 


const unsigned char PROGMEM ICON_BITMAP_32[] = {      //cat
0x00,0x00,
0x00,0x00,
0x60,0x0c,
0x50,0x14,
0x48,0x24,
0x47,0xc4,
0x40,0x04,
0x40,0x04,
0x48,0x24,
0x48,0x24,
0x00,0x00,
0x42,0x84,
0x01,0x00,
0x60,0x0c,
0x18,0x30,
0x00,0x00
}; 

const unsigned char PROGMEM ICON_BITMAP_65[] = {    //bar signal
0x00,0x00,
0x00,0x00,
0x3f,0xe0,
0x10,0x40,
0x08,0x80,
0x05,0x00,
0x02,0x00,
0x02,0x00,
0x02,0x00,
0x00,0x0c,
0x00,0x6c,
0x03,0x6c,
0x1b,0x6c,
0x1b,0x6c,
0x00,0x00,
0x00,0x00
};

const uint8_t bluetoothlogo[] U8G_PROGMEM = 
{
  0000011000, //1
  0000010100, //2
  1000010010, //3 
  0100010001, //4
  0010010010, //5
  0001010100, //6
  0000111000, //7
  0001010100, //8
  0010010010, //9
  0100010001, //11
  1000010010, //12
  0000010100, //13
  0000011000  //14
};

boolean clockUp = true;
boolean menuShow = false;
boolean selectButtonbool = false;
boolean newMessage = false;

byte hours = 0;
byte minutes = 0;
byte seconds = 0;
byte day = 0;
byte date = 0;
byte month = 0;
int year = 0;


char DateBuffer[30];
char* TimeStorage [6];
char* vout;

String monthw;
String message;     //Variable for SMS messages
String blReceived;    //Storage for strings received from bluetooth



char msg[150];    //Char array for SMS

byte menuSelection = 0;   //Menu
byte menuSelections = 0;  //Settings
byte menuSelectiona = 0;  //Apps
byte len;         //Length of message



static unsigned long lastTick = 0;

//U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // I2C / TWI
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
//U8GLIB_SH1106_128X64_2X u8g(13, 11, 10, 9,8); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8

void setup(void) {
  // flip screen, if required
  //u8g.setRot180();


  pinMode(nextButton,INPUT);  //Set digital pins as inputs
  pinMode(selectButton,INPUT);
  pinMode(previousButton,INPUT);
  pinMode(menuButton,INPUT);
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  digitalWrite(nextButton,HIGH);  //Enable internal pull up on all buttons
  digitalWrite(selectButton,HIGH);
  digitalWrite(previousButton,HIGH);
  digitalWrite(menuButton,HIGH);
  drawStartUp();
  delay(1000);
  

 
}//end of setup

void loop() {

  //Serial.println("Running");
  //Serial.print("Memory=");
  //Serial.println(freeMemory());
  if(bluetooth.available() > 0 /*&& bluetooth.find("(")*/)
  {
    blReceived = bluetooth.readString();
    if(blReceived.startsWith("1"))    //1 is the preamble date
    {
      Serial.println("Time");

      blReceived.toCharArray(DateBuffer,blReceived.length()+1);
      vout = strtok(DateBuffer,",");
      for(int i=0;i<=6;i++)
      {
        //if(TimeStorage[i] = NULL)
        //{Serial.println("Exited Loop");
        //  break;}
        TimeStorage[i] = strtok(NULL,",");
        Serial.print("i=");
        Serial.println(i);
        Serial.println(TimeStorage[i]);
        delay(10);
      }
      //int example = bluetooth.parseInt();
      day = atoi(TimeStorage[0] - 1);
      date = atoi(TimeStorage[1]);
      month = atoi(TimeStorage[2]);
      year = atoi(TimeStorage[3]);
      hours = atoi(TimeStorage[4]);
      minutes = atoi(TimeStorage[5]);
      seconds = atoi(TimeStorage[6]);
      
    
      Serial.println(day-1);
      Serial.println(date);
      Serial.println(month);
      Serial.println(year);
      Serial.println(hours);
      Serial.println(minutes);
      Serial.println(seconds);


    }

    else if (blReceived.startsWith("2"))
    {
      Serial.println("ENTERED MESSAGE");
      newMessage = true;
      message = blReceived;
      Serial.println(message);
      //sms();

      //delay(300000);

    }


    /*Serial.println(day-1);
    Serial.println(date);
    Serial.println(month);
    Serial.println(year);
    Serial.println(hours);
    Serial.println(minutes);
    Serial.println(seconds);*/

  }     //end of if for datetime

    /*Serial.println(day-1);
    Serial.println(date);
    Serial.println(month);
    Serial.println(year);
    Serial.println(hours);
    Serial.println(minutes);
    Serial.println(seconds);*/

  /*else if(bluetooth.available() > 0 && bluetooth.find("CMD")==true)
  {
    Serial.println("ENTERED MESSAGE");
    message = bluetooth.readString();
    Serial.println(message);
    //delay(300000);
  }
  */
  
  if(digitalRead(previousButton) == LOW&&digitalRead(nextButton) == LOW)  //button is activated
  {  
    
    Serial.println("Button Pressed");
    if(menuShow == false)    //To toggle the menu states
      {menuShow = true;
       delay(100);
       Serial.println("Entered Menu");
     
     
       menu();  //Get back the menu selection number
       Serial.print("Selected: ");
       Serial.println(menuSelection);

    if(menuSelection == 3)
      { 
        Serial.println("Entered Settings");
        settings();
        Serial.println(menuSelections);
        if(menuSelections == 3)
        {}//soft_restart();
    
    
      }

                  if(menuSelection == 2)
                  {Serial.println("Entered Apps");
                  apps();
          Serial.println(menuSelectiona);
                 
                  if(menuSelectiona == 2)
                 {
                   delay(100);
                   menuSelectiona = 0;
                   while(digitalRead(menuButton)== HIGH)
                  {torch();
                  delay(10);}
                 }

          if(menuSelectiona == 3)
          {
            Serial.println("Entered Message Display");
            delay(100);
            menuSelectiona = 0;
            while(digitalRead(menuButton)== HIGH)
            {sms();}
            delay(100);

          }

               
               }
       }
    
    if(menuShow == true)
      {menuShow = false;
       delay(100);
       
       Serial.println("Exited Menu");
  }
   
  }//end of if
  
  advanceTime();
    delay(30);

  }//end of loop
  
  
 void advanceTime()
 {
   if(millis()-lastTick>950)  //somehow
  {lastTick = millis();
   seconds++;}
   
  if (seconds > 59)
  {minutes++;
   seconds = 0;}
   
  if (minutes >59)
  {hours++;
   minutes = 0;}
   
  if (hours > 23)
  {hours = 0;
   minutes = 0;
  date+=1;}


  draw();
 }
 
  
  
//*****------------------U8Glib Functions------------------*****
void drawStartUp()
{
  u8g.firstPage();
  do{//u8g_prepare();
    u8g.setColorIndex(0);
    u8g.drawBox(0,0,127,63);
    u8g.setColorIndex(1);
    u8g.setFont(u8g_font_fur17r);
    u8g.setPrintPos(10,20);
    u8g.print("Arduino");
    u8g.setPrintPos(0,50);
    u8g.print("Smartwatch");
    }
  while(u8g.nextPage());
}//end of drawStartUp

void menu(){
  
  u8g.setColorIndex(1);
  //u8g.drawCircle(20,20,14);
  menuSelection = 1;
  while(digitalRead(menuButton) == HIGH)
  {
    
    if(digitalRead(nextButton) == LOW)
    {delay(20);
     menuSelection++;
     Serial.println("Up");
     delay(50);}
     
    if(digitalRead(previousButton) == LOW)
    {delay(20);
     menuSelection--;
     Serial.println("Down");
     delay(50);}
     
    if(menuSelection > 3)
    {menuSelection = 1;}

  if(menuSelection < 1)
  {menuSelection = 3;}
    //draw picture here
   drawMenu();

     delay(50); //For debouncing
    
     //draw menus here
  }//end of while loop
  
  
  
  
  



}//end of menu


void drawMenu()
  {
    u8g.firstPage();
   do{
      u8g.setFont(u8g_font_6x10r);
      u8g.setPrintPos(30,8);
      u8g.print("--- Menu ---");
    //u8g.setPrintPos(0,10);
      //u8g.print(menuSelection);
    u8g.drawFrame(0,(menuSelection*13)-3,128,13); //Box height is 13 pixels
    u8g.drawLine(0,10,128,10);

    //Draw menu options
    u8g.drawStr(3,20,"Return");
    u8g.drawStr(3,20+13,"Apps");
    u8g.drawStr(3,20+13+13,"Settings");
    //u8g.drawStr(3,20+13+13+13,"More");
    
      
     }
     
   while(u8g.nextPage());
  
  
  }


void apps()
{
    u8g.setColorIndex(1);
  //u8g.drawCircle(20,20,14);
  menuSelectiona = 1;
  delay(200);
  while(digitalRead(menuButton) == HIGH)
  {
    
    if(digitalRead(nextButton) == LOW)
    {delay(20);
     menuSelectiona++;
     Serial.println("Up");
     delay(50);}
     
    if(digitalRead(previousButton) == LOW)
    {delay(20);
     menuSelectiona--;
     Serial.println("Down");
     delay(50);}
     
    if(menuSelectiona > 3)
    {menuSelectiona = 1;}

  if(menuSelectiona < 1)
  {menuSelectiona = 3;}
    //draw picture here
   drawApps();

     delay(50); //For debouncing

}//end of while loop


}




void torch()
{
  u8g.firstPage();
  do{u8g.drawBox(0,0,127,63);}
  while(u8g.nextPage());
}







void drawApps(){

  u8g.firstPage();
  do
  {
  
  


  u8g.setFont(u8g_font_6x10r);
      u8g.setPrintPos(5,8);
      u8g.print("--- Applications ---");
    //u8g.setPrintPos(0,10);
      //u8g.print(menuSelection);
    u8g.drawFrame(0,(menuSelectiona*13)-3,128,13);  //Box height is 13 pixels
    u8g.drawLine(0,10,128,10);

    //Draw menu options
    u8g.drawStr(3,20,"Return");
    u8g.drawStr(3,20+13,"Torch");
    u8g.drawStr(3,20+13+13,"SMS");
    //u8g.drawStr(3,20+13+13+13,"More");

  }

  while(u8g.nextPage());

}



void settings()
{
    u8g.setColorIndex(1);
  //u8g.drawCircle(20,20,14);
  menuSelections = 1;
  delay(200);
  while(digitalRead(menuButton) == HIGH)
  {
    
    if(digitalRead(nextButton) == LOW)
    {delay(20);
     menuSelections++;
     Serial.println("Up");
     delay(50);}
     
    if(digitalRead(previousButton) == LOW)
    {delay(20);
     menuSelections--;
     Serial.println("Down");
     delay(50);}
     
    if(menuSelections > 4)
    {menuSelections = 1;}

  if(menuSelections < 1)
  {menuSelections = 4;}
    //draw picture here
   drawSettings();

     delay(50); //For debouncing

}//end of while loop


}



void sms()
{
  
  u8g.firstPage();
  do{

  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0,10);
  u8g.print(message);
  len = message.length();
  
  message.toCharArray(msg, len+1);
  //Serial.println(len);
  
  if(len>16)
  {
    //Serial.println("If");
    for (int i = 0;(i-1)<len;i++)
    {
      //Serial.println("Enter for loop");
      u8g.setPrintPos(i*8,30);
      u8g.print(msg[i+16]);
      delay(10);
      if(msg[i+17] == '\0')
      { //Serial.println("Break!!");
        break;
        //delay(3000);
      }
    }
    
  }


    if(len>32)
  {
    //Serial.println("If");
    for (int i = 0;(i-1)<len;i++)
    {
      //Serial.println("Enter for loop");
      u8g.setPrintPos(i*8,50);
      u8g.print(msg[i+32]);
      delay(10);
      if(msg[i+33] == '\0')
      { //Serial.println("Break!!");
        break;
        //delay(3000);
      }
    }
    
  }
  }
  while (u8g.nextPage());
  delay(30);
  newMessage = false;
}




void drawSettings(){

  u8g.firstPage();
  do
  {
  
  


  u8g.setFont(u8g_font_6x10r);
      u8g.setPrintPos(15,8);
      u8g.print("--- Settings ---");
    //u8g.setPrintPos(0,10);
      //u8g.print(menuSelection);
    u8g.drawFrame(0,(menuSelections*13)-3,128,13);  //Box height is 13 pixels
    u8g.drawLine(0,10,128,10);

    //Draw menu options
    u8g.drawStr(3,20,"Return");
    u8g.drawStr(3,20+13,"Set Time");
    u8g.drawStr(3,20+13+13,"Reset Arduino");
    u8g.drawStr(3,20+13+13+13,"More");

  }

  while(u8g.nextPage());

}


void draw() {

  u8g.firstPage();
  do
  {
  //u8g.drawLine(0,15,128,15);
  u8g.setFont(u8g_font_6x10r);
  u8g.drawStr(0,8,"Alestor Aldous");
  u8g.setFont(u8g_font_fur17r);
 
  
   
  
  String strDate = String("");
  char time[10];      //9
  if (hours < 10)
  strDate += "0";
  strDate += hours;
  strDate += ":";
  if (minutes < 10)
  strDate += "0";
  strDate += minutes;
  strDate += ":";
  if (seconds < 10)
  strDate += "0";
  strDate += seconds;
  strDate.toCharArray(time,10);     //9
  time[12] = 0x00;            //9
  
  
  u8g.drawStr(20,40,time);
  
  
  
  switch (month){
  case 01:
    monthw = "January";
    break;
  case 02:
    monthw = "February";
    break;
  case 03:
    monthw = "March";
    break;
  case 04:
    monthw = "April";
    break;
  case 05:
    monthw = "May";
    break;
  case 06:
    monthw = "June";
    break;
  case 07:
    monthw = "July";
    break;
  case 8:
    monthw = "August";
    break;
  case 9:
    monthw = "September";
    break;
  case 10:
    monthw = "October";
    break;
  case 11:
    monthw = "November";
    break;
  case 12:
    monthw = "December";
    break;

  } 
  u8g.setFont(u8g_font_6x10r);  //Smaller font for date
  //u8g.setFont(u8g_font_7x14r);
  u8g.setPrintPos(0,55);
  u8g.print(date);
  u8g.setPrintPos(15,55);
  u8g.print("/");
  /*switch (date)
  {
  case 1:
    u8g.print("st");
    break;
  case 2:
    u8g.print("nd");
    break;
  case 3:
    u8g.print("rd");
    break;
  default:
    u8g.print("th");
    break;
  }
  */


  u8g.setPrintPos(27,55);
  u8g.print(month); //formerly monthw
  u8g.setPrintPos(35,55);
  u8g.print("/");
  u8g.setPrintPos(43,55);
  u8g.print(year);

  //u8g.drawBitmapP(110,1,1,8,IMG_indicator_msg);
  u8g.drawBitmapP( 110, 1, 2, 16, ICON_BITMAP_65);
  if(newMessage == true)
  {u8g.drawBitmapP(110,50,2,16,ICON_BITMAP_20);}



  }
  while(u8g.nextPage());
}//end of draw
