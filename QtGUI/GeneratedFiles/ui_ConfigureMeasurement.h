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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTreeWidget>
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
    QPushButton *setDAQBtn;
    QPushButton *setEntryBtn;
    QTreeWidget *treeWidget;
    QPushButton *AddDAQBtn;
    QPushButton *AddODTBtn;
    QPushButton *AddEntryBtn;
    QGroupBox *daqBox;
    QGroupBox *modeBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QCheckBox *timestampBox;
    QCheckBox *pidBox;
    QCheckBox *ctrBox;
    QCheckBox *alternatingBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *eventBox;
    QLabel *label_2;
    QSpinBox *priorityBox;
    QLabel *label_3;
    QSpinBox *prescalerBox;
    QGroupBox *directionBox;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *DAQbutton;
    QRadioButton *stimButton;
    QGroupBox *entryBox;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *label_4;
    QLineEdit *addressInput;
    QLabel *label_5;
    QLineEdit *addressExtInput;
    QLabel *label_6;
    QComboBox *typeInput;
    QLabel *label_7;
    QPushButton *colorBtn;

    void setupUi(QDialog *ConfigureMeasurementUi)
    {
        if (ConfigureMeasurementUi->objectName().isEmpty())
            ConfigureMeasurementUi->setObjectName(QStringLiteral("ConfigureMeasurementUi"));
        ConfigureMeasurementUi->resize(721, 376);
        layoutWidget = new QWidget(ConfigureMeasurementUi);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(606, 10, 97, 151));
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

        setDAQBtn = new QPushButton(layoutWidget);
        setDAQBtn->setObjectName(QStringLiteral("setDAQBtn"));

        vboxLayout->addWidget(setDAQBtn);

        setEntryBtn = new QPushButton(layoutWidget);
        setEntryBtn->setObjectName(QStringLiteral("setEntryBtn"));

        vboxLayout->addWidget(setEntryBtn);

        treeWidget = new QTreeWidget(ConfigureMeasurementUi);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(10, 10, 291, 361));
        AddDAQBtn = new QPushButton(ConfigureMeasurementUi);
        AddDAQBtn->setObjectName(QStringLiteral("AddDAQBtn"));
        AddDAQBtn->setGeometry(QRect(310, 10, 75, 23));
        AddODTBtn = new QPushButton(ConfigureMeasurementUi);
        AddODTBtn->setObjectName(QStringLiteral("AddODTBtn"));
        AddODTBtn->setGeometry(QRect(390, 10, 75, 23));
        AddEntryBtn = new QPushButton(ConfigureMeasurementUi);
        AddEntryBtn->setObjectName(QStringLiteral("AddEntryBtn"));
        AddEntryBtn->setGeometry(QRect(470, 10, 75, 23));
        daqBox = new QGroupBox(ConfigureMeasurementUi);
        daqBox->setObjectName(QStringLiteral("daqBox"));
        daqBox->setGeometry(QRect(310, 40, 291, 201));
        daqBox->setInputMethodHints(Qt::ImhNone);
        daqBox->setFlat(false);
        daqBox->setCheckable(false);
        modeBox = new QGroupBox(daqBox);
        modeBox->setObjectName(QStringLiteral("modeBox"));
        modeBox->setGeometry(QRect(10, 20, 121, 171));
        verticalLayoutWidget = new QWidget(modeBox);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 20, 101, 141));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(5, 0, 0, 0);
        timestampBox = new QCheckBox(verticalLayoutWidget);
        timestampBox->setObjectName(QStringLiteral("timestampBox"));
        timestampBox->setChecked(false);

        verticalLayout->addWidget(timestampBox);

        pidBox = new QCheckBox(verticalLayoutWidget);
        pidBox->setObjectName(QStringLiteral("pidBox"));

        verticalLayout->addWidget(pidBox);

        ctrBox = new QCheckBox(verticalLayoutWidget);
        ctrBox->setObjectName(QStringLiteral("ctrBox"));

        verticalLayout->addWidget(ctrBox);

        alternatingBox = new QCheckBox(verticalLayoutWidget);
        alternatingBox->setObjectName(QStringLiteral("alternatingBox"));

        verticalLayout->addWidget(alternatingBox);

        formLayoutWidget = new QWidget(daqBox);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(140, 30, 141, 81));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(5, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        eventBox = new QSpinBox(formLayoutWidget);
        eventBox->setObjectName(QStringLiteral("eventBox"));
        eventBox->setMaximum(65535);

        formLayout->setWidget(0, QFormLayout::FieldRole, eventBox);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        priorityBox = new QSpinBox(formLayoutWidget);
        priorityBox->setObjectName(QStringLiteral("priorityBox"));
        priorityBox->setMaximum(255);

        formLayout->setWidget(1, QFormLayout::FieldRole, priorityBox);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        prescalerBox = new QSpinBox(formLayoutWidget);
        prescalerBox->setObjectName(QStringLiteral("prescalerBox"));
        prescalerBox->setMinimum(1);
        prescalerBox->setMaximum(255);

        formLayout->setWidget(2, QFormLayout::FieldRole, prescalerBox);

        directionBox = new QGroupBox(daqBox);
        directionBox->setObjectName(QStringLiteral("directionBox"));
        directionBox->setGeometry(QRect(140, 120, 141, 71));
        verticalLayoutWidget_2 = new QWidget(directionBox);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 20, 121, 42));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        DAQbutton = new QRadioButton(verticalLayoutWidget_2);
        DAQbutton->setObjectName(QStringLiteral("DAQbutton"));
        DAQbutton->setChecked(true);

        verticalLayout_3->addWidget(DAQbutton);

        stimButton = new QRadioButton(verticalLayoutWidget_2);
        stimButton->setObjectName(QStringLiteral("stimButton"));

        verticalLayout_3->addWidget(stimButton);

        entryBox = new QGroupBox(ConfigureMeasurementUi);
        entryBox->setObjectName(QStringLiteral("entryBox"));
        entryBox->setGeometry(QRect(310, 240, 291, 131));
        formLayoutWidget_2 = new QWidget(entryBox);
        formLayoutWidget_2->setObjectName(QStringLiteral("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(0, 20, 281, 133));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        formLayout_2->setContentsMargins(7, 0, 5, 5);
        label_4 = new QLabel(formLayoutWidget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_4);

        addressInput = new QLineEdit(formLayoutWidget_2);
        addressInput->setObjectName(QStringLiteral("addressInput"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, addressInput);

        label_5 = new QLabel(formLayoutWidget_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_5);

        addressExtInput = new QLineEdit(formLayoutWidget_2);
        addressExtInput->setObjectName(QStringLiteral("addressExtInput"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, addressExtInput);

        label_6 = new QLabel(formLayoutWidget_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);

        typeInput = new QComboBox(formLayoutWidget_2);
        typeInput->setObjectName(QStringLiteral("typeInput"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, typeInput);

        label_7 = new QLabel(formLayoutWidget_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_7);

        colorBtn = new QPushButton(formLayoutWidget_2);
        colorBtn->setObjectName(QStringLiteral("colorBtn"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, colorBtn);


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
        setDAQBtn->setText(QApplication::translate("ConfigureMeasurementUi", "Set DAQ Settings", Q_NULLPTR));
        setEntryBtn->setText(QApplication::translate("ConfigureMeasurementUi", "Set Entry Settings", Q_NULLPTR));
        AddDAQBtn->setText(QApplication::translate("ConfigureMeasurementUi", "Add DAQ", Q_NULLPTR));
        AddODTBtn->setText(QApplication::translate("ConfigureMeasurementUi", "Add ODT", Q_NULLPTR));
        AddEntryBtn->setText(QApplication::translate("ConfigureMeasurementUi", "Add Entry", Q_NULLPTR));
        daqBox->setTitle(QApplication::translate("ConfigureMeasurementUi", "DAQ Settings", Q_NULLPTR));
        modeBox->setTitle(QApplication::translate("ConfigureMeasurementUi", "Mode", Q_NULLPTR));
        timestampBox->setText(QApplication::translate("ConfigureMeasurementUi", "Timestamp", Q_NULLPTR));
        pidBox->setText(QApplication::translate("ConfigureMeasurementUi", "PID off", Q_NULLPTR));
        ctrBox->setText(QApplication::translate("ConfigureMeasurementUi", "CTR field", Q_NULLPTR));
        alternatingBox->setText(QApplication::translate("ConfigureMeasurementUi", "Alternating", Q_NULLPTR));
        label->setText(QApplication::translate("ConfigureMeasurementUi", "Event channel", Q_NULLPTR));
        label_2->setText(QApplication::translate("ConfigureMeasurementUi", "Priority", Q_NULLPTR));
        label_3->setText(QApplication::translate("ConfigureMeasurementUi", "Prescaler", Q_NULLPTR));
        directionBox->setTitle(QApplication::translate("ConfigureMeasurementUi", "Direction", Q_NULLPTR));
        DAQbutton->setText(QApplication::translate("ConfigureMeasurementUi", "DAQ direction", Q_NULLPTR));
        stimButton->setText(QApplication::translate("ConfigureMeasurementUi", "STIM direction", Q_NULLPTR));
        entryBox->setTitle(QApplication::translate("ConfigureMeasurementUi", "Entry Settings", Q_NULLPTR));
        label_4->setText(QApplication::translate("ConfigureMeasurementUi", "Address", Q_NULLPTR));
        label_5->setText(QApplication::translate("ConfigureMeasurementUi", "Address Extension", Q_NULLPTR));
        label_6->setText(QApplication::translate("ConfigureMeasurementUi", "Type", Q_NULLPTR));
        typeInput->clear();
        typeInput->insertItems(0, QStringList()
         << QApplication::translate("ConfigureMeasurementUi", "byte", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "ubyte", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "short", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "ushort", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "word", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "uword", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "long", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "ulong", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "float32", Q_NULLPTR)
         << QApplication::translate("ConfigureMeasurementUi", "float64", Q_NULLPTR)
        );
        label_7->setText(QApplication::translate("ConfigureMeasurementUi", "SeriesColor", Q_NULLPTR));
        colorBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ConfigureMeasurementUi: public Ui_ConfigureMeasurementUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGUREMEASUREMENT_H
