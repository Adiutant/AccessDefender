#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "QMessageBox"
#include <QtNetwork/QNetworkReply>

#include "administration.h"
#include "ui_loginwindow.h"
#include "networkrequestshandler.h"
#include <QString>

namespace Ui {
  class LoginWindow;
}
class Administration;

class NetworkRequestsHandler;

class LoginWindow : public QMainWindow, public Ui_LoginWindow
{
    Q_OBJECT
public :

    void notifyLoginSuccess();

public slots:
    void showErrorMessage(QString header, QString message);
    void showInfoMessage(QString header, QString message);

public:

    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:

    void on_loginButton_clicked();

    void on_registerButton_clicked();

    void on_resumeAutorisation_clicked();


    void on_createAdminSubmit_clicked();

private:

    NetworkRequestsHandler *networkRequestsHandler;
    Administration *administration;
    Ui::LoginWindow *ui;

};

#endif // LOGINWINDOW_H
