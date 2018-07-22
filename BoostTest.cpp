#include "BoostTest.hpp"

#include <thread>
#include <iostream>
#include <boost/asio.hpp>

//https://blog.csdn.net/byxdaz/article/details/71088812
//https://blog.csdn.net/KnightOnHourse/article/details/80333238
using io_service = boost::asio::io_service;

BoostTest::BoostTest() {

}

void BoostTest::test_boost_asio() {

	std::shared_ptr service = std::make_shared<io_service>();
	std::thread([service]() {
		boost::asio::io_service::work locker{ *service };
		service->run();
		std::cout << "boost service destoryed!" << std::endl;
	}).detach();

	service->post([]() { 
		std::cout << std::this_thread::get_id() << std::endl;
	});
	
}



