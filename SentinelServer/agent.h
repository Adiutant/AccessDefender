#ifndef AGENT_H
#define AGENT_H
#include <QByteArray>
#include <QDateTime>
class Agent
{
public:
    Agent();
    Agent(int agentCode,QDateTime lastActivity);
    void setLogs(QByteArray logs);
    ~Agent();
    bool checkActual();
    int getAgentCode();
    QByteArray getRawLog();
    void actualize();

private:
    int _agentCode;
    QDateTime _lastActivity;
    QByteArray _rawLog;
};

#endif // AGENT_H
