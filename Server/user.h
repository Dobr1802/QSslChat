#ifndef USER_H
#define USER_H

#include <QString>
#include <QProcess>
#include <QSslSocket>
#include <QTextCodec>
#include <QFile>

using namespace std;

class User : public QObject
{
  Q_OBJECT

  QString _name;
  QSslSocket *_socket;
  QProcess *_process;
  QFile *_file;

public:
  User(QSslSocket *s);
  ~User();

public slots:
  void sendMessage();
  void readCmd();
};

#endif // USER_H
