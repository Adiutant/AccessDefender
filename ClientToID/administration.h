#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <QWidget>
#include "ui_administration.h"
#include <QMessageBox>
#include "networkrequestshandler.h"

namespace Ui {
class Administration;
}
class NetworkRequestsHandler;

class Administration : public QWidget, public Ui_Administration
{
    Q_OBJECT

public:
    explicit Administration(QWidget *parent = 0 );
    ~Administration();
    void setNetHandler(NetworkRequestsHandler *netHandler);

public slots:
    void showErrorMessage(QString header, QString message);
    void showInfoMessage(QString header, QString message);

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_armList_itemClicked(QListWidgetItem *item);

    void on_addRowButton_clicked();

    void on_saveTable_clicked();

    void on_deleteRowButton_clicked();


private:
    Ui::Administration *ui;
    void fillArmsTable();
    void fillUsersTable();
    NetworkRequestsHandler *networkRequestsHandler;
};

#endif // ADMINISTRATION_H
