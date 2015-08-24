#ifndef SERVER_H
#define SERVER_H

#include <QList>
#include <QSslSocket>
#include <QMainWindow>
#include <QFile>
#include "sslserver.h"
#include "registrationdialog.h"
#include "user.h"

namespace Ui {
  class Server;
}

class Server : public QMainWindow
{
  Q_OBJECT

public:
  explicit Server(QWidget *parent = 0);
  ~Server();

protected slots:
  // Slots to receive signals from UI
  void startStopButtonClicked();
  void keyButtonClicked();
  void certificateButtonClicked();
  void keyFileChanged(const QString &filename);
  void certificateFileChanged(const QString &filename);

  // Slots to receive signals from sockets
  void acceptConnection();
  void handshakeComplete();
  void sslErrors(const QList<QSslError> &errors);
  void receiveMessage();
  void connectionClosed();
  void connectionFailure();

private slots:
  void on_regNewUsrButton_clicked();
  void addUser(QString name, QString pwd);

private:
  void checkFileStatus();
  void checkUsersFile();

private:
  QString key;
  QString certificate;
  SslServer server;
  QList<QSslSocket *> sockets;
  QList<User *> users;
  Ui::Server* ui;
  RegistrationDialog *regDialog;
  QFile* usersFile;
};

#endif // SERVER_H
