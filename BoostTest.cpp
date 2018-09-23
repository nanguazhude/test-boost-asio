#include "BoostTest.hpp"
#include <QtCore/qabstracteventdispatcher.h>
#include <QtCore/qabstractnativeeventfilter.h> 
#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
using namespace std::chrono_literals;

//https://blog.csdn.net/byxdaz/article/details/71088812
//https://blog.csdn.net/KnightOnHourse/article/details/80333238
using io_service = boost::asio::io_service;

BoostTest::BoostTest() {

}

class NativeEventFilter :
    public QAbstractNativeEventFilter  {
public:
};


void BoostTest::test_boost_asio() {

    if constexpr (false) {
        std::shared_ptr service = std::make_shared<io_service>();
        std::thread([service]() {
            boost::asio::io_service::work locker{ *service };
            service->run();
            std::cout << "boost service destoryed!" << std::endl;
        }).detach();

        for (int i = 0; i < 1000; ++i)
            service->post([]() {
            std::cout << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(1ms);
        });

        while (service->poll_one());
    }

    auto service = std::make_shared<io_service>() ;

    service->post([]() {std::cout << "Hellow Word!" << std::endl; });

    auto exec = service->get_executor();
    
    boost::asio::deadline_timer timer{ *service };
    //boost::asio::defer(*service);

    std::thread([service]() {
        boost::asio::io_service::work locker{ *service };
        service->run();
        std::cout << "boost service destoryed!" << std::endl;
    }).detach();

}



