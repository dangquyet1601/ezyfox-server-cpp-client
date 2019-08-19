#include <iostream>
#include <thread>
#include <chrono>
#include "EzyHeaders.h"
#include "gc/EzyAutoReleasePool.h"

using namespace EZY_NAMESPACE;

int count = 0;

class ExHandshakeHandler : public handler::EzyHandshakeHandler {
protected:
    request::EzyRequest* getLoginRequest() {
        auto request = request::EzyLoginRequest::create();
        request->setZoneName("freechat");
        request->setUsername("Dungtv");
        request->setPassword("");
        return request;
    };
};

class ExLoginSuccessHandler : public handler::EzyLoginSuccessHandler {
protected:
    void handleLoginSuccess(entity::EzyArray* joinedApps, entity::EzyValue* responseData) {
        auto request = request::EzyAppAccessRequest::create();
        request->setAppName("hello-world");
        request->setData(new entity::EzyArray());
        mClient->send(request);
    }
};

class ExAppAccessHandler : public handler::EzyAccessAppHandler {
protected:
    void postHandle(entity::EzyApp* app, entity::EzyArray* data) {
        auto obj = new entity::EzyObject();
        obj->setString("message", "Hi EzyFox, I'm from C++ client");
        app->send("broadcastMessage", obj);
    }
};

class MessageResponseHandler : public handler::EzyAbstractAppDataHandler<entity::EzyValue> {
protected:
    void process(entity::EzyApp* app, entity::EzyValue* data) {
        logger::log("recived message: ");
#ifdef EZY_DEBUG
        data->printDebug();
#endif
    }
};

void connectOnMainThread(EzyClient* client);

void connectOnDetachedThread(EzyClient* client);

void loopOnMainThread(EzyClient* client);

void loopOnDetachedThread(EzyClient* client);

void vectorTest();

class A {
public:
    void start();
    void run();
};

void A::start() {
    std::thread newThread(&A::run, this);
    newThread.detach();
}

void A::run() {
    std::cout << "a run\n";
    delete this;
}

int main(int argc, const char * argv[]) {
    srand( static_cast<unsigned int>(time(NULL)));
    A* a = new A();
    a->start();
    vectorTest();
//    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
#ifdef EZY_DEBUG
    gc::EzyAutoReleasePool::getInstance()->newPool("main");
#endif
    concurrent::EzyThread::setCurrentThreadName("main");
    auto config = config::EzyClientConfig::create();
    config->setClientName("first");
    config->setZoneName("example");
    auto client = EzyClients::getInstance()->newDefaultClient(config);
    auto setup = client->setup();
    setup->addEventHandler(event::ConnectionSuccess, new handler::EzyConnectionSuccessHandler());
    setup->addEventHandler(event::ConnectionFailure, new handler::EzyConnectionFailureHandler());
    setup->addDataHandler(constant::Handshake, new ExHandshakeHandler());
    setup->addDataHandler(constant::Login, new ExLoginSuccessHandler());
    setup->addDataHandler(constant::LoginError, new handler::EzyLoginErrorHandler());
    setup->addDataHandler(constant::AppAccess, new ExAppAccessHandler());
    auto appSetup = setup->setupApp("hello-world");
    appSetup->addDataHandler("broadcastMessage", new MessageResponseHandler());
    logger::log("start client");
//    loopOnDetachedThread(client);
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    connectOnMainThread(client);
//    for(int i = 0 ; i < 100 ; ++i)
//        connectOnDetachedThread(client);
//    loopOnMainThread(client);
//    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
//    EZY_SAFE_DELETE(client);
//    EZY_SAFE_DELETE(client);
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        client->processEvents();
        concurrent::EzyThread::executeMainHandler();
    }
//    logger::log("shutdown client");
    getchar();
    return 0;
}

void connectOnMainThread(EzyClient* client) {
    client->connect("ws.tvd12.com", 3005);
}

void connectOnDetachedThread(EzyClient* client) {
    std::thread nthread(connectOnMainThread, client);
    nthread.detach();
}

void loopOnMainThread(EzyClient* client) {
    concurrent::EzyThread::setCurrentThreadName("main");
    auto mainEventsLoop = new socket::EzyMainEventsLoop();
    mainEventsLoop->start();
}

void loopOnDetachedThread(EzyClient* client) {
    std::thread nthread(loopOnMainThread, client);
    nthread.detach();
}

void vectorTest() {
    
    std::vector<int> myvector;
    
    // set some values (from 1 to 10)
    for (int i = 1; i <= 10; i++) myvector.push_back(i);
    
    // erase the 6th element
    myvector.erase (myvector.begin()+5);
    
    // erase the first 3 elements:
    myvector.erase (myvector.begin(),myvector.begin()+3);
    
    myvector.pop_back();
    
    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); ++i)
        std::cout << ' ' << myvector[i];
    std::cout << '\n';
    
    std::stack<int> s;
    s.push(1);
    std::cout << "s0: " << s.top();
    
    std::cout << '\n';

}
