#include "QObjectBoostAsioEventLoop.hpp"
#include <QtCore/qthread.h>
#include <QtCore/qdebug.h>
#include <boost/asio.hpp>
#include <cassert>

namespace {
    using io_contex = boost::asio::io_context;
}

QObjectBoostAsioEventLoop::QObjectBoostAsioEventLoop(QAbstractEventDispatcher * arg) : mmm_Dispatcher(arg){
    assert(arg);
    mmm_IOContex = std::make_shared<io_contex>( 1 );
    connect(
        mmm_Dispatcher, &QAbstractEventDispatcher::awake, 
        this,           &QObjectBoostAsioEventLoop::ppp_RunOnce,
        Qt::DirectConnection);
}

void QObjectBoostAsioEventLoop::ppp_Exec() {
    auto varRunContex = mmm_IOContex;
    for (;;) {
        try {
            assert(QThread::currentThread() == mmm_Dispatcher->thread());
            ppp_RunOnce()/*we send runonce to the eventloop*/;
            boost::asio::io_service::work locker{ *varRunContex };
            varRunContex->run();
            break;
        }
        catch (...) {
            qDebug() << "this is a exception @ exec , ignore it";
        }
    }
}

void QObjectBoostAsioEventLoop::ppp_Quit() {
    auto varRunContex = mmm_IOContex;
    if (varRunContex &&(varRunContex->stopped()==false) ) {
        ppp_RunOnce()/*we send runonce*/;
        varRunContex->post([varRunContex]() {varRunContex->stop(); })/*we send stop to the eventloop*/;
    }
}

void QObjectBoostAsioEventLoop::ppp_RunOnce() {
    auto varRunContex  = mmm_IOContex;
    auto varDispatcher = mmm_Dispatcher;
    if (varRunContex) {
        varRunContex->post([varRunContex, varDispatcher]() {
            if (varDispatcher) {
                assert(QThread::currentThread() == varDispatcher->thread());
                varDispatcher->processEvents(QEventLoop::AllEvents);
            }
        });
    }
}



