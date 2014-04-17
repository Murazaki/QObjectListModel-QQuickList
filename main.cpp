#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlContext>
#include <QtQml>
#include "person.h"
#include "testobject.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qmlRegisterType<Person>("TestComponents", 1, 0, "Person");
    qmlRegisterType<QObjectListModel>("TestComponents", 1, 0, "QObjectListModel");
    TestObject * testobject = new TestObject();
    engine.rootContext()->setContextProperty("testobject", testobject);
    engine.load(QUrl("qrc:/main.qml"));
    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    window->show();
    return app.exec();
}
