#include "MainWindow.hpp"
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>
#include <iostream>

#include "BoostTest.hpp"
#include <QtCore/private/qeventdispatcher_win_p.h>

class Dispatcher : public QAbstractEventDispatcher {
    QAbstractEventDispatcher * const other;
public:
    Dispatcher(QAbstractEventDispatcher *o) :other(o) {}
    virtual void interrupt() override { return other->interrupt(); }
    virtual bool processEvents(QEventLoop::ProcessEventsFlags flags) { return other->processEvents(flags); }
    virtual bool registerEventNotifier(QWinEventNotifier *notifier) { return other->registerEventNotifier(notifier); }
    virtual void registerSocketNotifier(QSocketNotifier *notifier) { return other->registerSocketNotifier(notifier); }
    int registerTimer(int interval, Qt::TimerType timerType, QObject *object) { return other->registerTimer(interval, timerType, object); }
    virtual void registerTimer(int timerId, int interval, Qt::TimerType timerType, QObject *object) {
        return other->registerTimer(timerId, interval, timerType, object);
    }
    virtual QList<QAbstractEventDispatcher::TimerInfo> registeredTimers(QObject *object) const { return other->registeredTimers(object); }
    virtual int remainingTime(int timerId) { return remainingTime(timerId); }
    virtual void unregisterEventNotifier(QWinEventNotifier *notifier) { return other->unregisterEventNotifier(notifier); }
    virtual void unregisterSocketNotifier(QSocketNotifier *notifier) { return other->unregisterSocketNotifier(notifier); }
    virtual bool unregisterTimer(int timerId) { return other->unregisterTimer(timerId); }
    virtual bool unregisterTimers(QObject *object) { return other->unregisterTimers(object); }
    virtual void wakeUp() { return other->wakeUp(); }
    void flush() { return other->flush(); }
    bool hasPendingEvents() { return other->hasPendingEvents(); }
};

class Dispatcher1 : public QEventDispatcherWin32 {};

#include <boost/smart_ptr/detail/lightweight_mutex.hpp>
#include <QtCore/qthread.h> 
#include <atomic>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

class TestThread : public QThread {
public:
    int count = 0;
    virtual void run() {
        using namespace std::chrono_literals;
        while (++count < 600) {
            std::cout << count << std::endl;
            std::this_thread::sleep_for(1ms);
        }
    }

    ~TestThread() {
        std::cout << "destory this thread" << std::endl;
    }

};

#include <private/qthread_p.h>

class AThread : public QThread {
public:

};

#include <memory>



#include "QtThread.hpp"

class BB {
public:
    union {
        int * a;
        double * b;
    };

    struct {
        int m;
        double c;
    };

};

//qwindowsguieventdispatcher
//#include "qt_event_dispatcher/MainThreadEventDispatcher.hpp"
//class ThisMainThreadEventDispatcher :
//    public MainThreadEventDispatcher {
//public:
//
//};

class QObject1 : public QObject {
public:
    bool event(QEvent * e ) override {
        return QObject::event(e);
    }
    
    virtual bool eventFilter(QObject *watched, QEvent *event) {
        return false;
    }

};

int main(int argc, char *argv[]) {
    
    //QCoreApplication::setEventDispatcher(new ThisMainThreadEventDispatcher);
    QApplication app(argc, argv);

    //auto varEventDispatcher = QCoreApplication::eventDispatcher();
    //varEventDispatcher->wakeUp();
    //varEventDispatcher->wakeUp();
    //varEventDispatcher->wakeUp();

    //qDebug() << varEventDispatcher;
    
    /*auto test = QThread::create([]() {
        auto * test = new TestThread;
        test->moveToThread(qApp->thread());
        test->connect(test, &QThread::finished, test, &QThread::deleteLater);
        test->start();
    }) ;
    test->moveToThread(qApp->thread());
    test->connect(test, &QThread::finished, test, &QThread::deleteLater);
    test->start();*/

    //{

    //    class TestObject1 : public QObject {
    //    public:
    //        ~TestObject1() {
    //            std::cout << __func__ << std::endl;
    //        }
    //    };

    //    QtThread<TestObject1> test;
    //    test.start();

    //    auto test1{ test };

    //    std::cout << std::boolalpha << test.isQuit() << std::endl;
    //    //test.stop();
    //    std::cout << std::boolalpha << test.isQuit() << std::endl;
    //    {
    //        auto object = test.object();
    //        std::cout << object.use_count() << std::endl;
    //    }
    //}


    //std::cout << typeid(*app.eventDispatcher()).name() << std::endl;

    MainWindow window;

    BoostTest test;
    test.test_boost_asio();

    window.show();

    auto obj1 = new QObject1;
    QCoreApplication::postEvent(obj1,new QEvent(QEvent::MaxUser));
    QCoreApplication::instance()->installEventFilter(obj1);

    return app.exec();

}

static void preRoutineMyDebugTool(){
     
}

Q_COREAPP_STARTUP_FUNCTION(preRoutineMyDebugTool)

/**

There are five different ways that events can be processed; reimplementing this virtual function is just one of them. All five approaches are listed below:
Reimplementing paintEvent(), mousePressEvent() and so on. This is the most common, easiest, and least powerful way.
Reimplementing this function. This is very powerful, providing complete control; but only one subclass can be active at a time.
Installing an event filter on QCoreApplication::instance(). Such an event filter is able to process all events for all widgets, so it's just as powerful as reimplementing notify(); furthermore, it's possible to have more than one application-global event filter. Global event filters even see mouse events for disabled widgets. Note that application event filters are only called for objects that live in the main thread.
Reimplementing QObject::event() (as QWidget does). If you do this you get Tab key presses, and you get to see the events before any widget-specific event filters.
Installing an event filter on the object. Such an event filter gets all the events, including Tab and Shift+Tab key press events, as long as they do not change the focus widget.

**/


