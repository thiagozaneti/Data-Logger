#include "globalvariables.h"
#include <functions.h>

String nowISO() {
    // se ainda não foi injetado, retorna vazio
    if (baseEpoch == 0) return "";
    uint32_t delta = (millis() - baseMillis) / 1000;
    time_t t = baseEpoch + delta;
    tm tmInfo;
    gmtime_r(&t, &tmInfo);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmInfo);
    return String(buf);
  }
  
  // ————— ADICIONA UMA LINHA AO LOG —————
void addLog(const String& msg) {
    String primeirosCaracteres = msg.substring(0,2);
    int primeiroByte  = primeirosCaracteres.toInt();
    Serial.println("Primeiro caracter da mensagem:");
    Serial.print(primeirosCaracteres);
    Serial.println("");
  
    Serial.println("Primeiro caracter da mensagem convertido:");
    Serial.print(primeiroByte);
    Serial.println("");
  
    if (valueFiltroLog != -1) {
      if (valueFiltroLog != primeiroByte) {
        return;  // Ignora a mensagem, não faz parte do filtro
      }
    }
  
    // Adiciona o log (independente de filtro se chegou aqui)
    if (logCount < MAX_LOGS) {
      logs[logCount++] = { nowISO(), msg };
    } else {
      for (size_t i = 1; i < MAX_LOGS; i++) logs[i - 1] = logs[i];
      logs[MAX_LOGS - 1] = { nowISO(), msg };
    }
}