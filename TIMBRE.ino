#include <WiFi.h>
#include <WebSocketsClient.h>
#include "CAMConfig.hpp"
#include "base64.h"

WebSocketsClient ws1;
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;
int boton = 15;
int estadoBoton= 1;
bool fotoTomada = false;

void ws1Event(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("\nSocket 1 desconectado");
            break;
        case WStype_CONNECTED:
            Serial.println("\nSocket 1 conectado");
            break;
        case WStype_TEXT:
        case WStype_BIN:
        case WStype_ERROR:      
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}

void TakePhoto(){
  camera_fb_t *fb = esp_camera_fb_get();
  if(!fb){
    Serial.println("Error al capturar el frame");
    return;
  }
  ws1.sendBIN(fb->buf, fb->len);
  esp_camera_fb_return(fb);
}


void setup() {
  Serial.begin(115200);
  pinMode(boton, INPUT_PULLUP);
  btStop();
  configCamera();
  WiFi.begin("Totalplay-F39E", "Cosmos0970");
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conectado");
  String IP = WiFi.localIP().toString();
  Serial.print("IP address: " + IP);

  ws1.begin("192.168.100.9", 8080, "/");
  ws1.onEvent(ws1Event);
  ws1.setReconnectInterval(500);
}

void loop() {
  ws1.loop();
  estadoBoton = digitalRead(boton);
  if(estadoBoton == LOW){
    if(!fotoTomada){
      TakePhoto();
      TakePhoto();
      TakePhoto();
      Serial.println("boton pulsado");
      fotoTomada = true;
    }
  }else{ fotoTomada = false; }

  if( (millis()-lastTime) > timerDelay ){
    ws1.sendPing();
    lastTime = millis();
  }

}