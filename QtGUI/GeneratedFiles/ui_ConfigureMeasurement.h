/********************************************************************************
** Form generated from reading UI file 'ConfigureMeasurement.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGUREMEASUREMENT_H
#define UI_CONFIGUREMEASUREMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigureMeasurementUi
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *vboxLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *spacerItem;

    void setupUi(QDialog *ConfigureMeasurementUi)
    {
        if (ConfigureMeasurementUi->objectName().isEmpty())
            ConfigureMeasurementUi->setObjectName(QStringLiteral("ConfigureMeasurementUi"));
        ConfigureMeasurementUi->resize(400, 300);
        layoutWidget = new QWidget(ConfigureMeasurementUi);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(300, 20, 77, 106));
        vboxLayout = new QVBoxLayout(layoutWidget);
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));

        vboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        vboxLayout->addWidget(cancelButton);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);


        retranslateUi(ConfigureMeasurementUi);
        QObject::connect(okButton, SIGNAL(clicked()), ConfigureMeasurementUi, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), ConfigureMeasurementUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigureMeasurementUi);
    } // setupUi

    void retranslateUi(QDialog *ConfigureMeasurementUi)
    {
        ConfigureMeasurementUi->setWindowTitle(QApplication::translate("ConfigureMeasurementUi", "Dialog", Q_NULLPTR));
        okButton->setText(QApplication::translate("ConfigureMeasurementUi", "OK", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("ConfigureMeasurementUi", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ConfigureMeasurementUi: public Ui_ConfigureMeasurementUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGUREMEASUREMENT_H
