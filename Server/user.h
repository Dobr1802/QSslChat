#ifndef USER_H
#define USER_H

#include <QString>
#include <QProcess>
#include <QSslSocket>

using namespace std;

class User
{
  QString _name, _passwd, _textOfMsg = "";
  QSslSocket *_socket;
  QProcess *_process;

public:
  User(QString &name, QString &pwd, QSslSocket *s);

  const QString &name() const;
  const QString &passwd() const;
  const QSslSocket *socket() const;

  void takeMsg(QString &msg);
};

#endif // USER_H
