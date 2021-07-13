#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
//https://www.mycode.net.cn/language/cpp/2719.html

class FrameProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface)
public:
    QAbstractVideoSurface* videoSurface() const { return m_surface; }
private:
    QAbstractVideoSurface *m_surface = nullptr;
    QVideoSurfaceFormat m_format;
public:
    void setVideoSurface(QAbstractVideoSurface *surface)
    {
        if (m_surface && m_surface != surface  && m_surface->isActive()) {
            m_surface->stop();
        }

        m_surface = surface;

        if (m_surface && m_format.isValid())
        {
            m_format = m_surface->nearestFormat(m_format);
            m_surface->start(m_format);

        }
    }

    void setFormat(int width, int heigth, QVideoFrame::PixelFormat aFormat)
    {
        QSize size(width, heigth);
        QVideoSurfaceFormat format(size, aFormat);
        m_format = format;

        if (m_surface)
        {
            if (m_surface->isActive())
            {
                m_surface->stop();
            }
            m_format = m_surface->nearestFormat(m_format);
            m_surface->start(m_format);
        }
    }

public slots:
    void onNewVideoContentReceived(const QVideoFrame &frame);
};

class CustomFramesource : public QObject
{
    Q_OBJECT
public:
    CustomFramesource(int aWidth, int aHeight, QVideoFrame::PixelFormat aFormat = QVideoFrame::PixelFormat::Format_ARGB32){
        m_width = aWidth;
        m_height = aHeight;
        m_format = aFormat;
    }
    int width(){
        return m_width;
    }
    int height(){
        return m_height;
    }
    QVideoFrame::PixelFormat format(){
        return m_format;
    }
signals:
    void newFrameAvailable(const QVideoFrame &);
private:
    void setWidth(int aWidth){
        m_width = aWidth;
    }
    void setHeight(int aHeight){
        m_height = aHeight;
    }
    void setFormat(QVideoFrame::PixelFormat aFormat){
        m_format = aFormat;
    }
private:
    int m_height, m_width;
    QVideoFrame::PixelFormat m_format;
};
