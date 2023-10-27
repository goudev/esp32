const int analogPin = 4; // Substitua com o número do pino GPIO que você está usando
const float seriesResistor = 10000;  // resistor de série em ohms
const float thermistorNominal = 10000;  // resistência do termistor a 25 graus C
const float temperatureNominal = 25; // temperatura nominal
const float bCoef = 3950;  // coeficiente B para o termistor
const float calibrationFactor = 3.6;  // fator de correção de calibração

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(analogPin);
  float resistance = seriesResistor / ((4096.0 / (4096.0 - sensorValue)) - 1);
  float steinhart = resistance / thermistorNominal;     
  steinhart = log(steinhart);                  
  steinhart /= bCoef;                   
  steinhart += 1.0 / (temperatureNominal + 273.15); 
  steinhart = 1.0 / steinhart;                 
  steinhart -= 273.15;                       
  float correctedTemp = steinhart + calibrationFactor;  // aplica o fator de correção
  
  Serial.print("Leitura do sensor: ");
  Serial.print(sensorValue);
  Serial.print("\tTemperatura estimada: ");
  Serial.print(correctedTemp);
  Serial.println(" *C");
  
  delay(5000);
}
