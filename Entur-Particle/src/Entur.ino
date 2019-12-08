/*
* Project Entur
* Description: Exam in PG5500: Embedded Systems
* Author: 5016 - Markus Jahnsrud
* Date: 2019-12-10
*/

#include "Adafruit_ST7735/Adafruit_ST7735.h"
#include "HttpClient/HttpClient.h"
#include "ArduinoJson.h"

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

const String HOSTNAME = "192.168.1.117";
const int PORT = 3000;
const String PATH = "/";

String tempResponse;


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
  const char* timeUntilNext = root_0["timeUntilNext"]; // "17:49"
  const char* estimatedDepartureDate = root_0["estimatedDepartureDate"]; // "2019-12-08T16:49:31.000Z"
  const char* line = root_0["line"]; // "17"
  const char* destination = root_0["destination"]; // "Rikshospitalet"

  tempResponse = (String)"" + line + " " + destination + "(" + timeUntilNext + ")";

  Serial.println("What is this?");
  Serial.print(root_0);
  Serial.print(tempResponse);

}

void drawText(char *text, uint16_t color, int *textSize) {
  screen.setCursor(0, 0);
  screen.setTextColor(color);
  screen.setTextWrap(true);
  // screen.setTextSize(textSize);
  screen.print(text);
}

void loop() {
  screen.setTextWrap(true);
  screen.setTextSize(2);
  screen.setCursor(0, 0);
  screen.setTextColor(ST7735_BLACK);


  // screen.setCursor(30, 3);
  // screen.setFont(HERO_10);
  // screen.drawPixel(screen.width()/2, screen.height()/2, ST7735_GREEN);

  // screen.drawLine(0, 0, screen.width()-1, screen.height()-1, ST7735_YELLOW);
  // screen.drawLine(screen.width()-1, 0, 0, screen.height()-1, ST7735_YELLOW);
  // screen.drawPixel(0, screen.height()/2, ST7735_GREEN);


  screen.println("NESTE TRIKK");

  screen.println(tempResponse);

  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {

    /*
    screen.fillScreen(ST7735_WHITE);
    screen.println("Pressed! TEST TESTING TEST TESTING TEST TESTING TEST TESTING");
    screen.println("Pressed! TEST TESTING TEST TESTING TEST TESTING TEST TESTING");
    screen.println("Pressed! TEST TESTING TEST TESTING TEST TESTING TEST TESTING");

    // turn LED on:
    // digitalWrite(ledPin, HIGH);
    */
  } else {

    /*
    screen.fillScreen(ST7735_WHITE);

    // turn LED off:
    // digitalWrite(ledPin, LOW);
    screen.println("Not pressed!");
    screen.println("Not pressed!");

    screen.println("Not pressed!");

    */


  }

  delay(500);

}
