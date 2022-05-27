#include "user.h"

User::User()
{

}

 User User::fromJson(QJsonObject obj)
{
    User user{
        obj["username"].toString(),
        obj["password"].toString(),
        obj["lastname"].toString(),
        obj["patronymic"].toString(),
        obj["name"].toString(),
        obj["nodegroup"].toString(),
        obj["roles"].toString(),
        obj["phonenumber"].toString(),
        obj["email"].toString(),
        obj["userdescription"].toString(),
        obj["passwordexpiration"].toString(),
        obj["loginattemptsaviable"].toString(),
    };
    return user;



}
User::~User(){}

User::User(QString username, QString password, QString lastname, QString patronymic, QString name, QString nodegroup,
           QString roles, QString phonenumber, QString email, QString userdescription, QString passwordexpiration, QString loginattemptsaviable)
{
    _username = username;
    _password = password;
    _lastname = lastname;
    _patronymic = patronymic;
    _name = name;
    _nodegroup = nodegroup;
    _roles = roles;
    _phonenumber = phonenumber;
    _email = email;
    _userdescription = userdescription;
    _passwordexpiration =passwordexpiration;
    _loginattemptsaviable = loginattemptsaviable;


}

QVector<QString> User::getFeatures()
{
    return {_username,_password,_lastname,_patronymic, _name,_nodegroup, _roles, _phonenumber, _email, _userdescription, _passwordexpiration, _loginattemptsaviable};
}
