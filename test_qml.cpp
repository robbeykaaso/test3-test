#include "rea.h"
#include <QApplication>
#include <QQmlApplicationEngine>

static rea2::regPip<QQmlApplicationEngine*> test_qml([](rea2::stream<QQmlApplicationEngine*>* aInput){
    auto pth = QApplication::applicationDirPath() + "/test.qml";
    //QStringLiteral();
    aInput->data()->load(pth);
    aInput->out();
},  rea2::Json("name", "loadMain"));
