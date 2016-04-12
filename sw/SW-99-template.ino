//==================================================
// SW-99-testapp.ino
// 2016-04-11
//==================================================
int g_sw_id = 99;

#define CONF_LED_TWO        2
#define CONF_LED_THREE      3
#define CONF_LED_FOUR       4

#define CONF_DISPLAY_LCD    2
#define CONF_DISPLAY_OLED   3

#define CONF_SENSOR_OW      1

#define CONF_COMM_ETH       1
#define CONF_COMM_WIFI      2
#define CONF_COMM_SERIAL    3

#define INFO_START_READ_SENSOR_DATA 1
#define INFO_END_READ_SENSOR_DATA   2
#define INFO_START_SEND_SENSOR_DATA 3
#define INFO_END_SEND_SENSOR_DATA 4
#define INFO_REC_SERVER_DATA  5
//==================================================
// Configuration
//==================================================
int g_debug              = 1;
const char* g_clientName = "SW-99";
const char* g_confServer = "sercon.simuino.com";
int g_delay       = 2;
int g_connection_status = 0;
//==================================================

#define NFLOAT 2  // No of decimals i float value

//==================================================
#define MAX_SID 10 
int g_sid[MAX_SID] = {2,901,902,903,904,905,906,907,908};
int conf_display         = CONF_DISPLAY_OLED;
int conf_comm            = CONF_COMM_SERIAL;
int conf_sensor          = CONF_SENSOR_OW;
int conf_led             = CONF_LED_THREE;
float g_sensor_data[MAX_SID];
int g_nsensor = 0;
char g_server[40];
float g_data[MAX_SID];
char g_ip_address[40];
char g_msg[80];

#define NABTON_DATA     1 
 
#define greenLed  13
#define yellowLed 12
#define redLed     5
#define whiteLed   3
//=================================================
//
// D0 RX used for serial communication to server (Raspberry Pi)
// D1 TX used for serial communication to server (Raspberry Pi)
// D2
// D3 White LED
// D4 IR Data
// D5 Red LED
// D6 DIR Stepper
// D7 STEP Stepper
// D8 SLEEP Stepper
// D9 One Wire Data
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
//+++++++++++++++++++++++++++++++++++++++++++++++++
// API configration
//+++++++++++++++++++++++++++++++++++++++++++++++++
#include <OneWire.h>            // One Wire
#include <DallasTemperature.h>  // One Wire Temperature
#include <U8glib.h>             // OLED

//=================================================
// Local Library (LL) independent of ADD-ON libraries
//=================================================

void LL_software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  asm volatile ("  jmp 0");  
}

void LL_blinkLed(int led,int number, int onTime)
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


void LL_serialFlush()
{
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}


void LL_led(int led, int state)
{
   digitalWrite(led,state);
}

void LL_delay(int x)
{
   LL_led(greenLed,HIGH);
   delay(x*1000);
   LL_led(greenLed,LOW);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++
// ADD-ON Libraries
//+++++++++++++++++++++++++++++++++++++++++++++++++

//=================================================
// ADD-ON: Led  (LED)
//=================================================
     
void api_LED_init()
{
    pinMode(greenLed,          OUTPUT);
    pinMode(redLed,            OUTPUT);
    pinMode(yellowLed,         OUTPUT);
    pinMode(whiteLed,          OUTPUT);
    digitalWrite(greenLed,  HIGH); //Device ON
    digitalWrite(redLed,    HIGH); //No Server
    digitalWrite(yellowLed, HIGH); //No Network
    digitalWrite(whiteLed,  HIGH); 
    delay(1000);
     LL_led(greenLed,LOW);
     LL_led(redLed,LOW);
     LL_led(yellowLed,LOW);
     LL_led(whiteLed,LOW);
}

void api_LED_inform(int info)
{


}

//=================================================
// ADD-ON: One Wire Temperature  (OWT)
//=================================================

#define ONE_WIRE_BUS 9
#define TEMPERATURE_PRECISION 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress device[MAX_SID];

int api_OWT_init()
{
  int i;
  sensors.begin();
  int nsensors = sensors.getDeviceCount();
  if(nsensors > 0)
  {
    for(i=0;i<nsensors;i++)
    {
      sensors.getAddress(device[i],i);
      sensors.setResolution(device[i], TEMPERATURE_PRECISION);
    }
  }
  return(nsensors);
}

void api_OWT_requestTemperature()
{
    sensors.requestTemperatures();
}

float api_OWT_getTemperature(int i)
{
    return(sensors.getTempCByIndex(i));
}

//==================================================
// ADD-ON: OLED I2C (OLED)
//==================================================

//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);// SW SPI protocol(4 pins): SCK = 13, MOSI = 11, CS = 10, A0 = 9  
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); // Small display I2C protocol (2 pins)
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); // Large display
char dl[16][16],dm[16][16],dr[16][16];

void api_OLED_clear()
{
  int i;
  for(i=1;i<=4;i++)
  {
    strcpy(dl[i]," ");
    strcpy(dm[i]," ");
    strcpy(dr[i]," ");
  }
}

void api_OLED_init()
{
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

  api_OLED_clear();
}

void api_OLED_Display() 
{
 u8g.firstPage();  
  do {
        api_OLED_draw();
  } while( u8g.nextPage() ); 
}


void api_OLED_draw()
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

void api_OLED_write_string(int row, int col,char *x)
{
   if(col == 1)strcpy(dl[row],x);
   if(col == 2)strcpy(dm[row],x);
   if(col == 3)strcpy(dr[row],x);
}

void api_OLED_write_int(int row, int col,int x)
{
   if(col == 1)sprintf(dl[row],"%3d",x);
   if(col == 2)sprintf(dm[row],"%3d",x);
   if(col == 3)sprintf(dr[row],"%3d",x);
}

void api_OLED_write_float(int row, int col,float x)
{
   if(col == 1)sprintf(dl[row],"%.2f",x);
   if(col == 2)sprintf(dm[row],"%.2f",x);
   if(col == 3)sprintf(dr[row],"%.2f",x);
}



//==================================================
// ADD-ON: COMMUNICATION HTTP over SERIAL (CHS)
//==================================================

int api_CHS_send(int app_id, int mid, int sid, float data, int other)
{
     int ixSid = 0,i,negative=0;
     char msg1[100],msg2[50],checksum[20];
     strcpy(msg1," ");
     strcpy(msg2," ");
     digitalWrite(yellowLed,HIGH);
     // Mandatory part of message
     sprintf(msg1,"?sw=%d&mid=%d&nsid=%d&sid1=%d",app_id,mid,1,sid);
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

void api_CHS_receive()
{
  int i,k=0,ttemp,nx=0,nm=0;
  nx = Serial.available();
  char nbbuff[50],msg[5][100],command[48],stemp[100];
  int mid, sid;
  int dir,steps,vel;
  
  if (nx > 0) 
  {
     digitalWrite(redLed,HIGH); 
     Serial.readBytes(nbbuff,nx);
     sscanf(nbbuff,"%d %d",&mid,&sid);
     sprintf(dr[3],"%d",nx);
     if(sid == g_sid[1]) // Check if control sid correct
     {
       if(strstr(nbbuff,"DELAY") != NULL)
       {
          strcpy(dr[3],"DLY");
          sscanf(nbbuff,"%d %d %s %d",&mid,&sid,command,&g_delay);
          sprintf(dr[1],"%d",g_delay);
       }
       strcpy(dr[4],"-");
       api_OLED_draw();
     }
     digitalWrite(redLed,LOW); 
  }
}


//==================================================
// ADD-ON: COMMUNICATION HTTP over ETHERNET (CHE)
//==================================================

//==================================================
// ADD-ON: COMMUNICATION HTTP over WiFi (CHW)
//==================================================

//==================================================
// ADD-ON: COMMUNICATION HTTP over Bluetooth (CHB)
//==================================================

/**************************************************
Every system is based on:

communication between Arduino and:
  Serial -> RaspberryPi -> Wifi/Eth HTTP Server
  Ethernet HTTP Server
  Wifi HTTP Server
  
  
sensor data (float values and means of collection)
  One Wire (temperature, ...)

display:
  LCD
  OLED
led:
  2 leds
  3 leds
  4 leds
**************************************************/
//+++++++++++++++++++++++++++++++++++++++++++++++++
// SUB Libraries
//+++++++++++++++++++++++++++++++++++++++++++++++++
#define M_PI 1
#define M_VALUE 2
#define M_IP_ADDRESS 3
#define M_CONNECTION_STATUS 4
#define M_NSENSOR 5

void SUB_requestData()
{
   api_OWT_requestTemperature();  
}
void SUB_info(int id, int i, float f, char s[])
{
    api_OLED_write_int(1,2,g_delay);
    api_OLED_write_int(1,3,g_nsensor);
    api_OLED_write_string(3,1,g_server);
    api_OLED_Display();
}
void SUB_send(int sw_id,int msg_id,int sid,float value,int misc)
{

    if(conf_comm == CONF_COMM_SERIAL)
    {
      api_CHS_send(sw_id,msg_id,1,value,misc);
    }
  
}

void SUB_receive()
{
 strcpy(g_msg,"test");
}

int SUB_initSensors()
{
  int n;
  // get number of sensors and tag all of them with a SID 
  n = api_OWT_init();
  return(n);
}

void SUB_info_default()
{
    if(g_debug == 0)return;
    Serial.begin(9600);
    api_LED_init();
}

int  SUB_connect()
{
  
}

float  SUB_getData(int i)
{
  return(1.0);  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++
// SETUP Libraries
//+++++++++++++++++++++++++++++++++++++++++++++++++
void SETUP_collect()
{
   g_nsensor = SUB_initSensors();
}
void SETUP_info()
{
   SUB_info_default();
}
void SETUP_sendrec()
{
   g_connection_status = SUB_connect();
}
void SETUP_delay()
{
   
}

//+++++++++++++++++++++++++++++++++++++++++++++++++
// LOOP Libraries
//+++++++++++++++++++++++++++++++++++++++++++++++++
void LOOP_collect()
{
  if(g_debug == 1)Serial.println("LOOP_collect");
  int i;

  SUB_requestData();
  for(i=0;i<g_nsensor;i++)
  {
       g_data[i] = SUB_getData(i);
  }
}

void LOOP_info()
{
  if(g_debug == 1)Serial.println("LOOP_info");
  int i;
  SUB_info(M_CONNECTION_STATUS,g_connection_status,0.0,"-");
  SUB_info(M_IP_ADDRESS,0,0.0,g_ip_address);
  SUB_info(M_NSENSOR,g_nsensor,0.0,"-");
  for(i=0;i<g_nsensor;i++)
  {
    if(g_data[i] == 3.14159)
    {
      SUB_info(M_PI,0,3.14159,"-");
    }
    else
    {
      SUB_info(M_VALUE,0,g_data[i],"-");
    }
  }
}
void LOOP_sendrec()
{
    if(g_debug == 1)Serial.println("LOOP_sendrec");
    int i;
    for(i=0;i<g_nsensor;i++)
    {
      SUB_send(g_sw_id,NABTON_DATA,g_sid[i],g_data[i],0);
      delay(1500); // Wait for response from server  
      SUB_receive();
    }
}
void LOOP_delay()
{
   if(g_debug == 1)Serial.println("LOOP_delay");
   LL_delay(g_delay);
}
//=================================================
void setup()
//================================================= 
{
    SETUP_info();
    SETUP_collect();
    SETUP_sendrec();
    SETUP_delay();
}
//=================================================
void loop()
//=================================================
{
    LOOP_collect();
    LOOP_info();
    LOOP_sendrec();
    LOOP_delay();
}


