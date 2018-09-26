#include "mainwindow.h"
#include <QApplication>
#include<QTextCodec>
#include<QAxObject>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    MainWindow w;
    w.setWindowTitle("Hermes 通信测试");
    a.setWindowIcon(QIcon("comm.ico"));
    w.show();
    return a.exec();
}
