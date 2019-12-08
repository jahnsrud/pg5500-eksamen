/*
* Project Entur
* Description:
* Author:
* Date:
*/

#include "Adafruit_ST7735/Adafruit_ST7735.h"
#include "HttpClient/HttpClient.h"
#include "ArduinoJson-v6.13.0.h"

#define TFT_CS         A2
#define TFT_DC         A1
#define TFT_RST        A0

// OLED Display
Adafruit_ST7735 screen = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Button
const int buttonPin = D5;
int buttonState = 0;

// HTTP
HttpClient http;

http_header_t headers[] = {
  { "Content-Type", "application/json" },
  //  { "Accept" , "application/json" },
  { "Accept" , "*/*"},
  { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

const String HOSTNAME = "192.168.1.117";
const String PORT = "3000";
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


void drawText(char *text, uint16_t color, int *textSize) {
  screen.setCursor(0, 0);
  screen.setTextColor(color);
  screen.setTextWrap(true);
  // screen.setTextSize(textSize);
  screen.print(text);
}

void getRealtimeEstimate() {
  Serial.println();
  Serial.println("Application>\tStart of Loop.");
  // Request path and body can be set at runtime or at setup.
  request.hostname = HOSTNAME;
  request.port = PORT;
  request.path = PATH;

  // The library also supports sending a body with your request:
  //request.body = "{\"key\":\"value\"}";

  // Get request
  http.get(request, response, headers);
  Serial.print("Application>\tResponse status: ");
  Serial.println(response.status);

  Serial.print("Application>\tHTTP Response Body: ");
  Serial.println(response.body);

  tempResponse = response.body;

}

void parseResponse() {
  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);

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


  screen.println("@markus testing something");
  // screen.println("17: 0 min (Trikk)");
  // screen.println("30: 1 min (Buss)");

  //screen.println(tempResponse);
  Serial.println(tempResponse);

  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
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
