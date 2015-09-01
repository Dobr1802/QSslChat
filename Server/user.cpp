#include "user.h"
#include <QHostAddress>

User::User(QSslSocket *s) : _socket(s)
{
  file = new QFile("log.txt");

  _name = QString("%1:%2").arg(s->peerAddress().toString())
                          .arg(s->peerPort());

  _process = new QProcess(this);

  connect(_process, SIGNAL(readyRead()), this, SLOT(readCmd()));

  _codec = QTextCodec::codecForName("IBM 866");
  _process->start("cmd /K");
  _process->readyRead();
}

void User::sendMessage()
{
  if (_socket->canReadLine())
  {
    QString msg = _socket->readAll();

    file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream out(file);
    out << _name << ":" << msg << "\n";
    file->close();

    _process->write(msg.toStdString().c_str());
    _process->readyRead();
  }
}

void User::readCmd()
{
  _textOfMsg.append(_codec->toUnicode(_process->readAllStandardOutput()));
  QTextStream os(_socket);
  os.setCodec(_codec);
  os << _textOfMsg;
  _textOfMsg.clear();
}
