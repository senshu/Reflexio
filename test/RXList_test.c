
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup(0, NULL);
    
    RXObject_t* list = RXObject_respondTo(RXList_o, RXSymbol_symbolForCString("spawn"), RXNil_o, 0);

    RXNativeMethod_push(RXInteger_spawn(RXInteger_o, 125));
    RXObject_respondTo(list, RXSymbol_symbolForCString("append"), RXNil_o, 1);
    RXNativeMethod_push(RXInteger_spawn(RXInteger_o, 250));
    RXObject_respondTo(list, RXSymbol_symbolForCString("append"), RXNil_o, 1);
    RXNativeMethod_push(RXInteger_spawn(RXInteger_o, 375));
    RXObject_respondTo(list, RXSymbol_symbolForCString("append"), RXNil_o, 1);
    RXNativeMethod_push(RXInteger_spawn(RXInteger_o, 500));
    RXObject_respondTo(list, RXSymbol_symbolForCString("append"), RXNil_o, 1);

    RXObject_respondTo(list, RXSymbol_symbolForCString("print"), RXNil_o, 0);

    Reflexio_clean();
}
