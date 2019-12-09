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
#include "Colors.h"
#include "API.h"

// OLED Display
#define TFT_CS         A2
#define TFT_DC         A1
#define TFT_RST        A0

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

/**
* Draws line numbers with a background color
*/

void drawLineNumber(int row, String lineNumber) {

  int lineColor;

  if (lineNumber == "17") {
    lineColor = TRAM_BLUE;
  } else if (lineNumber == "30") {
    lineColor = BUS_RED;
  }

  int lineX = 0;
  int lineY = 0;

  // Set cordinates for rows
  if (row == 0) {
    lineY = 18;
  } else if (row == 1) {
    lineY = 50;
  }

  screen.fillRect(lineX, lineY, 36, 24, lineColor);
  screen.drawChar(lineX+8, lineY+4, lineNumber[0], ST7735_BLACK, lineColor, 2);
  screen.drawChar(lineX+18, lineY+4, lineNumber[1], ST7735_BLACK, lineColor, 2);


}

void drawDeparture(int row, String destination, String timeUntil) {

  int baselineX = 46;

  int destinationY = 16;
  int timeUntilY = 0;

  if (row == 0) {
    destinationY = 16;
    timeUntilY = 28;
  } else if (row == 1) {
    destinationY = 48;
    timeUntilY = 60;

  }

  // Destination
  screen.setTextSize(1);
  screen.setCursor(baselineX, destinationY);
  screen.setTextColor(ST7735_BLACK);
  screen.println(destination);

  // Time Until
  screen.setTextSize(2);
  screen.setCursor(baselineX, timeUntilY);
  screen.setTextColor(ST7735_BLACK);
  screen.println(timeUntil);

}

void drawHeadline(String headline) {
  screen.setCursor(0, 2);
  screen.setTextSize(1);
  screen.setTextColor(HEADLINE_COLOR);
  screen.println(headline);
  screen.print("\n");
}

void loop() {

  screen.setCursor(0, 0);
  screen.setTextWrap(false);

  drawHeadline("NESTE TRIKK");

  ////////
  // ROW 1
  ////////

  drawLineNumber(0, firstDeparture.line.c_str());
  drawDeparture(0, firstDeparture.destination, firstDeparture.timeUntil);

  ////////
  // ROW 2
  ////////

  drawLineNumber(1, secondDeparture.line);
  drawDeparture(1, secondDeparture.destination, secondDeparture.timeUntil);


  ////////
  // Coming soon
  ////////

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {

  } else {

  }

  delay(2500);

}
