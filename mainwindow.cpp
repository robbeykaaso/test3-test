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

namespace rea {

static QHash<QString, std::function<void(void)>> m_tests;

void test(const QString& aName){
    if (m_tests.contains(aName))
        m_tests.value(aName)();
}

#define addTest(aName, aTest) \
    rea::m_tests.insert(#aName, [](){aTest});

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
    m_webChannel->registerObject("Pipelinec++", rea::pipeline::instance("js"));
    m_webChannel->registerObject("Pipelineqml", rea::pipeline::instance("qmljs"));
    m_webView->page()->setWebChannel(m_webChannel);
    m_webView->setUrl(QApplication::applicationDirPath() + "/html/test.html");

    rea::pipeline::instance()->supportType<JsContext*>([](rea::stream0* aInput){
        return QVariant::fromValue<QObject*>(reinterpret_cast<rea::stream<JsContext*>*>(aInput)->data());
    });
    m_jsContext = new JsContext();
    connect(m_jsContext, &JsContext::recvdMsg, this, [this](const QString& msg) {
        rea::pipeline::instance()->run<QString>("testSuccess", "Pass: test29");
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
    void operator()(rea::stream<int>* aInput) {
        auto ret = aInput->data() + m_start;
        aInput->setData(ret)->out();
    }

    void memberFoo(rea::stream<int>* aInput){
        auto ret = aInput->data() + m_start;
        aInput->setData(ret)->out();
    }
private:
    int m_start;
};

namespace rea {

template <typename T, typename F>
class pipeCustomQML : public rea::pipe<T, F> {
public:
    pipeCustomQML(rea::pipeline* aParent, const QString& aName, int aThreadNo = 0) : rea::pipe<T, F>(aParent, aName, aThreadNo) {

    }
protected:
    bool event( QEvent* e) override{
        if(e->type()== rea::pipe0::streamEvent::type){
            auto eve = reinterpret_cast<rea::pipe0::streamEvent*>(e);
            if (eve->getName() == rea::pipe0::m_name){
                auto stm = std::dynamic_pointer_cast<rea::stream<T>>(eve->getStream());
                stm->scope()->template cache<QString>("flag", "test48");
                doEvent(stm);
                doNextEvent(rea::pipe0::m_next, stm);
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

    rea::pipeline::instance()->add<QJsonObject>([this](rea::stream<QJsonObject>* aInput){
        auto dt = aInput->data();
        for (auto i : dt.keys())
            test_sum += dt.value(i).toInt();
        for (auto i : dt.keys())
            rea::test(i);
        auto sw = "Received message: hello";
        ui->statusBar->showMessage(sw);
        aInput->out();
    }, rea::Json("name", "unitTestC++", "external", "js"));

    rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
        test_pass++;
        std::cout << QString("Success: %1 (%2/%3)")
                         .arg(aInput->data())
                         .arg(test_pass)
                         .arg(test_sum)
                         .toStdString() << std::endl;
        aInput->out();
    }, rea::Json("name", "testSuccess"));

    rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
        test_pass--;
        std::cout << QString("Fail: %1 (%2/%3)")
                         .arg(aInput->data())
                         .arg(test_pass)
                         .arg(test_sum)
                         .toStdString() << std::endl;
        aInput->out();
    }, rea::Json("name", "testFail"));

    }

    //test4;test5;test6;test7
    {

    rea::m_tests.insert("test4",[](){
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            assert(aInput->data() == 4.0);
            assert(aInput->scope()->data<QString>("hello") == "world");
            aInput->setData(aInput->data() + 1)->out();
        }, rea::Json("name", "test4_", "external", "js"));

        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            assert(aInput->data() == 5.0);
            aInput->scope(true)->cache<QString>("hello2", "world");
            aInput->setData(aInput->data() + 1)->out();
        }, rea::Json("name", "test4__", "external", "js"));
    });

    addTest(test5,
            rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
                assert(aInput->data() == "hello");
                aInput->setData("world")->out();
            }, rea::Json("name", "test5", "external", "js"));
        )

    rea::m_tests.insert("test6",[](){
        rea::pipeline::instance()->find("test6_")->removeNext("test6__");
        rea::pipeline::instance()->find("test6__")->removeNext("test_6");
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            aInput->scope()->cache<QString>("hello", "world");
            aInput->out();
        }, rea::Json("name", "test6"))
            ->next("test6_")
            ->next("test6__")
            ->nextF<double>([](rea::stream<double>* aInput){
                assert(aInput->data() == 6.0);
                assert(aInput->scope()->data<QString>("hello") == "");
                assert(aInput->scope()->data<QString>("hello2") == "world");
                aInput->outs<QString>("Pass: test6", "testSuccess");
            }, "", rea::Json("name", "test_6"));

        rea::pipeline::instance()->run<double>("test6", 4);
    });

    addTest(test7,
            rea::pipeline::instance()->find("test7")->removeNext("test_7");
            rea::pipeline::instance()->find("test7")
                ->nextF<QString>([](rea::stream<QString>* aInput){
                    assert(aInput->data() == "world");
                    aInput->outs<QString>("Pass: test7", "testSuccess");
                }, "", rea::Json("name", "test_7"));

            rea::pipeline::instance()->run<QString>("test7", "hello");
        )

    }

    //test8;test9;test11;test12
    {
    addTest(test8,
            rea::pipeline::instance()->run<QString>("test8", "hello");
        )

    addTest(test9,
            rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
                assert(aInput->data() == "hello");
                aInput->outs<QString>("Pass: test9", "testSuccess");
            }, rea::Json("name", "test9", "external", "js"));
        )

    addTest(test11,
            auto pip = rea::pipeline::instance()->add<int>([](rea::stream<int>* aInput){
                assert(aInput->data() == 3);
                aInput->outs<QString>("Pass: test11", "testSuccess");
            });
            rea::pipeline::instance()->add<int>([](rea::stream<int>* aInput){
                assert(aInput->data() == 3);
                aInput->out();
            }, rea::Json("name", "test11"))
                ->nextP(pip)
                ->next("testSuccess");

            rea::pipeline::instance()->run<int>("test11", 3);
            rea::pipeline::instance()->remove(pip->actName());
        )

        rea::m_tests.insert("test12_",[](){
            rea::pipeline::instance()->add<int>([](rea::stream<int>* aInput){
                assert(aInput->data() == 4);
                std::stringstream ss;
                ss << std::this_thread::get_id();
                aInput->outs<std::string>(ss.str(), "test12_0");
            }, rea::Json("name", "test12"))
                ->nextP(rea::pipeline::instance()->add<std::string>([](rea::stream<std::string>* aInput){
                    std::stringstream ss;
                    ss << std::this_thread::get_id();
                    assert(ss.str() != aInput->data());
                    aInput->outs<QString>("Pass: test12", "testSuccess");
                }, rea::Json("name", "test12_0", "thread", 7)))
                ->next("testSuccess");
        });

        rea::m_tests.insert("test12",[](){
            rea::pipeline::instance()->run<int>("test12", 4);
        });

    }

    //test13;test14;test16
    {
    addTest(test13,
            rea::pipeline::instance()->add<int>([](rea::stream<int>* aInput){
                assert(aInput->data() == 66);
                aInput->outs<QString>("test13", "test13_0");
            }, rea::Json("name", "test13"))
                ->next("test13_0")
                ->next("testSuccess");

            rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
                aInput->out();
            }, rea::Json("name", "test13_1"))
                ->next("test13__")
                ->next("testSuccess");

            rea::pipeline::instance()->find("test13_0")
                ->nextF<QString>([](rea::stream<QString>* aInput){
                    aInput->out();
                }, "", rea::Json("name", "test13__"))
                ->next("testSuccess");

            rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
                assert(aInput->data() == "test13");
                aInput->outs<QString>("Pass: test13", "testSuccess");
                aInput->outs<QString>("Pass: test13_", "test13__");
            }, rea::Json("name", "test13_0"));

            rea::pipeline::instance()->run<int>("test13", 66);
            rea::pipeline::instance()->run<QString>("test13_1", "Pass: test13__");
        )

    rea::m_tests.insert("test14", [](){
        rea::pipeline::instance()->add<int, rea::pipePartial>([](rea::stream<int>* aInput){
            assert(aInput->data() == 66);
            aInput->setData(77)->out();
        }, rea::Json("name", "test14"))
            ->nextPB(rea::pipeline::instance()->add<int>([](rea::stream<int>* aInput){
                        assert(aInput->data() == 77);
                        aInput->outs<QString>("Pass: test14", "testSuccess");
                    }, rea::Json("name", "test14_")), "test14")
            ->nextP(rea::pipeline::instance()->add<int>([](rea::stream<int>* aInput){
                       assert(aInput->data() == 77);
                       aInput->outs<QString>("Fail: test14", "testFail");
                   }, rea::Json("name", "test14__")), "test14_");

            rea::pipeline::instance()->run<int>("test14", 66, "test14");
        });

    addTest(test16,
            rea::pipeline::instance()->find("test16")->removeNext("test16_");
            rea::pipeline::instance()->find("test16")->removeNext("test16__");

            rea::pipeline::instance()->find("test16")
                ->nextPB(rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                            assert(aInput->data() == 77.0);
                            aInput->outs<QString>("Pass: test16", "testSuccess");
                        }, rea::Json("name", "test16_")), "test16")
                ->nextP(rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                           assert(aInput->data() == 77.0);
                           aInput->outs<QString>("Fail: test16", "testFail");
                       }, rea::Json("name", "test16__")), "test16_");

            rea::pipeline::instance()->run<double>("test16", 66, "test16");
        )
    }

    //test17;test18;test20;test21
    {

    rea::m_tests.insert("test17", [](){
        rea::pipeline::instance()->add<double, rea::pipePartial>([](rea::stream<double>* aInput){
            assert(aInput->data() == 66);
            aInput->setData(77)->out();
        }, rea::Json("name", "test17", "external", "js"));
    });

    rea::m_tests.insert("test18", [](){
        rea::pipeline::instance()->add<int, rea::pipeDelegate>([](rea::stream<int>* aInput){
            assert(aInput->data() == 66);
            aInput->out();
        }, rea::Json("name", "test18_0", "delegate", "test18"))
        ->next("testSuccess");

        rea::pipeline::instance()->add<int>([](rea::stream<int>* aInput){
            assert(aInput->data() == 56);
            aInput->outs<QString>("Pass: test18", "testSuccess");
        }, rea::Json("name", "test18"));

        rea::pipeline::instance()->run<int>("test18_0", 66);
        rea::pipeline::instance()->run<int>("test18", 56);
    });

    rea::m_tests.insert("test20", [](){
        rea::pipeline::instance()->add<double, rea::pipeDelegate>([](rea::stream<double>* aInput){
            assert(aInput->data() == 66.0);
            aInput->out();
        }, rea::Json("name", "test20_0", "delegate", "test20"))
            ->nextB("testSuccess")
            ->next("testSuccessJS");

        rea::pipeline::instance()->run<double>("test20_0", 66);
        rea::pipeline::instance()->run<double>("test20", 56);
    });

    rea::m_tests.insert("test21", [](){
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            assert(aInput->data() == 56.0);
            aInput->outs<QString>("Pass: test21");
        }, rea::Json("name", "test21", "external", "js"));
    });

    }

    //test22;test24;test25;test26
    {
    rea::m_tests.insert("test22", [](){
        rea::in<int>(0, "test22")
            ->asyncCallF<int>([](rea::stream<int>* aInput){
                aInput->setData(aInput->data() + 1)->out();
            }, rea::Json("thread", 2))
            ->asyncCallF<QString>([](rea::stream<int>* aInput){
                assert(aInput->data() == 1);
                aInput->outs<QString>("world");
            }, rea::Json("thread", 3))
            ->asyncCallF<QString>([](rea::stream<QString>* aInput){
                assert(aInput->data() == "world");
                aInput->setData("Pass: test22")->out();
            })
            ->asyncCall("testSuccess");
    });

    rea::m_tests.insert("test24", [](){
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            assert(aInput->data() == 24.0);
            aInput->outs<QString>("Pass: test24");
        }, rea::Json("name", "test24", "thread", 5, "external", "js"));
    });

    rea::m_tests.insert("test25_", [](){
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            rea::in<double>(25, "test25")
                ->asyncCall<QString>("test25", false, "c++", true)
                ->asyncCall("testSuccess", false);
        }, rea::Json("name", "test25_", "thread", 4));
    });

    rea::m_tests.insert("test25", [](){
        rea::pipeline::instance()->run<double>("test25_", 0);
    });

    rea::m_tests.insert("test26", [](){
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 1.0);
            aInput->setData(dt + 1)->out();
        }, rea::Json("name", "test__26", "before", "test_26", "replace", true));

        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 2.0);
            aInput->setData(dt + 1)->out();
        }, rea::Json("name", "test_26", "before", "test26", "replace", true));

        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 3.0);
            aInput->setData(dt + 1)->out();
        }, rea::Json("name", "test26", "replace", true));

        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 4.0);
            aInput->setData(dt + 1)->out();
        }, rea::Json("name", "test26_", "after", "test26", "replace", true));

        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            auto dt = aInput->data();
            assert(dt == 5.0);
            aInput->outs<QString>("Pass: test26", "testSuccess");
        }, rea::Json("name", "test26__", "after", "test26_", "replace", true));

        rea::pipeline::instance()->run<double>("test26", 1);
    });

    }

    //test27;test28;test29;test30
    {
    rea::m_tests.insert("test27", [](){
        auto tmp = foo(6);
        //rea::pipeline::instance()->add<int>(foo(2));
        rea::in<int>(3)
            ->asyncCallF<int>(foo(2))
            ->asyncCallF<int>(std::bind1st(std::mem_fun(&foo::memberFoo), &tmp))
            ->asyncCallF<QString>([](rea::stream<int>* aInput){
                assert(aInput->data() == 11);
                aInput->outs<QString>("Pass: test27")->out();
            })
            ->asyncCall("testSuccess");
    });

    rea::m_tests.insert("test28", [](){
        rea::pipeline::instance()->add<QJsonObject>([](rea::stream<QJsonObject>* aInput){
            aInput->scope()->cache<QString>("test28", "test28");
            aInput->out();
        }, rea::Json("name", "test28"))->next("test28_");

        rea::pipeline::instance()->add<QJsonObject>([](rea::stream<QJsonObject>* aInput){
            assert(aInput->data().value("test28") == "test28");
           // assert(aInput->scope()->data<QString>("test28") == "test28_");
            aInput->outs<QString>("Pass: test28", "testSuccess");
        }, rea::Json("name", "test28_0"))
            ->next("testSuccess");

        rea::pipeline::instance()->run("test28", rea::Json("test28", "test28"));
    });

    rea::m_tests.insert("test29", [this](){
        auto sp = std::make_shared<rea::scopeCache>();
        sp->cache<JsContext*>("ctx", m_jsContext);
        rea::pipeline::instance()->run<JsContext*>("test29", m_jsContext, "", sp);
    });

    rea::m_tests.insert("test30_", [](){
        static std::mutex mtx;
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                aInput->scope()->cache<std::shared_ptr<QSet<QThread*>>>("threads", std::make_shared<QSet<QThread*>>())
                ->cache<int>("count", 0);
            for (int i = 0; i < 200; ++i)
                aInput->outs<double>(i);
        }, rea::Json("name", "test30"))
            ->nextP(rea::pipeline::instance()->add<double, rea::pipeParallel>([](rea::stream<double>* aInput){
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                {
                    std::lock_guard<std::mutex> gd(mtx);
                    auto trds = aInput->scope()->data<std::shared_ptr<QSet<QThread*>>>("threads");
                    trds->insert(QThread::currentThread());
                    aInput->scope()->cache<int>("count", aInput->scope()->data<int>("count") + 1);
                }
                aInput->out();
            }, rea::Json("name", "test30_")))
            ->nextF<double>([](rea::stream<double>* aInput){
                std::lock_guard<std::mutex> gd(mtx);
                auto cnt = aInput->scope()->data<int>("count");
                if (cnt == 200){
                    aInput->scope()->cache<int>("count", cnt + 1);
                    assert(aInput->scope()->data<std::shared_ptr<QSet<QThread*>>>("threads")->size() == 8);
                    aInput->outs<QString>("Pass: test30", "testSuccess");
                }
            }, "test30", rea::Json("name", "test30__"));
    });

    rea::m_tests.insert("test30", [](){
        rea::pipeline::instance()->run<double>("test30", 0, "test30");
    });

    }

    //test34;test35;test36;test37
    {
    rea::m_tests.insert("test34",[](){
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            assert(aInput->data() == 4.0);
            assert(aInput->scope()->data<QString>("hello") == "world");
            aInput->setData(aInput->data() + 1)->out();
        }, rea::Json("name", "test34_", "external", "qml"));

        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
            assert(aInput->data() == 5.0);
            aInput->scope(true)->cache<QString>("hello2", "world");
            aInput->setData(aInput->data() + 1)->out();
        }, rea::Json("name", "test34__", "external", "qml"));
    });

    addTest(test35,
            rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
                assert(aInput->data() == "hello");
                aInput->setData("world")->out();
            }, rea::Json("name", "test35", "external", "qml"));
            )

    rea::m_tests.insert("test36",[](){
        rea::pipeline::instance()->find("test36__")->removeNext("test_36");
        rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                                      aInput->scope()->cache<QString>("hello", "world");
                                      aInput->out();
                                  }, rea::Json("name", "test36"))
            ->next("test36_")
            ->next("test36__")
            ->nextF<double>([](rea::stream<double>* aInput){
                assert(aInput->data() == 6.0);
                assert(aInput->scope()->data<QString>("hello") == "");
                assert(aInput->scope()->data<QString>("hello2") == "world");
                aInput->outs<QString>("Pass: test36", "testSuccess");
            }, "", rea::Json("name", "test_36"));

        rea::pipeline::instance()->run<double>("test36", 4);
    });

    addTest(test37,
            rea::pipeline::instance()->find("test37")->removeNext("test_37");
            rea::pipeline::instance()->find("test37")
                ->nextF<QString>([](rea::stream<QString>* aInput){
                    assert(aInput->data() == "world");
                    aInput->outs<QString>("Pass: test37", "testSuccess");
                }, "", rea::Json("name", "test_37"));

            rea::pipeline::instance()->run<QString>("test37", "hello");
            )
    }

    //test39;test40;test42;test43
    {
        rea::m_tests.insert("test39", [](){
            rea::pipeline::instance()->add<double, rea::pipePartial>([](rea::stream<double>* aInput){
                assert(aInput->data() == 66);
                aInput->setData(77)->out();
            }, rea::Json("name", "test39", "external", "qml"));
        });

        addTest(test40,
            rea::pipeline::instance()->find("test40")->removeNext("test40_");
            rea::pipeline::instance()->find("test40")->removeNext("test40__");

            rea::pipeline::instance()->find("test40")
                ->nextPB(rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                    assert(aInput->data() == 77.0);
                    aInput->outs<QString>("Pass: test40", "testSuccess");
                }, rea::Json("name", "test40_")), "test40")
                ->nextP(rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                    assert(aInput->data() == 77.0);
                    aInput->outs<QString>("Fail: test40", "testFail");
                }, rea::Json("name", "test40__")), "test40_");

            rea::pipeline::instance()->run<double>("test40", 66, "test40");
            )

        rea::m_tests.insert("test42", [](){
            rea::pipeline::instance()->add<double, rea::pipeDelegate>([](rea::stream<double>* aInput){
                                          assert(aInput->data() == 66.0);
                                          aInput->out();
                                      }, rea::Json("name", "test42_0", "delegate", "test42"))
                ->nextB("testSuccess")
                ->next("testSuccessQML");

            rea::pipeline::instance()->run<double>("test42_0", 66);
            rea::pipeline::instance()->run<double>("test42", 56);
        });

        rea::m_tests.insert("test43", [](){
            rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                assert(aInput->data() == 56.0);
                aInput->outs<QString>("Pass: test43");
            }, rea::Json("name", "test43", "external", "qml"));
        });
    }

    //test45;test46;test50;test52
    {
        rea::m_tests.insert("test45", [](){
            rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                assert(aInput->data() == 24.0);
                aInput->outs<QString>("Pass: test45");
            }, rea::Json("name", "test45", "external", "qml"));
        });

        rea::m_tests.insert("test46", [](){
            rea::in<double>(25, "test46")
                ->asyncCall<QString>("test46", true, "c++", true)
                ->asyncCall("testSuccess");
        });

        rea::m_tests.insert("test50", [](){
            rea::pipeline::instance()->add<double>([](rea::stream<double>* aInput){
                assert(aInput->data() == 1.0);
                aInput->out();
            }, rea::Json("name", "test50_c", "external", "js"));
        });

        rea::m_tests.insert("test52_", [](){
            static int test52_cnt = 0;
            rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
                QFile fl("F:/3M/config_.json");
                if (fl.open(QFile::ReadOnly)){
                    QJsonDocument doc = QJsonDocument::fromJson(fl.readAll());
                    fl.close();
                }
                if (aInput->data() == "0")
                    aInput->setData("2")->out();
                else if (aInput->data() == "1")
                    aInput->setData("3")->out();
            }, rea::Json("name", "test52__",
                         "thread", 22))
            ->nextF<QString>([](rea::stream<QString>* aInput){
                assert(aInput->data() == "2");
                if (++test52_cnt % 2000 == 0){
                    aInput->outs<QString>("Pass: test52", "testSuccess");
                }
            });
            rea::pipeline::instance()->add<QString, rea::pipeParallel>(nullptr, rea::Json("name", "test52___",
                                                                                          "delegate", "test52__"))
                    ->nextF<QString>([](rea::stream<QString>* aInput){
                assert(aInput->data() == "3");
                if (++test52_cnt % 2000 == 0){
                    aInput->outs<QString>("Pass: test52", "testSuccess");
                }
            }, "lala");
        });

        rea::m_tests.insert("test52", [](){
            for (int i = 0; i < 1000; i++)
                rea::pipeline::instance()->run<QString>("test52__", "0");
            for (int i = 0; i < 1000; i++)
                rea::pipeline::instance()->run<QString>("test52___", "1", "lala");
        });
    }

    rea::test("test4");
    rea::test("test5");
    rea::test("test9");
    rea::test("test12_");
    rea::test("test17");
    rea::test("test21");
    rea::test("test24");
    rea::test("test25_");
    rea::test("test30_");
    rea::test("test34");
    rea::test("test35");
    rea::test("test39");
    rea::test("test43");
    rea::test("test45");
    rea::test("test50");
    rea::test("test52_");

    rea::pipeline::instance()->add<QString>([](rea::stream<QString>* aInput){
        QString dt = "data:image/png;base64, " + rea::QImage2Base64(QImage("F:/3M/微信图片_20200916112142.png"));
        aInput->setData(dt)->out();
    }, rea::Json("name", "unitTestImage", "external", "js"));
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
static rea::regPip<double> reg_web([](rea::stream<double>* aInput){
    if (!wd)
        wd = new MainWindow();
    wd->show();
    aInput->out();
}, rea::Json("name", "openWebWindow", "external", "qml"));

static rea::regPip<double> ureg_web([](rea::stream<double>* aInput){
    if (wd){  //memory leak in debug mode; ref: https://forum.qt.io/topic/93582/qwebengine-process-termination-issue/13
        delete wd;
        wd = nullptr;
    }
}, rea::Json("name", "unloadMain"));
