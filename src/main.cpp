#include <Arduino.h>
#include <WebSocket.h>
#include <TemperatureSensor.h>
#include <LedHandler.h>
#include <MqttService.h>

const unsigned long timeIntervall = 60*1000/2; // 15 minutes
unsigned long timeStamp = 0;

void executePeriodicTasks() {
  const float temperature = TemperatureSensor::readSensor();
  WebSocket::notifyClients(String(temperature));
  MqttService::publish(String(temperature));
}

void setup() {
  Serial.begin(115200);
  setupWifiManager();
  setupWebServer();
  WebSocket::setUpWebSocket();
  TemperatureSensor::initTemperatureSensor();
  LedHandler::initLedHandler();
  MqttService::connectToMqtt();
}

void loop() {
  // Code in this section only runs every timeIntervall - rollover safe
  if(millis() - timeStamp > timeIntervall ){
    executePeriodicTasks();
    timeStamp = millis();  // reset the timer
   }
}