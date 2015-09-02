#include "user.h"
#include <QHostAddress>

User::User(QSslSocket *s) : _socket(s)
{
  _file = new QFile("log.txt");

  _name = QString("%1:%2").arg(s->peerAddress().toString())
                          .arg(s->peerPort());

  _process = new QProcess(this);

  connect(_process, SIGNAL(readyRead()), this, SLOT(readCmd()));
  connect(_process, SIGNAL(disconnect()), this, SLOT(deleteLater()));

  _process->start("cmd /K");
  _process->readyRead();
}

User::~User()
{
  delete _process;
  delete _file;
}

void User::sendMessage()
{
  if (_socket->canReadLine())
  {
    QString msg = _socket->readAll();

    _file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream out(_file);
    out << _name << ":" << msg << "\n";
    _file->close();

    _process->write(msg.toStdString().c_str());
    _process->readyRead();
  }
}

void User::readCmd()
{
  QByteArray out = _process->readAllStandardOutput();
  _socket->write(QTextCodec::codecForName("CP866")->toUnicode(out).toUtf8());
}
