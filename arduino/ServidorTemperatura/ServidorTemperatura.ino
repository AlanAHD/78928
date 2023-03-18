//Librerias Sensor
#include "DHTesp.h"
#define DHTpin 15
//Librerias Wifi
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>
//Variables Sensor
DHTesp dht;
//Variables WiFi
const char* ssid="AlanHD";
const char* password="Alancito";
AsyncWebServer server(80);
void setup()
{
  Serial.begin(115200);
  conectarse();
  Serial.println("Estado\tHumedad (%)\tTemperatura (C)\t(F)\tIndiceCalor (C)\t(F)");
  dht.setup(DHTpin, DHTesp::DHT11);

  delay(dht.getMinimumSamplingPeriod());
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  if (isnan(humedad) || isnan(temperatura)){
    Serial.println("No se pudo leer sensor DHT!");
    return;
  }

    server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
    int numParametros =request->params();
    Serial. print(numParametros);
    if(numParametros ==0){
      request->send(200,"text/html","<h1> Servidor Temperatura </h1>");
      
    }else{
      AsyncWebParameter *p=request->getParam(0);
      String html="<H1> hola " + p->value() + "desde ESP32</H1>";
      request->send(200,"text/html",html);
    }
  });
  
    server.on("/estado",HTTP_GET,[](AsyncWebServerRequest * request){
    String estado=dht.getStatusString();
    String html="<h1>Estado:"+estado+"</h1>";
    request->send(200,"text/html",html);
  });
    server.on("/cel",HTTP_GET,[](AsyncWebServerRequest * request){
    float temperatura=dht.getTemperature();
    String html="<h1>Temperatura: \t"+String(temperatura)+"</h1>";
    request->send(200,"text/html",html);
  });
    server.on("/fa",HTTP_GET,[](AsyncWebServerRequest * request){
    float temperatura=dht.getTemperature();
    float fa=dht.toFahrenheit(temperatura);
    String html="<h1>Temperatura: \t"+String(fa)+"</h1>";
    request->send(200,"text/html",html);
  });
    server.on("/humedad",HTTP_GET,[](AsyncWebServerRequest * request){
    float humedad=dht.getHumidity();
    String html="<h1>Humedad: \t"+String(humedad)+"</h1>";
    request->send(200,"text/html",html);
  });
    server.on("/indic",HTTP_GET,[](AsyncWebServerRequest * request){
    float humedad=dht.getHumidity();
    float temperatura=dht.getTemperature();
    float indic=dht.computeHeatIndex(temperatura,humedad,false);
    String html="<h1>Indice de calor: \t"+String(indic)+"</h1>";
    request->send(200,"text/html",html);
  });
      server.on("/indif",HTTP_GET,[](AsyncWebServerRequest * request){
    float humedad=dht.getHumidity();
    float temperatura=dht.getTemperature();
    float indif=dht.computeHeatIndex(dht.toFahrenheit(temperatura),humedad,true);
    String html="<h1>Indice de calor: \t"+String(indif)+"</h1>";
    request->send(200,"text/html",html);
  });

  server.begin();
}

void loop()
{
  /*
  delay(dht.getMinimumSamplingPeriod());
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();                            
  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("No se pudo leer sensor DHT!");
    return;

  }
  Serial.print( dht.getStatusString() );
  Serial.print("\t");
  Serial.print(humedad, 1);
  Serial.print("\t\t");
  Serial.print(temperatura, 1);
  Serial.print("\t\t");
  Serial.print( dht.toFahrenheit(temperatura), 1);
  Serial.print("\t\t");
  Serial.print( dht.computeHeatIndex(temperatura, humedad, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true), 1);
  delay(2000);
  */
}

  void conectarse(){
    Serial.print("conectando");
    Serial.println(ssid);
    WiFi.begin(ssid,password);
    while(WiFi.status()!=WL_CONNECTED){
      delay(500);
      Serial.print(".");
      }
      Serial.print(WiFi.localIP());
    }
