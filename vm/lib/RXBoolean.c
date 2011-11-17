
#include "RXLib.h"

RXObject_t* RXBoolean_o;
RXObject_t* RXBoolean_true_o;
RXObject_t* RXBoolean_false_o;
RXObject_t* RXSymbol_Boolean_o;
RXObject_t* RXSymbol_true_o;
RXObject_t* RXSymbol_false_o;

void RXBoolean_setup(void) {
    RXSymbol_Boolean_o = RXSymbol_symbolForCString("Boolean");
    RXSymbol_true_o = RXSymbol_symbolForCString("true");
    RXSymbol_false_o = RXSymbol_symbolForCString("false");
    
    RXBoolean_o = RXObject_spawn(RXObject_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_Boolean_o, RXBoolean_o);

    RXBoolean_true_o = RXObject_spawn(RXBoolean_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_true_o, RXBoolean_true_o);

    RXBoolean_false_o = RXObject_spawn(RXBoolean_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_false_o, RXBoolean_false_o);
}
