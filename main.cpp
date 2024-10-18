#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QQmlDebuggingEnabler>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

void embedPython() {
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    /* optional but recommended */
    // status = PyConfig_SetBytesString(&config, &config.program_name, argv[0]);
    // if (PyStatus_Exception(status)) {
    //     PyConfig_Clear(&config);
    //     Py_ExitStatusException(status);
    // }

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
    }
    PyConfig_Clear(&config);

    PyRun_SimpleString(
        "from time import time, ctime\n"
        "a = [1, 2, 3, 4]\n"
        "print (a[2:])\n"
        "print('Today is', ctime(time()))\n"
        );
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
}

int main(int argc, char *argv[])
{
    embedPython();

    QGuiApplication app(argc, argv);
    QString appDir = QCoreApplication::applicationDirPath();
    QDir::setCurrent(QDir(appDir).filePath(".."));
    qDebug() << "Current working directory (Project Directory):" <<
        QDir::currentPath();

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
