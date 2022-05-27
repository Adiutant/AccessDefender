#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtSql>
#include <QSql>
#include <QApplication>
#include <QMainWindow>
#include <QCryptographicHash>
#include <user.h>
#include <QVector>

class DbHelper
{
public:
    DbHelper();
    bool updateUsersTable(QVector<User> users);
    bool checkDatabaseConnection();
    bool checkAutorisation(QString username, QString password);
    bool createNewAdministrator(QString username, QString password, QString personalInfo, QString permissons);
    bool createNewUser(QString username, QString password, QString lastname,
                       QString patronymic, QString name, QString nodegroup,
                       QString roles, QString phonenumber, QString email,
                       QString userdescription, QString passwordexpiration, QString loginattemptsaviable);
    QVector<User>* getAllUsers();
    User* getUserByUsername(QString username);
};

#endif // DBHELPER_H
