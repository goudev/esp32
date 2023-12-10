#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 10;  // Substitua com o pino correto para o LED IR
IRsend irsend(kIrLed);      // Instância IRsend

const char *service_name = "PROV_ESPR1";
const char *pop = "12345";

static Device *tv_device = NULL;

void write_callback_tv(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx) {
    const char *param_name = param->getParamName();
    if (strcmp(param_name, "Power") == 0) {
        if (val.val.b) {
            irsend.sendNikai(0xD5F2A, 24);
            irsend.sendNikai(0xD5F2A, 24);
            irsend.sendNikai(0xD5F2A, 24);
            irsend.sendNikai(0xD5F2A, 24);
            irsend.sendNikai(0xD5F2A, 24);
        } else {
            irsend.sendNikai(0xD5F2A, 24);
            irsend.sendNikai(0xD5F2A, 24);
            irsend.sendNikai(0xD5F2A, 24);
            irsend.sendNikai(0xD5F2A, 24);
            irsend.sendNikai(0xD5F2A, 24);            
        }
    }else if (strcmp(param_name, "Volume") == 0) {
      if (val.val.b) {
          irsend.sendNikai(0xD5F2A, 24);            
      } else {
          irsend.sendNikai(0xD5F2A, 24);            
      }
    }

    param->updateAndReport(val);
}


void setup() {
  irsend.begin();
  Node my_node;
  my_node = RMaker.initNode("ESP32 C3");

//Power
  tv_device = new Device("TV TCL", ESP_RMAKER_DEVICE_TV, NULL);
  Param power_param("Power", ESP_RMAKER_PARAM_POWER, esp_rmaker_bool(false), PROP_FLAG_READ | PROP_FLAG_WRITE);
  tv_device->addParam(power_param);
  const esp_rmaker_param_t* internal_param_ptr = power_param.getParamHandle();
  esp_rmaker_param_add_ui_type(internal_param_ptr, "esp.ui.push-btn-big");

// Volume
    Param volume_param("Volume", NULL, esp_rmaker_int(0), PROP_FLAG_READ | PROP_FLAG_WRITE);
    tv_device->addParam(volume_param);
    const esp_rmaker_param_t* internal_volume_param_ptr = volume_param.getParamHandle();
    esp_rmaker_param_add_ui_type(internal_volume_param_ptr, ESP_RMAKER_UI_SLIDER);
    esp_rmaker_param_add_bounds(internal_volume_param_ptr, esp_rmaker_int(0), esp_rmaker_int(100), esp_rmaker_int(1));





  tv_device->addCb(write_callback_tv);

  my_node.addDevice(*tv_device);
  RMaker.start();
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
}

void loop() {
  // O loop pode ficar vazio
}
