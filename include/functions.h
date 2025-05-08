#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <globalvariables.h>

String nowISO();
void addLog(const String& msg);
int tempoEntre(const String& t1, const String& t2);
void processarPacote(const uint8_t* buffer, size_t length);

#endif