#ifndef WORKER_H
#define WORKER_H

#include "user.h"

class Worker : public QObject
{
  Q_OBJECT

  QList<User *> users;
public:
  explicit Worker(QObject *parent = 0) : QObject(parent) {}

  void takeCommand(QSslSocket *s);
  void addUser(QSslSocket *s);
};

#endif // WORKER_H
