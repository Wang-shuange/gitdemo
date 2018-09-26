/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <mycombobox.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QTextBrowser *textBrowser;
    QProgressBar *test_progressBar;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *result_label;
    QTableWidget *log_tableWidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QPushButton *start_pushButton;
    QLabel *label_3;
    MyComboBox *portNameComboBox;
    QLabel *label;
    QLabel *label_4;
    QLineEdit *virtual_lineEdit;
    QLineEdit *srv_port_lineEdit;
    QLineEdit *serverIP_lineEdit;
    QLineEdit *addr_lineEdit;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_2;
    QLineEdit *SN_lineEdit;
    QLabel *label_7;
    QLineEdit *resp_to_lineEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(821, 546);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        textBrowser = new QTextBrowser(groupBox_2);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        gridLayout_3->addWidget(textBrowser, 0, 0, 1, 1);

        test_progressBar = new QProgressBar(groupBox_2);
        test_progressBar->setObjectName(QString::fromUtf8("test_progressBar"));
        test_progressBar->setMaximumSize(QSize(16777215, 40));
        test_progressBar->setValue(24);

        gridLayout_3->addWidget(test_progressBar, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        result_label = new QLabel(groupBox_3);
        result_label->setObjectName(QString::fromUtf8("result_label"));
        result_label->setFrameShape(QFrame::Panel);
        result_label->setTextFormat(Qt::AutoText);
        result_label->setScaledContents(true);

        gridLayout_2->addWidget(result_label, 0, 0, 1, 1);

        log_tableWidget = new QTableWidget(groupBox_3);
        if (log_tableWidget->columnCount() < 7)
            log_tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setBackground(QColor(255, 0, 0));
        log_tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        log_tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        log_tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        log_tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        log_tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        log_tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        log_tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        log_tableWidget->setObjectName(QString::fromUtf8("log_tableWidget"));

        gridLayout_2->addWidget(log_tableWidget, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_3, 0, 1, 2, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        start_pushButton = new QPushButton(groupBox);
        start_pushButton->setObjectName(QString::fromUtf8("start_pushButton"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        start_pushButton->setFont(font);

        gridLayout_4->addWidget(start_pushButton, 6, 0, 1, 4);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 4, 0, 1, 1);

        portNameComboBox = new MyComboBox(groupBox);
        portNameComboBox->setObjectName(QString::fromUtf8("portNameComboBox"));

        gridLayout_4->addWidget(portNameComboBox, 0, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_4->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_4->addWidget(label_4, 3, 0, 1, 1);

        virtual_lineEdit = new QLineEdit(groupBox);
        virtual_lineEdit->setObjectName(QString::fromUtf8("virtual_lineEdit"));

        gridLayout_4->addWidget(virtual_lineEdit, 4, 3, 1, 1);

        srv_port_lineEdit = new QLineEdit(groupBox);
        srv_port_lineEdit->setObjectName(QString::fromUtf8("srv_port_lineEdit"));

        gridLayout_4->addWidget(srv_port_lineEdit, 3, 3, 1, 1);

        serverIP_lineEdit = new QLineEdit(groupBox);
        serverIP_lineEdit->setObjectName(QString::fromUtf8("serverIP_lineEdit"));

        gridLayout_4->addWidget(serverIP_lineEdit, 3, 1, 1, 1);

        addr_lineEdit = new QLineEdit(groupBox);
        addr_lineEdit->setObjectName(QString::fromUtf8("addr_lineEdit"));

        gridLayout_4->addWidget(addr_lineEdit, 4, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 3, 2, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 4, 2, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_4->addWidget(label_2, 0, 2, 1, 1);

        SN_lineEdit = new QLineEdit(groupBox);
        SN_lineEdit->setObjectName(QString::fromUtf8("SN_lineEdit"));

        gridLayout_4->addWidget(SN_lineEdit, 0, 3, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 5, 0, 1, 1);

        resp_to_lineEdit = new QLineEdit(groupBox);
        resp_to_lineEdit->setObjectName(QString::fromUtf8("resp_to_lineEdit"));

        gridLayout_4->addWidget(resp_to_lineEdit, 5, 1, 1, 3);

        label->raise();
        start_pushButton->raise();
        portNameComboBox->raise();
        addr_lineEdit->raise();
        label_3->raise();
        serverIP_lineEdit->raise();
        label_4->raise();
        srv_port_lineEdit->raise();
        label_5->raise();
        virtual_lineEdit->raise();
        label_6->raise();
        SN_lineEdit->raise();
        label_2->raise();
        label_7->raise();
        resp_to_lineEdit->raise();

        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 821, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\346\265\213\350\257\225LOG", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\346\265\213\350\257\225\347\273\223\346\236\234", 0, QApplication::UnicodeUTF8));
        result_label->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\347\273\223\346\236\234", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = log_tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "SN", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = log_tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Test Item", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = log_tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Spec", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = log_tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = log_tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Result", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = log_tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = log_tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "End", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\265\213\350\257\225\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        start_pushButton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\345\274\200\345\247\213\346\265\213\350\257\225\346\214\211\351\222\256</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        start_pushButton->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\345\234\260\345\235\200\357\274\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Port Name", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250IP:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        srv_port_lineEdit->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>\344\273\245\345\244\252\347\275\221\346\265\213\350\257\225\344\275\277\347\224\250\347\232\204\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\345\217\267</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        serverIP_lineEdit->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>\344\273\245\345\244\252\347\275\221\346\265\213\350\257\225\347\232\204\346\234\215\345\212\241\345\231\250IP\345\234\260\345\235\200\357\274\214\351\273\230\350\256\244\344\270\272\346\234\254\346\234\272IP\357\274\214\345\217\257\346\211\213\345\212\250\351\205\215\347\275\256</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        addr_lineEdit->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Hermes \347\224\250\344\272\216RS485 modbus\351\200\232\344\277\241\347\232\204\344\270\233\346\234\272\345\234\260\345\235\200\357\274\214\346\257\217\344\270\252\350\256\276\345\244\207\345\257\271\345\272\224\344\270\200\344\270\252</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_5->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "\350\231\232\346\213\237\344\270\262\345\217\243\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "\344\270\273\346\235\277SN\357\274\232", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        SN_lineEdit->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Hermes ID</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("MainWindow", "\345\223\215\345\272\224\350\266\205\346\227\266\350\256\276\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
