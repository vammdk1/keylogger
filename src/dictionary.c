#include "dicrionary.h"
#include <windows.h>

static dictionaryEntry dictionary[256] = {0};

void buildDictionary() {
    for (unsigned char i = 0x21; i < 256; ++i) {
        if (i == 0x7F) continue; //Skip delete key
        short kd = VkKeyScan(i);
        char keyCode = kd & 0xFF;
        if (keyCode < 0) continue;
        unsigned char shift = 0, control = 0, alt = 0;
        if (kd & 0x100) shift = 1;
        if (kd & 0x200) control = 1;
        if (kd & 0x400) alt = 1;
        dictionary[i].alt = alt;
        dictionary[i].control = control;
        dictionary[i].shift = shift;
        dictionary[i].vkCode = keyCode;
    }
}

char getKeyFromDictionary(unsigned long vkCode, unsigned char control, unsigned char alt, unsigned char shift) {
    for (unsigned char i = 0x21; i < 256; ++i) {
        if (dictionary[i].vkCode == vkCode && dictionary[i].control == control && dictionary[i].alt == alt && dictionary[i].shift == shift)
            return (char) i;
    }
    return 0;
}