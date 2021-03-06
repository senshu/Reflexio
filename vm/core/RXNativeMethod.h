
#ifndef __RX_CORE_NATIVE_METHOD_H__
#define __RX_CORE_NATIVE METHOD_H__

#include "RXObject.h"

/*
 * Function type for native method bodies.
 */
typedef RXObject_t* (*RXNativeMethodBody_t) (RXObject_t* self, RXObject_t* context, int argumentCount);

/*
 * The standard function name for methods attached to predefined object types.
 */
#define RXNativeMethod_functionName(type, name) \
    type##_respondTo##_##name

/*
 * The standard C function prototype for methods attached to predefined object types.
 *
 * Example:
 *
 * RXNativeMethod_define(MyType, someMessage) {
 *     // Method body
 * }
 *
 * The static keyword can be added before RXNativeMethod_define.
 */
#define RXNativeMethod_define(type, name) \
    static RXObject_t* RXNativeMethod_functionName(type, name) (RXObject_t* self, RXObject_t* context, int argumentCount)

/*
 * Attach a method to an object representing the given type.
 *
 * If type is MyType, we assume that there exists a variable MyType_o of type MyType_t*
 *
 * Example:
 *
 * MyType_t* MyType_o = MyType_new();
 * RXNativeMethod_attach(MyType, someMessage)
 */
#define RXNativeMethod_attach(type, name) ({ \
    RXObject_t* method = RXNativeMethod_spawn(RXNativeMethod_o, RXNativeMethod_functionName(type, name)); \
    RXObject_setSlot(type##_o, RXSymbol_symbolForCString(#name), method, false); \
    method; \
})

void RXNativeMethod_setup(void);

void RXNativeMethod_clean(void);

/*
 * Create a new native method with the given body.
 *
 * Use macro RXNativeMethod_attach to create a method and attach it to an object.
 */
RXObject_t* RXNativeMethod_spawn(RXObject_t* self, RXNativeMethodBody_t body);

#endif
