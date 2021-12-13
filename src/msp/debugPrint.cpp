#include "Energia.h"
#include "debugPrint.h"

DebugPrint::DebugPrint(/* args */)
{
}

void DebugPrint::print(String text) {
    #ifdef DEBUG
        Serial.print(text);
    #endif
}

void DebugPrint::println(String text) {
    #ifdef DEBUG
        Serial.println(text);
    #endif
}
