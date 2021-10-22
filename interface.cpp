#include "rea.h"
#include "reaJS.h"
#include <QQmlApplicationEngine>
#include "mainwindow.h"

static rea2::regPip<QQmlApplicationEngine*> reg_js_linker([](rea2::stream<QQmlApplicationEngine*>* aInput){
    qmlRegisterType<rea2::environmentJS>("EnvJS", 1, 0, "EnvJS");
    //ref from: https://stackoverflow.com/questions/25403363/how-to-implement-a-singleton-provider-for-qmlregistersingletontype
    auto m = rea2::getDefaultPipelineName(), m_qml = rea2::getDefaultQMLPipelineName();
    rea2::connectPipelines({m, "js", m_qml, "qmljs", m, m_qml, m_qml, m});
    rea2::pipeline::instance("js");
    rea2::pipeline::instance("qmljs");
    aInput->out();
}, rea2::Json("name", "install0_js"), "initRea");

QImage Uri2QImage(const QString& aUri){
    auto dt = aUri.mid(aUri.indexOf("64,") + 3, aUri.length()).trimmed();
    return QImage::fromData(QByteArray::fromBase64(dt.toLocal8Bit()));
}

static rea2::regPip<QQmlApplicationEngine*> test_qsg([](rea2::stream<QQmlApplicationEngine*>* aInput){
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
    extendTrigger(QJsonArray, deleteShapes_testbrd, qml);
    extendTrigger(QJsonObject, moveShapes_testbrd, qml);
    extendTrigger(QJsonObject, arrangeShapes_testbrd, qml);
    extendTrigger(QJsonObject, copyShapes_testbrd, qml);
    extendTrigger(QJsonObject, pasteShapes_testbrd, qml);
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
    //storage
    extendTrigger(bool, readImage, js);
    rea2::pipeline::instance()->add<bool>([](rea2::stream<bool>* aInput){
        if (aInput->data()){
            auto img = aInput->scope()->data<QImage>("data");
            auto uri = "data:image/png;base64, " + rea2::QImage2Base64(img);
            aInput->scope()->cache<QString>("uri", uri);
        }
        aInput->out();
    }, rea2::Json("after", "js_readImage"));
    rea2::pipeline::instance()->add<bool>([](rea2::stream<bool>* aInput){
        auto uri = aInput->scope()->data<QString>("uri");
        aInput->scope()->cache<QImage>("data", Uri2QImage(uri));
        aInput->out();
    }, rea2::Json("name", "js_writeImage",
                 "aftered", "writeImage",
                 "external",  "js"));
}, QJsonObject(), "initRea");
