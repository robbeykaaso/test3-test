#include "rea.h"
#include <QQmlApplicationEngine>

static rea::regPip<QQmlApplicationEngine*> test_qml([](rea::stream<QQmlApplicationEngine*>* aInput){
    aInput->data()->load(QUrl(QStringLiteral("test.qml")));
    aInput->out();
},  rea::Json("name", "loadMain"));
