#include "user.h"

User::User()
{

}
User::~User(){

}

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

User::User(QVector<QString> data)
{
    _username = data[0];
    _password = data[1];
    _lastname = data[2];
    _patronymic = data[3];
    _name = data[4];
    _nodegroup = data[5];
    _roles = data[6];
    _phonenumber = data[7];
    _email = data[8];
    _userdescription = data[9];
    _passwordexpiration =data[10];
    _loginattemptsaviable = data[11];

}

QJsonObject User::toJson() const
{
    QJsonObject obj;
    obj["username"] = _username;
    obj["password"] = _password;
    obj["lastname"] = _lastname;
    obj["patronymic"]=_patronymic;
    obj["name"] =     _name;
    obj["nodegroup"] =_nodegroup;
    obj["roles"] =    _roles;
    obj["phonenumber"]=_phonenumber;
    obj["email"] =    _email;
    obj["userdescription"]=_userdescription;
    obj["passwordexpiration"] =_passwordexpiration;
    obj["loginattemptsaviable"] = _loginattemptsaviable;
    return obj;
}

QVector<QString> User::getFeatures()
{
    return {_username,_password,_lastname,_patronymic, _name,_nodegroup, _roles, _phonenumber, _email, _userdescription, _passwordexpiration, _loginattemptsaviable};
}
