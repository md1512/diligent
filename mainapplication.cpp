#include "mainapplication.h"

MainApplication::MainApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName("diligent");
    QCoreApplication::setOrganizationDomain("diligent");
    QCoreApplication::setApplicationName("Diligent");
}

void MainApplication::setMainWindow(MainWindow *window)
{
    m_window = window;
}

MainWindow *MainApplication::getMainWindow()
{
    if (!m_window)
    {
        m_window = new MainWindow();
    }

    return m_window;
}

void MainApplication::showNotification(QString title, QString message)
{
    getMainWindow()->showNotification(title, message);
}

MainApplication::~MainApplication()
{
}
