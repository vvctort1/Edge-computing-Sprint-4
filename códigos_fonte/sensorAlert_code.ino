/////////--------IOT--------FIAP------------///////////
 
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
  }