#include <QApplication>
#include <QQuickView>

#include "MainWindow.h"

/*int main (int argc, char ** argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view(QUrl("qrc:/MainWindow.qml"));

    view.setTitle(QObject::tr("My App"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.showNormal();

    return app.exec();
}*/

int main(int argc, char* argv[])
{
    /*QGuiApplication app(argc, argv);

    QQuickView view(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
    QObject *item = view.rootObject();

    MainWindow window;
    QObject::connect(item, SIGNAL(mapClicked(QMouseEvent)),
                     &window, SLOT(mouseSlot(QMouseEvent)));

    view.showNormal();
    return app.exec();*/


    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));

    //QObject::connect()

    return app.exec();

    //QUrl(QStringLiteral("qrc:/MainWindow.qml"))
}
