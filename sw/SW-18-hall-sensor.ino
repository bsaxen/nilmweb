//==================================================
// SW-18-hall-sensor.ino
// 2016-08-01
//==================================================
int g_app_id = 18;
//==================================================
#include <U8glib.h>

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); // Large display
char dl[16][16],dm[16][16],dr[16][16];
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
void setup() 
//=================================================
{


  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
  {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) 
  {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) 
  {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) 
  {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);

  clearOled();
  strcpy(dl[1],"Volvo Amazon"); 
  //NB_oledDraw();
}

unsigned long t1,t2;
unsigned long dt;
int sig = 0;
unsigned long counter = 0;

unsigned long xmin = 999;
unsigned long xmax = 0;
unsigned long ss;
unsigned long rpm;
int sensitivity = 5;
unsigned long x,x1,x2,x3;
int rs = 3;
//=================================================
void loop() 
//=================================================
{
  counter++;

  delay(rs);
  x = analogRead(A0);
  x3 = x2;x2 = x1;x1 = x;


if(x1 < (x2 - sensitivity) && x3 < (x2 - sensitivity)) // maximum found
 {
  digitalWrite(8,HIGH);
  delay(rs);
  t2 = t1;
  t1 = millis();
  dt = t1 - t2;
 }

  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(9,LOW);

  if(counter > 600/rs)
  {
      if(dt > 0)
        rpm = 60*(1000/dt);
      else
        rpm = 0; 
      sprintf(dl[2],"data=%5ld",x);
      sprintf(dl[3],"rpm =%5ld",rpm);
      sprintf(dl[4],"dt  =%5ld",dt);
      digitalWrite(8,LOW);
      digitalWrite(10,HIGH);
    
    NB_oledDraw();
    counter = 0;
    digitalWrite(10,LOW);
    rpm = 0;
    t1 = millis();
    dt = 0;
  }
  
  
}
