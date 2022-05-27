#include "httpserver.h"


HttpServer::HttpServer(QObject *parent):
 QTcpServer(parent)
{
        if (listen(QHostAddress::Any,1337))
        {
            qDebug() << "Server started";
        }
        else
        {
            qDebug() <<"server starting error" <<errorString();

        }
        dbHelper = new DbHelper();
        _agents = new QVector<Agent>();
        _users = new QVector<User>();
        _updatingTimer = new QTimer();
        _updatingTimer->setInterval(20000);
        connect(_updatingTimer,SIGNAL(timeout()),this,SLOT(actualizeAgents()));
        _updatingTimer->start();


        //test

}

HttpServer::~HttpServer()
{

}
template <typename T1,typename T2>
bool HttpServer::contains(T1 set,T2 elem)
{
    for (auto item : set)
    {
        if (item == elem )
            return true;
    }
    return false;

}
//QString HttpServer::parseContent(QString row)
//{
//    QRegExp reg("[^Content: ][A-Z_]+");
//    QString match;
//    int pos=0;
//    while ((pos = reg.indexIn(row,pos))!=-1)
//    {
//        match = reg.cap(0);
//        pos += reg.matchedLength();
//        qDebug()<<match;

//    }
//    return match;

//}
void HttpServer::actualizeAgents()
{
    for (int i=0;i<_agents->size();i++)
    {
        if (!_agents->operator [](i).checkActual())
            _agents->erase(_agents->begin() + i);

    }
}

AgentCode HttpServer::makeAgentCode(int* code = nullptr)
{
    QDateTime now = QDateTime::currentDateTime();
    if (code != nullptr)
    {
        _agents->push_back(Agent(*code,now));
        return {QString("Agent" + QString::number(_agents->operator [](_agents->size()-1).getAgentCode())),*code};
    }


    if (_agents->size()==0)
    {


        _agents->push_back(Agent(0,now));

        return {QString("Agent" + QString::number(0)) ,0};
    }
    else
    {
        _agents->push_back(Agent(_agents->operator [](_agents->size()-1).getAgentCode() + 1,now));
        return {QString("Agent" + QString::number(_agents->operator [](_agents->size()-1).getAgentCode())),_agents->operator [](_agents->size()-1).getAgentCode()} ;

    }
}

void HttpServer::onReadyRead()
{
    QJsonDocument responseDocument;
    QByteArray response ;

    QTcpSocket* handlingSocket = qobject_cast<QTcpSocket*>(sender());

    QString content;
    QByteArray incomingRequest = handlingSocket->readAll();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(incomingRequest,&error);
    qDebug()<<error.errorString() << error.offset <<error.error;
    QJsonObject root  = document.object();
    if (root["request"] == NEW_CLIENT_SIGNATURE)
    {
        QJsonObject responseObject;

        responseObject["response"] = NEW_CLIENT_SIGNATURE_ACCEPTED;

        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();

    }
    else if (root["request"] == LOGIN_CLIENT_SIGNATURE)
    {

        //implement db credentials checking and access token generating
        QJsonObject responseObject;
        QString username = root["username"].toString();
        QString password = root["password"].toString();
        if (dbHelper->checkAutorisation(username, password))
        {
             responseObject["response"] = LOGIN_CLIENT_SIGNATURE_SUCCESS;
             QString credentials = username + password;
             QString newToken = QString(QCryptographicHash::hash(credentials.toUtf8(),QCryptographicHash::Md5).toHex());
             responseObject["access_token"] = newToken;
             _accessTokens.push_back(newToken);
        }


        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();

    }
    else if (root["request"] == CREATE_NEW_ADMINISTRATOR_SIGNATURE)
    {
        QJsonObject responseObject;
        QString username = root["username"].toString();
        QString password = root["password"].toString();
        QString personalInfo = root["personal_info"].toString();
        QString permissions = root["permissions"].toString();
        if (dbHelper->createNewAdministrator(username,password,personalInfo,permissions))
        {
            responseObject["response"] = CREATE_NEW_ADMINISTRATOR_SUCCESS;

        }


        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();
    }
    else if (root["request"] == GET_ARM_TABLE_SIGNATUTE && contains(_accessTokens,root["access_token"].toString()))
    {
        QJsonObject responseObject;
        QJsonArray armList;
        responseObject["response"] = GET_ARM_TABLE_SUCCESS;
        for (int i = 0; i< _agents->size(); i++)
        {

            armList.push_back(_agents->operator [](i).getAgentCode());
        }
        responseObject["arm_list"] = armList;

        qDebug() << responseObject["arm_list"];
        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();

    }
    else if (root["request"] == ONLINE_STATUS_CODE)
    {
        QJsonObject responseObject;

        responseObject["response"] = ONLINE_ACCEPTED_STATUS_CODE;
        responseObject["agent_code"] = makeAgentCode().code;
        qDebug() << responseObject["agent_code"].toString();
        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();

    }
    else if(root["request"] == HEALTHY_STATUS_CODE)
    {
        int counter = 0;
        int agentCode= root["agent_code"].toInt();
        for (int i=0;i<_agents->size();i++)
        {
            if (_agents->operator [](i).getAgentCode() == agentCode)
            {
                _agents->operator [](i).actualize();
                counter++;
            }
        }
        if (counter == 0)
        {
            makeAgentCode(&agentCode);
        }

        QJsonObject responseObject;
        qDebug() <<"alive from " << QString::number(agentCode);
        responseObject["response"] = HEALTHY_ACCEPTED_CODE;
        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();

    }
    else if (root["request"] == PUSH_LOGS_SIGNATURE)
    {

        for (int i=0;i<_agents->size();i++)
        {
            if (_agents->operator [](i).getAgentCode() == root["agent_code"].toInt())
                _agents->operator [](i).setLogs(root["raw_logs"].toString().toUtf8());
        }
        QJsonObject responseObject;

        responseObject["response"] = LOGS_ACCEPTED_CODE;

        qDebug() << root["agent_code"];
        qDebug() << "send logs";
        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();
    }
    else if (root["request"] == GET_ARM_LOGS_SIGNATURE&& contains(_accessTokens,root["access_token"].toString()))
    {
        QByteArray requestedLog;

        for (int i=0;i<_agents->size();i++)
        {
            if (_agents->operator [](i).getAgentCode() == root["agent_code"].toInt())
                requestedLog = _agents->operator [](i).getRawLog();
        }
        QJsonObject responseObject;

        responseObject["response"] = GET_ARM_LOGS_SUCCESS;

        responseObject["raw_logs"] = QString(requestedLog);
        qDebug() << "get logs";
        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();
    }
    else if (root["request"] == GET_USERS_TABLE_SIGNATUTE && contains(_accessTokens,root["access_token"].toString()))
    {
        QJsonObject responseObject;
        QJsonArray userList;

        responseObject["response"] = GET_USERS_TABLE_SUCCESS;
        _users = dbHelper->getAllUsers();
        for (int i = 0; i< _users->size(); i++)
        {
            QJsonArray featureList;
            for (auto feature : _users->operator [](i).getFeatures())
            {
                featureList.push_back(feature);
            }
            userList.push_back(featureList);

        }
        responseObject["user_list"] =
                userList;

        qDebug() << "get users";
        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();
    }
    else if (root["request"] == UPDATE_USERS_TABLE_SIGNATURE && contains(_accessTokens,root["access_token"].toString()))
    {
        QJsonObject responseObject;
        QJsonArray userList;

        userList = root["users"].toArray();
        QVector<User> users{};
        for (auto item : userList)
            users.append(User::fromJson(item.toObject()));

        if (dbHelper->updateUsersTable(users))
            responseObject["response"] = GET_USERS_TABLE_SUCCESS;
        qDebug() << "update users";
        responseDocument.setObject(responseObject);
        response = responseDocument.toJson();
    }
//    else if (root["request"] == CREATE_NEW_USER)
//    {
//        QByteArray requestedLog;
//        QJsonObject responseObject;

//        responseObject["response"] = CREATE_USER_SUCCESS;


//        qDebug() << "get logs";
//        responseDocument.setObject(responseObject);
//        response = responseDocument.toJson();
//    }
    qDebug() << root["request"];

    handlingSocket->write(response);
    handlingSocket->disconnectFromHost();

}

void HttpServer::onDisconnect()
{
    QTcpSocket* handlingSocket = qobject_cast<QTcpSocket*>(sender());
    handlingSocket->close();
    handlingSocket->deleteLater();

}

void HttpServer::incomingConnection(qintptr handle)
{
    QTcpSocket* newSocket = new QTcpSocket();
    newSocket->setSocketDescriptor(handle);
    connect(newSocket, SIGNAL(readyRead()), this,SLOT(onReadyRead()));
    connect(newSocket,SIGNAL(disconnected()), this, SLOT(onDisconnect()));

}
