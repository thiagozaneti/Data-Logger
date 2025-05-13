#include "globalvariables.h"
#include <ArduinoJson.h>  // v6.18.5 compatível
#include "jsonFunctions.h"

void lerArquivoJson() {
    File file = SPIFFS.open("/logs.json", FILE_READ);
    if (!file) {
        Serial.println("Erro ao abrir logs.json para leitura");
        return;
    }

    // Verifica se o arquivo está vazio
    if (file.size() == 0) {
        Serial.println("Arquivo logs.json está vazio");
        file.close();
        return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.print("Erro de deserialização: ");
        Serial.println(error.c_str());
        return;
    }

    Serial.println("Conteúdo do JSON:");
    serializeJsonPretty(doc, Serial);
}

void adicionaInformacaoArquivoJson(const String& timestamp, const String& message, const String& status) {
    JsonDocument doc;
    
    // Abre o arquivo existente
    File file = SPIFFS.open("/logs.json", FILE_READ);
    if (file) {
        // Verifica se o arquivo está vazio
        if (file.size() > 0) {
            DeserializationError error = deserializeJson(doc, file);
            if (error) {
                Serial.print("Erro ao ler JSON existente, criando novo: ");
                Serial.println(error.c_str());
                doc.clear();
                doc = JsonArray();
            }
        } else {
            // Se o arquivo estiver vazio, inicializa como um array vazio
            doc = JsonArray();
        }
        file.close();
    } else {
        // Se o arquivo não existir, inicializa como um array vazio
        doc = JsonArray();
    }

    // Garante que o doc seja um array
    if (!doc.is<JsonArray>()) {
        doc.clear();
        doc = JsonArray();
    }

    // Limita número de logs (opcional)
    const size_t MAX_LOGS_JSON = 50;
    if (doc.size() >= MAX_LOGS_JSON) {
        // Remove o item mais antigo
        for (size_t i = 0; i < doc.size() - 1; i++) {
            doc[i] = doc[i + 1];
        }
        doc.remove(doc.size() - 1);
    }

    // Adiciona nova entrada
    JsonObject logEntry = doc.createNestedObject();
    logEntry["timestamp"] = timestamp;
    logEntry["message"] = message;  // Corrigido para corresponder com a leitura nas páginas
    logEntry["status"] = status;

    // Salva o JSON de volta no arquivo
    file = SPIFFS.open("/logs.json", FILE_WRITE);
    if (!file) {
        Serial.println("Erro ao abrir logs.json para escrita");
        return;
    }

    serializeJson(doc, file);
    file.close();

    Serial.println("Log salvo com sucesso:");
    serializeJsonPretty(doc, Serial);
}