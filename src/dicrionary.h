#pragma once

typedef struct {
    unsigned long vkCode;
    unsigned char control;
    unsigned char alt;
    unsigned char shift;
} dictionaryEntry;

void buildDictionary();
char getKeyFromDictionary(unsigned long vkCode, unsigned char control, unsigned char alt, unsigned char shift);