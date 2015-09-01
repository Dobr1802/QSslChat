#include "worker.h"

Worker::~Worker()
{
  foreach (User *usr, users)
  {
    delete usr;
  }
}

void Worker::addUser(QSslSocket *s)
{
  User *user = new User(s);
  connect(s, SIGNAL(readyRead()), user, SLOT(sendMessage()));
  users.push_back(user);
}
