#pragma once

#include "EventLoopDispatcherSelect.hpp"

class MainThreadEventDispatcher :public QMainThreadEventDispatcher {
    Q_OBJECT
public:
    MainThreadEventDispatcher(QObject * p=nullptr);
private:
    using Super = QMainThreadEventDispatcher;
};


