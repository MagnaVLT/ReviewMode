/********************************************************************************
** Form generated from reading UI file 'simple_reviewmode_gui.ui'
**
** Created: Mon Jul 13 17:36:58 2015
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUISIMPLEREVIEW_H
#define UI_GUISIMPLEREVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QPushButton *btn_clip;
    QLineEdit *txt_clip_directory;
    QPushButton *btn_next;
    QPushButton *btn_previous;
    QPushButton *btn_showclip;
    QComboBox *cbo_mode;
    QLabel *label;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(525, 102);
        btn_clip = new QPushButton(Form);
        btn_clip->setObjectName(QString::fromUtf8("btn_clip"));
        btn_clip->setGeometry(QRect(388, 42, 121, 21));
        txt_clip_directory = new QLineEdit(Form);
        txt_clip_directory->setObjectName(QString::fromUtf8("txt_clip_directory"));
        txt_clip_directory->setGeometry(QRect(10, 42, 371, 21));
        btn_next = new QPushButton(Form);
        btn_next->setObjectName(QString::fromUtf8("btn_next"));
        btn_next->setGeometry(QRect(120, 72, 101, 23));
        btn_previous = new QPushButton(Form);
        btn_previous->setObjectName(QString::fromUtf8("btn_previous"));
        btn_previous->setGeometry(QRect(10, 72, 101, 23));
        btn_showclip = new QPushButton(Form);
        btn_showclip->setObjectName(QString::fromUtf8("btn_showclip"));
        btn_showclip->setGeometry(QRect(240, 72, 271, 23));
        cbo_mode = new QComboBox(Form);
        cbo_mode->setObjectName(QString::fromUtf8("cbo_mode"));
        cbo_mode->setGeometry(QRect(140, 9, 371, 22));
        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 11, 111, 16));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        btn_clip->setText(QApplication::translate("Form", "Clip Directory Browse", 0, QApplication::UnicodeUTF8));
        btn_next->setText(QApplication::translate("Form", "Next >>", 0, QApplication::UnicodeUTF8));
        btn_previous->setText(QApplication::translate("Form", "<< Previous", 0, QApplication::UnicodeUTF8));
        btn_showclip->setText(QApplication::translate("Form", "Show Selected Clip", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "Confiruation Selection", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUISIMPLEREVIEW_H
