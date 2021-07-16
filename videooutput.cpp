#include "rea.h"
#include "videooutput.h"
#include <QQmlApplicationEngine>

void FrameProvider::onNewVideoContentReceived(const QVideoFrame &frame)
{
    if (m_surface)
        m_surface->present(frame);
}

static rea::regPip<QQmlApplicationEngine*> reg_imageboard([](rea::stream<QQmlApplicationEngine*>* aInput){
    std::vector<QImage> imgs;
    //for (auto i = 0; i < 10; ++i)
    //    imgs.push_back(QImage("F:/3M/ttt/" + QString::number(i) + ".png"));
    if (!imgs.size())
        return;
    FrameProvider* provider = new FrameProvider();
    auto source = std::make_shared<CustomFramesource>(imgs[0].width(), imgs[0].height(), QVideoFrame::Format_ARGB32);
    auto src = source.get();
    // Set the correct format for the video surface (Make sure your selected format is supported by the surface)
    provider->setFormat(source->width(),source->height(), source->format());

    // Connect your frame source with the provider
    QObject::connect(src, SIGNAL(newFrameAvailable(const QVideoFrame &)), provider, SLOT(onNewVideoContentReceived(const QVideoFrame &)));

    rea::pipeline::instance()->add<QString>([source, imgs](rea::stream<QString>* aInput){
        int idx = 0;
        for (auto j = 0; j < 100; ++j){
            for (auto i : imgs){
                auto frm = std::make_shared<QVideoFrame>(i);
                frm->setStartTime(idx++);
                //frm->unmap();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                emit source->newFrameAvailable(*frm);
            }
        }
    }, rea::Json("name", "testFrameProvider", "thread", 40));

    aInput->data()->rootContext()->setContextProperty("frameProvider", provider);

    aInput->out();
}, rea::Json("name", "install1_VideoOutput"), "initRea");
