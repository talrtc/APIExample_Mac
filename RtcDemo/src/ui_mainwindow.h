/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *TabWiget;
    QWidget *login;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_9;
    QWidget *widget_2;
    QTextEdit *roomTx;
    QLabel *label_2;
    QPushButton *leaveRoomBtn;
    QTextEdit *appIDText;
    QLabel *label_23;
    QLabel *label;
    QTextEdit *user;
    QPushButton *initBtn;
    QPushButton *loginRoomBtn;
    QWidget *User;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(950, 700);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        TabWiget = new QTabWidget(centralwidget);
        TabWiget->setObjectName(QString::fromUtf8("TabWiget"));
        TabWiget->setStyleSheet(QString::fromUtf8("QTabWidget::tab-bar { alignment: center; }"));
        login = new QWidget();
        login->setObjectName(QString::fromUtf8("login"));
        horizontalLayout_2 = new QHBoxLayout(login);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_3 = new QWidget(login);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_9 = new QHBoxLayout(widget_3);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        widget_2 = new QWidget(widget_3);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(500, 0));
        widget_2->setMaximumSize(QSize(500, 16777215));
        roomTx = new QTextEdit(widget_2);
        roomTx->setObjectName(QString::fromUtf8("roomTx"));
        roomTx->setGeometry(QRect(90, 180, 337, 30));
        roomTx->setMaximumSize(QSize(16777215, 30));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(24, 230, 30, 21));
        leaveRoomBtn = new QPushButton(widget_2);
        leaveRoomBtn->setObjectName(QString::fromUtf8("leaveRoomBtn"));
        leaveRoomBtn->setGeometry(QRect(90, 410, 337, 23));
        appIDText = new QTextEdit(widget_2);
        appIDText->setObjectName(QString::fromUtf8("appIDText"));
        appIDText->setGeometry(QRect(90, 280, 337, 30));
        appIDText->setMaximumSize(QSize(16777215, 30));
        label_23 = new QLabel(widget_2);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(24, 290, 36, 21));
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(24, 180, 30, 21));
        user = new QTextEdit(widget_2);
        user->setObjectName(QString::fromUtf8("user"));
        user->setGeometry(QRect(90, 230, 337, 30));
        user->setMaximumSize(QSize(16777215, 30));
        initBtn = new QPushButton(widget_2);
        initBtn->setObjectName(QString::fromUtf8("initBtn"));
        initBtn->setGeometry(QRect(90, 330, 337, 23));
        loginRoomBtn = new QPushButton(widget_2);
        loginRoomBtn->setObjectName(QString::fromUtf8("loginRoomBtn"));
        loginRoomBtn->setGeometry(QRect(90, 370, 337, 23));

        horizontalLayout_9->addWidget(widget_2);


        horizontalLayout_2->addWidget(widget_3);

        TabWiget->addTab(login, QString());
        User = new QWidget();
        User->setObjectName(QString::fromUtf8("User"));
        verticalLayout_2 = new QVBoxLayout(User);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(User);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 942, 673));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));

        gridLayout->addLayout(gridLayout_2, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);

        TabWiget->addTab(User, QString());

        verticalLayout->addWidget(TabWiget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        TabWiget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        roomTx->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.SF NS Text'; font-size:13pt;\">123456789</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "user:", nullptr));
        leaveRoomBtn->setText(QApplication::translate("MainWindow", "\347\246\273\345\274\200\346\210\277\351\227\264", nullptr));
        appIDText->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">d46144</p></body></html>", nullptr));
        label_23->setText(QApplication::translate("MainWindow", "appID:", nullptr));
        label->setText(QApplication::translate("MainWindow", "room:", nullptr));
        initBtn->setText(QApplication::translate("MainWindow", "\345\210\235\345\247\213\345\214\226", nullptr));
        loginRoomBtn->setText(QApplication::translate("MainWindow", "\345\212\240\345\205\245\346\210\277\351\227\264", nullptr));
        TabWiget->setTabText(TabWiget->indexOf(login), QApplication::translate("MainWindow", "login", nullptr));
        TabWiget->setTabText(TabWiget->indexOf(User), QApplication::translate("MainWindow", "User", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
