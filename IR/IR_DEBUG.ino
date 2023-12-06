#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// Defina o pino que o receptor IR está conectado
const uint16_t kRecvPin = 23;

// Define o tamanho do buffer de captura. Ajuste conforme necessário.
const uint16_t kCaptureBufferSize = 1024;

// Cria uma instância do receptor IR
IRrecv irrecv(kRecvPin, kCaptureBufferSize, 15, true);

// Estrutura para armazenar os resultados da decodificação
decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Inicia o receptor
  Serial.println("Pronto para receber sinais IR...");
}

void loop() {
  if (irrecv.decode(&results)) {
    // Exibe o código IR recebido
    Serial.print("Código IR Recebido: 0x");
    Serial.println(results.value, HEX);

    // Exibe o número de bits do código IR
    Serial.print("Bits: ");
    Serial.println(results.bits);

    // Exibe o tipo de protocolo
    if (results.decode_type == UNKNOWN) {
      Serial.println("Protocolo desconhecido");
    } else {
      Serial.print("Protocolo: ");
      Serial.println(typeToString(results.decode_type));
    }

    irrecv.resume();  // Prepara para receber o próximo valor
  }
}
