#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardPaths>
#include <QWebPage>
#include <QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QWebSettings>
#include <QFontDatabase>
#include <QFileInfo>
#include <QMessageBox>
#include <QVBoxLayout>

#include "teamform.h"

MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)) );
  connect(ui->actionNew_Tab, SIGNAL(triggered(bool)), this, SLOT(createNewTab()) );
  connect(ui->actionClose, SIGNAL(triggered(bool)), this, SLOT(realClose()) );
  ui->tabWidget->clear();
  //Now create any tabs as needed
  QSettings settings;
    QStringList lasttabs = settings.value("teams_lastopen").toStringList();
  if(lasttabs.isEmpty()){ createNewTab(); }
  else{
    for(int i=0; i<lasttabs.length(); i++){
      createWebView(lasttabs[i]);
    }
  }
    //createWebView();
    //setUrl();
    createActions();
    createTray();
    setIcons();
#ifdef Q_OS_WIN32
    button = new QWinTaskbarButton(this);
    button->setWindow(this->windowHandle());
    button->setOverlayIcon(QIcon("://images/png/Slack.png"));
#endif
    notification = new AsemanNativeNotification(this);
    readSettings();
}

MainWindow::~MainWindow()
{
}

void MainWindow::readSettings()
{
    QRect rec = QApplication::desktop()->screenGeometry();
    this->resize(rec.width() * 0.75, rec.height() * 0.75);
    this->setMinimumSize(600, 300);
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/windowState").toByteArray());

    hideOnClose = settings.value("mainwindow/hideonclose").toBool();
    hideOnCloseAction->setChecked(hideOnClose);
}

void MainWindow::createNewTab(){
  TeamForm *teamform = new TeamForm(this);
  teamform->setWhatsThis("selector");
  connect(teamform, SIGNAL(openTeam(QString)), this, SLOT(createWebView(QString)) );
  int index =  ui->tabWidget->addTab(teamform, tr("Select Team"));
  ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::closeTab(int tab){
  QWidget *widget =ui-> tabWidget->widget(tab);
  if(widget==0){ return; }
 ui-> tabWidget->removeTab(tab);
  widget->deleteLater();
}

void MainWindow::createWebView(QString team)
{
  //Get the current tab first
  QWidget *cur = ui->tabWidget->currentWidget();
    if(cur!=0){
      //ensure the current widget  was the selector
      if(cur->whatsThis()=="selector"){ 
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
        cur->deleteLater();
      }
    }
    WebView *webView = new WebView(this);
    int index =ui-> tabWidget->addTab(webView, team);
    ui->tabWidget->setCurrentIndex(index);
    webView->setUrl( QUrl(teamLoginUrl.arg(team) ) );
}

void MainWindow::createTray()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(hideOnCloseAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(closeAction);

    trayIcon = new QSystemTrayIcon(QIcon(QString("://images/png/icon32.png")), this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::setIcons()
{
    setWindowIcon(QIcon(QString("://images/png/icon32.png")));
}

void MainWindow::hideOnCloseChanged(bool value)
{
    hideOnClose = value;
    QSettings settings;
    settings.setValue("mainwindow/hideonclose", hideOnClose);
}

void MainWindow::realClose()
{
    userWantsToClose = true;
    close();
}

void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    {
        if (isHidden())
            show();
        else
            hide();

        break;
    }

    default:
        break;
    }
}

void MainWindow::createActions()
{
    hideOnCloseAction = new QAction(tr("Hide on close"), this);
    hideOnCloseAction->setCheckable(true);
    connect(hideOnCloseAction, SIGNAL(toggled(bool)), this, SLOT(hideOnCloseChanged(bool)));

    closeAction = new QAction(tr("Close"), this);
    connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(realClose()));
}

void MainWindow::showNotification(QString title, QString message)
{
    notification->sendNotify(title, message, "://images/png/Slack.png", 0, 100000);
    QApplication::alert(this);
}

void MainWindow::hideEvent(QHideEvent *event)
{
    QMainWindow::hideEvent(event);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/windowState", saveState());
    settings.setValue("mainwindow/hideonclose", hideOnClose);
    //Now save which tabs are open right now for re-opening on next run
    QStringList lasttabs;
    for(int i=0; i<ui->tabWidget->count(); i++){
      if(ui->tabWidget->widget(i)->whatsThis().isEmpty()){
        lasttabs << ui->tabWidget->tabText(i);
      }
    }
     settings.setValue("teams_lastopen", lasttabs);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (hideOnClose)
    {
        if (userWantsToClose)
        {
            saveSettings();
            trayIcon->hide();
            QMainWindow::closeEvent(event);
        }
        else
        {
            qDebug() << "User doesn't want to close";
            event->ignore();
            QMainWindow::hide();
        }
    }
    else
    {
        saveSettings();
        trayIcon->hide();
        QMainWindow::closeEvent(event);
    }
}
