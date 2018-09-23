#pragma once

#include <memory>
#include <atomic>
#include <functional>
#include <QtCore/qthread.h>
#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>
#include <QtCore/qabstracteventdispatcher.h>

namespace boost {
    namespace asio {
        class io_context;
    }/*asio*/
}/*boost*/

class QObjectBoostAsioEventLoop : public QObject {
    Q_OBJECT
public:
    QObjectBoostAsioEventLoop(QAbstractEventDispatcher * ed = QAbstractEventDispatcher::instance(nullptr));
    Q_SLOT void exec() { ppp_Exec(); }
    Q_SLOT void quit() { ppp_Quit(); }
protected:
    QPointer<QAbstractEventDispatcher> mmm_Dispatcher;
    std::shared_ptr<boost::asio::io_context> mmm_IOContex;
    void ppp_Exec();
    void ppp_Quit();
    Q_SLOT void ppp_RunOnce();
    using Super = QObject;
};










