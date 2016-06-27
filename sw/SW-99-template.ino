//==================================================
// SW-99-testapp.ino
// 2016-06-27
//==================================================



//==================================================
// User Configuration
//==================================================
int g_debug              = 1;
const char* g_clientName = "SW-99";
int g_device_delay       = 2;
//==================================================
// Development Configuration
//==================================================
int g_sw_id = 99;
#define NFLOAT 2    // No of decimals i float value
#define MAX_SID 10 
int g_sid[MAX_SID] = {2,901,902,903,904,905,906,907,908};
#define greenLed  13
#define yellowLed 12
#define redLed     5

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
//=================================================
void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
//=================================================
{
asm volatile ("  jmp 0");  
}   
//=================================================
void blinkLed(int led,int number, int onTime)
//================================================= 
{
  int i;
  for(i=0;i<number;i++)
  {
    digitalWrite(led,HIGH);
    delay(100);
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
     digitalWrite(redLed,HIGH); 
     Serial.readBytes(nbbuff,nx);
     sscanf(nbbuff,"%d %d",&mid,&sid);
  
     if(sid == SID1) // Check if control sid correct
     {
       if(strstr(nbbuff,"DELAY") != NULL)
       {
      
          sscanf(nbbuff,"%d %d %s %d",&mid,&sid,command,&g_device_delay);
      
       }

     }
     digitalWrite(redLed,LOW); 
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
  digitalWrite(redLed,    HIGH); //No Server
  digitalWrite(yellowLed, HIGH); //No Network
 
}
//=================================================
void loop()
//=================================================
{
  int i;
  float value;
  String str;


      sprintf(dl[4],"%3d",g_sids[i+1]);
      tempC = sensors.getTempCByIndex(i);
      dtostrf(tempC,5, NFLOAT, dm[4]);
      i = NB_sendToGwy(NABTON_DATA,g_sids[i+1],tempC,i);

      delay(2000);  
      recSerial();

    
    delay(g_device_delay*1000);   

}


