/********************************************************************************
** Form generated from reading UI file 'canmapping.ui'
**
** Created: Tue Jul 7 01:49:10 2015
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIANNOTATION_H
#define UI_GUIANNOTATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *txt_vehicle;
    QLineEdit *txt_fusion;
    QLineEdit *txt_eyeq;
    QPushButton *btn_browse_vehicle;
    QPushButton *btn_browse_fusion;
    QPushButton *btn_browse_eyeq;
    QPushButton *btn_apply;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(797, 213);
        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 50, 61, 16));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 80, 61, 16));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(70, 110, 91, 16));
        txt_vehicle = new QLineEdit(Form);
        txt_vehicle->setObjectName(QString::fromUtf8("txt_vehicle"));
        txt_vehicle->setEnabled(false);
        txt_vehicle->setGeometry(QRect(170, 50, 461, 20));
        txt_fusion = new QLineEdit(Form);
        txt_fusion->setObjectName(QString::fromUtf8("txt_fusion"));
        txt_fusion->setEnabled(false);
        txt_fusion->setGeometry(QRect(170, 80, 461, 20));
        txt_eyeq = new QLineEdit(Form);
        txt_eyeq->setObjectName(QString::fromUtf8("txt_eyeq"));
        txt_eyeq->setEnabled(false);
        txt_eyeq->setGeometry(QRect(170, 110, 461, 20));
        btn_browse_vehicle = new QPushButton(Form);
        btn_browse_vehicle->setObjectName(QString::fromUtf8("btn_browse_vehicle"));
        btn_browse_vehicle->setGeometry(QRect(640, 50, 75, 23));
        btn_browse_fusion = new QPushButton(Form);
        btn_browse_fusion->setObjectName(QString::fromUtf8("btn_browse_fusion"));
        btn_browse_fusion->setGeometry(QRect(640, 80, 75, 23));
        btn_browse_eyeq = new QPushButton(Form);
        btn_browse_eyeq->setObjectName(QString::fromUtf8("btn_browse_eyeq"));
        btn_browse_eyeq->setGeometry(QRect(640, 110, 75, 23));
        btn_apply = new QPushButton(Form);
        btn_apply->setObjectName(QString::fromUtf8("btn_apply"));
        btn_apply->setGeometry(QRect(170, 140, 545, 23));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "Vehicle CAN", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Form", "Fusion CAN", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Form", "EyeQ Private CAN", 0, QApplication::UnicodeUTF8));
        btn_browse_vehicle->setText(QApplication::translate("Form", "Browse", 0, QApplication::UnicodeUTF8));
        btn_browse_fusion->setText(QApplication::translate("Form", "Browse", 0, QApplication::UnicodeUTF8));
        btn_browse_eyeq->setText(QApplication::translate("Form", "Browse", 0, QApplication::UnicodeUTF8));
        btn_apply->setText(QApplication::translate("Form", "Apply", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIANNOTATION_H
