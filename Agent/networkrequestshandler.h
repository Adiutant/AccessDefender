#ifndef NETWORKREQUESTSHANDLER_H
#define NETWORKREQUESTSHANDLER_H
#include <string>
#include <QObject>
#include <iostream>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QString>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTcpSocket>
#include <QTimer>
#include <logscrapper.h>


class NetworkRequestsHandler : public QObject
{
    Q_OBJECT
public slots:
    void throwAnyStatus(QString statusCode);
    void stopTryingToThrow();
    bool tryToThrowStatus();
    void prepareToEmitStatus();
private:
    void connectToHost(QTcpSocket& jSocket);
    void pushLogs();
    const QString ONLINE_STATUS_CODE = "ONLINE_CODE";
    const QString HEALTHY_STATUS_CODE = "HEALTHY_CODE";
    const QString UNHEALTHY_STATUS_CODE = "UNHEALTHY_CODE";
    const QString ONLINE_ACCEPTED_STATUS_CODE = "AGENT_ONLINE_CODE_ACCEPTED";
    const QString PUSH_LOGS_SIGNATURE = "PUSH_LOGS";
    const QString LOGS_ACCEPTED_CODE = "LOGS_ACCEPTED";
    const QString HEALTHY_ACCEPTED_CODE = "HEALTHY_STATE_CONFIRMED";


    QTimer *_updateTimer;
    LogScrapper *_logScrapper;
    int _agentCode;
public:
    NetworkRequestsHandler();

signals:
    void connected();
    void emitStatus(QString statusCode);



};

#endif // NETWORKREQUESTSHANDLER_H
