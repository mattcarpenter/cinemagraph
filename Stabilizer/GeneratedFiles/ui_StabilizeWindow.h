/********************************************************************************
** Form generated from reading UI file 'StabilizeWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STABILIZEWINDOW_H
#define UI_STABILIZEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "videoview.h"

QT_BEGIN_NAMESPACE

class Ui_StabilizeWindow
{
public:
    QAction *actionOpen;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QSpacerItem *horizontalSpacer_2;
    VideoView *preview;
    QVBoxLayout *verticalLayout_2;
    QSlider *horizontalSlider;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *reset_mask_button;
    QPushButton *detect_features_button;
    QPushButton *stabilize_button;

    void setupUi(QMainWindow *StabilizeWindow)
    {
        if (StabilizeWindow->objectName().isEmpty())
            StabilizeWindow->setObjectName(QStringLiteral("StabilizeWindow"));
        StabilizeWindow->resize(1012, 626);
        actionOpen = new QAction(StabilizeWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralWidget = new QWidget(StabilizeWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pushButton_5);

        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setEnabled(false);
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pushButton_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        preview = new VideoView(centralWidget);
        preview->setObjectName(QStringLiteral("preview"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(preview->sizePolicy().hasHeightForWidth());
        preview->setSizePolicy(sizePolicy1);
        preview->setStyleSheet(QStringLiteral("background-color: black;"));

        verticalLayout->addWidget(preview);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalSlider);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setEnabled(false);
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setBaseSize(QSize(0, 0));

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(false);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setEnabled(false);

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setEnabled(false);

        horizontalLayout->addWidget(pushButton_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        reset_mask_button = new QPushButton(centralWidget);
        reset_mask_button->setObjectName(QStringLiteral("reset_mask_button"));
        reset_mask_button->setEnabled(false);

        horizontalLayout->addWidget(reset_mask_button);

        detect_features_button = new QPushButton(centralWidget);
        detect_features_button->setObjectName(QStringLiteral("detect_features_button"));
        detect_features_button->setEnabled(false);

        horizontalLayout->addWidget(detect_features_button);

        stabilize_button = new QPushButton(centralWidget);
        stabilize_button->setObjectName(QStringLiteral("stabilize_button"));
        stabilize_button->setEnabled(false);

        horizontalLayout->addWidget(stabilize_button);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addLayout(verticalLayout_2);

        StabilizeWindow->setCentralWidget(centralWidget);

        retranslateUi(StabilizeWindow);
        QObject::connect(pushButton_5, SIGNAL(clicked()), StabilizeWindow, SLOT(on_loadButton_clicked()));
        QObject::connect(preview, SIGNAL(AddMask(QPoint,QPoint)), StabilizeWindow, SLOT(on_addMask(QPoint,QPoint)));

        QMetaObject::connectSlotsByName(StabilizeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *StabilizeWindow)
    {
        StabilizeWindow->setWindowTitle(QApplication::translate("StabilizeWindow", "StabilizeWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("StabilizeWindow", "Open...", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("StabilizeWindow", "Load", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("StabilizeWindow", "Save", Q_NULLPTR));
        preview->setText(QString());
        pushButton_2->setText(QApplication::translate("StabilizeWindow", "<<", Q_NULLPTR));
        pushButton->setText(QApplication::translate("StabilizeWindow", "Pause", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("StabilizeWindow", "Play", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("StabilizeWindow", ">>", Q_NULLPTR));
        reset_mask_button->setText(QApplication::translate("StabilizeWindow", "Reset Mask", Q_NULLPTR));
        detect_features_button->setText(QApplication::translate("StabilizeWindow", "Detect Features", Q_NULLPTR));
        stabilize_button->setText(QApplication::translate("StabilizeWindow", "Stabilize", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class StabilizeWindow: public Ui_StabilizeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STABILIZEWINDOW_H
