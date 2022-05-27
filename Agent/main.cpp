#include <QCoreApplication>
#include "logscrapper.h"
#include "networkrequestshandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    LogScrapper log;
    NetworkRequestsHandler netManager;
    return a.exec();
}
