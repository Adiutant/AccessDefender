#include "dbhelper.h"

DbHelper::DbHelper()
{

    if(!checkDatabaseConnection())
    {
        qDebug() << "Ошибка базы данных!";
        exit(0);
    }
}

bool DbHelper::updateUsersTable(QVector<User> users)
{
    QSqlQuery query;
    if (!query.exec("TRUNCATE users;"))
    {
        qDebug() << "Db Error" + query.lastError().text();
        return false;
    }
    for (User user: users)
    {
        query.prepare("INSERT INTO users (username,password,lastname,otchestvo,name,nodegroup,"
                      "roles,phonenumber,email,userdescription,"
                      "passwordexpiration,loginattemptsaviable) VALUES (?,?,?,?,?,?,?,?,?,?,?,?);");
        QVector<QString> features = user.getFeatures();
        for (int i =0;i<features.length();i++)
        {
            if (i==1)
                query.addBindValue(QString(QCryptographicHash::hash(features[i].toUtf8(),QCryptographicHash::Md5).toHex()));
            else
                query.addBindValue(features[i]);

        }

        if (!query.exec())
        {
            qDebug()<<query.lastQuery();
            qDebug() << "Db Error" + query.lastError().text();
            return false;
        }

    }
    return true;
}

bool DbHelper::checkDatabaseConnection()
{
    QSqlDatabase dbObj = QSqlDatabase::addDatabase("QPSQL");
    dbObj.setDatabaseName("sentinel_db");
    dbObj.setUserName("postgres");
    //dbObj.setPassword("123");

    if (!dbObj.open()){
        qDebug() << "Db Error" + dbObj.lastError().text()<< endl;
        return false;


    }
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS admins (id SERIAL PRIMARY KEY, username TEXT, password TEXT, personal_info TEXT, permissions TEXT);"))
    {
        return false;
    }
    if (!query.exec("CREATE TABLE IF NOT EXISTS users (id SERIAL PRIMARY KEY, username TEXT, password TEXT,"
                    " lastname TEXT, otchestvo TEXT, name TEXT, nodegroup TEXT, roles TEXT, phonenumber TEXT,"
                    " email TEXT, userdescription TEXT,passwordexpiration TEXT, loginattemptsaviable TEXT);"))
    {
        return false;
    }



    return true;
}
bool DbHelper::checkAutorisation(QString username, QString password)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM admins WHERE username=? AND password=?;");
    query.addBindValue(username);
    query.addBindValue(QString(QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5).toHex()));

    query.exec();
    if (query.size()<=0){return false;}

    return true;
}


bool DbHelper::createNewAdministrator(QString username, QString password, QString personalInfo, QString permissons)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM admins WHERE username=?;");
    query.addBindValue(username);


    query.exec();
    qDebug()<<query.size();
           if(query.size()>0){
               QMessageBox::warning(0,"Warning","Это имя пользователя уже используется");
               return false;

         }


    query.prepare("INSERT INTO admins (username,password,personal_info,permissions) VALUES (?,?,?,?);");
    query.addBindValue(username);
    query.addBindValue(QString(QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5).toHex()));
    query.addBindValue(personalInfo);
    query.addBindValue(permissons);
    if (!query.exec())
    {
        return false;
    }
    return true;

}
bool DbHelper::createNewUser(QString username, QString password, QString lastname, QString patronymic, QString name,
                             QString nodegroup, QString roles, QString phonenumber,
                             QString email, QString userdescription, QString passwordexpiration, QString loginattemptsaviable)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM users WHERE username=?;");
    query.addBindValue(username);


    query.exec();
    qDebug()<<query.size();
           if(query.size()>0){
               QMessageBox::warning(0,"Warning","Это имя пользователя уже используется");
               return false;

         }


    query.prepare("INSERT INTO users (username,password,lastname,"
                  "patronymic,name,nodegroup,roles,phonenumber,email,userdescripton, passwordexpiration"
                  "loginattemptsavible) VALUES (?,?,?,?,?,?,?,?,?,?,?,?);");
    query.addBindValue(username);
    query.addBindValue(QString(QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5).toHex()));
    query.addBindValue(lastname);
    query.addBindValue(patronymic);
    query.addBindValue(name);
    query.addBindValue(nodegroup);
    query.addBindValue(roles);
    query.addBindValue(phonenumber);
    query.addBindValue(email);
    query.addBindValue(userdescription);
    query.addBindValue(passwordexpiration);
    query.addBindValue(loginattemptsaviable);

    if (!query.exec())
    {
        return false;
    }
    return true;
}
User* DbHelper::getUserByUsername(QString username)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username=?;");
    query.addBindValue(username);

    query.exec();
    if (query.size()<=0){return nullptr;}
    if (query.size()>1){return nullptr;}


    return new User(query.value(0).toString(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString(),query.value(4).toString(),query.value(5).toString(),
                    query.value(6).toString(),query.value(7).toString(),query.value(8).toString(),query.value(9).toString(),query.value(10).toString(),query.value(11).toString());

}

QVector<User>* DbHelper::getAllUsers()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM users;");

    query.exec();
//if (query.size()<=0){return nullptr;}
    QVector<User>* _result = new QVector<User>();
    while (query.next())
    {
        _result->push_back({query.value(1).toString(),query.value(2).toString(),query.value(3).toString(),query.value(4).toString(),
                            query.value(5).toString(),query.value(6).toString(),
                            query.value(7).toString(),query.value(8).toString(),
                            query.value(9).toString(),query.value(10).toString(),query.value(11).toString(),query.value(12).toString()});
    }

    return _result;

}
