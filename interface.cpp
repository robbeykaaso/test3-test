#include "rea.h"
#include <QQmlApplicationEngine>

static rea::regPip<QQmlApplicationEngine*> test_qsg([](rea::stream<QQmlApplicationEngine*>* aInput){
    //interface adapter
    //qsg
    extendTrigger(QJsonArray, updateQSGAttr_testbrd, qml);
    extendListener(QJsonArray, QSGAttrUpdated_testbrd, qml);
    extendTrigger(QJsonArray, updateQSGAttr_testbrd, js);
    extendListener(QJsonArray, QSGAttrUpdated_testbrd, js);
    extendTrigger(QJsonArray, updateQSGCtrl_testbrd, qml);
    extendListener(QJsonObject, updateQSGMenu_testbrd, qml);
    extendTrigger(bool, doCommand, qml);
    //edit
    extendTrigger(QJsonArray, testbrd_deleteShapes, qml);
    extendTrigger(QJsonObject, testbrd_moveShapes, qml);
    extendTrigger(QJsonObject, testbrd_arrangeShapes, qml);
    extendTrigger(QJsonObject, testbrd_copyShapes, qml);
    extendTrigger(QJsonObject, testbrd_pasteShapes, qml);
    //draw
    extendTrigger(double, cancelDrawCircle_testbrd, qml);
    extendTrigger(double, cancelDrawEllipse_testbrd, qml);
    extendTrigger(double, cancelDrawLine_testbrd, qml);
    extendTrigger(double, cancelDrawPoint_testbrd, qml);
    extendTrigger(double, cancelDrawRect_testbrd, qml);
    extendTrigger(double, cancelDrawPoly_testbrd, qml);
    extendTrigger(double, cancelDrawPolyLast_testbrd, qml);
    extendTrigger(QJsonObject, completeDrawPoly_testbrd, qml);
    extendTrigger(double, cancelDrawFree_testbrd, qml);
    extendTrigger(double, cancelDrawFreeLast_testbrd, qml);
    extendTrigger(QJsonObject, completeDrawFree_testbrd, qml);
}, QJsonObject(), "initRea");
