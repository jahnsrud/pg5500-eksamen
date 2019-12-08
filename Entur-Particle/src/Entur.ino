/*
* Project Entur
* Description:
* Author:
* Date:
*/

#include "Adafruit_ST7735/Adafruit_ST7735.h"
#include "HttpClient/HttpClient.h"

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
  //  { "Content-Type", "application/json" },
  //  { "Accept" , "application/json" },
  { "Accept" , "*/*"},
  { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

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
  request.hostname = "192.168.1.117";
  request.port = 3000;
  request.path = "/";

  // https://jsonplaceholder.typicode.com/posts/1

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


/*

// This #include statement was automatically added by the Particle IDE.
#include <HttpClient.h>

#define RED_LED    D1
#define GREEN_LED  D2
#define BLUE_LED   D4
#define BUTTON_PIN D3

#define WIFI_SSID "SSID"
#define WIFI_PASS "password"

void initWifi();
char* getWifiStatusStr(int stat);

void blink(int ledpin, int timeON = 250, int timeOFF = 250);
void blinkAll(int times = 1, int timeON = 100, int timeOFF = 100);

void setup() {
pinMode(RED_LED,   OUTPUT);
pinMode(BLUE_LED,  OUTPUT);
pinMode(GREEN_LED, OUTPUT);
pinMode(BUTTON_PIN,INPUT_PULLUP);
blinkAll(3, 150,150);

Serial.begin(9600);
initWifi();
}

char* ENTUR_HOST  = "api.entur.io";
char* ENTUR_URL   = "journey-planner/v2/graphql/";
char* GRAPHQL_REQ = "?query=%7B%0A%20%20stopPlace(id%3A%20\"NSR%3AStopPlace%3A6219\")%20%7B%0A%20%20%20%20id%0A%20%20%20%20name%0A%20%20%20%20estimatedCalls(timeRange%3A%205000%2C%20numberOfDepartures%3A%204%2C%20whiteListed%3A%20%7Blines%3A%20\"RUT%3ALine%3A76\"%7D)%20%7B%0A%20%20%20%20%20%20realtime%0A%20%20%20%20%20%20expectedArrivalTime%0A%20%20%20%20%20%20expectedDepartureTime%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D%0A";

void enturTest(){
blink(BLUE_LED);
WiFiClient client;

Serial.printf("\n[Connecting to %s ... ", ENTUR_HOST);
if (client.connect(ENTUR_HOST, 80))
{
Serial.println("connected]");
blink(GREEN_LED);

String address = String(ENTUR_HOST) + "/" + String(ENTUR_URL);
Serial.printf("address : %s\n", &address[0]);
Serial.printf("post req: %s\n", GRAPHQL_REQ);

HTTPClient http;
http.begin(address);

http.addHeader("Content-Type",  "application/json");
//http.addHeader("Content-Type",  "application/x-www-form-urlencoded");
http.addHeader("ET-Client-Name","id-id"); //required from the EnTur API docs

int    httpCode = http.POST(GRAPHQL_REQ);
String payload  = http.getString();

Serial.println(httpCode);
Serial.println(payload);
http.end();

Serial.println("\n[Disconnected]");
}
else{
Serial.println("connected]");
blink(RED_LED);
}
}

void loop() {
if(digitalRead(BUTTON_PIN) == LOW){
enturTest();
}
delay(10);
}*/
