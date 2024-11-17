#include <DHT.h>

// Definições de pinos e parâmetros
#define BUTTON_PIN_P 18
#define LED_PIN_P 19
#define BUTTON_PIN_K 22
#define LED_PIN_K 21
#define LDR_PIN_PH 34
#define DHT_PIN 23
#define RELAY_PIN 4
#define DHT_MODEL DHT22

// Parâmetros ideais para cultura de "BANANA"
const float IDEAL_P = 82.00;       // Nível ideal de fósforo (P)
const float IDEAL_K = 50.00;       // Nível ideal de potássio (K)
const int IDEAL_PH = 6;            // Nível ideal de pH
const float IDEAL_HUMIDITY = 80.00; // Umidade ideal (%)

// Instância do sensor DHT
DHT dht(DHT_PIN, DHT_MODEL);

// Função para configurar pinos
void configurePins() {
  pinMode(BUTTON_PIN_P, INPUT_PULLUP);
  pinMode(LED_PIN_P, OUTPUT);
  pinMode(BUTTON_PIN_K, INPUT_PULLUP);
  pinMode(LED_PIN_K, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
}

// Função para leitura de botão
bool readButton(int buttonPin, int ledPin, const char* nutrient) {
  bool isNormal = digitalRead(buttonPin) == LOW;
  digitalWrite(ledPin, isNormal ? HIGH : LOW);

  Serial.print("Nível de ");
  Serial.print(nutrient);
  Serial.print(": ");
  Serial.println(isNormal ? "NORMAL" : "INSUFICIENTE");

  return isNormal;
}

// Função para leitura do sensor LDR (pH)
int readPH() {
  int rawValue = analogRead(LDR_PIN_PH);
  int scaledPH = map(rawValue, 0, 1023, 0, 14);
  Serial.print("Nível de pH (0-14): ");
  Serial.println(scaledPH);
  return scaledPH;
}

// Função para leitura do sensor DHT
bool readDHT(float& humidity, float& temperature) {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return false;
  }

  Serial.print("Umidade: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println("°C");

  return true;
}

// Função para controle de irrigação
void controlIrrigation(bool nivelP, bool nivelK, int ph, float humidity) {
  int irrigationTime;

  if (!nivelP || !nivelK || ph < IDEAL_PH || humidity < IDEAL_HUMIDITY) {
    irrigationTime = 3000; // Tempo mais longo de irrigação (3 segundos)
    Serial.println("Tempo de irrigação: 3 segundos");
  } else {
    irrigationTime = 2000; // Tempo moderado de irrigação (2 segundos)
    Serial.println("Tempo de irrigação: 2 segundos");
  }

  // Aciona a bomba
  digitalWrite(RELAY_PIN, HIGH);
  delay(irrigationTime);
  digitalWrite(RELAY_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  configurePins();
}

void loop() {
  Serial.println("");

  // Leitura dos sensores
  bool nivelP = readButton(BUTTON_PIN_P, LED_PIN_P, "Fósforo (P)");
  bool nivelK = readButton(BUTTON_PIN_K, LED_PIN_K, "Potássio (K)");
  int ph = readPH();

  float humidity, temperature;
  if (!readDHT(humidity, temperature)) {
    delay(2000); // Aguarda antes de tentar novamente
    return;
  }

  // Controle de irrigação
  controlIrrigation(nivelP, nivelK, ph, humidity);

  delay(1000); // Ajuste o intervalo de leitura conforme necessário
}
