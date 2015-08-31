#ifndef USERWRAPPER_H
#define USERWRAPPER_H

#include <QThread>
#include "user.h"

class UserWrapper
{
  int _usersNum = 0;
  QHash<QSslSocket *, User *> _users;
  QThread _thread;
  QTextCodec _codec;

public:
  UserWrapper(QTextCodec *codec);

  bool addUser(QString &name, QString &pwd, QSslSocket *s);
  void takeCommand(QSslSocket *s);
  bool isRegistred(QSslSocket *s);

private:
  const User *getUser(QSslSocket *s);
};

#endif // USERWRAPPER_H
