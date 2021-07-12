#include "rea.h"
#include <QImage>
#include <QDateTime>
#include <QQmlApplicationEngine>

static rea::regPip<QQmlApplicationEngine*> test_qsg([](rea::stream<QQmlApplicationEngine*>* aInput){
    static QString pth = "F:/ttt/Hearthstone Screenshot 03-30-20 20.54.09.png";
    static QString pth2 = "D:/mywork2/qsgboardtest/微信图片_20200916112142.png";

    rea::pipeline::instance()->add<QJsonObject>([](rea::stream<QJsonObject>* aInput){
        auto scp = aInput->scope();
        pth = scp->data<QString>("image1");
        pth2 = scp->data<QString>("image2");
        QImage img(pth);

        QHash<QString, QImage> imgs;
        imgs.insert(pth, img);

        QJsonObject img_data;
        img_data.insert(pth2, QString(rea::QImage2Base64(QImage(pth2))));

        auto cfg = rea::Json("width", img.width() ? img.width() : 600,
                             "height", img.height() ? img.height() : 600,
                             "arrow", rea::Json("visible", false,
                                                "pole", true),
                             "face", 200,
                             "text", rea::Json("visible", false,
                                               "size", rea::JArray(100, 50),
                                               "location", "bottom"),
                             "transform", rea::JArray(1, 0, 0, 0, 1, 0, 0, 0, 1),
                             "color", "blue",
                             "max_ratio", 100,
                             "min_ratio", 0.01,
                             "objects", rea::Json(
                                            "img_2", rea::Json(
                                                         "type", "image",
                                                         "range", rea::JArray(0, 0, img.width(), img.height()),
                                                         "path", pth,
                                                         "caption", "Text",
                                                         "color", "green"//,
                                                                          // "text", rea::Json("visible", true,
                                                                          //                   "size", rea::JArray(100, 50),
                                                                          //                   "location", "cornerLT")
                                                                                                                      ),
                                            "img_3", rea::Json(
                                                         "type", "image",
                                                         "range", rea::JArray(img.width(), 0, 2 * img.width(), img.height()),
                                                         "path", pth2,
                                                         "caption", "Text",
                                                         "color", "green"//,
                                                         // "text", rea::Json("visible", true,
                                                         //                   "size", rea::JArray(100, 50),
                                                         //                   "location", "cornerLT")
                                                         ),
                                            "shp_0", rea::Json(
                                                         "type", "poly",
                                                         "points", rea::JArray(QJsonArray(),
                                                                               rea::JArray(0, 0,
                                                                                           200, 0,
                                                                                           200, 200,
                                                                                           0, 200,
                                                                                           0, 0),
                                                                               rea::JArray(80, 70,
                                                                                           120, 100,
                                                                                           120, 70,
                                                                                           80, 70)),
                                                         "color", "red",
                                                         "width", 3,
                                                         "caption", "poly",
                                                         "style", "dash",
                                                         "face", 50,
                                                         "text", rea::Json("visible", true,
                                                                           "size", rea::JArray(100, 50))
                                                                                                                      ),
                                            "shp_1", rea::Json(
                                                         "type", "ellipse",
                                                         "center", rea::JArray(200, 200),
                                                         "radius", rea::JArray(300, 200),
                                                         "width", 5,
                                                         "style", "dash",
                                                         "ccw", false,
                                                         "angle", 45,
                                                         "caption", "ellipse"
                                                                                                                      )
                                                                                                ));

        auto view = aInput->data();
        for (auto i : view.keys())
            cfg.insert(i, view.value(i));
        aInput->scope(true)
                ->cache<QJsonObject>("model", cfg)
                ->cache<QHash<QString, QImage>>("image", imgs)
                ->cache<QJsonObject>("image_data", img_data);

        aInput->outs<QJsonArray>(QJsonArray(), "updateQSGAttr_testbrd");
    }, rea::Json("name", "testQSGModel", "external", "qml"));

    rea::pipeline::instance()->add<QJsonObject>([](rea::stream<QJsonObject>* aInput){
        QImage img2(pth);
        auto img = img2.scaled(600, 400);
        auto cfg = rea::Json("width", img.width() ? img.width() : 600,
                             "height", img.height() ? img.height() : 600,
                             "objects", rea::Json(
                                            "img_2", rea::Json(
                                                         "type", "image",
                                                         "range", rea::JArray(0, 0, img.width(), img.height()),
                                                         "path", pth)));
        auto tm0 = QDateTime::currentDateTime().toMSecsSinceEpoch();
        for (int i = 0; i < 1000; ++i){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 50));
            QHash<QString, QImage> imgs;
            //rea::imagePool::cacheImage(pth, img);
            imgs.insert(pth, img2.scaled(600, 400));
            auto scp = std::make_shared<rea::scopeCache>();
            rea::pipeline::instance()->run<QJsonArray>("updateQSGAttr_testbrd",
                                                        {},
                                                        "",
                                                        scp->cache<QJsonObject>("model", cfg)
                                                           ->cache<QHash<QString, QImage>>("image", imgs));
        }
        std::cout << "cost: " << QDateTime::currentDateTime().toMSecsSinceEpoch() - tm0 << std::endl;
    }, rea::Json("name", "testFPS", "thread", 5, "external", "qml"));

    aInput->out();
}, QJsonObject(), "initRea");
