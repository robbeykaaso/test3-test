#include "rea.h"
#include <QQmlApplicationEngine>

static rea::regPip<QQmlApplicationEngine*> test_qsg([](rea::stream<QQmlApplicationEngine*>* aInput){
    //interface adapter
    extendTrigger(QJsonArray, updateQSGAttr_testbrd, qml);
    extendListener(QJsonArray, QSGAttrUpdated_testbrd, qml);
    extendTrigger(QJsonArray, updateQSGAttr_testbrd, js);
    extendListener(QJsonArray, QSGAttrUpdated_testbrd, js);
    extendTrigger(QJsonArray, updateQSGCtrl_testbrd, qml);
    extendListener(QJsonObject, updateQSGMenu_testbrd, qml);
    extendTrigger(bool, doCommand, qml);
}, QJsonObject(), "initRea");
