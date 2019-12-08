/*
* Project Entur
* Description: Exam in PG5500: Embedded Systems
* Author: 5016 - Markus Jahnsrud
* Date: 2019-12-10
*/

#include "Adafruit_ST7735/Adafruit_ST7735.h"
#include "HttpClient/HttpClient.h"
#include "ArduinoJson.h"
#include "Adafruit_ST7735/fonts.h"

// OLED Display
#define TFT_CS         A2
#define TFT_DC         A1
#define TFT_RST        A0
Adafruit_ST7735 screen = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// #define TRAM_BLUE 0x3d93e8;

// Button
const int buttonPin = D5;
int buttonState = 0;

// HTTP
HttpClient http;

http_header_t headers[] = {
  { "Content-Type", "application/json" },
  { "Accept" , "application/json" },
  { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

const String HOSTNAME = "192.168.1.117";
const int PORT = 3000;
const String PATH = "/";

// TODO: Improve
String line; // String, because it may contain letters like "11N"
String destination;
String timeUntil;


void setup() {

  Serial.begin (9600);
  Serial.println("Hello, Kristiania!");
  configureDisplay();

  pinMode(buttonPin, INPUT);

  getRealtimeEstimate();
}

void configureDisplay() {

  // ST7735S 0.96"
  screen.initR(INITR_MINI160x80);

  // Landscape orientation
  screen.setRotation(3);

  // Clear the display
  screen.fillScreen(ST7735_WHITE);

}

void getRealtimeEstimate() {
  Serial.println("Application>\tFetching...");

  // Request path and body can be set at runtime or at setup.
  request.hostname = HOSTNAME;
  request.port = PORT;
  request.path = PATH;

  // Get request
  http.get(request, response, headers);

  Serial.print("Application>\tResponse status: ");
  Serial.println(response.status);

  Serial.print("Application>\tHTTP Response Body: ");
  Serial.println(response.body);

  parseResponse(response.body);

}

void parseResponse(String response) {

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + 280;
  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, response.c_str());

  JsonObject root_0 = doc[0];
  const char* rTimeUntilNext = root_0["timeUntilNext"]; // "17:49"
  const char* rEstimatedDepartureDate = root_0["estimatedDepartureDate"]; // "2019-12-08T16:49:31.000Z"
  const char* rLine = root_0["line"]; // "17"
  const char* rDestination = root_0["destination"]; // "Rikshospitalet"

  // tempResponse = (String)"" + line + " " + destination + "(" + timeUntilNext + ")";

  line = rLine;
  destination = rDestination;
  timeUntil = rTimeUntilNext;


}

// TODO: Dynamic
void drawLineNumber() {
  int lineX = 2;
  int lineY = 22;

  // Ruter Red: d42c1f
  // Ruter Blue: 3d93e8

  screen.fillRect(lineX, lineY, 36, 24, 0x3d93e8);
  screen.drawChar(lineX+8, lineY+4, '1', ST7735_BLACK, 0x3d93e8, 2);
  screen.drawChar(lineX+18, lineY+4, '7', ST7735_BLACK, 0x3d93e8, 2);

  buttonState = digitalRead(buttonPin);
}

void drawDestination() {

}

void loop() {
  screen.setTextWrap(true);
  screen.setTextSize(2);
  screen.setCursor(0, 0);
  screen.setTextColor(ST7735_BLACK);

  screen.setTextSize(1);
  screen.setTextColor(ST7735_BLUE);
  screen.println("NESTE TRIKK");
  screen.print("\n");

  drawLineNumber();

  // Destination
  screen.setTextSize(1);
  screen.setCursor(46, 18);
  screen.setTextColor(ST7735_BLACK);
  screen.println(destination);

// Time Until
  screen.setTextSize(2);
  screen.setCursor(46, 34);
  screen.setTextColor(ST7735_BLACK);
  screen.println(timeUntil);

  if (buttonState == HIGH) {

  } else {
  }

  delay(3000);

}
