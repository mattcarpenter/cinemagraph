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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "preview.h"
#include "previewgl.h"

QT_BEGIN_NAMESPACE

class Ui_CinemagraphClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *main;
    PreviewGL *preview_gl;
    Preview *preview;
    QWidget *sidebar;
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;
    QPushButton *load_video;
    QPushButton *pushButton;
    QPushButton *load_still_frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *new_mask_layer;
    QPushButton *pushButton_3;
    QWidget *timeline;
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
        CinemagraphClass->resize(767, 487);
        centralWidget = new QWidget(CinemagraphClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        main = new QHBoxLayout();
        main->setSpacing(6);
        main->setObjectName(QStringLiteral("main"));
        preview_gl = new PreviewGL(centralWidget);
        preview_gl->setObjectName(QStringLiteral("preview_gl"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(preview_gl->sizePolicy().hasHeightForWidth());
        preview_gl->setSizePolicy(sizePolicy);

        main->addWidget(preview_gl);

        preview = new Preview(centralWidget);
        preview->setObjectName(QStringLiteral("preview"));
        sizePolicy.setHeightForWidth(preview->sizePolicy().hasHeightForWidth());
        preview->setSizePolicy(sizePolicy);
        preview->setStyleSheet(QStringLiteral("background-color: #222;"));

        main->addWidget(preview);

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

        timeline = new QWidget(centralWidget);
        timeline->setObjectName(QStringLiteral("timeline"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(timeline->sizePolicy().hasHeightForWidth());
        timeline->setSizePolicy(sizePolicy2);
        timeline->setMinimumSize(QSize(0, 50));
        timeline->setBaseSize(QSize(0, 50));
        timeline->setStyleSheet(QStringLiteral("background-color: #222;"));

        verticalLayout_2->addWidget(timeline);

        CinemagraphClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CinemagraphClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 767, 21));
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
        preview->setText(QString());
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
