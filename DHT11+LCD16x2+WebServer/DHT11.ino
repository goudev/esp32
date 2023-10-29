#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "NomeDaRedeWifi";
const char* password = "SenhaDaRedeWifi";

// DHT11 settings
#define DHT11_PIN  15
DHT dht11(DHT11_PIN, DHT11);

// LCD settings
LiquidCrystal_I2C LCD(0x27, 16, 2);

// Web server
AsyncWebServer server(80);

// Function declarations
void initializeComponents();
void readAndDisplaySensorData();
String getSensorData();

void setup() {
  initializeComponents();

  // Serve web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body style='background-color: #336699; color: white; text-align: center; font-size: 64px;'>";
    html += getSensorData();
    html += "</body></html>";
    request->send(200, "text/html", html);
  });


  // Start server
  server.begin();
}

void loop() {
  readAndDisplaySensorData();
  delay(1000);
}

void initializeComponents() {
  // Initialize Serial
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dht11.begin();

  // Initialize LCD
  LCD.init();
  LCD.backlight();

  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    LCD.setCursor(0, 0);
    LCD.print("Conectando a ");
    LCD.setCursor(0, 1);
    LCD.print("rede Wifi...");
    delay(1000);
  }
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Conexao OK!");
  LCD.setCursor(0, 1);
  LCD.print(WiFi.localIP().toString());  // Exibir o IP atribuído
  delay(10000);
}

void readAndDisplaySensorData() {
  float humi  = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  if (isnan(tempC) || isnan(humi)) {
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Falha na leitura");
    LCD.setCursor(0, 1);
    LCD.print("do sensor DHT11!");
    return;
  }

  LCD.setCursor(0, 0);
  LCD.print("Temperatura: ");
  LCD.setCursor(13, 0);
  LCD.print((int)tempC);
  LCD.print("c");

  LCD.setCursor(0, 1);
  LCD.print("Umidade ar:  ");
  LCD.setCursor(13, 1);
  LCD.print((int)humi);
  LCD.print("%");
}

String getSensorData() {
  float humi  = dht11.readHumidity();
  float tempC = dht11.readTemperature();
  
  if (isnan(tempC) || isnan(humi)) {
    return "Failed to read from DHT11 sensor!";
  }

  String data = "Temperatura: " + String((int)tempC) + "c\n<br>";
  data += "Umidade relativa do ar: " + String((int)humi) + "%";
  
  return data;
}