#include <pages.h>
#include <functions.h>
#include <globalvariables.h>


// Função modificada indexPage
// Função modificada indexPage com o link para limpar logs
void indexPage() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* req) {
    String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='5'><title>LoRa Logger</title>"
                  "<style>body{font-family:sans-serif;padding:10px;} "
                  "table{width:100%;border-collapse:collapse;} "
                  "th,td{border:1px solid #444;padding:5px;}"
                  ".menu{display:flex;gap:10px;margin-bottom:15px;flex-wrap:wrap;}"
                  ".menu a{text-decoration:none;padding:5px 10px;background:#f0f0f0;border-radius:3px;color:#333;}"
                  ".menu a:hover{background:#e0e0e0;}"
                  ".danger{background:#ffdddd !important;color:#cc0000 !important;}"
                  ".danger:hover{background:#ffcccc !important;}"
                  "</style>"
                  "</head><body>"
                  "<h2>Log de Mensagens LoRa</h2>"
                  "<div class='menu'>"
                  "<a href=\"/settime\">Configurar Data/Hora</a>"
                  "<a href=\"/filtrar\">Filtrar Logs</a>"
                  "<a href=\"/clear\" class='danger'>Limpar Logs</a>"
                  "</div>";

    String now = nowISO();
    html += "<p>Hora atual: " + (now != "" ? now : "não configurada") + "</p>";

    html += "<table><tr><th>Horário</th><th>Mensagem (hex)</th><th>Status</th></tr>";

    File file = SPIFFS.open("/logs.json", FILE_READ);
    if (!file) {
      html += "<tr><td colspan='3'>Erro ao abrir logs.json</td></tr>";
    } else {
      // Verifica se o arquivo está vazio
      if (file.size() == 0) {
        html += "<tr><td colspan='3'>Arquivo logs.json está vazio</td></tr>";
        file.close();
      } else {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, file);
        file.close();

        if (error) {
          html += "<tr><td colspan='3'>Erro ao ler JSON: ";
          html += error.c_str();
          html += "</td></tr>";
        } else {
          JsonArray arr = doc.as<JsonArray>();

          // Verifica se o parâmetro 'log' foi enviado
          String filtro = "";
          if (req->hasParam("log")) {
            filtro = req->getParam("log")->value();
          }

          // Itera sobre o array em ordem reversa (mais recente primeiro)
          for (int i = arr.size() - 1; i >= 0; i--) {
            JsonObject obj = arr[i];
            String horario = obj["timestamp"] | "N/A";
            String mensagem = obj["message"] | "N/A";  
            String status = obj["status"] | "N/A";

            // Aplica o filtro se houver
            if (filtro == "" || mensagem.indexOf(filtro) != -1) {
              html += "<tr><td>" + horario + "</td><td>" + mensagem + "</td><td>" + status + "</td></tr>";
            }
          }
        }
      }
    }

    html += "</table></body></html>";

    req->send(200, "text/html", html);
  });
}



void filtrarPage(){
    server.on("/filtrar", HTTP_GET, [](AsyncWebServerRequest* req) {
        String html = "<!DOCTYPE html><html><head><meta charset='utf-8'><title>LoRa Logger</title>"
                      "<style>body{font-family:sans-serif;padding:10px;} table{width:100%;border-collapse:collapse;} th,td{border:1px solid #444;padding:5px;}</style>"
                      "</head><body>"
                      "<h2>Filtrar por logs</h2>"
                      "<form action=\"/\" method=\"get\">"
                      "<label>Filtrar Logs:</label><input type=\"number\" name=\"log\"><br>"
                      "<input type=\"submit\" value=\"Filtrar\">"
                      "</form>";
        html += "</table></body></html>";
    
    
        req->send(200, "text/html", html);
      });
}

void settimePage(){
    server.on("/settime", HTTP_GET, [](AsyncWebServerRequest* req) {
        // Se vier com parâmetros, processa e atualiza baseEpoch/baseMillis
        if (req->hasParam("year") && req->hasParam("month")
            && req->hasParam("day") && req->hasParam("hour")
            && req->hasParam("minute") && req->hasParam("second")) {
    
          int Y = req->getParam("year")->value().toInt();
          int M = req->getParam("month")->value().toInt();
          int D = req->getParam("day")->value().toInt();
          int h = req->getParam("hour")->value().toInt();
          int m = req->getParam("minute")->value().toInt();
          int s = req->getParam("second")->value().toInt();
    
          tm tm0;
          tm0.tm_year = Y - 1900;
          tm0.tm_mon = M - 1;
          tm0.tm_mday = D;
          tm0.tm_hour = h;
          tm0.tm_min = m;
          tm0.tm_sec = s;
          tm0.tm_isdst = 0;
          baseEpoch = mktime(&tm0);
          baseMillis = millis();
    
          // Confirmação
          req->send(200, "text/html",
                    "<!DOCTYPE html><html><body>"
                    "<p>Data/Hora ajustada para: "
                      + nowISO() + "</p>"
                                   "<p><a href=\"/\">Voltar ao log</a></p>"
                                   "</body></html>");
        } else {
          // Exibe o formulário
          String html =
            "<!DOCTYPE html><html><head><meta charset='utf-8'><title>Configurar Data/Hora</title>"
            "<style>body{font-family:sans-serif;padding:10px;} label{display:inline-block;width:80px;} input{width:60px;margin:2px;}</style>"
            "</head><body>"
            "<h2>Configurar Data e Hora</h2>"
            "<form action=\"/settime\" method=\"get\">"
            "<label>Ano:</label><input type=\"number\" name=\"year\" value=\"2025\" min=\"2000\" max=\"2099\"><br>"
            "<label>Mês:</label><input type=\"number\" name=\"month\" value=\"01\" min=\"1\" max=\"12\"><br>"
            "<label>Dia:</label><input type=\"number\" name=\"day\" value=\"01\" min=\"1\" max=\"31\"><br>"
            "<label>Hora:</label><input type=\"number\" name=\"hour\" value=\"00\" min=\"0\" max=\"23\"><br>"
            "<label>Minuto:</label><input type=\"number\" name=\"minute\" value=\"00\" min=\"0\" max=\"59\"><br>"
            "<label>Segundo:</label><input type=\"number\" name=\"second\" value=\"00\" min=\"0\" max=\"59\"><br>"
            "<input type=\"submit\" value=\"Ajustar\">"
            "</form>"
            "<p><a href=\"/\">Cancelar</a></p>"
            "</body></html>";
          req->send(200, "text/html", html);
        }
      });
}

// Função para limpar todos os logs em SPIFFS

// Implementação no arquivo functions.cpp
void clearAllLogs() {
  // Abre o arquivo e reescreve com um array vazio
  File file = SPIFFS.open("/logs.json", FILE_WRITE);
  if (file) {
    file.print("[]");
    file.close();
    Serial.println("Arquivo de logs foi limpo com sucesso.");
  } else {
    Serial.println("Erro ao abrir o arquivo para limpá-lo!");
  }
}

// Adicionar à página web em pages.cpp a função que cria o botão para limpar logs
void clearLogsPage() {
  server.on("/clear_logs", HTTP_GET, [](AsyncWebServerRequest* req) {
    clearAllLogs();
    req->redirect("/");
  });

  // Adicionar ao menu de navegação
  server.on("/clear", HTTP_GET, [](AsyncWebServerRequest* req) {
    String html = "<!DOCTYPE html><html><head><title>Limpar Logs</title>"
                  "<style>body{font-family:sans-serif;padding:20px;text-align:center;} "
                  ".button{background-color:#f44336;color:white;padding:15px 32px;text-align:center;"
                  "text-decoration:none;display:inline-block;font-size:16px;margin:10px;cursor:pointer;border:none;border-radius:4px;}"
                  ".button:hover{background-color:#d32f2f;}"
                  "</style>"
                  "</head><body>"
                  "<h2>Limpar todos os logs</h2>"
                  "<p>Esta ação irá remover todos os registros permanentemente.</p>"
                  "<p><a href='/clear_logs' class='button'>Limpar Todos os Logs</a></p>"
                  "<p><a href='/'>Voltar</a></p>"
                  "</body></html>";
    req->send(200, "text/html", html);
  });
}