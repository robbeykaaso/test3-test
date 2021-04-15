#include "rea.h"
#include <QApplication>
#include <QQmlApplicationEngine>

static rea::regPip<QQmlApplicationEngine*> test_qml([](rea::stream<QQmlApplicationEngine*>* aInput){
    auto pth = QApplication::applicationDirPath() + "/test.qml";
    //QStringLiteral();
    aInput->data()->load(pth);
    aInput->out();
},  rea::Json("name", "loadMain"));
