//feito por: Thiago Zaneti
#include "globalvariables.h"
#include "functions.h"
#include "pages.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  //servir páginas estáticas futuras)
  SPIFFS.begin(true);
  Serial.println("Spiffs ativo...");

  // configura LEDs
  pinMode(led_Status, OUTPUT);
  pinMode(led_Tx, OUTPUT);
  pinMode(led_Rx, OUTPUT);

  pinMode(in_wifi, INPUT);

  // liga fisicamente o rádio
  pinMode(on_Lora, OUTPUT);
  digitalWrite(on_Lora, HIGH);
  delay(100);

  // coloca E220 em modo normal
  pinMode(pin_M0_Lora, OUTPUT);
  pinMode(pin_M1_Lora, OUTPUT);
  digitalWrite(pin_M0_Lora, LOW);
  digitalWrite(pin_M1_Lora, LOW);

  // inicializa driver (Serial1 dentro de e220ttl.begin)
  e220ttl.begin();

  // configura AP Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PSK);
  IPAddress ip = WiFi.softAPIP();
  Serial.println("AP ativo em: http://" + ip.toString());

  indexPage();
  filtrarPage();
  settimePage();

  server.begin();
  Serial.println("Logger ativo");
}

// ————— LOOP PRINCIPAL —————
void loop() {
  // pisca LED de status
  static uint32_t lastBlink = 0;
  if (millis() - lastBlink > 500) {
    digitalWrite(led_Status, !digitalRead(led_Status));
    lastBlink = millis();
  }

  // lê payload bruto quando AUX=HIGH
  if (digitalRead(pin_AUX_Lora) == HIGH) {
    dataLength = Serial1.readBytes(buffer, MAX_BUFFER_SIZE);
    if (dataLength) {
      digitalWrite(led_Rx, HIGH);
      delay(30);
      digitalWrite(led_Rx, LOW);

      // converte em hex string
      String hex;
      for (size_t i = 0; i < dataLength; i++) {
        if (buffer[i] < 0x10) hex += "0";
        hex += String(buffer[i], HEX) + " ";
      }
      Serial.println("Recebido: " + hex);
      addLog(hex);
    }
  }
}
