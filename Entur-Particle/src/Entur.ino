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
#include "Departure.h"

// OLED Display
#define TFT_CS         A2
#define TFT_DC         A1
#define TFT_RST        A0


// HEX:
// Ruter Red: d42c1f
// Ruter Blue: 3d93e8

#define TRAM_BLUE      0x2bd3e0
#define BUS_RED        0x07FF
#define HEADLINE_COLOR 0xd42c1f

Adafruit_ST7735 screen = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

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
Departure firstDeparture;
Departure secondDeparture;


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

  firstDeparture.line = rLine;
  firstDeparture.destination = rDestination;
  firstDeparture.timeUntil = rTimeUntilNext;

  // OBS: DENNE MÅ FIKSES
  JsonObject root_1 = doc[1];
  const char* rTimeUntilNext1 = root_1["timeUntilNext"]; // "17:49"
  const char* rEstimatedDepartureDate1 = root_1["estimatedDepartureDate"]; // "2019-12-08T16:49:31.000Z"
  const char* rLine1 = root_1["line"]; // "17"
  const char* rDestination1 = root_1["destination"]; // "Rikshospitalet"

  secondDeparture.line = rLine1;
  secondDeparture.destination = rDestination1;
  secondDeparture.timeUntil = rTimeUntilNext1;

}

void drawLineNumber(int row, String lineNumber) {

  if (row == 0) {

    // Row 1
    int lineX = 0;
    int lineY = 18;

    screen.fillRect(lineX, lineY, 36, 24, TRAM_BLUE);
    screen.drawChar(lineX+8, lineY+4, lineNumber[0], ST7735_BLACK, TRAM_BLUE, 2);
    screen.drawChar(lineX+18, lineY+4, lineNumber[1], ST7735_BLACK, TRAM_BLUE, 2);

  } else if (row == 1) {

    // Row 2
    int r2LineX = 0;
    int r2LineY = 50;

    screen.fillRect(r2LineX, r2LineY, 36, 24, BUS_RED);
    screen.drawChar(r2LineX+8, r2LineY+4, lineNumber[0], ST7735_BLACK, BUS_RED, 2);
    screen.drawChar(r2LineX+18, r2LineY+4, lineNumber[1], ST7735_BLACK, BUS_RED  , 2);

  }


}

void drawDestination() {

}

void loop() {

  screen.setCursor(0, 0);
  screen.setTextWrap(false);

  screen.setCursor(0, 2);
  screen.setTextSize(1);
  screen.setTextColor(HEADLINE_COLOR);
  screen.println("NESTE TRIKK");
  screen.print("\n");

  ////////
  // ROW 1
  ////////

  // Line Number
  drawLineNumber(0, firstDeparture.line.c_str());

  // Destination
  screen.setTextSize(1);
  screen.setCursor(46, 16);
  screen.setTextColor(ST7735_BLACK);
  screen.println(firstDeparture.destination);

  // Time Until
  screen.setTextSize(2);
  screen.setCursor(46, 28);
  screen.setTextColor(ST7735_BLACK);
  screen.println(firstDeparture.timeUntil);

  ////////
  // ROW 2
  ////////

  // Line Number
  drawLineNumber(1, secondDeparture.line);

  // Destination
  screen.setTextSize(1);
  screen.setCursor(46, 48);
  screen.setTextColor(ST7735_BLACK);
  screen.println(secondDeparture.destination);

  // Time Until
  screen.setTextSize(2);
  screen.setCursor(46, 60);
  screen.setTextColor(ST7735_BLACK);
  screen.println(secondDeparture.timeUntil);


  ////////
  // Coming soon
  ////////

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {

  } else {
  }

  delay(3000);

}
