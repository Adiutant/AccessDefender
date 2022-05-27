#include "logscrapper.h"

LogScrapper::LogScrapper()
{

}
QString LogScrapper::getRawSysLogs(){
    return QString::fromUtf8(readFile("/var/log/syslog") + "\0");
}
QByteArray LogScrapper::readFile(QString filename)
{
    QFile logFile(filename);
    if (!logFile.open(QIODevice::ReadOnly))
        return 0;
    QByteArray data;
    data  = logFile.readAll();
    qDebug() << data;
    return data;

}
