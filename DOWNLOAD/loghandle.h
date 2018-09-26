#ifndef LOGHANDLE_H
#define LOGHANDLE_H

#include <QObject>
#include<QAxObject>
#include<QDir>
#include<windows.h>
#include<QDateTime>
#include<QDebug>
class logHandle : public QObject
{
    Q_OBJECT
public:
    explicit logHandle(QObject *parent = 0);
    ~logHandle();

signals:

public slots:
    void newExcel(const QString &fileName);
    void appendSheet(const QString &sheetName, int cnt);
    void setCellValue(int row, int column, const QString &value);
    void freeExcel();
    void saveExcel(const QString &fileName);
    void LogAdd(QString log);
    void addTestLog(QString sn, QString item, QString spec, QString value, QString res, QString start, QString end);
public:
    QAxObject *pApplication;
    QAxObject *pWorkBooks;
    QAxObject *pWorkBook;
    QAxObject * pSheets;
    QAxObject * pSheet;
};

#endif // LOGHANDLE_H
