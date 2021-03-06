
#include "RXLib.h"

// Private -------------------------------------------------------------

static RXObject_t* RXClosure_spawn(RXObject_t* isMethod, RXObject_t* context, int argumentCount) {
    // Fill parameter list
    // TODO replace List with Array
    RXObject_t* parameterList = RXList_spawn(RXList_o, NULL);
    for (int i = 0; i < argumentCount - 1; i ++) {
        RXObject_t* expression = RXNativeMethod_argumentAt(i);
        RXObject_t* message = RXList_at(expression, 0);
        RXObject_t* name = RXObject_valueOfSlot(message, RXSymbol_name_o);
        RXList_append(parameterList, name);
    }
    
    RXObject_t* body = RXNativeMethod_argumentAt(argumentCount - 1);
    
    RXObject_t* result = RXObject_spawn(RXClosure_o);
    RXObject_setSlot(result, RXSymbol_context_o, context, false);
    RXObject_setSlot(result, RXSymbol_body_o, body, false);
    RXObject_setSlot(result, RXSymbol_parameters_o, parameterList, false);
    RXObject_setSlot(result, RXSymbol_isMethod_o, isMethod, false);
    
    return result;
}

// Methods -------------------------------------------------------------

/*
 * Create a new method.
 *
 * Arguments:
 *  - parameter names...
 *  - an expression
 *
 * Returns:
 *  - a new closure with slot isMethod set to true
 */
RXNativeMethod_define(RXProtoObject, method) {
    return RXClosure_spawn(RXBoolean_true_o, context, argumentCount);    
}

/*
 * Create a new method.
 *
 * Arguments:
 *  - parameter names...
 *  - an expression
 *
 * Returns:
 *  - a new closure with slot isMethod set to false
 */
RXNativeMethod_define(RXProtoObject, block) {
    return RXClosure_spawn(RXBoolean_false_o, context, argumentCount);    
}

/*
 * Activate a closure.
 *
 * Arguments:
 *  - the object known as self inside the closure body, ignored if slot
 *    isMethod is not true
 *  - a variable number of arguments
 *
 * Returns:
 *  - the result of evaluating the closure
 */
RXNativeMethod_define(RXClosure, activate) {
    // The locals object will hold arguments values and local slots
    // assigned inside the closure body.
    RXObject_t* localContext = RXObject_spawn(RXObject_valueOfSlot(self, RXSymbol_context_o));

    // We keep a reference to the calling context for arguments of the closure execution.
    RXObject_setSlot(localContext, RXSymbol_context_o, context, false);
    
    // The locals object has a named reference to itself, if there is
    // a need to pass it as an argument in a message.
    RXObject_setSlot(localContext, RXSymbol_locals_o, localContext, false);

    // If the closure defines a method, assign slot "self" with the first argument.
    // Otherwise, the first argument is ignored.
    if (RXObject_valueOfSlot(self, RXSymbol_isMethod_o) == RXBoolean_true_o && argumentCount > 0) {
        RXObject_setSlot(localContext, RXSymbol_self_o, RXExpression_valueOfArgumentAt(0, context), false);
    }

    // Get the list of parameters of the current closure.
    // FIXME It is assumed to be an RXList containing RXSymbols.
    RXObject_t* parameterList = RXObject_valueOfSlot(self, RXSymbol_parameters_o);
    int parameterCount = RXList_count(parameterList);

    // Process the arguments of the closure execution
    // TODO replace List with Array
    RXObject_t* argumentList = RXList_spawn(RXList_o, NULL);
    RXObject_setSlot(localContext, RXSymbol_arguments_o, argumentList, false);
    for(int i = 1; i < argumentCount; i ++) {
        // Raw argument expressions are put into an argument list
        RXList_append(argumentList, RXNativeMethod_argumentAt(i));
        if (i <= parameterCount) {
            RXObject_t* parameter = RXList_at(parameterList, i - 1);
            // Parameters whose name start with '$' are not evaluated
            if (*(char*)parameter == '$') {
                RXObject_setSlot(localContext, parameter, RXNativeMethod_argumentAt(i), false);
            }
            else {
                RXObject_setSlot(localContext, parameter, RXExpression_valueOfArgumentAt(i, context), false);
            }
        }
    }
    
    // Evaluate the closure body in the context of the locals object.
    RXNativeMethod_push(localContext);
    return RXObject_respondTo(RXObject_valueOfSlot(self, RXSymbol_body_o), RXSymbol_valueInContext_o, RXNil_o, 1);
}

// Public --------------------------------------------------------------

RXObject_t* RXClosure_o;
RXObject_t* RXSymbol_Closure_o;
RXObject_t* RXSymbol_self_o;
RXObject_t* RXSymbol_locals_o;
RXObject_t* RXSymbol_context_o;
RXObject_t* RXSymbol_arguments_o;
RXObject_t* RXSymbol_parameters_o;
RXObject_t* RXSymbol_body_o;
RXObject_t* RXSymbol_isMethod_o;
RXObject_t* RXSymbol_method_o;
RXObject_t* RXSymbol_block_o;

void RXClosure_setup(void) {
    RXSymbol_self_o = RXSymbol_symbolForCString("self");
    RXSymbol_locals_o = RXSymbol_symbolForCString("locals");
    RXSymbol_context_o = RXSymbol_symbolForCString("context");
    RXSymbol_arguments_o = RXSymbol_symbolForCString("arguments");
    RXSymbol_parameters_o = RXSymbol_symbolForCString("parameters");
    RXSymbol_body_o = RXSymbol_symbolForCString("body");
    RXSymbol_isMethod_o = RXSymbol_symbolForCString("isMethod");
    RXSymbol_method_o = RXSymbol_symbolForCString("method");
    RXSymbol_block_o = RXSymbol_symbolForCString("block");
    
    RXClosure_o = RXObject_spawn(RXObject_o);
    // TODO replace List with Array
    RXObject_setSlot(RXClosure_o, RXSymbol_parameters_o, RXList_spawn(RXList_o, NULL), false);
    RXObject_setSlot(RXClosure_o, RXSymbol_context_o, RXNil_o, false);
    RXObject_setSlot(RXClosure_o, RXSymbol_body_o, RXExpression_spawn(RXExpression_o, NULL), false);
    RXObject_setSlot(RXClosure_o, RXSymbol_isMethod_o, RXBoolean_false_o, false);
    
    RXNativeMethod_attach(RXClosure, activate);
    RXNativeMethod_attach(RXProtoObject, method);
    RXNativeMethod_attach(RXProtoObject, block);
    
    RXSymbol_Closure_o = RXSymbol_symbolForCString("Closure");
    RXObject_setSlot(RXLobby_o, RXSymbol_Closure_o, RXClosure_o, false);
}
