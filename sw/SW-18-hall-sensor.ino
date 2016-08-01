//==================================================
// SW-18-hall-sensor.ino
// 2016-08-01
//==================================================
int g_app_id = 18;
//==================================================
#include <U8glib.h>

int limit = 512;
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
void setup() 
//=================================================
{
  int x,i;
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);

  for(i=1;i<100;i++)
  {
    x = analogRead(A0);
    delay(10);
    limit = x;
  }
  
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
  strcpy(dl[1],"Volvo Amazon"); 
  NB_oledDraw();
}

long t1,t2;
int dt;
int sig = 0;
int counter = 0;
int freq;

int xmin = 999;
int xmax = 0;
int ss;
int sensitivity = 5;
//=================================================
void loop() 
//=================================================
{
  counter++;
  int x = analogRead(A0);
  //int delta = x - limit;
  if(xmax < x)xmax = x;
  if(xmin > x)xmin = x;

 if(x > xmax - sensitivity) 
 {
  digitalWrite(8,HIGH);
  //digitalWrite(10,LOW);
  if(sig == 0)
  {
  t2 = t1;
  t1 = millis();
  dt = t1 - t2;
  sig = 1;
  }
 }
 else if(x < xmin + sensitivity) 
 {
  digitalWrite(8,LOW);
  //digitalWrite(10,HIGH);
    sig = 0;
 }


  if(counter == 10000)
  {
    //sprintf(dl[2],"min=%d",xmin);
    //sprintf(dm[2],"max=%d",xmax);
    ss = xmax - xmin;
    if(ss <= 2*sensitivity)
    {
      strcpy(dl[2],"signal = no");
      digitalWrite(10,HIGH);
    }
    else
    {
      sprintf(dl[2],"signal=%d",ss);
      digitalWrite(10,LOW);
    }
    freq = 1000/dt;
    sprintf(dl[3],"freq=%d",freq);
    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);
    
    NB_oledDraw();
    counter = 0;
    xmin = 999;
    xmax = 0;
    digitalWrite(9,LOW);
  }
  
  
}
