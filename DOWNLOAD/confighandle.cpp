#include "confighandle.h"
#include<QDebug>
#include<QFile>
#include<QDomDocument>
#include<QSettings>
configHandle::configHandle(QObject *parent) : QObject(parent)
{

}

configHandle::~configHandle()
{

}

void configHandle::read()
{
    QSettings *configIniRead = new QSettings("station.ini", QSettings::IniFormat);
       //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
       QString ipResult = configIniRead->value("/ip/second").toString();
       QString portResult = configIniRead->value("/port/open").toString();
       //打印得到的结果
       qDebug() << ipResult;
       qDebug() << portResult;
       //读入入完成后删除指针
       delete configIniRead;

}

void configHandle::add_xmlnode(QString filename,QString rmt_name, QString ipa, QString ipb)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug()<<"open for add error..." ;
    }
    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        qDebug()<<"add setcontent error..." ;
        file.close();
    }
    //QDomNode node = doc.firstChild();
    file.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
    {
        root = doc.createElement("ipconfig");
    }
    QDomElement element_root = doc.createElement(tr("remote"));
    QDomAttr attr_id = doc.createAttribute(tr("id"));
    QDomElement element_rmt = doc.createElement(tr("rmt_name"));
    QDomElement element_ipa = doc.createElement(tr("ipa"));
    QDomElement element_ipb = doc.createElement(tr("ipb"));
    QString str_id;
    if(root.lastChild().isNull())
    {
        str_id = "1";
        attr_id.setValue(str_id);
    }
    else
    {
        str_id = root.lastChild().toElement().attribute(tr("id"));
        int count = str_id.toInt()+1;
        attr_id.setValue(QString::number(count));
    }
    QDomText text;
    text =doc.createTextNode(rmt_name);
    element_rmt.appendChild(text);
    text = doc.createTextNode(ipa);
    element_ipa.appendChild(text);
    text = doc.createTextNode(ipb);
    element_ipb.appendChild(text);
    text.clear();
    element_root.appendChild(element_rmt);
    element_root.appendChild(element_ipa);
    element_root.appendChild(element_ipb);
    element_root.setAttributeNode(attr_id);
    root.appendChild(element_root);

    if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
        qDebug() << "open for add error!";
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
}

void configHandle::read_xml(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug()<<"open for read error..." ;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        qDebug()<<"setcontent error..." ;
        file.close();
    }
    file.close();
    QDomElement root = doc.documentElement();
    if (root.tagName() != "ipconfig") {
        qDebug()<<"root.tagname != ipconfig..." ;
    }
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            qDebug() << qPrintable(element.tagName())<<qPrintable(element.attribute("id"));
            QDomNodeList list = element.childNodes();
            for(int i = 0;i < list.count();i++)
            {
                QDomNode nodechild = list.at(i);
                if(nodechild.isElement())
                {
                    qDebug() << "    " << qPrintable(nodechild.toElement().tagName()) << qPrintable(nodechild.toElement().text());
                }
            }
        }
        node = node.nextSibling();
    }
}

void configHandle::create_xml(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadWrite);
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"GB2312\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("ipconfig");
    doc.appendChild(root);
    QDomText text = doc.createTextNode("");
    root.appendChild(text);
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
}
void configHandle::do_xml(const QString opt,QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open for do erro";
        file.close();
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        qDebug() << "setcontent for do error";
        file.close();
    }
    file.close();
    QDomNodeList lists = doc.elementsByTagName("remote");
    QDomElement ele = lists.at(1).toElement();
    if(ele.attribute(tr("id")) == "3")
    {
        if("delete" == opt || "update" == opt)
        {
            QDomElement root = doc.documentElement();
            if("delete" == opt)
            {
                root.removeChild(lists.at(1));
                qDebug() << "remove ok !";
            }
            else
            {
                QDomNodeList child=lists.at(1).childNodes();
                child.at(0).toElement().firstChild().setNodeValue("namechanged");
                child.at(1).toElement().firstChild().setNodeValue("ipachanged");
                child.at(2).toElement().firstChild().setNodeValue("ipbchanged");
                qDebug() << "modify ok !";
            }
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "open for remove error!";
            }
            QTextStream out(&file);
            doc.save(out,4);
            file.close();

        }
    }
}
