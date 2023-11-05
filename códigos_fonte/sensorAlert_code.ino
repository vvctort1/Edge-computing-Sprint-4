/////////--------IOT--------FIAP------------///////////
#include <LiquidCrystal_I2C.h> 
#include <WiFi.h>
#include <PubSubClient.h>


 
// Configurações de WiFi
const char *SSID = "";
const char *PASSWORD = "";  // Substitua pelo sua senha
 
// Configurações de MQTT
const char *BROKER_MQTT = "46.17.108.113";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "fiware_sensorAlert9";
const char *TOPIC_PUBLISH_GAS = "/TEF/sensoralert9/attrs/g";
 
 
// Configurações de Hardware
 
#define PUBLISH_DELAY 2000
 
// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
unsigned long publishUpdate = 0;
const int TAMANHO = 200;
int sensorAlert = 34; //SENSOR ALERT PINO 34
float valorSensorGas = 0;
int buzzerPin = 35;
const int pinoLEDR = 13;
const int pinoLEDG = 12;
const int pinoLEDB = 14;

LiquidCrystal_I2C lcd(0x27, 20, 4);
 
// Protótipos de funções
void initWiFi();
void initMQTT();
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();


 
void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}
 
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}
 
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT: ");
    Serial.println(BROKER_MQTT);
 
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
     } else {
      Serial.println("Falha na conexão com MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}
 
void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}
 
void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED)
    return;
 
  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.print("Wifi conectado com sucesso");
  Serial.print(SSID);
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}
 
void setup() {
  Serial.begin(115200);
  pinMode(sensorAlert, INPUT);
  pinMode(buzzerPin , OUTPUT);
  pinMode(pinoLEDR,OUTPUT);
  pinMode(pinoLEDG,OUTPUT);
  pinMode(pinoLEDB,OUTPUT);
  lcd.init();
  lcd.backlight();
  initWiFi();
  initMQTT();

}
 
void loop() {
  valorSensorGas = analogRead(sensorAlert); 
  checkWiFIAndMQTT();
  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    //sensor mq2
    char msgBuffer2[7];
    Serial.print("Gás: ");
    Serial.println(valorSensorGas);
    dtostrf (valorSensorGas, 4, 2, msgBuffer2);
    MQTT.publish(TOPIC_PUBLISH_GAS,msgBuffer2);
    MQTT.loop();
 
 
    }

  if (valorSensorGas < 750){
    analogWrite(pinoLEDG, 0);
    analogWrite(pinoLEDR, 0);
    analogWrite(pinoLEDB, 255);
    noTone(buzzerPin);
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Baixo nivel");
    lcd.setCursor(3,1);
    lcd.print("de Poluicao :)");
    lcd.setCursor(1, 2);
    lcd.print("Quantidade de Gas:");
    lcd.setCursor(7,3);
    lcd.print(valorSensorGas);
    delay(1500);
  } else if (valorSensorGas >= 750 && valorSensorGas < 900) {
    analogWrite(pinoLEDR, 0);
    analogWrite(pinoLEDG, 255);
    analogWrite(pinoLEDB, 0);
    noTone(buzzerPin);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Atencao! Quantidade");
    lcd.setCursor(2, 1);
    lcd.print("elevada de gases!");
    lcd.setCursor(1, 2);
    lcd.print("Quantidade de Gas:");
    lcd.setCursor(7, 3);
    lcd.print(valorSensorGas);
    delay(1500);
  } else if (valorSensorGas >= 900) {
    analogWrite(pinoLEDR, 255);
    analogWrite(pinoLEDG, 0);
    analogWrite(pinoLEDB, 0);
    tone(buzzerPin, 1500);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("ALERTA VERMELHO!");
    lcd.setCursor(0, 1);
    lcd.print("Levar para conserto!");
    lcd.setCursor(1, 2);
    lcd.print("Quantidade de Gas:");
    lcd.setCursor(7, 3);
    lcd.print(valorSensorGas);
    delay(1500);
  }
  }