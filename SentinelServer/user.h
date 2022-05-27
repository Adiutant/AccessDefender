#ifndef USER_H
#define USER_H
#include <QString>
#include <QVector>
#include <QJsonObject>
class User
{

private:
    QString _username;
    QString _password;
    QString _lastname;
    QString _patronymic;
    QString _name;
    QString _nodegroup;
    QString _roles;
    QString _phonenumber;
    QString _email;
    QString _userdescription;
    QString _passwordexpiration;
    QString _loginattemptsaviable;
public:
    QVector<QString> getFeatures();
    User();
    static User fromJson(QJsonObject json);
    User(QString username, QString password, QString lastname,
          QString patronymic, QString name, QString nodegroup,
          QString roles, QString phonenumber, QString email,
          QString userdescription, QString passwordexpiration, QString loginattemptsaviable);
    ~User();
};

#endif // USER_H
