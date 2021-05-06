#include "rea.h"
#include "storage.h"
#include <QJsonDocument>
#include <QDir>
#include <QQmlApplicationEngine>
#include <QDateTime>

class fsStorage2 : public rea::fsStorage{
public:
    fsStorage2(const QString& aRoot = "") : fsStorage(aRoot){}
    void initialize() override{
        rea::pipeline::instance()->add<bool, rea::pipeParallel>([this](rea::stream<bool>* aInput) {
            QJsonObject dt;
            auto ret = readJsonObject(aInput->scope()->data<QString>("path"), dt);
            aInput->scope()->cache("data", dt);
            aInput->setData(ret)->out();
        }, rea::Json("name", m_root + "readJsonObject2", "thread", 12));
    }
};

using namespace rea;
static regPip<QQmlApplicationEngine*> test_stg([](stream<QQmlApplicationEngine*>*){
    static fsStorage local_storage;
    local_storage.initialize();
    static fsStorage2 local_storage2;
    local_storage2.initialize();
    static QString tag = "testStorage";

    static int count0, count1;
    static int tm0, tm1;
    pipeline::instance()->add<QJsonObject>([](stream<QJsonObject>* aInput){
        auto stm = in(false, tag, std::make_shared<scopeCache>()
                           ->cache<QString>("path", "testFS.json")
                           ->cache("data", Json("hello", "world2")))
                   ->asyncCall("writeJsonObject")
                   ->asyncCall("readJsonObject");
        assert(stm->data());
        auto ret = stm->scope()->data<QJsonObject>("data");
        assert(ret.value("hello") == "world2");

        stm->scope(true)->cache<QString>("path", "testFS.json")->cache("data", QJsonDocument(Json("hello", "world")).toJson());
        stm = stm->asyncCall("writeByteArray")->asyncCall("readByteArray");
        assert(stm->data());
        auto ret2 = stm->scope()->data<QByteArray>("data");
        assert(QJsonDocument::fromJson(ret2).object().value("hello") == "world");

        stm->scope(true)->cache<QString>("path", "testDir/")->cache("data", QByteArray());
        stm = stm->asyncCall("writeByteArray");
        assert(!stm->data());

        auto stm2 = in<QString>("testDir", tag)->asyncCall("listFiles");
        auto ret3 = stm2->scope()->data<std::vector<QString>>("data");
        assert(ret3.size() == 2);

        stm2->asyncCall("deletePath");
        in<QString>("testFS.json", tag)->asyncCall("deletePath");

        auto fls = in<QString>("test_storage", tag)->asyncCall("listFiles")->scope()->data<std::vector<QString>>("data");

/*        tm0 = QDateTime::currentDateTime().toTime_t();
        count0 = (fls.size() - 2) * 5;
        for (int i = 0; i < 5; ++i)
            for (auto i : fls)
                if (i != "." && i != "..")
                    aInput->outs(false, "readJsonObject", tag + "2")->scope(true)->cache<QString>("path", "test_storage/" + i);
*/
        tm1 = QDateTime::currentDateTime().toTime_t();
        count1 = (fls.size() - 2) * 5;
        for (int i = 0; i < 5; ++i)
            for (auto i : fls)
                if (i != "." && i != "..")
                    aInput->outs(false, "readJsonObject2", tag + "2")->scope(true)->cache<QString>("path", "test_storage/" + i);

        aInput->outs(true);
    }, Json("name", tag, "external", "js"));

    //prove reading of file system by multithreads
    rea::pipeline::instance()->find("readJsonObject")->nextF<bool>([](rea::stream<bool>* aInput){
        auto dt = aInput->scope()->data<QJsonObject>("data");
        count0--;
        std::cout << count0 << std::endl;
        if (!count0){
            std::cout << QDateTime::currentDateTime().toTime_t() - tm0 << std::endl;
        }
    }, tag + "2");

    rea::pipeline::instance()->find("readJsonObject2")->nextF<bool>([](rea::stream<bool>* aInput){
        auto dt = aInput->scope()->data<QJsonObject>("data");
        count1--;
        std::cout << count1 << std::endl;
        if (!count1){
            std::cout << QDateTime::currentDateTime().toTime_t() - tm1 << std::endl;
        }
    }, tag + "2");
}, QJsonObject(), "initRea");
