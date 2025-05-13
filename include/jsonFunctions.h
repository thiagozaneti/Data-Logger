#ifndef JSONFUNCTIONS_H
#define JSONFUNCTIONS_H
#include <globalvariables.h>

void lerArquivoJson();
void adicionaInformacaoArquivoJson(const String& timestamp, const String& message, const String& status);
#endif