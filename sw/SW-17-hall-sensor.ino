loat limit = 512;

void setup() {
  int x,i;
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
  Serial.begin(9600);

  for(i=1;i<100;i++)
  {
    x = analogRead(A0);
    delay(10);
    limit = x;
  }
  Serial.print(limit); Serial.print(" ");
  Serial.println(x);
}


void loop() {

  int x = analogRead(A0);

 if(x > limit + 1) 
 {
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
 }
 else if(x < limit-1) 
 {
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
 }
 else
 {
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
 }
  //Serial.print(limit); Serial.print(" ");
  //Serial.println(x);
  delay(10); 
  limit = limit + (x-limit)*0.0001;
}
