#include "agent.h"

Agent::Agent()
{

}
Agent::~Agent(){}
Agent::Agent(int agentCode, QDateTime lastActivity)
{
    _agentCode = agentCode;
    _lastActivity = lastActivity;
}
void Agent::setLogs(QByteArray logs)
{
    _rawLog = logs;
}
QByteArray Agent::getRawLog()
{
    return _rawLog;
}
bool Agent::checkActual()
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch() - _lastActivity.toMSecsSinceEpoch()<10000;
}
int Agent::getAgentCode()
{
    return _agentCode;
}
void Agent::actualize()
{
    _lastActivity = QDateTime::currentDateTime();
}
