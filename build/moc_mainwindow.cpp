/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "cameraInfo"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "cameraOpen"
QT_MOC_LITERAL(4, 34, 14), // "doCameraMirror"
QT_MOC_LITERAL(5, 49, 10), // "stopCamera"
QT_MOC_LITERAL(6, 60, 12), // "calculateFPS"
QT_MOC_LITERAL(7, 73, 11), // "updateFrame"
QT_MOC_LITERAL(8, 85, 8), // "cv::Mat*"
QT_MOC_LITERAL(9, 94, 3), // "mat"
QT_MOC_LITERAL(10, 98, 9), // "updateFPS"
QT_MOC_LITERAL(11, 108, 3), // "fps"
QT_MOC_LITERAL(12, 112, 5), // "width"
QT_MOC_LITERAL(13, 118, 6), // "height"
QT_MOC_LITERAL(14, 125, 18), // "recordingStartStop"
QT_MOC_LITERAL(15, 144, 23), // "updateVideoRecordStatus"
QT_MOC_LITERAL(16, 168, 13) // "closeCapturer"

    },
    "MainWindow\0cameraInfo\0\0cameraOpen\0"
    "doCameraMirror\0stopCamera\0calculateFPS\0"
    "updateFrame\0cv::Mat*\0mat\0updateFPS\0"
    "fps\0width\0height\0recordingStartStop\0"
    "updateVideoRecordStatus\0closeCapturer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    1,   69,    2, 0x08 /* Private */,
      10,    3,   72,    2, 0x08 /* Private */,
      14,    0,   79,    2, 0x08 /* Private */,
      15,    2,   80,    2, 0x08 /* Private */,
      16,    1,   85,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Float, QMetaType::Int, QMetaType::Int,   11,   12,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->cameraInfo(); break;
        case 1: _t->cameraOpen(); break;
        case 2: _t->doCameraMirror(); break;
        case 3: _t->stopCamera(); break;
        case 4: _t->calculateFPS(); break;
        case 5: _t->updateFrame((*reinterpret_cast< cv::Mat*(*)>(_a[1]))); break;
        case 6: _t->updateFPS((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->recordingStartStop(); break;
        case 8: _t->updateVideoRecordStatus((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->closeCapturer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
