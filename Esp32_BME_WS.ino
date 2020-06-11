#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>
//#include <ArduinoJson.h>

WebSocketsServer webSocket = WebSocketsServer(81);
const char *ssid = "ESP32_soft_AP";
const char *password = "ESP32";
long randNumber;
unsigned long delayTime;


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:

            Serial.print("Disconnected\n");
            break;
        case WStype_CONNECTED:
         {
                IPAddress ip = webSocket.remoteIP(num);
                
            Serial.printf("Connected from %d.%d.%d.%d\n" ,ip[0], ip[1], ip[2], ip[3]);
                
                
            }
            break;
        case WStype_TEXT:
        {

          
            
          //}

        }
          
          break;
    
        
           
          
        
        case WStype_BIN:
     
            break;
    }

}


void setup() {

 
Serial.begin(115200);
   WiFi.softAP(ssid, password);

 IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);


Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x77);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
}



void loop() {

//Read data from BME280
float temperature = bme.readTemperature();
float pressure  = bme.readPressure()/100;
float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
float humidity = bme.readHumidity();


//Print data to serial 
  Serial.print("Temperature = ");
  Serial.print(1.8 * temperature + 32);
  Serial.println(" *F");
  
  Serial.print("Pressure = ");
  Serial.print(pressure / 100.0F);
  Serial.println(" hPa");
  
  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");
  
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
  
//Send to websocket
String tempstr = String(temperature);
String presstr = String(pressure);
String altstr = String(altitude);
String humstr = String(humidity);

char msg[200];
sprintf(msg, "%f,%f,%f,%f", temperature,pressure,altitude,humidity);
webSocket.sendTXT(0,msg);

 

//Using Json format

//DynamicJsonDocument  doc(200);

//doc["Temperature"] = tempstr;
//  doc["Pressure"] = presstr;
//
//  doc["Pressure"] = altstr;
//
//  doc["Humidity"] = humstr;

//String output;
//serializeJson(doc, output);
//webSocket.sendTXT(0,output);

webSocket.loop();

delay(20);

}
