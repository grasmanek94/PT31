#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QQuickItem>

class MainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MainWindow(QObject *parent = 0);
    Q_INVOKABLE QString gimmeText();
signals:

public slots:
};

#endif // MAINWINDOW_H
