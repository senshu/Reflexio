
#include "RXCore.h"

RXObject_t* RXNil_o;
RXObject_t* RXProtoObject_o;
RXObject_t* RXObject_o;
RXObject_t* RXLobby_o;
RXObject_t* RXSymbol_Lobby_o;
RXObject_t* RXSymbol_Object_o;
RXObject_t* RXSymbol_ProtoObject_o;
RXObject_t* RXSymbol_nil_o;
RXObject_t* RXSymbol_NativeMethod_o;

RXObject_t* RXSymbol_o;
RXObject_t* RXSymbol_activate_o;
RXObject_t* RXSymbol_lookup_o;
RXObject_t* RXSymbol_forward_o;

RXObject_t* RXNativeMethod_o;

RXNativeMethod_define(RXNativeMethod, default) {
    return self;
}

void RXCore_setup(void) {
    RXObject_setup();
    RXSymbol_setup();
    RXNativeMethod_setup();

    // Creating core objects
    RXNil_o = RXObject_new();
    RXObject_setDelegate(RXNil_o, RXNil_o);
    
    RXProtoObject_o = RXObject_spawn(RXNil_o);
    RXLobby_o = RXObject_spawn(RXProtoObject_o);
    RXObject_o = RXObject_spawn(RXLobby_o);

    RXSymbol_o = RXSymbol_new("Symbol");
    RXObject_setDelegate(RXSymbol_o, RXObject_o);
    
    RXNativeMethod_o = RXNativeMethod_spawn(RXObject_o, RXNativeMethod_functionName(RXNativeMethod, default));
    
    // Registering core objects in the lobby
    RXSymbol_nil_o = RXSymbol_symbolForCString("nil");
    RXSymbol_ProtoObject_o = RXSymbol_symbolForCString("ProtoObject");
    RXSymbol_Lobby_o = RXSymbol_symbolForCString("Lobby");
    RXSymbol_Object_o = RXSymbol_symbolForCString("Object");
    RXSymbol_NativeMethod_o = RXSymbol_symbolForCString("NativeMethod");

    RXObject_setSlot(RXLobby_o, RXSymbol_nil_o, RXNil_o, false);
    RXObject_setSlot(RXLobby_o, RXSymbol_ProtoObject_o, RXProtoObject_o, false);
    RXObject_setSlot(RXLobby_o, RXSymbol_Lobby_o, RXLobby_o, false);
    RXObject_setSlot(RXLobby_o, RXSymbol_Object_o, RXObject_o, false);
    RXObject_setSlot(RXLobby_o, RXSymbol_o, RXSymbol_o, false);
    RXObject_setSlot(RXLobby_o, RXSymbol_NativeMethod_o, RXNativeMethod_o, false);
    
    // Creating symbols for core methods
    RXSymbol_activate_o = RXSymbol_symbolForCString("activate");
    RXSymbol_lookup_o = RXSymbol_symbolForCString("lookup");
    RXSymbol_forward_o = RXSymbol_symbolForCString("forward");
}

void RXCore_clean(void) {
    RXNativeMethod_clean();
    RXSymbol_clean();
    RXObject_clean();
}
