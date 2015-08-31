#include "userwrapper.h"

UserWrapper::UserWrapper(QTextCodec *codec) : _codec(codec) {}

bool UserWrapper::addUser(QString &name, QString &pwd, QSslSocket *s)
{
  if (_usersNum < 2)
  {
    _users.insert(s, new User(name, pwd, s));
    _usersNum++;
    return true;
  }
  return false;
}

void UserWrapper::takeCommand(QSslSocket *s)
{
  if (User *user = getUser != NULL)
    user->takeMsg(s->readLine());
}

bool UserWrapper::isRegistred(QSslSocket *s)
{
  if (getUser(s))
    return true;
  return false;
}

const User *UserWrapper::getUser(QSslSocket *s)
{
  if (_users.contains(s))
    return _users.value(s);
  return NULL;
}

