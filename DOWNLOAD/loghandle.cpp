#include "loghandle.h"
extern QStringList testLog;
logHandle::logHandle(QObject *parent) : QObject(parent)
{

}

logHandle::~logHandle()
{

}
//1.新建一个excel
void logHandle::newExcel(const QString &fileName)
{        HRESULT r = OleInitialize(0);
         CoInitialize(0);
              if (r != S_OK && r != S_FALSE) {
                  qWarning("Qt: Could not initialize OLE (error %x)\n", (unsigned int)r);
              }
              pApplication = new QAxObject("Excel.Application");
                   if(pApplication == NULL){
                       qWarning("pApplication\n");return;
                   }
                   pApplication->dynamicCall("SetVisible(bool)", false);//false不显示窗体
                        pApplication->setProperty("DisplayAlerts", false);//不显示任何警告信息。
                             pWorkBooks = pApplication->querySubObject("Workbooks");
                                  QFile file(fileName);
                                       if (file.exists())
                                       {
                                           pWorkBook = pWorkBooks->querySubObject("Open(const QString &)", fileName);
                                       }
                                       else
                                       {
                                           pWorkBooks->dynamicCall("Add");
                                           pWorkBook = pApplication->querySubObject("ActiveWorkBook");
                                       }
                                            pSheets = pWorkBook->querySubObject("Sheets");
                                                 pSheet = pSheets->querySubObject("Item(int)", 1);
}
//2.增加1个Worksheet
void logHandle::appendSheet(const QString &sheetName,int cnt)
{
    QAxObject *pLastSheet = pSheets->querySubObject("Item(int)", cnt);
    pSheets->querySubObject("Add(QVariant)", pLastSheet->asVariant());
    pSheet = pSheets->querySubObject("Item(int)", cnt);
    pLastSheet->dynamicCall("Move(QVariant)", pSheet->asVariant());
    pSheet->setProperty("Name", sheetName);
}
//3.向Excel单元格中写入数据
void logHandle::setCellValue(int row, int column, const QString &value)
{
    QAxObject *pRange = pSheet->querySubObject("Cells(int,int)", row, column);
    pRange->dynamicCall("Value", value);
}
//4.保存Excel
void logHandle::saveExcel(const QString &fileName)
{
    pWorkBook->dynamicCall("SaveAs(const QString &)",
                           QDir::toNativeSeparators(fileName));
}
//5.释放Excel
void  logHandle::freeExcel()
{
    if (pApplication != NULL)
    {
        pApplication->dynamicCall("Quit()");
        delete pApplication;
        pApplication = NULL;
    }
}
void logHandle::LogAdd(QString log)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd-hh-mm-ss").left(10); //设置显示格式并截取当前日期；
    qDebug()<<str;
    QString file_txt =  QDir::currentPath()+"/"+str+"_total_log.txt"; //文件名可从此处修改
    QFile *file;
    file=new QFile(file_txt);
    if(!file->open(QIODevice::ReadWrite | QIODevice::Text|QIODevice::Append)) //以文本文式写入
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QTextStream out(file);
    out << log << endl;

}
void logHandle::addTestLog(QString sn,QString item,QString spec,QString value,QString res,QString start,QString end)
{
    OleInitialize(0);
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd-hh-mm-ss").left(10); //设置显示格式并截取当前日期；
    qDebug()<<str;
    QString fileNametmp =  QDir::currentPath()+"/"+str+"_test_log.xlsx"; //文件名可从此处修改
    QDir::toNativeSeparators(fileNametmp);
    newExcel(fileNametmp);
    setCellValue(1, 1,"SN");
    setCellValue(1, 2,"TestItem");
    setCellValue(1, 3,"Spec");
    setCellValue(1, 4,"Value");
    setCellValue(1, 5,"Result");
    setCellValue(1, 6,"Start");
    setCellValue(1, 7,"End");
    QAxObject * usedrange = pSheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = usedrange->querySubObject("Rows");
    int intRows = rows->property("Count").toInt();
    qDebug()<<"total_row"<<intRows;
    int last_row=intRows;
    qDebug()<<"last row:"<<last_row;
    setCellValue(last_row+1, 1,sn);
    setCellValue(last_row+1, 2,item);
    setCellValue(last_row+1, 3,spec);
    setCellValue(last_row+1, 4,value);
    setCellValue(last_row+1, 5,res);
    setCellValue(last_row+1, 6,start);
    setCellValue(last_row+1, 7,end);
   saveExcel(fileNametmp);
    freeExcel();
    return;
}
