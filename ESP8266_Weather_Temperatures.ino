/*
 * 
 * Fran Romero
 * https://github.com/yatan
 * yatan87@gmail.com
 * 
 * 
 * 
 */

#include <math.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DHT.h>

DHT dht1(D3, DHT11);

const char* ssid = "";             //!!!!!!!!!!!!!!!!!!!!! modify this
const char* password = "";          //!!!!!!!!!!!!!!!!!!!!! modify this
String api_key = "";              //!!!!!!!!!!!!!!!!!!!!! modify this

int ledPin = D4;          //connect led pin to d4 and ground


IPAddress tweet(184, 106, 153, 149); 

WiFiClient clientWeb;
 
void setup() {
  dht1.begin();
  Serial.begin(115200);
  delay(10);
 
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledPin, LOW);
    delay(500);
    Serial.print(".");
    digitalWrite(ledPin, HIGH);
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  WiFi.hostname("Node1");
 
  // Print the IP address
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());

}


void SendTweet(float temperatura, float humitat) {

  String tsData = "Temperatura Almacelles%0a";

  // Read DHT temperature and humidity values
  float DHT11_t = temperatura;
  float DHT11_h = humitat;

  Serial.print("DHT11  ");
  Serial.print(DHT11_t,1); Serial.print(String(char(176))+"C  ");
  Serial.print(DHT11_h,1); Serial.println("%RH");
  Serial.println();

  tsData += "Temperatura: ";
  tsData += DHT11_t;
  tsData += "ºC ";  
  tsData += "%0a";
  tsData += "Humitat: ";
  tsData += DHT11_h;
  tsData += "%25";  
  tsData += "%0a";
  tsData += "#Almacelles #Temperatura";
  

  Serial.println("\nStarting connection to server...");
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  
  // if you get a connection, report back via serial:
  if (clientWeb.connect(tweet, 80)) 
  {
    
    String PostData = "api_key=" + api_key + "&status=" + tsData;
    
    Serial.println("connected to server");
    // Make a HTTP request:
    clientWeb.println("POST /apps/thingtweet/1/statuses/update HTTP/1.1");
    clientWeb.println("Host: api.thingspeak.com");
    clientWeb.println("Connection: close");
    clientWeb.println("Content-Type: application/x-www-form-urlencoded");
    clientWeb.println("Cache-Control: no-cache");
    clientWeb.print("Content-Length: ");
    clientWeb.print(PostData.length());
    clientWeb.print("\n\n");

    clientWeb.print(PostData);
    // Data sended
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
  }
  
}
 
void loop() {

  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(ledPin, LOW);
      delay(500);
      Serial.print(".");
      digitalWrite(ledPin, HIGH);
      delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    WiFi.hostname("Node");
  }
  else
  {
      // RONDA 1
      float temp1 = dht1.readTemperature();
      while(isnan(temp1))
      {
        delay(50);
        temp1 = dht1.readTemperature();
      }
      delay(30);
      float hum1 = dht1.readHumidity();
      while(isnan(hum1))
      {
        delay(50);
        hum1 = dht1.readHumidity();
      }  
      delay(1000 * 60 * 5);
    
      // RONDA 2
      float temp2 = dht1.readTemperature();
      while(isnan(temp2))
      {
        delay(50);
        temp2 = dht1.readTemperature();
      }
      delay(30);
      float hum2 = dht1.readHumidity();
      while(isnan(hum2))
      {
        delay(50);
        hum2 = dht1.readHumidity();
      }    
      delay(1000 * 60 * 5);
    
      // RONDA 3
      float temp3 = dht1.readTemperature();
      while(isnan(temp3))
      {
        delay(50);
        temp3 = dht1.readTemperature();
      }  
      delay(30);
      float hum3 = dht1.readHumidity();
      while(isnan(hum3))
      {
        delay(50);
        hum3 = dht1.readHumidity();
      }    
      delay(1000 * 60 * 5);
    
      // Calcul mitjana
      float temp = ( temp1 + temp2 + temp3 ) / 3;
      float hum = ( hum1 + hum2 + hum3 ) / 3;
      
      // Send tweet
      SendTweet(temp, hum);
  }

 
}

