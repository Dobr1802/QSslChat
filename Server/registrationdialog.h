#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include "user.h"

namespace Ui {
class RegistrationDialog;
}

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = 0);
    ~RegistrationDialog();

signals:
    pushButtonAccept(QString name, QString pwd);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::RegistrationDialog *ui;
};

#endif // REGISTRATIONDIALOG_H
