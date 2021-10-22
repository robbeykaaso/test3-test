/****************************************************************************
ref: https://www.jianshu.com/p/3c3888329732
****************************************************************************/

#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QtWebEngineWidgets>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMessageBox>
#include <QTextStream>
#include <QWebChannel>
#include "reaJS.h"
#include "reaQML.h"
#include "util.h"
#include <sstream>
#include"storage.h"


static int test_sum = 0;
static int test_pass = 0;

namespace rea2 {

static QHash<QString, std::function<void(void)>> m_tests;

void test(const QString& aName){
    if (m_tests.contains(aName))
        m_tests.value(aName)();
}

#define addTest(aName, aTest) \
    rea2::m_tests.insert(#aName, [](){aTest});

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");
    ui->setupUi(this);
    QStackedLayout* layout = new QStackedLayout(ui->frame);
    ui->frame->setLayout(layout);

    m_webView = new QWebEngineView(this);
    layout->addWidget(m_webView);
    m_webView->setContextMenuPolicy(Qt::CustomContextMenu);
    /*m_inspector = nullptr;
    connect(m_webView, &QWidget::customContextMenuRequested, this, [this]() {
        QMenu* menu = new QMenu(this);
        QAction* action = menu->addAction("Inspect");
        connect(action, &QAction::triggered, this, [this](){
            if(m_inspector == nullptr)
            {
                m_inspector = new Inspector(this);
            }
            else
            {
                m_inspector->show();
            }
        });
        menu->exec(QCursor::pos());
    });*/

    m_webChannel = new QWebChannel();
    m_webChannel->registerObject("Pipelinec++", rea2::pipeline::instance("js"));
    m_webChannel->registerObject("Pipelineqml", rea2::pipeline::instance("qmljs"));
    m_webView->page()->setWebChannel(m_webChannel);
    m_webView->setUrl(QApplication::applicationDirPath() + "/html/test.html");

    rea2::pipeline::instance()->supportType<JsContext*>([](rea2::stream0* aInput){
        return QVariant::fromValue<QObject*>(reinterpret_cast<rea2::stream<JsContext*>*>(aInput)->data());
    });
    m_jsContext = new JsContext();
    connect(m_jsContext, &JsContext::recvdMsg, this, [this](const QString& msg) {
        rea2::pipeline::instance()->run<QString>("testSuccess", "Pass: test29");
        auto sw = "Received message: " + msg;
        ui->statusBar->showMessage(sw);
    });
    unitTest();
}

class foo{
public:
    foo(int aStart){
        m_start = aStart;
    }
    void operator()(rea2::stream<int>* aInput) {
        auto ret = aInput->data() + m_start;
        aInput->setData(ret)->out();
    }

    void memberFoo(rea2::stream<int>* aInput){
        auto ret = aInput->data() + m_start;
        aInput->setData(ret)->out();
    }
private:
    int m_start;
};

namespace rea2 {

template <typename T, typename F>
class pipeCustomQML : public rea2::pipe<T, F> {
public:
    pipeCustomQML(rea2::pipeline* aParent, const QString& aName, int aThreadNo = 0) : rea2::pipe<T, F>(aParent, aName, aThreadNo) {

    }
protected:
    bool event( QEvent* e) override{
        if(e->type()== rea2::pipe0::streamEvent::type){
            auto eve = reinterpret_cast<rea2::pipe0::streamEvent*>(e);
            if (eve->getName() == rea2::pipe0::m_name){
                auto stm = std::dynamic_pointer_cast<rea2::stream<T>>(eve->getStream());
                stm->scope()->template cache<QString>("flag", "test48");
                doEvent(stm);
                doNextEvent(rea2::pipe0::m_next, stm);
            }
        }
        return true;
    }
};

regQMLPipe(CustomQML);

}

void MainWindow::unitTest(){
    //unitTestC++;testSuccess;testFail
    {

    rea2::pipeline::instance()->add<QJsonObject>([this](rea2::stream<QJsonObject>* aInput){
        auto dt = aInput->data();
        for (auto i : dt.keys())
            test_sum += dt.value(i).toInt();
        for (auto i : dt.keys())
            rea2::test(i);
        auto sw = "Received message: hello";
        ui->statusBar->showMessage(sw);
        aInput->out();
    }, rea2::Json("name", "unitTestC++", "external", "js"));

    rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
        test_pass++;
        std::cout << QString("Success: %1 (%2/%3)")
                         .arg(aInput->data())
                         .arg(test_pass)
                         .arg(test_sum)
                         .toStdString() << std::endl;
        aInput->out();
    }, rea2::Json("name", "testSuccess"));

    rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
        test_pass--;
        std::cout << QString("Fail: %1 (%2/%3)")
                         .arg(aInput->data())
                         .arg(test_pass)
                         .arg(test_sum)
                         .toStdString() << std::endl;
        aInput->out();
    }, rea2::Json("name", "testFail"));

    }

    //test4;test5;test6;test7
    {

    rea2::m_tests.insert("test4",[](){
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            assert(aInput->data() == 4.0);
            assert(aInput->scope()->data<QString>("hello") == "world");
            aInput->setData(aInput->data() + 1)->out();
        }, rea2::Json("name", "test4_", "external", "js"));

        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            assert(aInput->data() == 5.0);
            aInput->scope(true)->cache<QString>("hello2", "world");
            aInput->setData(aInput->data() + 1)->out();
        }, rea2::Json("name", "test4__", "external", "js"));
    });

    addTest(test5,
            rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
                assert(aInput->data() == "hello");
                aInput->setData("world")->out();
            }, rea2::Json("name", "test5", "external", "js"));
        )

    rea2::m_tests.insert("test6",[](){
        rea2::pipeline::instance()->find("test6_")->removeNext("test6__");
        rea2::pipeline::instance()->find("test6__")->removeNext("test_6");
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            aInput->scope()->cache<QString>("hello", "world");
            aInput->out();
        }, rea2::Json("name", "test6"))
            ->next("test6_")
            ->next("test6__")
            ->nextF<double>([](rea2::stream<double>* aInput){
                assert(aInput->data() == 6.0);
                assert(aInput->scope()->data<QString>("hello") == "");
                assert(aInput->scope()->data<QString>("hello2") == "world");
                aInput->outs<QString>("Pass: test6", "testSuccess");
            }, "", rea2::Json("name", "test_6"));

        rea2::pipeline::instance()->run<double>("test6", 4);
    });

    addTest(test7,
            rea2::pipeline::instance()->find("test7")->removeNext("test_7");
            rea2::pipeline::instance()->find("test7")
                ->nextF<QString>([](rea2::stream<QString>* aInput){
                    assert(aInput->data() == "world");
                    aInput->outs<QString>("Pass: test7", "testSuccess");
                }, "", rea2::Json("name", "test_7"));

            rea2::pipeline::instance()->run<QString>("test7", "hello");
        )

    }

    //test8;test9;test11;test12
    {
    addTest(test8,
            rea2::pipeline::instance()->run<QString>("test8", "hello");
        )

    addTest(test9,
            rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
                assert(aInput->data() == "hello");
                aInput->outs<QString>("Pass: test9", "testSuccess");
            }, rea2::Json("name", "test9", "external", "js"));
        )

    addTest(test11,
            auto pip = rea2::pipeline::instance()->add<int>([](rea2::stream<int>* aInput){
                assert(aInput->data() == 3);
                aInput->outs<QString>("Pass: test11", "testSuccess");
            });
            rea2::pipeline::instance()->add<int>([](rea2::stream<int>* aInput){
                assert(aInput->data() == 3);
                aInput->out();
            }, rea2::Json("name", "test11"))
                ->nextP(pip)
                ->next("testSuccess");

            rea2::pipeline::instance()->run<int>("test11", 3);
            rea2::pipeline::instance()->remove(pip->actName());
        )

        rea2::m_tests.insert("test12_",[](){
            rea2::pipeline::instance()->add<int>([](rea2::stream<int>* aInput){
                assert(aInput->data() == 4);
                std::stringstream ss;
                ss << std::this_thread::get_id();
                aInput->outs<std::string>(ss.str(), "test12_0");
            }, rea2::Json("name", "test12"))
                ->nextP(rea2::pipeline::instance()->add<std::string>([](rea2::stream<std::string>* aInput){
                    std::stringstream ss;
                    ss << std::this_thread::get_id();
                    assert(ss.str() != aInput->data());
                    aInput->outs<QString>("Pass: test12", "testSuccess");
                }, rea2::Json("name", "test12_0", "thread", 7)))
                ->next("testSuccess");
        });

        rea2::m_tests.insert("test12",[](){
            rea2::pipeline::instance()->run<int>("test12", 4);
        });

    }

    //test13;test14;test16
    {
    addTest(test13,
            rea2::pipeline::instance()->add<int>([](rea2::stream<int>* aInput){
                assert(aInput->data() == 66);
                aInput->outs<QString>("test13", "test13_0");
            }, rea2::Json("name", "test13"))
                ->next("test13_0")
                ->next("testSuccess");

            rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
                aInput->out();
            }, rea2::Json("name", "test13_1"))
                ->next("test13__")
                ->next("testSuccess");

            rea2::pipeline::instance()->find("test13_0")
                ->nextF<QString>([](rea2::stream<QString>* aInput){
                    aInput->out();
                }, "", rea2::Json("name", "test13__"))
                ->next("testSuccess");

            rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
                assert(aInput->data() == "test13");
                aInput->outs<QString>("Pass: test13", "testSuccess");
                aInput->outs<QString>("Pass: test13_", "test13__");
            }, rea2::Json("name", "test13_0"));

            rea2::pipeline::instance()->run<int>("test13", 66);
            rea2::pipeline::instance()->run<QString>("test13_1", "Pass: test13__");
        )

    rea2::m_tests.insert("test14", [](){
        rea2::pipeline::instance()->add<int, rea2::pipePartial>([](rea2::stream<int>* aInput){
            assert(aInput->data() == 66);
            aInput->setData(77)->out();
        }, rea2::Json("name", "test14"))
            ->nextPB(rea2::pipeline::instance()->add<int>([](rea2::stream<int>* aInput){
                        assert(aInput->data() == 77);
                        aInput->outs<QString>("Pass: test14", "testSuccess");
                    }, rea2::Json("name", "test14_")), "test14")
            ->nextP(rea2::pipeline::instance()->add<int>([](rea2::stream<int>* aInput){
                       assert(aInput->data() == 77);
                       aInput->outs<QString>("Fail: test14", "testFail");
                   }, rea2::Json("name", "test14__")), "test14_");

            rea2::pipeline::instance()->run<int>("test14", 66, "test14");
        });

    addTest(test16,
            rea2::pipeline::instance()->find("test16")->removeNext("test16_");
            rea2::pipeline::instance()->find("test16")->removeNext("test16__");

            rea2::pipeline::instance()->find("test16")
                ->nextPB(rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                            assert(aInput->data() == 77.0);
                            aInput->outs<QString>("Pass: test16", "testSuccess");
                        }, rea2::Json("name", "test16_")), "test16")
                ->nextP(rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                           assert(aInput->data() == 77.0);
                           aInput->outs<QString>("Fail: test16", "testFail");
                       }, rea2::Json("name", "test16__")), "test16_");

            rea2::pipeline::instance()->run<double>("test16", 66, "test16");
        )
    }

    //test17;test18;test20;test21
    {

    rea2::m_tests.insert("test17", [](){
        rea2::pipeline::instance()->add<double, rea2::pipePartial>([](rea2::stream<double>* aInput){
            assert(aInput->data() == 66);
            aInput->setData(77)->out();
        }, rea2::Json("name", "test17", "external", "js"));
    });

    rea2::m_tests.insert("test18", [](){
        rea2::pipeline::instance()->add<int, rea2::pipeDelegate>([](rea2::stream<int>* aInput){
            assert(aInput->data() == 66);
            aInput->out();
        }, rea2::Json("name", "test18_0", "delegate", "test18"))
        ->next("testSuccess");

        rea2::pipeline::instance()->add<int>([](rea2::stream<int>* aInput){
            assert(aInput->data() == 56);
            aInput->outs<QString>("Pass: test18", "testSuccess");
        }, rea2::Json("name", "test18"));

        rea2::pipeline::instance()->run<int>("test18_0", 66);
        rea2::pipeline::instance()->run<int>("test18", 56);
    });

    rea2::m_tests.insert("test20", [](){
        rea2::pipeline::instance()->add<double, rea2::pipeDelegate>([](rea2::stream<double>* aInput){
            assert(aInput->data() == 66.0);
            aInput->out();
        }, rea2::Json("name", "test20_0", "delegate", "test20"))
            ->nextB("testSuccess")
            ->next("testSuccessJS");

        rea2::pipeline::instance()->run<double>("test20_0", 66);
        rea2::pipeline::instance()->run<double>("test20", 56);
    });

    rea2::m_tests.insert("test21", [](){
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            assert(aInput->data() == 56.0);
            aInput->outs<QString>("Pass: test21");
        }, rea2::Json("name", "test21", "external", "js"));
    });

    }

    //test22;test24;test25;test26
    {
    rea2::m_tests.insert("test22", [](){
        rea2::in<int>(0, "test22")
            ->asyncCallF<int>([](rea2::stream<int>* aInput){
                aInput->setData(aInput->data() + 1)->out();
            }, rea2::Json("thread", 2))
            ->asyncCallF<QString>([](rea2::stream<int>* aInput){
                assert(aInput->data() == 1);
                aInput->outs<QString>("world");
            }, rea2::Json("thread", 3))
            ->asyncCallF<QString>([](rea2::stream<QString>* aInput){
                assert(aInput->data() == "world");
                aInput->setData("Pass: test22")->out();
            })
            ->asyncCall("testSuccess");
    });

    rea2::m_tests.insert("test24", [](){
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            assert(aInput->data() == 24.0);
            aInput->outs<QString>("Pass: test24");
        }, rea2::Json("name", "test24", "thread", 5, "external", "js"));
    });

    rea2::m_tests.insert("test25_", [](){
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            rea2::in<double>(25, "test25")
                ->asyncCall<QString>("test25", false, "c++", true)
                ->asyncCall("testSuccess", false);
        }, rea2::Json("name", "test25_", "thread", 4));
    });

    rea2::m_tests.insert("test25", [](){
        rea2::pipeline::instance()->run<double>("test25_", 0);
    });

    rea2::m_tests.insert("test26", [](){
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 1.0);
            aInput->setData(dt + 1)->out();
        }, rea2::Json("name", "test__26", "before", "test_26", "replace", true));

        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 2.0);
            aInput->setData(dt + 1)->out();
        }, rea2::Json("name", "test_26", "before", "test26", "replace", true));

        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 3.0);
            aInput->setData(dt + 1)->out();
        }, rea2::Json("name", "test26", "replace", true));

        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 4.0);
            aInput->setData(dt + 1)->out();
        }, rea2::Json("name", "test26_", "after", "test26", "replace", true));

        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 5.0);
            aInput->outs<QString>("Pass: test26", "testSuccess");
        }, rea2::Json("name", "test26__", "after", "test26_", "replace", true));

        rea2::pipeline::instance()->run<double>("test26", 1);
    });

    }

    //test27;test28;test29;test30
    {
    rea2::m_tests.insert("test27", [](){
        auto tmp = foo(6);
        //rea2::pipeline::instance()->add<int>(foo(2));
        rea2::in<int>(3)
            ->asyncCallF<int>(foo(2))
            ->asyncCallF<int>(std::bind1st(std::mem_fun(&foo::memberFoo), &tmp))
            ->asyncCallF<QString>([](rea2::stream<int>* aInput){
                assert(aInput->data() == 11);
                aInput->outs<QString>("Pass: test27")->out();
            })
            ->asyncCall("testSuccess");
    });

    rea2::m_tests.insert("test28", [](){
        rea2::pipeline::instance()->add<QJsonObject>([](rea2::stream<QJsonObject>* aInput){
            aInput->scope()->cache<QString>("test28", "test28");
            aInput->out();
        }, rea2::Json("name", "test28"))->next("test28_");

        rea2::pipeline::instance()->add<QJsonObject>([](rea2::stream<QJsonObject>* aInput){
            assert(aInput->data().value("test28") == "test28");
           // assert(aInput->scope()->data<QString>("test28") == "test28_");
            aInput->outs<QString>("Pass: test28", "testSuccess");
        }, rea2::Json("name", "test28_0"))
            ->next("testSuccess");

        rea2::pipeline::instance()->run("test28", rea2::Json("test28", "test28"));
    });

    rea2::m_tests.insert("test29", [this](){
        auto sp = std::make_shared<rea2::scopeCache>();
        sp->cache<JsContext*>("ctx", m_jsContext);
        rea2::pipeline::instance()->run<JsContext*>("test29", m_jsContext, "", sp);
    });

    rea2::m_tests.insert("test30_", [](){
        static std::mutex mtx;
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                aInput->scope()->cache<std::shared_ptr<QSet<QThread*>>>("threads", std::make_shared<QSet<QThread*>>())
                ->cache<int>("count", 0);
            for (int i = 0; i < 200; ++i)
                aInput->outs<double>(i);
        }, rea2::Json("name", "test30"))
            ->nextP(rea2::pipeline::instance()->add<double, rea2::pipeParallel>([](rea2::stream<double>* aInput){
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                {
                    std::lock_guard<std::mutex> gd(mtx);
                    auto trds = aInput->scope()->data<std::shared_ptr<QSet<QThread*>>>("threads");
                    trds->insert(QThread::currentThread());
                    aInput->scope()->cache<int>("count", aInput->scope()->data<int>("count") + 1);
                }
                aInput->out();
            }, rea2::Json("name", "test30_")))
            ->nextF<double>([](rea2::stream<double>* aInput){
                std::lock_guard<std::mutex> gd(mtx);
                auto cnt = aInput->scope()->data<int>("count");
                if (cnt == 200){
                    aInput->scope()->cache<int>("count", cnt + 1);
                    assert(aInput->scope()->data<std::shared_ptr<QSet<QThread*>>>("threads")->size() == 8);
                    aInput->outs<QString>("Pass: test30", "testSuccess");
                }
            }, "test30", rea2::Json("name", "test30__"));
    });

    rea2::m_tests.insert("test30", [](){
        rea2::pipeline::instance()->run<double>("test30", 0, "test30");
    });

    }

    //test34;test35;test36;test37
    {
    rea2::m_tests.insert("test34",[](){
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            assert(aInput->data() == 4.0);
            assert(aInput->scope()->data<QString>("hello") == "world");
            aInput->setData(aInput->data() + 1)->out();
        }, rea2::Json("name", "test34_", "external", "qml"));

        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
            assert(aInput->data() == 5.0);
            aInput->scope(true)->cache<QString>("hello2", "world");
            aInput->setData(aInput->data() + 1)->out();
        }, rea2::Json("name", "test34__", "external", "qml"));
    });

    addTest(test35,
            rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
                assert(aInput->data() == "hello");
                aInput->setData("world")->out();
            }, rea2::Json("name", "test35", "external", "qml"));
            )

    rea2::m_tests.insert("test36",[](){
        rea2::pipeline::instance()->find("test36__")->removeNext("test_36");
        rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                                      aInput->scope()->cache<QString>("hello", "world");
                                      aInput->out();
                                  }, rea2::Json("name", "test36"))
            ->next("test36_")
            ->next("test36__")
            ->nextF<double>([](rea2::stream<double>* aInput){
                assert(aInput->data() == 6.0);
                assert(aInput->scope()->data<QString>("hello") == "");
                assert(aInput->scope()->data<QString>("hello2") == "world");
                aInput->outs<QString>("Pass: test36", "testSuccess");
            }, "", rea2::Json("name", "test_36"));

        rea2::pipeline::instance()->run<double>("test36", 4);
    });

    addTest(test37,
            rea2::pipeline::instance()->find("test37")->removeNext("test_37");
            rea2::pipeline::instance()->find("test37")
                ->nextF<QString>([](rea2::stream<QString>* aInput){
                    assert(aInput->data() == "world");
                    aInput->outs<QString>("Pass: test37", "testSuccess");
                }, "", rea2::Json("name", "test_37"));

            rea2::pipeline::instance()->run<QString>("test37", "hello");
            )
    }

    //test39;test40;test42;test43
    {
        rea2::m_tests.insert("test39", [](){
            rea2::pipeline::instance()->add<double, rea2::pipePartial>([](rea2::stream<double>* aInput){
                assert(aInput->data() == 66);
                aInput->setData(77)->out();
            }, rea2::Json("name", "test39", "external", "qml"));
        });

        addTest(test40,
            rea2::pipeline::instance()->find("test40")->removeNext("test40_");
            rea2::pipeline::instance()->find("test40")->removeNext("test40__");

            rea2::pipeline::instance()->find("test40")
                ->nextPB(rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                    assert(aInput->data() == 77.0);
                    aInput->outs<QString>("Pass: test40", "testSuccess");
                }, rea2::Json("name", "test40_")), "test40")
                ->nextP(rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                    assert(aInput->data() == 77.0);
                    aInput->outs<QString>("Fail: test40", "testFail");
                }, rea2::Json("name", "test40__")), "test40_");

            rea2::pipeline::instance()->run<double>("test40", 66, "test40");
            )

        rea2::m_tests.insert("test42", [](){
            rea2::pipeline::instance()->add<double, rea2::pipeDelegate>([](rea2::stream<double>* aInput){
                                          assert(aInput->data() == 66.0);
                                          aInput->out();
                                      }, rea2::Json("name", "test42_0", "delegate", "test42"))
                ->nextB("testSuccess")
                ->next("testSuccessQML");

            rea2::pipeline::instance()->run<double>("test42_0", 66);
            rea2::pipeline::instance()->run<double>("test42", 56);
        });

        rea2::m_tests.insert("test43", [](){
            rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                assert(aInput->data() == 56.0);
                aInput->outs<QString>("Pass: test43");
            }, rea2::Json("name", "test43", "external", "qml"));
        });
    }

    //test45;test46;test50;test52
    {
        rea2::m_tests.insert("test45", [](){
            rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                assert(aInput->data() == 24.0);
                aInput->outs<QString>("Pass: test45");
            }, rea2::Json("name", "test45", "external", "qml"));
        });

        rea2::m_tests.insert("test46", [](){
            rea2::in<double>(25, "test46")
                ->asyncCall<QString>("test46", true, "c++", true)
                ->asyncCall("testSuccess");
        });

        rea2::m_tests.insert("test50", [](){
            rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
                assert(aInput->data() == 1.0);
                aInput->out();
            }, rea2::Json("name", "test50_c", "external", "js"));
        });

        rea2::m_tests.insert("test52_", [](){
            static int test52_cnt = 0;
            rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
                QFile fl("F:/3M/config_.json");
                if (fl.open(QFile::ReadOnly)){
                    QJsonDocument doc = QJsonDocument::fromJson(fl.readAll());
                    fl.close();
                }
                if (aInput->data() == "0")
                    aInput->setData("2")->out();
                else if (aInput->data() == "1")
                    aInput->setData("3")->out();
            }, rea2::Json("name", "test52__",
                         "thread", 22))
            ->nextF<QString>([](rea2::stream<QString>* aInput){
                assert(aInput->data() == "2");
                if (++test52_cnt % 2000 == 0){
                    aInput->outs<QString>("Pass: test52", "testSuccess");
                }
            });
            rea2::pipeline::instance()->add<QString, rea2::pipeParallel>(nullptr, rea2::Json("name", "test52___",
                                                                                          "delegate", "test52__"))
                    ->nextF<QString>([](rea2::stream<QString>* aInput){
                assert(aInput->data() == "3");
                if (++test52_cnt % 2000 == 0){
                    aInput->outs<QString>("Pass: test52", "testSuccess");
                }
            }, "lala");
        });

        rea2::m_tests.insert("test52", [](){
            for (int i = 0; i < 1000; i++)
                rea2::pipeline::instance()->run<QString>("test52__", "0");
            for (int i = 0; i < 1000; i++)
                rea2::pipeline::instance()->run<QString>("test52___", "1", "lala");
        });
    }

    rea2::test("test4");
    rea2::test("test5");
    rea2::test("test9");
    rea2::test("test12_");
    rea2::test("test17");
    rea2::test("test21");
    rea2::test("test24");
    rea2::test("test25_");
    rea2::test("test30_");
    rea2::test("test34");
    rea2::test("test35");
    rea2::test("test39");
    rea2::test("test43");
    rea2::test("test45");
    rea2::test("test50");
    rea2::test("test52_");

    rea2::pipeline::instance()->add<QString>([](rea2::stream<QString>* aInput){
        QString dt = "data:image/png;base64, " + rea2::QImage2Base64(QImage("F:/3M/微信图片_20200916112142.png"));
        aInput->setData(dt)->out();
    }, rea2::Json("name", "unitTestImage", "external", "js"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

JsContext::JsContext(QObject*){

}

void JsContext::sendMsg(QWebEnginePage* page, const QString& msg){
    page->runJavaScript(QString("recvMessage('%1');").arg(msg));
}

void JsContext::onMsg(const QJsonValue &msg)
{
    if (msg.isString())
        emit recvdMsg(msg.toString());
}

void JsContext::onMsg2(const QJsonValue &msg)
{
    if (msg.isString())
        emit recvdMsg(msg.toString());
}

static MainWindow* wd = nullptr;
static rea2::regPip<double> reg_web([](rea2::stream<double>* aInput){
    if (!wd)
        wd = new MainWindow();
    wd->show();
    aInput->out();
}, rea2::Json("name", "openWebWindow", "external", "qml"));

static rea2::regPip<double> ureg_web([](rea2::stream<double>* aInput){
    if (wd){  //memory leak in debug mode; ref: https://forum.qt.io/topic/93582/qwebengine-process-termination-issue/13
        delete wd;
        wd = nullptr;
    }
}, rea2::Json("name", "unloadMain"));
