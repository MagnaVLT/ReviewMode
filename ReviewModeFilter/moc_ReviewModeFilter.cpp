/****************************************************************************
** Meta object code from reading C++ file 'ReviewModeFilter.h'
**
** Created: Wed May 27 17:29:06 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "ReviewModeFilter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReviewModeFilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReviewModeFilter[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   17,   18,   17, 0x0a,
      48,   17,   18,   17, 0x0a,
      73,   17,   18,   17, 0x0a,
     105,   17,   18,   17, 0x0a,
     129,   17,   18,   17, 0x0a,
     154,   17,   18,   17, 0x0a,
     176,   17,   18,   17, 0x0a,
     197,   17,   18,   17, 0x0a,
     218,   17,   18,   17, 0x0a,
     239,   17,   18,   17, 0x0a,
     260,   17,   18,   17, 0x0a,
     281,   17,   18,   17, 0x0a,
     302,   17,   18,   17, 0x0a,
     324,   17,   18,   17, 0x0a,
     346,   17,   18,   17, 0x0a,
     370,   17,   18,   17, 0x0a,
     398,   17,   18,   17, 0x0a,
     425,   17,   18,   17, 0x0a,
     454,   17,   18,   17, 0x0a,
     482,   17,   18,   17, 0x0a,
     510,   17,   18,   17, 0x0a,
     534,   17,   18,   17, 0x0a,
     567,   17,   18,   17, 0x0a,
     602,   17,   18,   17, 0x0a,
     626,   17,   18,   17, 0x0a,
     650,   17,   18,   17, 0x0a,
     668,   17,   18,   17, 0x0a,
     686,   17,   18,   17, 0x0a,
     705,   17,   18,   17, 0x0a,
     732,   17,   18,   17, 0x0a,
     758,   17,   18,   17, 0x0a,
     804,   17,   18,   17, 0x0a,
     828,   17,   18,   17, 0x0a,
     864,  852,   18,   17, 0x0a,
     894,   17,   18,   17, 0x0a,
     917,   17,   18,   17, 0x0a,
     942,   17,   18,   17, 0x0a,
     969,   17,   18,   17, 0x0a,
    1000,   17,   18,   17, 0x0a,
    1024,   17,   18,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ReviewModeFilter[] = {
    "ReviewModeFilter\0\0tResult\0"
    "on_btn_clip_clicked()\0on_btn_setting_clicked()\0"
    "on_btn_setting_cancel_clicked()\0"
    "on_btn_browse_clicked()\0"
    "on_btn_execute_clicked()\0on_btn_copy_clicked()\0"
    "on_btn_LR1_clicked()\0on_btn_RL1_clicked()\0"
    "on_btn_LR2_clicked()\0on_btn_RL2_clicked()\0"
    "on_btn_LR3_clicked()\0on_btn_RL3_clicked()\0"
    "on_dateEdit_changed()\0on_chk_date_clicked()\0"
    "on_dateEdit_2_changed()\0"
    "on_chk_eyeq_event_clicked()\0"
    "on_chk_fcm_event_clicked()\0"
    "on_chk_radar_event_clicked()\0"
    "on_chk_user_event_clicked()\0"
    "on_chk_annotation_clicked()\0"
    "on_chk_search_clicked()\0"
    "on_chk_text_annotation_clicked()\0"
    "on_chk_i_text_annotation_clicked()\0"
    "on_btn_cancel_clicked()\0on_btn_submit_clicked()\0"
    "on_txt_m_edited()\0on_txt_s_edited()\0"
    "on_txt_ms_edited()\0on_cbo_i_feature_changed()\0"
    "on_btn_showclip_clicked()\0"
    "on_list_event_annotation_clicked(QModelIndex)\0"
    "on_btn_udpate_clicked()\0on_btn_insert_clicked()\0"
    "search_text\0on_txt_search_edited(QString)\0"
    "on_btn_login_clicked()\0on_cbo_project_changed()\0"
    "on_btn_change_pw_clicked()\0"
    "on_btn_change_cancel_clicked()\0"
    "on_btn_change_clicked()\0on_btn_logout_clicked()\0"
};

const QMetaObject ReviewModeFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ReviewModeFilter,
      qt_meta_data_ReviewModeFilter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ReviewModeFilter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ReviewModeFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ReviewModeFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReviewModeFilter))
        return static_cast<void*>(const_cast< ReviewModeFilter*>(this));
    if (!strcmp(_clname, "cBaseQtFilter"))
        return static_cast< cBaseQtFilter*>(const_cast< ReviewModeFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int ReviewModeFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { tResult _r = on_btn_clip_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 1: { tResult _r = on_btn_setting_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 2: { tResult _r = on_btn_setting_cancel_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 3: { tResult _r = on_btn_browse_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 4: { tResult _r = on_btn_execute_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 5: { tResult _r = on_btn_copy_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 6: { tResult _r = on_btn_LR1_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 7: { tResult _r = on_btn_RL1_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 8: { tResult _r = on_btn_LR2_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 9: { tResult _r = on_btn_RL2_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 10: { tResult _r = on_btn_LR3_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 11: { tResult _r = on_btn_RL3_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 12: { tResult _r = on_dateEdit_changed();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 13: { tResult _r = on_chk_date_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 14: { tResult _r = on_dateEdit_2_changed();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 15: { tResult _r = on_chk_eyeq_event_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 16: { tResult _r = on_chk_fcm_event_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 17: { tResult _r = on_chk_radar_event_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 18: { tResult _r = on_chk_user_event_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 19: { tResult _r = on_chk_annotation_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 20: { tResult _r = on_chk_search_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 21: { tResult _r = on_chk_text_annotation_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 22: { tResult _r = on_chk_i_text_annotation_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 23: { tResult _r = on_btn_cancel_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 24: { tResult _r = on_btn_submit_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 25: { tResult _r = on_txt_m_edited();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 26: { tResult _r = on_txt_s_edited();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 27: { tResult _r = on_txt_ms_edited();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 28: { tResult _r = on_cbo_i_feature_changed();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 29: { tResult _r = on_btn_showclip_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 30: { tResult _r = on_list_event_annotation_clicked((*reinterpret_cast< QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 31: { tResult _r = on_btn_udpate_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 32: { tResult _r = on_btn_insert_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 33: { tResult _r = on_txt_search_edited((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 34: { tResult _r = on_btn_login_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 35: { tResult _r = on_cbo_project_changed();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 36: { tResult _r = on_btn_change_pw_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 37: { tResult _r = on_btn_change_cancel_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 38: { tResult _r = on_btn_change_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        case 39: { tResult _r = on_btn_logout_clicked();
            if (_a[0]) *reinterpret_cast< tResult*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 40;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
