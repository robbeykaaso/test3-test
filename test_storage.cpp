#include "rea.h"
#include "storage.h"
#include <QJsonDocument>
#include <QDir>
#include <QQmlApplicationEngine>

void testStorage(const QString& aRoot = ""){ //for fs: aRoot == ""; for minIO: aRoot != ""
    auto tag = "testStorage";

 /*   pipeline::find(aRoot + "writeJson")
        ->next(local(aRoot + "readJson"), tag)
        ->nextF<stgJson>([aRoot](rea::stream<stgJson>* aInput){
            auto js = aInput->data().getData();
            assert(js.value("hello") == "world2");
            aInput->outs<stgByteArray>(stgByteArray(QJsonDocument(Json("hello", "world")).toJson(), "testFS.json"), aRoot + "writeByteArray");
        })
        ->next(local(aRoot + "writeByteArray"))
        ->next(local(aRoot + "readByteArray"))
        ->nextF<stgByteArray>([aRoot](rea::stream<stgByteArray>* aInput){
            auto dt = aInput->data().getData();
            auto cfg = QJsonDocument::fromJson(dt).object();
            assert(cfg.value("hello") == "world");

            std::vector<stgByteArray> dts;
            dts.push_back(stgByteArray(dt, "testFS.json"));
            stgVector<stgByteArray> stm(dts, "testDir");
            aInput->outs<stgVector<stgByteArray>>(stm, aRoot + "writeDir");
        })
        ->next(local(aRoot + "writeDir"))
        ->next(local(aRoot + "readDir"))
        ->nextF<stgVector<stgByteArray>>([aRoot](rea::stream<stgVector<stgByteArray>>* aInput){
            auto dt = aInput->data().getData();
            if (aRoot == "")
                assert(QDir().exists(aInput->data() + "/" + dt.at(0)));
            aInput->outs<stgVector<QString>>(stgVector<QString>(std::vector<QString>(), aInput->data()), aRoot + "listFiles");
        })
        ->next(local(aRoot + "listFiles"))
        ->nextF<stgVector<QString>>([aRoot](rea::stream<stgVector<QString>>* aInput){
            auto dt = aInput->data().getData();
            if (aRoot == "")
                assert(dt.size() == 3);
            else
                assert(dt.size() == 1);
            aInput->outs<QString>("testDir", aRoot + "deletePath");
            aInput->outs<QString>("testFS.json", aRoot + "deletePath");
        })
        ->next(local(aRoot + "deletePath"))
        ->next(buffer<QString>(2))
        ->nextF<std::vector<QString>>([aRoot](rea::stream<std::vector<QString>>* aInput){
            aInput->outs<stgVector<QString>>(stgVector<QString>(std::vector<QString>(), aRoot + "/testDir"), aRoot + "listFiles");
        })
        ->next(local(aRoot + "listFiles"))
        ->nextF<stgVector<QString>>([aRoot](rea::stream<stgVector<QString>>* aInput){
            auto dt = aInput->data().getData();
            assert(dt.size() == 0);
            aInput->outs<QString>("Pass: testStorage " + aRoot, "testSuccess");
        })
        ->next("testSuccess");

    pipeline::instance()->run<stgJson>(aRoot + "writeJson", stgJson(Json("hello", "world2"), "testFS.json"), tag);
    */
}

using namespace rea;
static regPip<QQmlApplicationEngine*> test_qsg([](stream<QQmlApplicationEngine*>*){
    static fsStorage local_storage;
    static QString tag = "testStorage";
    pipeline::instance()->add<QJsonObject>([](stream<QJsonObject>*){
        auto stm = in(false, tag, std::make_shared<scopeCache>()
                           ->cache("data", std::make_shared<storageCache>()
                                            ->cache(Json("hello", "world2"), "testFS.json")))
                   ->asyncCall("writeJsonObject")
                   ->asyncCall("readJsonObject");
        QJsonObject ret;
        stm->scope()->data<std::shared_ptr<storageCache>>("data")->nextData<QJsonObject>(ret);
        assert(ret.value("hello") == "world2");

        stm->scope(true)->cache("data", std::make_shared<storageCache>()->cache(QJsonDocument(Json("hello", "world")).toJson(), "testFS.json"));
        stm = stm->asyncCall("writeByteArray")->asyncCall("readByteArray");
        QByteArray ret2;
        stm->scope()->data<std::shared_ptr<storageCache>>("data")->nextData<QByteArray>(ret2);
        assert(QJsonDocument::fromJson(ret2).object().value("hello") == "world");

        stm->scope(true)->cache("data", std::make_shared<storageCache>()->cache(QByteArray(), "testDir/"));
        stm = stm->asyncCall("writeByteArray");

        auto stm2 = in<QString>("testDir", tag)->asyncCall("listFiles");
        QJsonArray ret3;
        stm2->scope()->data<std::shared_ptr<storageCache>>("data")->nextData<QJsonArray>(ret3);
        assert(ret3.size() == 2);

        stm2->asyncCall("deletePath");
        in<QString>("testFS.json", tag)->asyncCall("deletePath");


    }, Json("name", tag, "external", "js"));
}, QJsonObject(), "initRea");
