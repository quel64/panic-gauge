#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <SPI.h>

//Configuration
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey = "CFXTEHJXRNQR26X1";

//Pins
#define BTN_TIMEFRAME 2
#define BTN_THEME 3

//Globals
TFT_eSPI tft = TFT_eSPI(); 
const int numPoints = 30; //Number of data points for the graph
float vixData[numPoints];
float currentVix = 0.0;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 60000; //Fetch every 60 seconds

//State Variables
bool isDarkMode = true;
String timeframe = "1min";

//Colors
#define DARK_BG    TFT_BLACK
#define DARK_TEXT  TFT_WHITE
#define DARK_GRID  0x4208
#define LIGHT_BG   TFT_WHITE
#define LIGHT_TEXT TFT_BLACK
#define LIGHT_GRID 0xBDF7
#define VIX_CALM   TFT_GREEN
#define VIX_WARN   TFT_YELLOW
#define VIX_PANIC  TFT_RED

void setup() {
  Serial.begin(115200);
  
  //Initialize Buttons
  pinMode(BTN_TIMEFRAME, INPUT_PULLUP);
  pinMode(BTN_THEME, INPUT_PULLUP);

  //Initialize Screen
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(DARK_BG);

  //Draw Boot Screen
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(50, 100, 4);
  tft.print("Booting VIX Terminal...");

  //Connect to WiFi
  WiFi.begin(ssid, password);
  tft.setCursor(50, 130, 2);
  tft.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
    Serial.print(".");
  }
  
  //Initial Fetch & Draw
  tft.fillScreen(isDarkMode ? DARK_BG : LIGHT_BG);
  drawStaticUI();
  fetchVIXData();
}

void loop() {
  //1. Handle Timeframe Button (Pin 2)
  if (digitalRead(BTN_TIMEFRAME) == LOW) {
    delay(200);
    timeframe = (timeframe == "1min") ? "5min" : "1min";
    tft.fillScreen(isDarkMode ? DARK_BG : LIGHT_BG);
    drawStaticUI();
    fetchVIXData();
  }

  //2. Handle Theme Button (Pin 3)
  if (digitalRead(BTN_THEME) == LOW) {
    delay(200);
    isDarkMode = !isDarkMode;
    tft.fillScreen(isDarkMode ? DARK_BG : LIGHT_BG);
    drawStaticUI();
    drawGraph();
    updateStatusText();
  }

  //3. Auto-update data every 60 seconds
  if (millis() - lastUpdate >= updateInterval) {
    fetchVIXData();
    lastUpdate = millis();
  }
}

//Fetch Data from API
void fetchVIXData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    //Alpha Vantage URL
    String url = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=VIX&interval=5min&apikey=" + String(apiKey);
    
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      
      DynamicJsonDocument doc(8192);
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        JsonObject timeSeries = doc["Time Series (5min)"];
        int index = numPoints - 1;
        
        int count = 0;
        for (JsonPair kv : timeSeries) {
          if (count < numPoints) {
            vixData[index] = kv.value()["4. close"].as<float>();
            if (count == 0) currentVix = vixData[index];
            index--;
            count++;
          }
        }
        
        drawGraph();
        updateStatusText();
      }
    }
    http.end();
  }
}

//Draw the UI
void drawStaticUI() {
  uint16_t bg = isDarkMode ? DARK_BG : LIGHT_BG;
  uint16_t text = isDarkMode ? DARK_TEXT : LIGHT_TEXT;
  uint16_t grid = isDarkMode ? DARK_GRID : LIGHT_GRID;

  //Header
  tft.setTextColor(text, bg);
  tft.drawString("VIX INDEX", 10, 10, 4);
  
  tft.setTextColor(TFT_DARKGREY, bg);
  tft.drawString("REAL-TIME VOLATILITY (" + timeframe + ")", 10, 40, 2);

  //Graph Border
  tft.drawRect(10, 70, 300, 130, grid);
  
  //Graph Gridlines
  for (int i = 1; i < 4; i++) tft.drawFastHLine(10, 70 + (i * 32), 300, grid);
  for (int i = 1; i < 6; i++) tft.drawFastVLine(10 + (i * 50), 70, 130, grid);
}

//Render the Graph
void drawGraph() {
  uint16_t bg = isDarkMode ? DARK_BG : LIGHT_BG;
  uint16_t grid = isDarkMode ? DARK_GRID : LIGHT_GRID;
  
  //Clear the inside of the graph box to redraw
  tft.fillRect(11, 71, 298, 128, bg);
  
  //Redraw inside gridlines
  for (int i = 1; i < 4; i++) tft.drawFastHLine(11, 70 + (i * 32), 298, grid);
  for (int i = 1; i < 6; i++) tft.drawFastVLine(10 + (i * 50), 71, 128, grid);

  //Find Min and Max for auto-scaling
  float minVix = vixData[0];
  float maxVix = vixData[0];
  for (int i = 1; i < numPoints; i++) {
    if (vixData[i] < minVix) minVix = vixData[i];
    if (vixData[i] > maxVix) maxVix = vixData[i];
  }
  
  //Add 10% padding to top and bottom of graph scaling
  float padding = (maxVix - minVix) * 0.1;
  if (padding == 0) padding = 1.0;
  minVix -= padding;
  maxVix += padding;

  //Determine line color based on latest VIX level
  uint16_t lineColor = VIX_CALM;
  if (currentVix > 20) lineColor = VIX_WARN;
  if (currentVix >= 30) lineColor = VIX_PANIC;

  //Draw the lines connecting the data points
  int graphW = 298;
  int graphH = 128;
  int graphX = 11;
  int graphY = 199;

  for (int i = 0; i < numPoints - 1; i++) {
    int x1 = graphX + (i * (graphW / (numPoints - 1)));
    int x2 = graphX + ((i + 1) * (graphW / (numPoints - 1)));
    
    int y1 = graphY - ((vixData[i] - minVix) / (maxVix - minVix) * graphH);
    int y2 = graphY - ((vixData[i + 1] - minVix) / (maxVix - minVix) * graphH);

    tft.drawLine(x1, y1, x2, y2, lineColor);
    tft.drawLine(x1, y1+1, x2, y2+1, lineColor);
  }
}

//Update Text Labels
void updateStatusText() {
  uint16_t bg = isDarkMode ? DARK_BG : LIGHT_BG;
  
  //Print Big Current Price
  tft.setTextColor(currentVix >= 30 ? VIX_PANIC : (currentVix > 20 ? VIX_WARN : VIX_CALM), bg);
  tft.drawRightString(String(currentVix, 2) + "   ", 310, 10, 6); 

  //Print Status at bottom
  tft.setTextColor(isDarkMode ? DARK_TEXT : LIGHT_TEXT, bg);
  tft.drawString("STATUS: ", 10, 210, 2);
  
  tft.fillRect(70, 210, 150, 20, bg);
  if (currentVix >= 30) {
    tft.setTextColor(VIX_PANIC, bg);
    tft.drawString("EXTREME FEAR", 70, 210, 2);
  } else if (currentVix > 20) {
    tft.setTextColor(VIX_WARN, bg);
    tft.drawString("ELEVATED RISK", 70, 210, 2);
  } else {
    tft.setTextColor(VIX_CALM, bg);
    tft.drawString("MARKET CALM", 70, 210, 2);
  }
}