#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QQuickItem>

class MainWindow : public QObject
{
    Q_OBJECT
public:
    //MainWindow(QObject* parent);

public slots:
    void mouseSlot(const QMouseEvent &arg);
};

#endif // MAINWINDOW_H
