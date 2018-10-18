#ifndef CONFIGHANDLE_H
#define CONFIGHANDLE_H

#include <QObject>

class configHandle : public QObject
{
    Q_OBJECT
public:
    explicit configHandle(QObject *parent = 0);
    ~configHandle();

signals:

public slots:
    void add_xmlnode(QString filename, QString rmt_name, QString ipa, QString ipb);
    void read_xml(QString filename);
    void create_xml(QString filename);
    void do_xml(const QString opt, QString filename);
    void read();
};

#endif // CONFIGHANDLE_H
