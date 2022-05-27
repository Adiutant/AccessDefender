#include "loginwindow.h"
#include <QApplication>
#include "networkrequestshandler.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;
   // NetworkRequestsHandler netHandler;
//    netHandler.checkConnection(&w);
    w.show();

    return a.exec();
}
