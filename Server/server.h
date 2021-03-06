#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QFile>
#include "sslserver.h"
#include "user.h"
#include "worker.h"

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
  void connectionClosed();
  void connectionFailure();

private:
  void checkFileStatus();

private:
  QString key;
  QString certificate;
  SslServer server;
  Ui::Server* ui;
  Worker *worker;
};

#endif // SERVER_H
