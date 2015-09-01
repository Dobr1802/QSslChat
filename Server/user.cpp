#include "user.h"
#include <QHostAddress>

User::User(QSslSocket *s) : _socket(s)
{
  _name = QString("%1:%2").arg(s->peerAddress().toString())
                          .arg(s->peerPort());

  _process = new QProcess(this);

//  connect(_process, SIGNAL(readyRead()), this, SLOT(readCmd()));

  _codec = QTextCodec::codecForName("IBM 866");
  _process->start("cmd /K");
  _process->waitForReadyRead(-1);
  _textOfMsg.append(_codec->toUnicode(_process->readAll()));
}

void User::sendMessage()
{
  if (_socket->canReadLine())
  {
    QString msg = _socket->readAll();

    _process->write(msg.toStdString().c_str());
    _process->waitForReadyRead(-1);
    _textOfMsg.append(_codec->toUnicode(_process->readAll()));
    _process->waitForReadyRead(-1);
    _textOfMsg.append(_codec->toUnicode(_process->readAll()));
    QTextStream os(_socket);
    os.setCodec(_codec);
    os << _textOfMsg;
    _textOfMsg.clear();
  }
}

//void User::readCmd()
//{
//  _textOfMsg.append(_codec->toUnicode(_process->readAllStandardOutput()));
//}
