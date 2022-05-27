#include "networkrequestshandler.h"

NetworkRequestsHandler::NetworkRequestsHandler()
{
    _logScrapper = new LogScrapper();
    _updateTimer = new QTimer();
    _updateTimer->setInterval(2000);
    connect(this, SIGNAL(connected()),this,SLOT(stopTryingToThrow()));
    connect(_updateTimer,SIGNAL(timeout()),this,SLOT(tryToThrowStatus()));
    _updateTimer->start();

}
void NetworkRequestsHandler::stopTryingToThrow()
{
    disconnect(_updateTimer,SIGNAL(timeout()),this,SLOT(tryToThrowStatus()));
    disconnect(this, SIGNAL(connected()),this,SLOT(stopTryingToThrow()));
    connect(this,SIGNAL(emitStatus(QString)),this,SLOT(throwAnyStatus(QString)));
    connect(_updateTimer,SIGNAL(timeout()),this,SLOT(prepareToEmitStatus()));

}

void NetworkRequestsHandler::prepareToEmitStatus()
{
    emit emitStatus(HEALTHY_STATUS_CODE);
}

void NetworkRequestsHandler::pushLogs(){
    QTcpSocket jSocket;
    QJsonObject obj;
    obj["request"] = PUSH_LOGS_SIGNATURE;
    obj["agent_code"] = _agentCode;
    obj["raw_logs"] = _logScrapper->getRawSysLogs();
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    connectToHost(jSocket);
    if (jSocket.waitForConnected(1000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(1000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == LOGS_ACCEPTED_CODE)
        {

            return;
        }
        else
        {
            qDebug() << "Не прием логов, ошибка ";
            return;
        }

    }
    else
    {
        qDebug() << "Соединение сброшено, попытка переподключения...";
        return;
    }

}

void NetworkRequestsHandler::throwAnyStatus(QString statusCode)
{
    QTcpSocket jSocket;
    QJsonObject obj;
    obj["request"] = statusCode;


    obj["agent_code"] = _agentCode;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    connectToHost(jSocket);
    if (jSocket.waitForConnected(1000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(1000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == HEALTHY_ACCEPTED_CODE)
        {
            pushLogs();
            return;
        }
        else
        {
            qDebug() << "Не прием статуса, ошибка ";
        }

    }
    else
    {
        qDebug() << "Соединение сброшено, попытка переподключения...";
    }
    return ;
}

void NetworkRequestsHandler::connectToHost(QTcpSocket& jSocket)
{
    jSocket.connectToHost("127.0.0.1",1337);

}


bool NetworkRequestsHandler::tryToThrowStatus(){
    QTcpSocket jSocket;
    QJsonObject obj;
    obj["request"] = ONLINE_STATUS_CODE;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    connectToHost(jSocket);
    if (jSocket.waitForConnected(1000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(1000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == ONLINE_ACCEPTED_STATUS_CODE)
        {
            _agentCode = obj["agent_code"].toInt();
            qDebug() << "Соединение установлено";
            emit connected();
            return true;
        }

    }

    qDebug() << "Соединение НЕ установлено";

    return false;
}
