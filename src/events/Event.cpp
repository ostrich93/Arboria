#include "Event.h"
#include "../Heap.h"

namespace Arboria {
    AEvent::~AEvent()
    {
        if (eventPtr != NULL) {
            Mem_Free(eventPtr);
        }
        eventType = EventType::EVENT_UNDEFINED;
        eventValue = 0;
        eventValue2 = 0;
        isHandled = false;
    }
}