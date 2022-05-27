#ifndef NETWORKREQUESTSHANDLER_H
#define NETWORKREQUESTSHANDLER_H
#include <QSqlDatabase>
#include <QtSql>
#include "QMessageBox"
#include <string>
#include <QtWidgets>
#include <QObject>
#include <iostream>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QSqlDatabase>
#include <QSql>
#include <QString>
#include <QtNetwork/QNetworkReply>
#include <QHttpPart>
#include "user.h"
#include "loginwindow.h"
#include <QVector>
#include<administration.h>

class User;
class LoginWindow;
class Administration;

class NetworkRequestsHandler :public QObject {

private :
    QNetworkAccessManager *netManager;

    const QString HEALTHY_CODE = "HEALTHY_ACT";
    const QString NEW_CLIENT_SIGNATURE = "NEW_CLIENT_ATTACHED";
    const QString NEW_CLIENT_SIGNATURE_ACCEPTED = "NEW_CLIENT_ATTACHED_YES";
    const QString LOGIN_CLIENT_SIGNATURE = "LOGIN_CLIENT";
    const QString LOGIN_CLIENT_SIGNATURE_SUCCESS = "LOGIN_SUCCESS";
    const QString CREATE_NEW_ADMINISTRATOR_SIGNATURE = "CREATE_NEW_ADMINISTRATOR";
    const QString CREATE_NEW_ADMINISTRATOR_SUCCESS = "SUCCESS_CREATED_NEW_ADMINISTRATOR";
    const QString GET_ARM_TABLE_SIGNATUTE  = "GET_ARM_TABLE";
    const QString GET_ARM_TABLE_SUCCESS  = "GET_ARM_TABLE_YES";
    const QString GET_ARM_LOGS_SIGNATURE = "GET_ARM_LOGS";
    const QString GET_ARM_LOGS_SUCCESS = "LOGS_DELIVERED";
    const QString GET_USERS_TABLE_SIGNATUTE  = "GET_USERS_TABLE";
    const QString GET_USERS_TABLE_SUCCESS  = "GET_USERS_TABLE_YES";
    const QString UPDATE_USERS_TABLE_SIGNATURE = "UPDATE_USERS";
    const QString UPDATE_USERS_TABLE_SUCCESS = "USERS_UPDATED";

    QString _accessToken;

Q_OBJECT

public:
    explicit NetworkRequestsHandler(QObject *parent=0);
     ~NetworkRequestsHandler();
    void checkConnection(LoginWindow *window);
    void checkAccess(QString username, QString password , LoginWindow *window );
    void createNewAdministrator(QString username, QString password, QString personal_info, QString permissions,LoginWindow *window);
    void updateUsersTable(QVector<User> users,Administration* window);/*QString username, QString password,QString lastname ,
                      QString patronymic, QString name, QString nodegroup, QString roles, QString phonenumber,
                      QString email, QString userdescription, QString passwordexpiration, QString loginattemptsaviable);*/
    std::vector<QString> getArmTable();
    std::vector<User> getUsersTable();
    QString getLogs(int agentCode);
private:
    void makeNetRequest(QString request,QJsonObject &obj );
};

#endif // NETWORKREQUESTSHANDLER_H
