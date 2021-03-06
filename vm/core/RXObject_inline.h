
#ifndef __RX_CORE_OBJECT_INLINE_H__
#define __RX_CORE_OBJECT_INLINE_H__

#include "RXCore.h"
#include <stdbool.h>
#include <assert.h>

/*
 * Accessor to the core data of a given object.
 */
#define RXObject_coreData(self) (self)->__coreData__[-1]

/*
 * Allocate memory for a new object with a given amount of payload bytes.
 * This macro allocates memory for the core object data and the payload.
 */
RX_INLINE RXObject_t* RXObject_allocateSize(const size_t size) {
    return (RXObject_t*)((char*)malloc(size + sizeof(RXObjectCoreData_t)) + sizeof(RXObjectCoreData_t));
}

/*
 * Allocate memory for a new object with type t.
 * This macro allocates memory for the core object data and the payload.
 */
#define RXObject_allocateType(t) RXObject_allocateSize(sizeof(t))

/*
 * Deallocate memory for a previously allocated object.
 */
RX_INLINE void RXCore_deallocateObject(RXObject_t* self) {
    free((char*)self - sizeof(RXObjectCoreData_t));
}

/*
 * Initialize a newly allocated object.
 * A new object has an empty slot list.
 */
RX_INLINE void RXObject_initialize(RXObject_t* self) {
    RXObject_coreData(self).slots = NULL;
    RXObject_coreData(self).flags = 0;
}

/*
 * Allocate and initialize a new object.
 * This function does not set the delegate slot.
 * See RXObject_spawn.
 */
RX_INLINE RXObject_t* RXObject_new(void) {
    RXObject_t* self = RXObject_allocateType(RXObject_t);
    RXObject_initialize(self);
    return self;
}

RX_INLINE RXObject_t* RXObject_delegate(RXObject_t* self) {
    return (RXObject_t*)((intptr_t)RXObject_coreData(self).delegate & ~3);
}

/*
 * Allocate and initialize a new object.
 * Set the delegate slot of the new object to self.
 */
RX_INLINE RXObject_t* RXObject_spawn(RXObject_t* self) {
    RXObject_t* result = RXObject_new();
    RXObject_setDelegate(result, self);
    return result;
}

/*
 * Prepare the given object to be deleted.
 * This function removes all slots, but does not deallocate the object itself.
 * See RXCore_deallocateObject.
 */
RX_INLINE void RXObject_finalize(RXObject_t* self) {
    eina_rbtree_delete(RXObject_coreData(self).slots, EINA_RBTREE_FREE_CB(free), NULL);
}

#endif
