#include "worker.h"

void Worker::takeCommand(QSslSocket *s)
{
  if (s->canReadLine())
  {
    foreach (UserWrapper *uw, uWrappers)
    {
      uw->takeCommand(QSslSocket *s);
    }
  }
}

void Worker::addUser(QString &name, QString &pwd, QSslSocket *s)
{
  foreach (UserWrapper *uw, uWrappers)
  {
    if (uw->addUser(name, pwd, s))
      return;
  }
  uWrappers.push_back(new UserWrapper(_codec));
  uWrappers.back()->addUser(name, pwd, s);
}

bool Worker::isRegistred(QSslSocket *s)
{
  foreach (UserWrapper *uw, uWrappers)
  {
    if (uw->isRegistred(s))
    {
      return true;
    }
  }
  return false;
}
