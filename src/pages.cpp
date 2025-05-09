#include <pages.h>
#include <functions.h>


void indexPage(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* req) {
    String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='5'><title>LoRa Logger</title>"
                  "<style>body{font-family:sans-serif;padding:10px;} table{width:100%;border-collapse:collapse;} th,td{border:1px solid #444;padding:5px;}</style>"
                  "</head><body>"
                  "<h2>Log de Mensagens LoRa</h2>"
                  "<p><a href=\"/settime\">Configurar Data/Hora</a></p>"
                  "<p><a href =\"/filtrar\">Filtrar Logs</p>";

    String now = nowISO();
    html += "<p>Hora atual: " + (now != "" ? now : "não configurada") + "</p>";

    html += "<table><tr><th>Horário</th><th>Mensagem (hex)</th><th>Status</th></tr>";
    for (size_t i = 0; i < logCount; i++) {
      html += "<tr><td>" + logs[i].timestamp + "</td><td>" + logs[i].message + "</td><td>" + logs[i].status + "</td></tr>";
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