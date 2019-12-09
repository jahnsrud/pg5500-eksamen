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
  { NULL, NULL } // Terminate headers with NULL
};

http_request_t request;
http_response_t response;

// Departures
Departure departures[4];
int departureTablePage = 0;

#define UPDATE_IN_SECONDS 20;
int refreshInSeconds = UPDATE_IN_SECONDS;

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

  screen.setTextWrap(false);


}

void getRealtimeEstimate() {
  Serial.println("Application>\tFetching...");

  request.hostname = HOSTNAME;
  // request.port = PORT;
  request.path = PATH;

  // GET request to our server
  http.get(request, response, headers);

  Serial.print("Response status: ");
  Serial.println(response.status);

  // Only proceed if status code is 200
  if (response.status == 200) {
    Serial.println("All good!");
    parseResponse(response.body);

  } else {
    drawHeadline("Error. Retrying...");
    delay(4000);
    screen.fillScreen(ST7735_WHITE);
    getRealtimeEstimate();

  }


}

void parseResponse(String response) {

  // From Documentation: Allocate the JSON document
  // arduinojson.org/v6/assistant is incredibly useful for calculating the capacity variable

  const size_t capacity = JSON_ARRAY_SIZE(4) + 4*JSON_OBJECT_SIZE(4) + 560;
  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, response.c_str());

  // Add the 4 results to the departures array.

  for (int i = 0; i < 4; i++) {
    JsonObject jsonDeparture = doc[i];

    const char* rTimeUntilNext = jsonDeparture["timeUntilNext"]; // "17:49"
    const char* rEstimatedDepartureDate = jsonDeparture["estimatedDepartureDate"]; // "2019-12-08T16:49:31.000Z"
    const char* rLine = jsonDeparture["line"]; // "17"
    const char* rDestination = jsonDeparture["destination"]; // "Rikshospitalet"

    // Convert to Departure
    Departure departure;
    departure.line = rLine;
    departure.destination = rDestination;
    departure.timeUntil = rTimeUntilNext;

    departures[i] = departure;

  }

  drawTable();

}

void drawTable() {

  // Prepare Data for our table
  Departure dep1;
  Departure dep2;

  if (departureTablePage == 0) {
    dep1 = departures[0];
    dep2 = departures[1];
    drawHeadline("HEIMDALSGATA (1/2)");

  } else {
    dep1 = departures[2];
    dep2 = departures[3];
    drawHeadline("HERSLEBS GATE (2/2)");

  }

  ////////
  // ROW 1
  ////////

  drawDeparture(0, dep1.destination, dep1.timeUntil, dep1.line);

  ////////
  // ROW 2
  ////////

  drawDeparture(1, dep2.destination, dep2.timeUntil, dep2.line);

}

void drawDeparture(int row, String destination, String timeUntil, String lineNumber) {

  // Set coordinates for our table rows.
  // The table supports only 2 rows at a time.

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

  // Line number (for example 17 or 30)
  drawLineNumber(row, lineNumber);

  // Destination
  screen.setTextSize(1);
  screen.setCursor(baselineX, destinationY);
  screen.setTextColor(ST7735_BLACK);
  screen.println(destination);

  // Time Until label
  screen.setTextSize(2);
  screen.setCursor(baselineX, timeUntilY);
  screen.setTextColor(ST7735_BLACK);
  screen.println(timeUntil);



}

/**
* Draws tram/bus line numbers with a background color
*/

void drawLineNumber(int row, String lineNumber) {

  int lineColor = HEADLINE_COLOR;

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

void drawHeadline(String headline) {
  screen.setCursor(0, 2);
  screen.setTextSize(1);
  screen.setTextColor(HEADLINE_COLOR);
  screen.println(headline);
  screen.print("\n");
}

void changePage() {

  // Clear the table first
  screen.fillScreen(ST7735_WHITE);

  if (departureTablePage == 0) {
    departureTablePage = 1;
  } else {
    departureTablePage = 0;
  }

  drawTable();
}

void refresh() {
  screen.fillScreen(ST7735_WHITE);
  getRealtimeEstimate();

}

void loop() {

  refreshInSeconds--;

  if (refreshInSeconds == 0) {
    refresh();
    refreshInSeconds = UPDATE_IN_SECONDS;
  }

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    changePage();
  }

  delay(1000);

}
