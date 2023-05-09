#include <SimpleDHT.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "Aula Dionne"
#define STAPSK "dionnefacompis"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "10.16.1.211"; //Selecionar o host informado pelo professor (escolhido pelo servidor python)
const uint16_t port = 45678;      //Seleciunar a port informada pelo professor (escolhido pelo servidor python)

ESP8266WiFiMulti WiFiMulti;

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}

void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  Serial.println(WiFi.localIP());
  
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");

    // This will send the request to the server
  client.print("Da um 10 pra gente");
  client.print((int)temperature); client.print(" *C, "); 
  client.print((int)humidity); client.println(" H");
  // client.println("");
  
  client.stop();
  // DHT11 sampling rate is 1HZ.
  delay(1500);
}