
//==================================================
// automower-detector.ino
// 2016-06-03
//==================================================
#include <U8glib.h>

int oneLed    = 13;
int detectLed = 13;
char appName[30] = {"AUTOMOWER"};
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
  do { pinMode(oneLed, OUTPUT);
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
  u8g.drawStr( 45, 27, dm[2]);
  u8g.drawStr( 45, 45, dm[3]);
  u8g.drawStr( 45, 62, dm[4]);

  u8g.drawStr( 100, 10, dr[1]);
  u8g.drawStr( 100, 27, dr[2]);
  u8g.drawStr( 100, 45, dr[3]);
  u8g.drawStr( 100, 62, dr[4]);  
}
//=================================================
void clearOled()
//================================== pinMode(oneLed, OUTPUT);=============== 
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
void detect()
//================================================= 
{
  int i,tmax=0,xmax = 30,nmax = 0,prev = 0,ifreq = 0;
  int x,x1=0,x2=0,x3=0,ax;
  float freq;
  digitalWrite(detectLed, HIGH);
  for(i=1;i<=10000;i++)
  {
    //digitalWrite(oneLed, LOW); 
    int x = analogRead(A0);
    ax = (x+x1+x2)/3;
    if(tmax < ax)tmax = ax;
    if(ax > xmax)
    {
      nmax++;
      ifreq = ifreq + (i - prev);
      prev = i;
    }
    x2 = x1;
    x1 = x;
    //Serial.println(x); 
  }
  freq = (float)ifreq/nmax;
  sprintf(dm[2],"%d",xmax);
  sprintf(dr[2],"%d",nmax);
  sprintf(dr[4],"%d",(int)freq);
  sprintf(dr[3],"%d",tmax);
  digitalWrite(detectLed, LOW);
}
//=================================================
void setup() 
//=================================================
{
  Serial.begin(9600);
  pinMode(oneLed, OUTPUT);
  pinMode(detectLed, OUTPUT);
  pinMode(12, OUTPUT);
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
  //digitalWrite(oneLed, HIGH); //Device ON
  sprintf(dl[1],"%s",appName);
  sprintf(dl[2],"Signal");
  sprintf(dl[3],"Code");
  sprintf(dl[4],"Freq");
  NB_oledDraw();
  tone(12,10000);
}

void loop() 
{
  detect();
  NB_oledDraw();                      
}
