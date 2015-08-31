#include "registrationdialog.h"
#include "ui_registrationdialog.h"

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
    ui->userPwdLineEdit->setEchoMode(QLineEdit::Password);
}

RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

void RegistrationDialog::on_buttonBox_accepted()
{
    QString name = ui->userNameLineEdit->text(),
            pswd = ui->userPwdLineEdit->text();
    pushButtonAccept(name, pswd);
}
