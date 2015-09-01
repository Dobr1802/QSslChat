#include <QApplication>
#include <QTextCodec>
#include "server.h"

int main(int argc, char *argv[])
{
    QTextCodec *_codec = QTextCodec::codecForName("IBM 866");
    QApplication a(argc, argv);
    Server w;
    w.show();

    return a.exec();
}
