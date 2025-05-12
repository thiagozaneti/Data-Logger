#include "globalvariables.h"


void lerArquivoJson(){
    File file = SPIFFS.open("/docs/logs.json", FILE_WRITE);
    if (!file) {
    Serial.println("Erro ao abrir dados.json para escrita");
    return;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);

  if(error){
    Serial.println("error de deserialização");
    return;
  }

}

void adicionaInformacaoArquivoJson( const LogEntry& addLogInVetor ){
    JsonDocument doc;
    doc["timestamp"] = addLogInVetor.timestamp;
    doc["messge"] = addLogInVetor.message;
    doc["status"] = addLogInVetor.status;

    File file = SPIFFS.open("/docs/logs.json", FILE_WRITE);
    if (!file) {
    Serial.println("Erro ao abrir dados.json para escrita");
    return;
    }
    serializeJson(doc, file);
    file.close();
    Serial.println("Salvo com sucesso");
    // Serial.print(doc["timestamp"]);
}
//timestamp, message, status;