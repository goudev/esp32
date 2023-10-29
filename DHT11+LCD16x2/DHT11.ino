#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Constants
#define DHT11_PIN  15

// Global objects
LiquidCrystal_I2C LCD(0x27, 16, 2);
DHT dht11(DHT11_PIN, DHT11);

// Function declarations
void initializeComponents();
void readAndDisplaySensorData();

void setup() {
  initializeComponents();
}

void loop() {
  readAndDisplaySensorData();
  delay(10000);
}

/**
 * Initialize all components
 */
void initializeComponents() {
  // Initialize Serial
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dht11.begin();

  // Initialize LCD
  LCD.init();
  LCD.backlight();
}

/**
 * Read sensor data and display on LCD
 */
void readAndDisplaySensorData() {
  float humi  = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  // Check for invalid readings
  if (isnan(tempC) || isnan(humi)) {
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Falha na leitura");
    LCD.setCursor(0, 1);
    LCD.print("do sensor DHT11!");
    return;
  }

  // Display temperature
  LCD.setCursor(0, 0);
  LCD.print("Temperatura: ");
  LCD.setCursor(13, 0);
  LCD.print((int)tempC);
  LCD.print("c");

  // Display humidity
  LCD.setCursor(0, 1);
  LCD.print("Umidade ar:  ");
  LCD.setCursor(13, 1);
  LCD.print((int)humi);
  LCD.print("%");
}
