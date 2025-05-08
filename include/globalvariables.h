#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LoRa_E220.h>

// ————— DEFINIÇÕES DE PINOS —————
#define led_Status 17
#define led_Tx 16
#define led_Rx 4
#define pinRX_Lora 12
#define pinTX_Lora 14
#define pin_M0_Lora 25
#define pin_M1_Lora 26
#define pin_AUX_Lora 27
#define on_Lora 33

// ————— BUFFER DE RECEBIMENTO —————
#define MAX_BUFFER_SIZE 128
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern size_t dataLength;

// ————— LOG EM MEMÓRIA —————
const size_t MAX_LOGS = 100;
struct LogEntry {
  String timestamp, message;
};
extern LogEntry logs[MAX_LOGS];
extern size_t logCount;

// ————— CONTROLE DE TEMPO —————
extern time_t baseEpoch;
extern uint32_t baseMillis;

// ————— CREDENCIAIS DO AP —————
extern const char* AP_SSID;
extern const char* AP_PSK;

// ————— SERVIDOR WEB —————
extern AsyncWebServer server;

// ———— Variável de filtro de log ————
extern int valueFiltroLog;

// ————— DRIVER LoRa E220 —————
extern LoRa_E220 e220ttl;

#endif
