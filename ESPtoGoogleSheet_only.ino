//----------------------------------------Include the NodeMCU ESP32 Library

#ifdef ESP32
//  #include <WiFi.h>
  #include <HTTPClient.h>
  #include <IoTtweetESP32.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Replace with your network credentials
const char* ssid     = "smartmonitor";
const char* password = "01004714";

//----------------------------------------Host & httpsPort

const char* host = "script.google.com";
const int httpsPort = 443;

//----------------------------------------

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;  // I2C
//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI


WiFiClientSecure client; //--> Create a WiFiClientSecure object.

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

String GAS_ID = "AKfycbzR-_4XTIuvUlCx0rEbsZ-_wjMGFOLfF7hPdGN6IxXNAvZQAJCqrHVIpvR4DA4_g15S"; //--> spreadsheet script ID

//============================================ void setup

void setup() {

  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    while (1);
  }
}

//==============================================================================

//============================================================================== void loop

void loop() {


  now = millis();
  // Publishes new temperature and humidity every 3 seconds
  if (now - lastMeasure > 10000) {
    lastMeasure = now;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = bme.readHumidity();
    // Read temperature as Celsius (the default)
    float t = bme.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float f = bme.readTemperature(true);
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from BME sensor!");
      return;
    }

    // Computes temperature values in Celsius
    //float hic = bme.computeHeatIndex(t, h, false);
   // static char temperatureTemp[7];
    //dtostrf(hic, 6, 2, temperatureTemp);
  
    // Uncomment to compute temperature values in Fahrenheit
    // float hif = bme.computeHeatIndex(f, h);
    // static char temperatureTemp[7];
    // dtostrf(hif, 6, 2, temperatureTemp);
   
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
   // Serial.print(f);
   // Serial.print(" *F\t Heat index: ");
   // Serial.print(hic);
    //Serial.println(" *C ");
    // Serial.print(hif);
    // Serial.println(" *F");
     sendData(t,h);
     }
}

//*****
//==============================================================================
void sendData(float value,float value2) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------Proses dan kirim data  
  float string_temp = value;
  float string_humi = value2;
  String url = "/macros/s/" + GAS_ID + "/exec?temp=" + string_temp + "&humi="+string_humi; //  2 variables
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP32\r\n" +
         "Connection: close\r\n\r\n");
  Serial.println("request sent");
  //----------------------------------------

  //---------------------------------------
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp32/Arduino CI successfull!");
  } else {
    Serial.println("esp32/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  client.stop();
  //----------------------------------------
}

//===============================================
