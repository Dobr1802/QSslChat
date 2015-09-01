#include "worker.h"

void Worker::addUser(QSslSocket *s)
{
  User *user = new User(s);
  connect(s, SIGNAL(readyRead()), user, SLOT(sendMessage()));
  users.push_back(user);
}
