#include "loginwindow.h"


LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    administration = new Administration();
    networkRequestsHandler = new NetworkRequestsHandler();
    networkRequestsHandler->checkConnection(this);
}

LoginWindow::~LoginWindow()
{

}



void LoginWindow::showErrorMessage(QString header, QString message)
{
    QMessageBox::warning(this,header,message);
}

void LoginWindow::showInfoMessage(QString header, QString message)
{
    QMessageBox::information(this,header,message);
}


void LoginWindow::notifyLoginSuccess()
{
    QMessageBox::information(this, "Добро пожаловать","Авторизация успешна");
    administration->setNetHandler(networkRequestsHandler);
    administration->show();
    this->close();

}

void LoginWindow::on_loginButton_clicked()
{
    networkRequestsHandler->checkAccess(usernameField->text(), passwordField->text(),this);
}

void LoginWindow::on_registerButton_clicked()
{

    //administration->show();
    //this->hide();
    stackedWidget->setCurrentIndex(1);
}



void LoginWindow::on_resumeAutorisation_clicked()
{
     stackedWidget->setCurrentIndex(0);
}


void LoginWindow::on_createAdminSubmit_clicked()
{
    networkRequestsHandler->createNewAdministrator(username_input->text(),password_input->text(),personal_info_input->text(), permissions_choser->currentText(),this);
}
