#ifndef USER_H
#define USER_H

#include <QString>
#include <QProcess>
#include <QSslSocket>
#include <QTextCodec>

using namespace std;

class User : public QObject
{
  Q_OBJECT

  QString _name, _textOfMsg;
  QSslSocket *_socket;
  QProcess *_process;
  QTextCodec *_codec;

public:
  User(QSslSocket *s);

public slots:
  void sendMessage();
//  void readCmd();
};

#endif // USER_H
