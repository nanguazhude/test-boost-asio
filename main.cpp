#include "MainWindow.hpp"
#include <QApplication>

#include "BoostTest.hpp"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    MainWindow window;
	BoostTest test;
	test.test_boost_asio();
    window.show();

    return app.exec();
}
