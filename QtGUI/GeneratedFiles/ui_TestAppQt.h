/********************************************************************************
** Form generated from reading UI file 'TestAppQt.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTAPPQT_H
#define UI_TESTAPPQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestAppQtClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *MeasurementBtn;
    QPushButton *TestSend;
    QHBoxLayout *horizontalLayout_2;
    QSlider *verticalSlider;
    QWidget *ChartWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TestAppQtClass)
    {
        if (TestAppQtClass->objectName().isEmpty())
            TestAppQtClass->setObjectName(QStringLiteral("TestAppQtClass"));
        TestAppQtClass->resize(881, 805);
        centralWidget = new QWidget(TestAppQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        MeasurementBtn = new QPushButton(centralWidget);
        MeasurementBtn->setObjectName(QStringLiteral("MeasurementBtn"));

        verticalLayout->addWidget(MeasurementBtn);

        TestSend = new QPushButton(centralWidget);
        TestSend->setObjectName(QStringLiteral("TestSend"));

        verticalLayout->addWidget(TestSend);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 5, -1, -1);
        verticalSlider = new QSlider(centralWidget);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setMinimum(0);
        verticalSlider->setMaximum(65535);
        verticalSlider->setValue(127);
        verticalSlider->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalSlider);

        ChartWidget = new QWidget(centralWidget);
        ChartWidget->setObjectName(QStringLiteral("ChartWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ChartWidget->sizePolicy().hasHeightForWidth());
        ChartWidget->setSizePolicy(sizePolicy);
        ChartWidget->setMinimumSize(QSize(0, 500));
        TestSend->raise();

        horizontalLayout_2->addWidget(ChartWidget);


        verticalLayout->addLayout(horizontalLayout_2);

        TestAppQtClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TestAppQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 881, 21));
        TestAppQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TestAppQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TestAppQtClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TestAppQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TestAppQtClass->setStatusBar(statusBar);

        retranslateUi(TestAppQtClass);

        QMetaObject::connectSlotsByName(TestAppQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *TestAppQtClass)
    {
        TestAppQtClass->setWindowTitle(QApplication::translate("TestAppQtClass", "TestAppQt", Q_NULLPTR));
        MeasurementBtn->setText(QApplication::translate("TestAppQtClass", "Configure Measurement", Q_NULLPTR));
        TestSend->setText(QApplication::translate("TestAppQtClass", "Test XCP", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TestAppQtClass: public Ui_TestAppQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTAPPQT_H
