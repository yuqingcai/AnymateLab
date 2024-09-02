#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    qputenv("QSG_RHI_BACKEND", "opengl");
#endif

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("AnymateLab", "Main");
    // engine.loadFromModule("AnymateLab", "ProjectManager");


    return app.exec();
}
