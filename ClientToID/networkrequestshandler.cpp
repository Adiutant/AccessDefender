#include "networkrequestshandler.h"
using namespace std;

NetworkRequestsHandler::NetworkRequestsHandler(QObject *parent)
{
    //netManager = new QNetworkAccessManager(parent);

    //manager->get(request);



}
NetworkRequestsHandler::~NetworkRequestsHandler()
{
    //free(netManager);
    //free(request);
}
void NetworkRequestsHandler::makeNetRequest(QString request,QJsonObject &obj)
{
    obj["request"] = request;
    obj["access_token"] = _accessToken;


}
void NetworkRequestsHandler::checkConnection(LoginWindow* window){
    QTcpSocket jSocket;
    QJsonObject obj;
    makeNetRequest(NEW_CLIENT_SIGNATURE,obj);
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    jSocket.connectToHost("127.0.0.1",1337);
    if (jSocket.waitForConnected(2000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(2000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == NEW_CLIENT_SIGNATURE_ACCEPTED)
        {
             window->showInfoMessage("Успех", "Соединение установлено");
        }
        else
        {
             window->showErrorMessage("Ошибка", "Ошибка данных");
        }

    }
    else
    {
         window->showErrorMessage("Ошибка", "Ошибка соединения");
    }
    jSocket.close();

}
QString NetworkRequestsHandler::getLogs(int agentCode)
{
    QTcpSocket jSocket;
    QJsonObject obj;
    makeNetRequest(GET_ARM_LOGS_SIGNATURE, obj);
    obj["agent_code"] = agentCode;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    jSocket.connectToHost("127.0.0.1",1337);
    if (jSocket.waitForConnected(2000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(2000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == GET_ARM_LOGS_SUCCESS)
        {
            return obj["raw_logs"].toString();
        }
        else
        {
            return tr("");
        }

    }
    else
    {
        return tr("");
    }
    jSocket.close();
}
void NetworkRequestsHandler::checkAccess(QString username, QString password , LoginWindow* window){
    QTcpSocket jSocket;
    QJsonObject obj;
    makeNetRequest(LOGIN_CLIENT_SIGNATURE,obj);
    obj["username"] = username;
    obj["password"] = password;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    jSocket.connectToHost("127.0.0.1",1337);
    if (jSocket.waitForConnected(2000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(2000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == LOGIN_CLIENT_SIGNATURE_SUCCESS)
        {
            _accessToken = obj["access_token"].toString();
             window->notifyLoginSuccess();
        }
        else
        {
             window->showInfoMessage("Ошибка", "Ошибка входа");
        }

    }
    else
    {
         window->showErrorMessage("Ошибка", "Ошибка соединения");
    }
    jSocket.close();
}
void NetworkRequestsHandler::createNewAdministrator(QString username, QString password, QString personal_info, QString permissions, LoginWindow* window)
{
    QTcpSocket jSocket;
    QJsonObject obj;
    makeNetRequest(CREATE_NEW_ADMINISTRATOR_SIGNATURE,obj);
    obj["username"] = username;
    obj["password"] = password;
    obj["personal_info"] = personal_info;
    obj["permissions"] = permissions;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    jSocket.connectToHost("127.0.0.1",1337);
    if (jSocket.waitForConnected(2000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(2000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == CREATE_NEW_ADMINISTRATOR_SUCCESS)
        {

            window->showInfoMessage("Успех", "Администратор создан вернитесь к этапу входа");
        }
        else
        {
          window->showInfoMessage("Ошибка", "Ошибка создания администратора");
        }

    }
    else
    {
        //window->notifyConnectionDown();
    }
    jSocket.close();
}
vector<QString> NetworkRequestsHandler::getArmTable()
{
    QTcpSocket jSocket;
    QJsonObject obj;
    makeNetRequest(GET_ARM_TABLE_SIGNATUTE,obj);
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    jSocket.connectToHost("127.0.0.1",1337);
    vector<QString> result{};
    if (jSocket.waitForConnected(2000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(2000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == GET_ARM_TABLE_SUCCESS)
        {
            QJsonArray armList =  obj["arm_list"].toArray();
            qDebug() << armList;

            for (auto item : armList)
            {
                result.push_back(QString::number(item.toInt()));
            }

        }

    }
    return result;
    jSocket.close();
}
vector<User> NetworkRequestsHandler::getUsersTable()
{
    QTcpSocket jSocket;
    QJsonObject obj;
    makeNetRequest(GET_USERS_TABLE_SIGNATUTE,obj);
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    jSocket.connectToHost("127.0.0.1",1337);
    vector<User> users{};
    if (jSocket.waitForConnected(2000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(2000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == GET_USERS_TABLE_SUCCESS)
        {
            QJsonArray userList =  obj["user_list"].toArray();
            qDebug() << userList;

            for (auto user : userList)
            {
                QJsonArray features = user.toArray();
                users.push_back({features[0].toString() ,features[1].toString(),features[2].toString(),features[3].toString(),features[4].toString(),features[5].toString(),
                                 features[6].toString(),features[7].toString(),features[8].toString(),features[9].toString(),features[10].toString(),features[11].toString()});

            }
            return users;

        }
        else
        {
            return users;
        }

    }
    else
    {
        //window->notifyConnectionDown();
        return users;
    }
    jSocket.close();
}
void NetworkRequestsHandler::updateUsersTable(QVector<User> users, Administration* window){

    QTcpSocket jSocket;
    QJsonObject obj;
    makeNetRequest(UPDATE_USERS_TABLE_SIGNATURE,obj);
    QJsonArray usersArray;
    for (auto user : users)
        usersArray.append(user.toJson());
    obj["users"] = usersArray;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    jSocket.connectToHost("127.0.0.1",1337);
    if (jSocket.waitForConnected(2000))
    {
    jSocket.write(data);
    }
    if (jSocket.waitForReadyRead(2000))
    {
        doc = QJsonDocument::fromJson(jSocket.readAll());
        obj = doc.object();
        if (obj["response"] == GET_USERS_TABLE_SUCCESS)
        {
            window->showInfoMessage("Успех", "Таблица пользователей отправлена на сервер.");
            return;
        }


    }
    window->showErrorMessage("Ошибка", "Ошибка отправки таблицы");

}

