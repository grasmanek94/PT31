
#include "MainWindow.h"

/*MainWindow::MainWindow(QObject* parent) : QObject(parent)\
{

}*/

void MainWindow::mouseSlot(const QMouseEvent &arg)
{
    qDebug() << "Mouse click! ";
    qDebug() << arg.x() << " " << arg.y();
}
