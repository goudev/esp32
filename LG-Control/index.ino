#include <WiFi.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WebServer.h>

const uint16_t kRecvPin = 23;
const uint16_t kIrLed = 19;
IRsend irsend(kIrLed);
WebServer server(80);

const char* ssid = "Gouveia";
const char* password = "982111481";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
   irsend.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Estabelecendo conexao com a rede wifi");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    display.print(".");
    display.display();
  }

  display.print("\n\n");
  display.display();

  server.begin();
  Serial.println("Servidor iniciado");

  display.setTextSize(2);
  display.print(WiFi.localIP().toString());
  display.display();

  setupServerRoutes();
}

void loop() {
  server.handleClient();
}

void setupServerRoutes() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/ligar", HTTP_GET, handleLigar);
  server.on("/desligar", HTTP_GET, handleDesligar);
  server.on("/18graus", HTTP_GET, handle18Graus);
  server.on("/19graus", HTTP_GET, handle19Graus);
  server.on("/20graus", HTTP_GET, handle20Graus);
}

void handleRoot() {
  sendHtml();
}

void handleLigar() {
  irsend.sendLG(0x880084C, 28);
  Serial.println("IR para ligar o ar");
  sendHtml();
}

void handleDesligar() {
  irsend.sendLG(0x88C0051, 28);
  sendHtml();
}

void handle18Graus() {
  irsend.sendLG(0x880834F, 28);
  sendHtml();
}

void handle19Graus() {
  irsend.sendLG(0x8808440, 28);
  sendHtml();
}

void handle20Graus() {
  irsend.sendLG(0x8808541, 28);
  sendHtml();
}

void sendHtml() {
  String html = "<html><body>";
  html += "<h1>Controle Remoto</h1>";
  html += "<a href=\"/ligar\"><button>Ligar</button></a><br>";
  html += "<a href=\"/desligar\"><button>Desligar</button></a><br>";
  html += "<a href=\"/18graus\"><button>18 Graus</button></a><br>";
  html += "<a href=\"/19graus\"><button>19 Graus</button></a><br>";
  html += "<a href=\"/20graus\"><button>20 Graus</button></a><br>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}
