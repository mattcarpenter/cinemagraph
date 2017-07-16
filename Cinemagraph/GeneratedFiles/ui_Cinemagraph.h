/********************************************************************************
** Form generated from reading UI file 'Cinemagraph.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CinemagraphClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *stabilizeButton;
    QPushButton *pushButton_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CinemagraphClass)
    {
        if (CinemagraphClass->objectName().isEmpty())
            CinemagraphClass->setObjectName(QStringLiteral("CinemagraphClass"));
        CinemagraphClass->resize(293, 90);
        CinemagraphClass->setDocumentMode(false);
        CinemagraphClass->setUnifiedTitleAndToolBarOnMac(false);
        centralWidget = new QWidget(CinemagraphClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        stabilizeButton = new QPushButton(centralWidget);
        stabilizeButton->setObjectName(QStringLiteral("stabilizeButton"));

        verticalLayout->addWidget(stabilizeButton);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        CinemagraphClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CinemagraphClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CinemagraphClass->setStatusBar(statusBar);

        retranslateUi(CinemagraphClass);
        QObject::connect(stabilizeButton, SIGNAL(clicked()), CinemagraphClass, SLOT(on_stabilizeButton_clicked()));

        QMetaObject::connectSlotsByName(CinemagraphClass);
    } // setupUi

    void retranslateUi(QMainWindow *CinemagraphClass)
    {
        CinemagraphClass->setWindowTitle(QApplication::translate("CinemagraphClass", "Cinemagraph", Q_NULLPTR));
        stabilizeButton->setText(QApplication::translate("CinemagraphClass", "Stabilize Video", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("CinemagraphClass", "Edit Cinemagraph", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CinemagraphClass: public Ui_CinemagraphClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CINEMAGRAPH_H
