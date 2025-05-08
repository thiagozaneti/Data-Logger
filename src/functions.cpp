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
  
//função que armazena dois timestamp e concatena em data - calcula a diferença de hora, minuto e seguno
int tempoEntre(const String& t1, const String& t2) {
    struct tm tm1, tm2;
    sscanf(t1.c_str(), "%d-%d-%d %d:%d:%d",
           &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday,
           &tm1.tm_hour, &tm1.tm_min, &tm1.tm_sec);
    sscanf(t2.c_str(), "%d-%d-%d %d:%d:%d",
           &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday,
           &tm2.tm_hour, &tm2.tm_min, &tm2.tm_sec);
    tm1.tm_year -= 1900; tm1.tm_mon -= 1;
    tm2.tm_year -= 1900; tm2.tm_mon -= 1;
    return difftime(mktime(&tm2), mktime(&tm1));
}

//função feita para calcular a posição correta do gps
void calcPosicaoGps(){
    if (byte5a8 && byte9a12)
    {
        
    }
    
}

//função feita para calcular qual o status do canhão a partir de suas informações
void calcSituacaoCanhao(){
    
}
  
  
// ————— ADICIONA UMA LINHA AO LOG —————
void addLog(const String& msg) {
    // Só processa pacotes que comecem com "41" - canhão
    if (msg.startsWith("41")) {
        Serial.println("Recebido buffer HEX:");

        //Remove espaços e possíveis quebras de linha
        String hexStr = msg;
        hexStr.trim();             // remove espaços nas bordas
        hexStr.replace(" ", "");   // remove todos os espaços internos

        //Converte a string HEX limpa em bytes
        uint8_t buffer[64];
        size_t byteCount = 0;
        for (size_t i = 0; i + 1 < hexStr.length() && byteCount < sizeof(buffer); i += 2) {
            String byteStr = hexStr.substring(i, i + 2);
            buffer[byteCount++] = (uint8_t) strtol(byteStr.c_str(), nullptr, 16);
        }

        //exibe os bytes 3 e 4
        Serial.print("Byte 3 (hex): 0x"); Serial.println(buffer[3], HEX);
        Serial.print("Byte 4 (hex): 0x"); Serial.println(buffer[4], HEX);

        //Combina byte 3 e 4 (byte 4 é MSB)
        byte3e4 = (uint16_t(buffer[4]) << 8) | buffer[3];

        //Outros campos (ex.: bytes 5–8 e 9–12)
        byte5a8  = (uint32_t(buffer[5])  << 24) | (uint32_t(buffer[6])  << 16)
                          | (uint32_t(buffer[7])  << 8)  | buffer[8];
        byte9a12 = (uint32_t(buffer[9])  << 24) | (uint32_t(buffer[10]) << 16)
                          | (uint32_t(buffer[11]) << 8)  | buffer[12];

        //Imprime resultados
        Serial.println("----------- Decodificação -----------");
        Serial.print("Byte 3 e 4 combinados (decimal): ");
        Serial.println(byte3e4);
        Serial.print("Byte 3 e 4 combinados (hex): 0x");
        Serial.println(byte3e4, HEX);

        Serial.print("Byte 5 a 8 (decimal): ");
        Serial.println(byte5a8);

        Serial.print("Byte 9 a 12 (decimal): ");
        Serial.println(byte9a12);
        Serial.println("-------------------------------------");
    }
    String primeirosCaracteres = msg.substring(0, 2);
    int primeiroByte = primeirosCaracteres.toInt();
    if (valueFiltroLog != -1 && valueFiltroLog != primeiroByte) return;

    if (logCount < MAX_LOGS) {
        logs[logCount++] = { nowISO(), msg };
    } else {
        for (size_t i = 1; i < MAX_LOGS; i++) logs[i - 1] = logs[i];
        logs[MAX_LOGS - 1] = { nowISO(), msg };
    }

}