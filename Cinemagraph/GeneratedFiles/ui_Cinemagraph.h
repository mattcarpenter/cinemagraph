/********************************************************************************
** Form generated from reading UI file 'Cinemagraph.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CINEMAGRAPH_H
#define UI_CINEMAGRAPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CinemagraphClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CinemagraphClass)
    {
        if (CinemagraphClass->objectName().isEmpty())
            CinemagraphClass->setObjectName(QStringLiteral("CinemagraphClass"));
        CinemagraphClass->resize(600, 400);
        menuBar = new QMenuBar(CinemagraphClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        CinemagraphClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CinemagraphClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CinemagraphClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(CinemagraphClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        CinemagraphClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CinemagraphClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CinemagraphClass->setStatusBar(statusBar);

        retranslateUi(CinemagraphClass);

        QMetaObject::connectSlotsByName(CinemagraphClass);
    } // setupUi

    void retranslateUi(QMainWindow *CinemagraphClass)
    {
        CinemagraphClass->setWindowTitle(QApplication::translate("CinemagraphClass", "Cinemagraph", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CinemagraphClass: public Ui_CinemagraphClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CINEMAGRAPH_H
