#include <QDateTime>
#include <QMessageBox>
#include "client.h"
#include "ui_client.h"
#include <QFileDialog>

Client::Client(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Client)
{
  ui->setupUi(this);

  // Check for SSL support.  If SSL support is not available, show a
  // message to the user describing what to do to enable SSL support.
  if (QSslSocket::supportsSsl())
  {
    ui->connectDisconnectButton->setEnabled(true);
  }
  else
  {
    QString noSslMsg = QString("%1\n%2")
        .arg("*** Your version of Qt does support SSL ***")
        .arg("You must obtain a version of Qt that has SSL"
             "support enabled.  If you believe that your "
             "version of Qt has SSL support enabeld, you may "
             "need to install the OpenSSL run-time libraries.");

    ui->chatDisplayTextEdit->setText(noSslMsg);
  }

  // QSslSocket emits the encrypted() signal after the encrypted connection is established
  connect(&socket, SIGNAL(encrypted()), this, SLOT(connectedToServer()));

  // Report any SSL errors that occur
  connect(&socket, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));

  connect(&socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
  connect(&socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
  connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
}

Client::~Client()
{
  if (socket.isOpen())
  {
    socket.close();
  }

  delete ui;
}

void Client::connectDisconnectButtonPressed()
{
  ui->connectDisconnectButton->setEnabled(false);

  if (socket.state() == QAbstractSocket::UnconnectedState)
  {
    // Initiate an SSL connection to the chat server.
    socket.setPeerVerifyMode(QSslSocket::VerifyPeer);
    socket.connectToHostEncrypted(ui->hostnameLineEdit->text(), ui->portSpinBox->value());
  }
  else
  {
    socket.close();
  }
}

void Client::sendButtonPressed()
{
  QString message = ui->inputLineEdit->text();
  if (!message.isEmpty())
  {
    message += '\n';
    socket.write(message.toLocal8Bit().constData());
    ui->inputLineEdit->clear();
  }
}

void Client::connectedToServer()
{
  ui->connectDisconnectButton->setText("Disconnect");
  ui->connectDisconnectButton->setEnabled(true);
  ui->inputLineEdit->setEnabled(true);
  ui->sendButton->setEnabled(true);
  ui->chatDisplayTextEdit->clear();
}

// Process SSL errors
void Client::sslErrors(const QList<QSslError> &errors)
{
  QString errorStrings;
  foreach (QSslError error, errors)
  {
    errorStrings += error.errorString();
    if (error != errors.last())
    {
      errorStrings += '\n';
    }
  }

  // Display error details to user and ask for permission to proceed anyway
  QMessageBox::StandardButton result = QMessageBox::question(this, "SSL Errors",
    QString("The following errors were encountered while negotiating the SSL connection:\n\n%1\n\nProceed anyway?").arg(errorStrings),
    QMessageBox::Yes|QMessageBox::No);
  if (result == QMessageBox::Yes)
  {
    socket.ignoreSslErrors();
  }
}

void Client::receiveMessage()
{
  if (socket.canReadLine())
  {
    ui->chatDisplayTextEdit->append(socket.readAll().constData());
  }
}

void Client::connectionClosed()
{
  ui->connectDisconnectButton->setText("Connect");
  ui->connectDisconnectButton->setEnabled(true);
  ui->inputLineEdit->setEnabled(false);
  ui->sendButton->setEnabled(false);
}

void Client::socketError()
{
  ui->chatDisplayTextEdit->setText(QString("Socket Error: %1").arg(socket.errorString()));
  if (socket.state() != QAbstractSocket::ConnectedState)
  {
    connectionClosed();
  }
  socket.close();
}
