#include "globalvariables.h"

uint8_t buffer[MAX_BUFFER_SIZE];
size_t dataLength = 0;

LogEntry logs[MAX_LOGS];
size_t logCount = 0;

time_t baseEpoch = 0;
uint32_t baseMillis = 0;

const char* AP_SSID = "DataLog - RADCOM";
const char* AP_PSK = "radcom22";

AsyncWebServer server(80);

int valueFiltroLog = -1;

LoRa_E220 e220ttl(
  pinRX_Lora, pinTX_Lora, &Serial1,
  pin_AUX_Lora, pin_M0_Lora, pin_M1_Lora,
  UART_BPS_RATE_9600);
