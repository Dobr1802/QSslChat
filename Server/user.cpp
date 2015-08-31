#include "user.h"

User::User(QString &name, QString &pwd, QSslSocket *s) : _name(name), _passwd(pwd), _socket(s)
{
  _process = new QProcess(this);
  _process->start("cmd /K");
  _textOfMsg.append(codec->toUnicode(process->readAll()));
}

const QString &User::name() const
{
  return _name;
}

const QString &User::passwd() const
{
  return _passwd;
}

const QSslSocket *User::socket() const
{
  return _socket;
}

void User::takeMsg(QString &msg)
{
  if (msg.indexOf("=") != -1)
  {
    msg = msg.split("=")[1].split(" ")[0];
    msg.append("\n");
    process->write(msg.toStdString().c_str());
    process->waitForReadyRead(-1);
    _textOfMsg.append(codec->toUnicode(process->readAll()));
    process->waitForReadyRead(-1);
    _textOfMsg.append(codec->toUnicode(process->readAll()));
  }

  int j = 0;
  while ((j = _textOfMsg.indexOf("\n", j)) != -1) {
    _textOfMsg.replace(j, 2, "<br>");
    ++j;
  }
  _textOfMsg.remove("<DIR>");

  QTextStream os(socket);
  os.setCodec(codec);
  os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"UTF8\"\r\n"
        "\r\n"
        "<h4>" << _textOfMsg << "</h4>\r\n";
}

// Accept connection from server and initiate the SSL handshake
void Server::acceptConnection()
{
  QSslSocket *socket = dynamic_cast<QSslSocket *>(server.nextPendingConnection());
  assert(socket);

  // QSslSocket emits the encrypted() signal after the encrypted connection is established
  connect(socket, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));

  // Report any SSL errors that occur
  connect(socket, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));

  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailure()));

  socket->setPrivateKey(key);
  socket->setLocalCertificate(certificate);

  socket->setPeerVerifyMode(QSslSocket::AutoVerifyPeer);
  socket->startServerEncryption();
}

// Receive notification that the SSL handshake has completed successfully
void Server::handshakeComplete()
{
  QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
  assert(socket);

  connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(sendMessage()));

  tempSockets.push_back(socket);

//  Read messege and add user, or delete socket, if user do not known.
//  QString msg = socket->readLine();
//  QStringList nameWhithPwd = msg.split("::");
//  if (isKnownUser(msg))
//  {
//    usersWhithSockets.insert(socket, new User(nameWhithPwd[0], nameWhithPwd[1]));
//  }
//  else
//  {
//    socket->write(QString("Wrong name or password.").toLocal8Bit().constData());
//    socket->disconnect();
//    socket->deleteLater();
//    return;
//}

  ui->logTextEdit->append(QString("[%1] Accepted connection from %2:%3.")
                          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                          .arg(socket->peerAddress().toString())
                          .arg(socket->peerPort()));
}

void Server::sslErrors(const QList<QSslError> &errors)
{
  QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
  assert(socket);

  QString errorStrings;
  foreach (QSslError error, errors)
  {
    errorStrings += error.errorString();
    if (error != errors.last())
    {
      errorStrings += ';';
    }
  }

  ui->logTextEdit->append(QString("[%1] %2:%3 reported the following SSL errors: %4")
      .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                          .arg(socket->peerAddress().toString())
                          .arg(socket->peerPort())
                          .arg(errorStrings));
}

void Server::sendMessage()
{
  QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
  assert(socket);

  if (worker->isRegistred(socket))
  {
    worker->takeCommand(socket);
  }
  else
  {
    initRegistration();
  }



    QString sender = QString("%1::%2")
        .arg(socket->peerAddress().toString())
        .arg(socket->peerPort());

//    ui->logTextEdit->append(QString("[%1] %2: %3")
//        .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
//        .arg(sender)
//        .arg(textOfMsg));
}

void Server::connectionClosed()
{
  QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
  assert(socket);

  ui->logTextEdit->append(QString("[%1] Connection from %2:%3 closed: %4")
                          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                          .arg(socket->peerAddress().toString())
                          .arg(socket->peerPort())
                          .arg(socket->errorString()));
//  usersWhithSockets.remove(socket);
  sockets.removeOne(socket);
  socket->disconnect();
  socket->deleteLater();
}

void Server::connectionFailure()
{
  QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
  assert(socket);

  ui->logTextEdit->append(QString("[%1] Connection from %2:%3 failed: %4")
                          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                          .arg(socket->peerAddress().toString())
                          .arg(socket->peerPort())
                          .arg(socket->errorString()));
  sockets.removeOne(socket);
  socket->disconnect();
  socket->deleteLater();
}
