//==================================================
// SW-12-temperature.ino
// 2016-07-02
//==================================================
int g_app_id = 12;
//==================================================
// Configuration
//==================================================
int g_debug               = 0;
int g_device_delay        = 20;
const char* g_clientName = "SW-12 Test";
//==================================================
#define NFLOAT 2  // No of decimals i float value
#define SIDN 2    // No of SIDs
#define SID1 901  
#define SID2 902  
#define SID3 903  
#define SID4 904  
#define SID5 905 
#define SID6 906
#define SID7 907
#define SID8 908

#define greenLed  13
#define yellowLed 12
#define redLed     5
//==================================================
#define MAX_SID 8
#define MAX_ORDERS 100
int  g_sids[10] = {SIDN,SID1,SID2,SID3,SID4,SID5,SID6,SID7,SID8};
char g_rbuf[4000];

// Arduino-RPi protocol
#define NABTON_DATA     1 
#define NABTON_LATEST   2 
#define NABTON_MAILBOX  3 

//=================================================
//
// D0 RX used for serial communication to server (Raspberry Pi)
// D1 TX used for serial communication to server (Raspberry Pi)
// D2
// D3 Status Pin - Serial data received
// D4 IR Data
// D5 Red LED, Status Pin - Serial data sent
// D6 DIR Stepper
// D7 STEP Stepper
// D8 SLEEP Stepper
// D9 One Wire Data - 4.7 kOhm pullup
// D10  RX Bluetooth device
// D11  TX Bluetooth device
// D12  Yellow LED
// D13  Green LED
//
// A0
// A1
// A2
// A3
// A4 SDA I2C OLED
// A5 SCL I2C OLED

// MEGA
// D20 SDA I2C OLED
// D21 SCL I2C OLED
//=================================================
//==================================================
// Nabton Client Application Configuration
//==================================================
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <U8glib.h>

//=================================================
// One Wire
//=================================================

#define ONE_WIRE_BUS 9
#define TEMPERATURE_PRECISION 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress device[MAX_SID];
int nsensors = 0;

//=================================================
void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
//=================================================
{
asm volatile ("  jmp 0");  
}   

//==================================================
// OLED I2C
//==================================================

//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);// SW SPI protocol(4 pins): SCK = 13, MOSI = 11, CS = 10, A0 = 9  
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); // Small display I2C protocol (2 pins)
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); // Large display
char dl[16][16],dm[16][16],dr[16][16];
//=================================================
void NB_oledDraw() 
//=================================================
{
 u8g.firstPage();  
  do {
        draw();
  } while( u8g.nextPage() ); 
}
//=================================================
void draw()
//=================================================
{
  // Horizontal pixels: 0 - 120
  // Vertical pixels: 0 - 63
  u8g.setFont(u8g_font_6x10);
  //u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  
  u8g.drawStr( 0, 10, dl[1]);
  u8g.drawStr( 0, 27, dl[2]);
  u8g.drawStr( 0, 45, dl[3]);
  u8g.drawStr( 0, 62, dl[4]);

  u8g.drawStr( 45, 10, dm[1]);
  u8g.drawStr( 99, 27, dm[2]);
  u8g.drawStr( 99, 45, dm[3]);
  u8g.drawStr( 45, 62, dm[4]);

  u8g.drawStr( 100, 10, dr[1]);
  u8g.drawStr( 100, 27, dr[2]);
  u8g.drawStr( 100, 45, dr[3]);
  u8g.drawStr( 100, 62, dr[4]);  

}
//=================================================
void blinkLed(int led,int number, int onTime)
//================================================= 
{
  int i;
  for(i=0;i<number;i++)
  {
    digitalWrite(led,HIGH);
    delay(onTime);
    digitalWrite(led,LOW);
    delay(onTime);
  }
}
//=================================================
void NB_serialFlush()
//=================================================
{
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}   
//=================================================
int NB_sendToGwy(int mid, int sid, float data, int other)
//=================================================
{
  int ixSid = 0,i,negative=0;
  char msg1[100],msg2[50],checksum[20];
     strcpy(msg1," ");
     strcpy(msg2," ");
     digitalWrite(yellowLed,HIGH);
     // Mandatory part of message
     sprintf(msg1,"?appid=%d&mid=%d&nsid=%d&sid1=%d",g_app_id,mid,1,sid);
if(g_debug==1){Serial.print("data:");Serial.println(data);}      
     if(mid == NABTON_DATA)
     {
       negative = 0;
       if(data < 0.0)
       {
          negative = 1;
          data = data*(-1.0);
       }
       // Get non-decimal part
       int part1 = floor(data);
if(g_debug==1){Serial.print("part1:");Serial.println(part1);}       
       // Get decimalpart
       float ftemp = (data - part1);
       for(i=1;i<=NFLOAT;i++)ftemp=ftemp*10;
if(g_debug==1){Serial.print("ftemp:");Serial.println(ftemp);}   
       int part2 = round(ftemp);
if(g_debug==1){Serial.print("part2:");Serial.println(part2);}          
       // if negative
       if(negative == 0)
       {
         if(part2 < 10)
           sprintf(msg2,"&dat1=%d.0%d",part1,part2);
         else 
           sprintf(msg2,"&dat1=%d.%d",part1,part2);
       }
       if(negative == 1)
       {
         if(part2 < 10)
           sprintf(msg2,"&dat1=-%d.0%d",part1,part2);
         else 
           sprintf(msg2,"&dat1=-%d.%d",part1,part2);
       }
       strcat(msg1,msg2);
     }
    
     // Create checksum
     sprintf(checksum,": %d",strlen(msg1));
     strcat(msg1,checksum);
     
     // Send meassage
     Serial.println(msg1);
     digitalWrite(yellowLed,LOW);
    return(other);
}
//=================================================
void recSerial()
//=================================================
{
  int i,k=0,ttemp,nx=0,nm=0;
  nx = Serial.available();
  char nbbuff[50],msg[5][100],command[48],stemp[100];
  int mid, sid;
  int dir,steps,vel;
  
  if (nx > 0) 
  {
     strcpy(dr[3],"-");
     digitalWrite(redLed,HIGH); 
     Serial.readBytes(nbbuff,nx);
     sscanf(nbbuff,"%d %d",&mid,&sid);
     sprintf(dl[3],"%d",nx);
     if(sid == SID1) // Check if control sid correct
     {
       if(strstr(nbbuff,"DELAY") != NULL)
       {
          strcpy(dr[3],"DLY");
          sscanf(nbbuff,"%d %d %s %d",&mid,&sid,command,&g_device_delay);
          sprintf(dr[2],"%d",g_device_delay);
       }
       NB_oledDraw();
     }
     digitalWrite(redLed,LOW); 
  }
}

//=================================================
void clearOled()
//================================================= 
{
  int i;
  for(i=1;i<=4;i++)
  {
    strcpy(dl[i]," ");
    strcpy(dm[i]," ");
    strcpy(dr[i]," ");
  }
}

//=================================================
void setup()
//================================================= 
{
  int i;
  float tempC;  
  String str;
  Serial.begin(9600);
  NB_serialFlush();
  
  pinMode(greenLed,          OUTPUT);
  pinMode(redLed,            OUTPUT);
  pinMode(yellowLed,         OUTPUT);

  digitalWrite(greenLed,  HIGH); //Device ON
  digitalWrite(redLed,    LOW); //Message received
  digitalWrite(yellowLed, LOW); //Message sent
  // OLED
//=================================================

  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  clearOled();
// One Wire
//=================================================
  digitalWrite(greenLed, HIGH); //Device ON
  sprintf(dl[1],"%s",g_clientName);
  sprintf(dr[2],"-");
  sprintf(dr[3],"-");
  sprintf(dr[4],"-");
  sensors.begin();
  nsensors = sensors.getDeviceCount();
  sprintf(dr[1],"%2d",nsensors);
  NB_oledDraw();
  
  if(nsensors > 0)
  {
    for(i=0;i<nsensors;i++)
    {
      sensors.getAddress(device[i], i);
      sensors.setResolution(device[i], TEMPERATURE_PRECISION);
    }
  }

}
//=================================================
void loop()
//=================================================
{
  int i;
  float tempC;
  String str;

    sprintf(dr[2],"%3d",g_device_delay);
     
    sensors.requestTemperatures();
    for(i=0;i<nsensors;i++)
    {
      sprintf(dl[4],"%3d",g_sids[i+1]);
      tempC = sensors.getTempCByIndex(i);
      dtostrf(tempC,5, NFLOAT, dm[4]);
      if(tempC != -127)i = NB_sendToGwy(NABTON_DATA,g_sids[i+1],tempC,i);
      //strcpy(dm[i+1],"*"); 
      NB_oledDraw();
      delay(2000);  
      recSerial();
      sprintf(dl[4],"%d",g_sids[i+1]);
      NB_oledDraw();
    }
    delay(g_device_delay*1000);   
}
