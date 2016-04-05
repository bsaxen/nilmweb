//==================================================
// serial-to-wifi-ESP8266.ino
// 2016-04-05
//==================================================
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial adam(4,5 ); // RX, TX
const char* ssid     = "gateway ssid";
const char* password = "wifi pswd";
const char* host = "server ip address";
IPAddress ipAddress;


//==================================================
void setup() {
//==================================================
  Serial.begin(115200);
  local.begin(9600);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  ipAddress = WiFi.localIP();
  Serial.println(ipAddress);
}

int value = 0;

//==================================================
void loop() {
//==================================================
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String urlPath        = "/sxndata/index.php";
  String arduinoSim     = "?mid=1&nsid="+String(SIDN)+"&sid1="+String(sid)+"&dat1="+String(sValue);
  String nameString     = "&name=" + String(clientName);;
  String ipString       = "&ip=" + String(g_sIp);
 
  client.print(String("GET ") + urlPath + arduinoSim + nameString + ipString + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n"); 
 
  delay(10);
  
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  local.write("s");
  delay(3000);
  
  Serial.println();
  Serial.println("closing connection");
}
