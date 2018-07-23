#include "QtThread.hpp"
#include <QtCore/qcoreapplication.h>

QCoreApplication * $T$QtThread::getQApp(){
	return qApp;
}

QThread * $T$QtThread::getQAppThread() {
	return getQApp()->thread() ;
}

void $T$QtThread::connnectQAppQuit(QObject * obj, std::function<void(void)> func) {
	getQApp()->connect(
		getQApp(),&QCoreApplication::aboutToQuit,
		obj, func);
}

void $T$QtThread::RunObject::postEvent(std::function<void(void)> arg){
	auto e = new RunEvent;
	e->$m$RunFunction = std::move(arg);
	QCoreApplication::postEvent(this,e);
}
