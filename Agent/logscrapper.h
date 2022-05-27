#ifndef LOGSCRAPPER_H
#define LOGSCRAPPER_H
#include "QString"
#include "QFile"
#include <QDebug>
class LogScrapper
{
public:
    LogScrapper();
    QString getRawSysLogs();
private:

    QByteArray readFile(QString filename);
};

#endif // LOGSCRAPPER_H
