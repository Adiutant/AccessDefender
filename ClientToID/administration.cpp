#include "administration.h"


Administration::Administration(QWidget *parent) :
    QWidget(parent)
{

    setupUi(this);

}

Administration::~Administration()
{

}
void Administration::setNetHandler(NetworkRequestsHandler *netHandler)
{

    networkRequestsHandler = netHandler;
}
void Administration::fillArmsTable()
{
    armList->clear();
     for (auto item :networkRequestsHandler->getArmTable())
     {
        armList->addItem(new QListWidgetItem(QString::fromStdString("ARM") + item));
     }

}
void Administration::fillUsersTable()
{
    std::vector<User> users = networkRequestsHandler->getUsersTable();
    usersTable->setRowCount(users.size());
    for (int i =0 ;i< users.size();i++)
    {
        QVector<QString>features =users[i].getFeatures();
        for (int j=0 ;j <features.size();j++)
        {
          usersTable->setItem(i,j,new QTableWidgetItem(features[j]));
        }
    }
    if (usersTable->rowCount() == 0)
    {
        usersTable->insertRow(usersTable->rowCount());
    }
}
void Administration::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->text(0)=="Просмотр событий")
    {
        stackedWidget->setCurrentIndex(0);
        fillArmsTable();
    }
    else if(item->text(0)=="Управление пользователями")
    {
        stackedWidget->setCurrentIndex(1);
        fillUsersTable();

        //QMessageBox::critical(this,)
    }
}

void Administration::on_armList_itemClicked(QListWidgetItem *item)
{
    int agentCode  = item->text()[item->text().length()-1].digitValue();
    logsField->setText(networkRequestsHandler->getLogs(agentCode));
}

void Administration::on_addRowButton_clicked()
{
    usersTable->insertRow(usersTable->rowCount());
}

void Administration::on_saveTable_clicked()
{
    QVector<User> users{};
    for(int i = 0; i < usersTable->rowCount();i++)
    {
        QVector<QString> data{};
        for (int j=0; j< usersTable->columnCount();j++)
        {
            QTableWidgetItem *item = usersTable->item(i,j);
            data.push_back(item == nullptr ? tr(""): item->text() );

        }
        users.push_back(User{data});
    }
    networkRequestsHandler->updateUsersTable(users,this);

}
void Administration::showErrorMessage(QString header, QString message)
{
    QMessageBox::warning(this,header,message);
}

void Administration::showInfoMessage(QString header, QString message)
{
    QMessageBox::information(this,header,message);
}
void Administration::on_deleteRowButton_clicked()
{
    if (usersTable->focusWidget()== nullptr) return;
    usersTable->removeRow(usersTable->focusWidget()->property("row").toInt());
}
