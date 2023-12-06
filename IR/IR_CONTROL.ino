#include <WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Substitua com suas credenciais de Wi-Fi e informações do servidor MQTT
const char* ssid = "sid";
const char* password = "senha";
const char* mqtt_server = "url";

// Pino do LED IR
const uint16_t kIrLedPin = 15;
IRsend irsend(kIrLedPin);

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  if (String(topic) == "controle/tv_tcl") {
    if(messageTemp == "ligar") {
      irsend.sendNikai(0xD5F2A, 24);
    } else if(messageTemp == "aumentar_volume") {
      irsend.sendNikai(0xD0F2F, 24);
    } else if(messageTemp == "abaixar_volume") {
      irsend.sendNikai(0xD1F2E, 24);
    } else if(messageTemp == "desligar") {
      irsend.sendNikai(0xD5F2A, 24);
    } else if(messageTemp == "configuracoes") {
      irsend.sendNikai(0x30FCF, 24);
    }else if(messageTemp == "voltar") {
      irsend.sendNikai(0xD8F27, 24);
    }else if(messageTemp == "inicio") {
      irsend.sendNikai(0xF7F08, 24);
    }else if(messageTemp == "ok") {
      irsend.sendNikai(0xBFF4, 24);
    }else if(messageTemp == "netflix") {
      irsend.sendNikai(0x10FEF, 24);
    }
    
    
  }else if (String(topic) == "controle/ar_lg") {
    if(messageTemp == "ligar") {
      irsend.sendLG(0x880085D, 28);
    } else if(messageTemp == "desligar") {
      irsend.sendLG(0x88C0051, 28);
    } else if(messageTemp == "frio_vento_fraco") {
      irsend.sendLG(0x8808800, 28);
    } else if(messageTemp == "frio_vento_medio") {
      irsend.sendLG(0x8808822, 28);
    } else if(messageTemp == "frio_vento_forte") {
      irsend.sendLG(0x8808844, 28);
    }else if(messageTemp == "vento_automatico") {
      irsend.sendLG(0x8808855, 28);
    }else if(messageTemp == "ar_quente") {
      irsend.sendLG(0x880C949, 28);
    }else if(messageTemp == "ar_frio") {
      irsend.sendLG(0x880834F, 28);
    }else if(messageTemp == "desumidificar") {
      irsend.sendLG(0x8809845, 28);
    }
    
    
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", "usuario", "senha")) { // Substitua com seu nome de usuário e senha
      client.subscribe("controle/tv");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  irsend.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
