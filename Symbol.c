#include "Symbol.h"

Symbol* Symbol_create(char* symbol, long address)
{
    if (symbol == NULL)return NULL;
    
    Symbol* s = malloc(sizeof(Symbol));
    if (s == NULL)return NULL;
    int len = strlen(symbol);
    s->symbol = malloc(len + 1);
    if (s->symbol == NULL)
    {
        free(s);
        return NULL;
    }
    strncpy(s->symbol, symbol, len);
    s->symbol[len] = '\x0';
    s->address = address;
    return s;
}
