#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
//#include <QtWidgets>
#include <iostream>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUrl>
#include <string>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <vector>
#include "RSA.h"
#include "dbhelper.h"
#include <QCryptographicHash>
#include <map>
#include <agent.h>
#include <QTimer>
#include <user.h>
#include <QVector>


struct AgentCode
{
    QString codeRepresentation;
    int code;
};
class Agent;
class User;
class HttpServer :public QTcpServer
{

Q_OBJECT

public slots:
    void onReadyRead();
    void onDisconnect();
    void actualizeAgents();

public:
    explicit HttpServer(QObject *parent=0);
    ~HttpServer();

    void incomingConnection(qintptr handle);
private:
    DbHelper *dbHelper;
    QVector<QString> _accessTokens;
    QVector<Agent> *_agents;
    QVector<User> *_users;
    QTimer *_updatingTimer;
    //QString parseContent(QString row);
    AgentCode makeAgentCode(int*);
    template <typename T1,typename T2>
    bool contains(T1 set, T2 elem);
    const QString NEW_CLIENT_SIGNATURE = "NEW_CLIENT_ATTACHED";
    const QString CREATE_NEW_ADMINISTRATOR_SIGNATURE = "CREATE_NEW_ADMINISTRATOR";
    const QString CREATE_NEW_ADMINISTRATOR_SUCCESS = "SUCCESS_CREATED_NEW_ADMINISTRATOR";
    const QString NEW_CLIENT_SIGNATURE_ACCEPTED = "NEW_CLIENT_ATTACHED_YES";
    const QString LOGIN_CLIENT_SIGNATURE = "LOGIN_CLIENT";
    const QString LOGIN_CLIENT_SIGNATURE_SUCCESS = "LOGIN_SUCCESS";
    const QString GET_ARM_TABLE_SIGNATUTE  = "GET_ARM_TABLE";
    const QString GET_ARM_TABLE_SUCCESS  = "GET_ARM_TABLE_YES";
    const QString GET_ARM_LOGS_SIGNATURE = "GET_ARM_LOGS";
    const QString GET_ARM_LOGS_SUCCESS = "LOGS_DELIVERED";
    const QString UPDATE_USERS_TABLE_SIGNATURE = "UPDATE_USERS";
    const QString UPDATE_USERS_TABLE_SUCCESS = "USERS_UPDATED";
    const QString GET_USERS_TABLE_SIGNATUTE  = "GET_USERS_TABLE";
    const QString GET_USERS_TABLE_SUCCESS  = "GET_USERS_TABLE_YES";


    const QString HEALTHY_ACCEPTED_CODE = "HEALTHY_STATE_CONFIRMED";
    const QString ONLINE_STATUS_CODE = "ONLINE_CODE";
    const QString HEALTHY_STATUS_CODE = "HEALTHY_CODE";
    const QString UNHEALTHY_STATUS_CODE = "UNHEALTHY_CODE";
    const QString ONLINE_ACCEPTED_STATUS_CODE = "AGENT_ONLINE_CODE_ACCEPTED";
    const QString PUSH_LOGS_SIGNATURE = "PUSH_LOGS";
    const QString LOGS_ACCEPTED_CODE = "LOGS_ACCEPTED";


};

#endif // HTTPSERVER_H
