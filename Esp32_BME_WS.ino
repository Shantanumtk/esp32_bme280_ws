#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SparkFunTMP102.h>
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
TMP102 sensor0;

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

}



void loop() {

//Read data from BME280
bme.begin(0x77);
float temperaturebme280 = bme.readTemperature();
float pressure  = bme.readPressure()/100;
float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
float humidity = bme.readHumidity();

sensor0.begin(0x48);

float temperaturetmp102;
temperaturetmp102 = sensor0.readTempC();
Serial.print("\nTemperature TMP102: ");
 Serial.print(temperaturetmp102);

//Print data to serial 
  Serial.print("Temperature = ");
  Serial.print(temperaturebme280);
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
String temp280str = String(temperaturebme280);
String temp102str = String(temperaturetmp102);
String presstr = String(pressure);
String altstr = String(altitude);
String humstr = String(humidity);


char msg[200];
sprintf(msg, "%f,%f,%f,%f,%f", temperaturebme280,pressure,altitude,humidity,temperaturetmp102);
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
