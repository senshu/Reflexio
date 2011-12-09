
#include "RXCore.h"
#include <Eina.h>

// Private -------------------------------------------------------------

/*
 * A hash table of all created symbols.
 */
static Eina_Hash* RXSymbol_all;

RXObject_defineType(Symbol,
    char chars[0]; // Correct number of characters allocated when creating a new symbol
);

// Public --------------------------------------------------------------

void RXSymbol_setup(void) {
    // TODO add callback
    RXSymbol_all = eina_hash_string_djb2_new(NULL);
}

void RXSymbol_clean(void) {
    eina_hash_free(RXSymbol_all);
}

RXObject_t* RXSymbol_new(const char* str) {
    RXObject_t* self = RXObject_allocateSize(strlen(str) + 1);
    RXObject_initialize(self);
    strcpy((char*)self, str);
    eina_hash_direct_add(RXSymbol_all, self, self);
    return self;
}

RXObject_t* RXSymbol_symbolForCString(const char* str) {
    RXObject_t* symbol = eina_hash_find(RXSymbol_all, str);
    if(symbol == NULL) {
        symbol = RXSymbol_new(str);
        RXObject_setSlot(symbol, RXSymbol_delegate_o, RXSymbol_o);
    }
    return symbol;
}
