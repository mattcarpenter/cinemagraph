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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "previewgl.h"
#include "transport.h"

QT_BEGIN_NAMESPACE

class Ui_CinemagraphClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *main;
    QVBoxLayout *pvw_trnsprt_btns;
    PreviewGL *preview_gl;
    QHBoxLayout *control_buttons;
    QSpacerItem *horizontalSpacer;
    QPushButton *play_button;
    QPushButton *pause_button;
    QSpacerItem *horizontalSpacer_2;
    QWidget *sidebar;
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;
    QPushButton *load_video;
    QPushButton *pushButton;
    QPushButton *load_still_frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *new_mask_layer;
    QPushButton *pushButton_3;
    Transport *transport_bar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CinemagraphClass)
    {
        if (CinemagraphClass->objectName().isEmpty())
            CinemagraphClass->setObjectName(QStringLiteral("CinemagraphClass"));
        CinemagraphClass->resize(945, 659);
        centralWidget = new QWidget(CinemagraphClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        main = new QHBoxLayout();
        main->setSpacing(6);
        main->setObjectName(QStringLiteral("main"));
        pvw_trnsprt_btns = new QVBoxLayout();
        pvw_trnsprt_btns->setSpacing(6);
        pvw_trnsprt_btns->setObjectName(QStringLiteral("pvw_trnsprt_btns"));
        preview_gl = new PreviewGL(centralWidget);
        preview_gl->setObjectName(QStringLiteral("preview_gl"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(preview_gl->sizePolicy().hasHeightForWidth());
        preview_gl->setSizePolicy(sizePolicy);

        pvw_trnsprt_btns->addWidget(preview_gl);

        control_buttons = new QHBoxLayout();
        control_buttons->setSpacing(6);
        control_buttons->setObjectName(QStringLiteral("control_buttons"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        control_buttons->addItem(horizontalSpacer);

        play_button = new QPushButton(centralWidget);
        play_button->setObjectName(QStringLiteral("play_button"));

        control_buttons->addWidget(play_button);

        pause_button = new QPushButton(centralWidget);
        pause_button->setObjectName(QStringLiteral("pause_button"));

        control_buttons->addWidget(pause_button);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        control_buttons->addItem(horizontalSpacer_2);


        pvw_trnsprt_btns->addLayout(control_buttons);


        main->addLayout(pvw_trnsprt_btns);

        sidebar = new QWidget(centralWidget);
        sidebar->setObjectName(QStringLiteral("sidebar"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sidebar->sizePolicy().hasHeightForWidth());
        sidebar->setSizePolicy(sizePolicy1);
        sidebar->setMinimumSize(QSize(200, 0));
        sidebar->setBaseSize(QSize(0, 0));
        sidebar->setStyleSheet(QStringLiteral(""));
        verticalLayout = new QVBoxLayout(sidebar);
        verticalLayout->setSpacing(4);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        treeView = new QTreeView(sidebar);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setStyleSheet(QStringLiteral("background-color: #222; "));

        verticalLayout->addWidget(treeView);

        load_video = new QPushButton(sidebar);
        load_video->setObjectName(QStringLiteral("load_video"));

        verticalLayout->addWidget(load_video);

        pushButton = new QPushButton(sidebar);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        load_still_frame = new QPushButton(sidebar);
        load_still_frame->setObjectName(QStringLiteral("load_still_frame"));

        verticalLayout->addWidget(load_still_frame);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        new_mask_layer = new QPushButton(sidebar);
        new_mask_layer->setObjectName(QStringLiteral("new_mask_layer"));

        horizontalLayout->addWidget(new_mask_layer);

        pushButton_3 = new QPushButton(sidebar);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);


        verticalLayout->addLayout(horizontalLayout);


        main->addWidget(sidebar);


        verticalLayout_2->addLayout(main);

        transport_bar = new Transport(centralWidget);
        transport_bar->setObjectName(QStringLiteral("transport_bar"));
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(transport_bar->sizePolicy().hasHeightForWidth());
        transport_bar->setSizePolicy(sizePolicy2);
        transport_bar->setMinimumSize(QSize(0, 50));
        transport_bar->setBaseSize(QSize(0, 50));
        transport_bar->setStyleSheet(QStringLiteral("background-color: #222;"));

        verticalLayout_2->addWidget(transport_bar);

        CinemagraphClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CinemagraphClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 945, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        CinemagraphClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CinemagraphClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setEnabled(false);
        mainToolBar->setMovable(false);
        mainToolBar->setFloatable(false);
        CinemagraphClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CinemagraphClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CinemagraphClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());

        retranslateUi(CinemagraphClass);
        QObject::connect(preview_gl, SIGNAL(Initialized()), CinemagraphClass, SLOT(OpenGLInitialized()));

        QMetaObject::connectSlotsByName(CinemagraphClass);
    } // setupUi

    void retranslateUi(QMainWindow *CinemagraphClass)
    {
        CinemagraphClass->setWindowTitle(QApplication::translate("CinemagraphClass", "Cinemagraph", Q_NULLPTR));
        play_button->setText(QApplication::translate("CinemagraphClass", "Play", Q_NULLPTR));
        pause_button->setText(QApplication::translate("CinemagraphClass", "Pause", Q_NULLPTR));
        load_video->setText(QApplication::translate("CinemagraphClass", "Load Video", Q_NULLPTR));
        pushButton->setText(QApplication::translate("CinemagraphClass", "Set Current Frame as Still", Q_NULLPTR));
        load_still_frame->setText(QApplication::translate("CinemagraphClass", "Load Still Frame", Q_NULLPTR));
        new_mask_layer->setText(QApplication::translate("CinemagraphClass", "New Mask Layer", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("CinemagraphClass", "New GND Layer", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("CinemagraphClass", "File", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("CinemagraphClass", "Edit", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("CinemagraphClass", "View", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("CinemagraphClass", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CinemagraphClass: public Ui_CinemagraphClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CINEMAGRAPH_H
