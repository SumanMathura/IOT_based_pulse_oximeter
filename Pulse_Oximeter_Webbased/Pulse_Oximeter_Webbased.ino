#include <ESP8266WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000

float BPM, SpO2,temp;
const char* ssid = "realme 8i";  // Enter SSID here
const char* password = "Suman2001#";  //Enter Password here

PulseOximeter pox;
uint32_t tsLastReport = 0;
ESP8266WebServer server(80);
void setup() {
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  delay(100);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");  
  Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
    
  }
}
void loop() {
  server.handleClient();
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    Serial.print("BPM: ");
    Serial.println(BPM);
    Serial.print("SpO2: ");
    Serial.print(SpO2);
    Serial.println("%");
    Serial.println("*********************************");
    Serial.println();
    tsLastReport = millis();
  }
}
void handle_OnConnect() {

  server.send(200, "text/html", SendHTML(BPM, SpO2, temp));
}
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
String SendHTML(float BPM, float SpO2, float temp) {
  String ptr = "<!DOCTYPE html>";
  ptr += "<html>";
  ptr += "<head>";
  ptr += "<title>ESP8266 WebServer</title>";
  ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.7.2/css/all.min.css'>";
  ptr += "<link rel='stylesheet' type='text/css' href='styles.css'>";
  ptr += "</style>";
  ptr += "<script>\n";
  ptr += "</head>";
  ptr += "<body>";
  ptr += "<div id='page'>";
  ptr += "<div class='header'>";
  ptr += "<h1>Iot Based Pulse Oximeter</h1>";    
  ptr += "<h1>Netaji Subhash Engineering College</h1>";
  ptr += "</div>";
  ptr += "<div id='content' align='center'>";
  ptr += "<div class='box-full' align='left'>";
  ptr += "<h2>Sensor Readings</h2>";
  ptr += "<div class='sensors-container'>";
  //For Heart Rate
  ptr += "<p class='sensor'>";
  ptr += "<i class='fas fa-heartbeat' style='color:#cc3300'></i>";
  ptr += "<span class='sensor-labels'> Heart Rate </span>";
  ptr += (int)BPM;
  ptr += "<sup class='units'>BPM</sup>";
  ptr += "</p>";
  ptr += "<hr>";
  //For Sp02
  ptr += "<p class='sensor'>";
  ptr += "<i class='fas fa-burn' style='color:#f7347a'></i>";
  ptr += "<span class='sensor-labels'> Sp02 </span>";
  ptr += (int)SpO2;
  ptr += "<sup class='units'>%</sup>";
  ptr += "</p>";
  ptr += "<hr>";
  return ptr;
}
