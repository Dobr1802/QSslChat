#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "userwrapper.h"

class Worker : public QObject
{
  Q_OBJECT

public:
  explicit Worker(QTextCodec codec, QObject *parent = 0) : _codec(codec), QObject(parent) {}

  void takeCommand(QSslSocket *s);
  void addUser(QString &name, QString &pwd, QSslSocket *s);
  bool isRegistred(QSslSocket *s);

private:
  QList<UserWrapper *> uWrappers;
  QTextCodec _codec;
};

#endif // WORKER_H
