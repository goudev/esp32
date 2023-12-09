#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <DHT.h>
#define DHT11_PIN  4
#include <SimpleTimer.h>

DHT dht11(DHT11_PIN, DHT11);

const uint16_t kIrLed = 10;  // Substitua com o pino correto para o LED IR
IRsend irsend(kIrLed);      // Instância IRsend

const char *service_name = "PROV_ESPR1";
const char *pop = "12345";

bool switch1_state = false;
bool switch2_state = false;

static Switch *switch1 = NULL;
static Switch *switch2 = NULL;

static TemperatureSensor temperature("Temperature");
static TemperatureSensor humidity("Humidity");

SimpleTimer Timer;

void ligarTv() {
    irsend.sendNikai(0xD5F2A, 24);
}

void desligarTv() {
    irsend.sendNikai(0xD5F2A, 24);
}

void write_callback(Device *device, Param *param, const param_val_t val,
  void *priv_data, write_ctx_t *ctx) {
    const char *param_name = param->getParamName();
    if (strcmp(param_name, "Power") == 0) {
        switch1_state = val.val.b;
        if (switch1_state) {
            ligarTv();  // Enviar sinal IR quando o switch for ativado
        }
        param->updateAndReport(val);
    }
}

void write_callback2(Device *device, Param *param, const param_val_t val,
  void *priv_data, write_ctx_t *ctx) {
    const char *param_name = param->getParamName();
    if (strcmp(param_name, "Power") == 0) {
        switch2_state = val.val.b;
        if (switch2_state) {
            desligarTv();
        }
        param->updateAndReport(val);
    }
}

void setup() {
  irsend.begin();
  dht11.begin(); 
  Timer.setInterval(3000);
  Node my_node;
  my_node = RMaker.initNode("ESP32 C3");

  switch1 = new Switch("Ligar TV", NULL);
  if (!switch1) {
    return;
  }

  switch2 = new Switch("Desligar TV", NULL);
  if (!switch2) {
      return;
  }

  my_node.addDevice(temperature);
  my_node.addDevice(humidity);

  switch1->addCb(write_callback);
  my_node.addDevice(*switch1);

  switch2->addCb(write_callback2);
  my_node.addDevice(*switch2);

  RMaker.start();
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
}

void loop() {
  float humi  = dht11.readHumidity();
  float tempC = dht11.readTemperature();
  float tempF = dht11.readTemperature(true);

  if (Timer.isReady()) {
    Send_Sensor();
    Timer.reset();
  }
}

void Send_Sensor() {
    float h = dht11.readHumidity();
    float t = dht11.readTemperature();

    if (!isnan(h) && !isnan(t)) {
        temperature.updateAndReportParam("Temperatura", t);
        humidity.updateAndReportParam("Umidade", h);
    }
}
