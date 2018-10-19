#include "../logger/EzyLogger.h"
#include "EzyEventHandler.h"
#include "EzyEventHandlers.h"
#include "../EzyClient.h"

EZY_NAMESPACE_START_WITH(handler)
    
EzyEventHandlers::EzyEventHandlers(EzyClient* client) {
    this->mClient = client;
}

EzyEventHandlers::~EzyEventHandlers() {
    EZY_FOREACH_MAP(mHandlers) {
        EZY_SAFE_DELETE(it->second);
    }
    mHandlers.clear();
}
    
void EzyEventHandlers::handle(event::EzyEvent *event) {
    auto eventType = event->getType();
    auto hanlder = mHandlers[eventType];
    if(hanlder)
        hanlder->handle(event);
    else
        logger::log("has no handler for event type: %d", eventType);
}

EzyEventHandler* EzyEventHandlers::getHandler(event::EzyEventType eventType) {
    auto handler = mHandlers[eventType];
    return handler;
}

void EzyEventHandlers::addHandler(event::EzyEventType eventType, EzyEventHandler *handler) {
    handler->setClient(mClient);
    mHandlers[eventType] = handler;
}

EZY_NAMESPACE_END_WITH
