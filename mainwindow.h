#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebKitWidgets>
#include <QSystemTrayIcon>
#include <QAction>
#include <QWebNotificationData>
#include <QtNetwork/QNetworkAccessManager>
#include "qwebkitplatformplugin.h"
#include "cookiejar.h"
#include "webview.h"
#include "asemantools/asemannativenotification.h"

#ifdef Q_OS_WIN32
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#include <QtWinExtras>
#endif


class CookieJar;
class NetworkAccessManager;

namespace Ui{
	class MainWindow;
};

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);

        virtual void showEvent(QShowEvent *event)  override;
        virtual void hideEvent(QHideEvent *event)  override;
        virtual void closeEvent(QCloseEvent *event) override;

        void showNotification(QString title, QString message);

        ~MainWindow();


    public slots:
        void trayActivated(QSystemTrayIcon::ActivationReason reason);
        void hideOnCloseChanged(bool value);
        void realClose();

    private:
	Ui::MainWindow *ui;
        AsemanNativeNotification *notification;
        bool hideOnClose;
        bool userWantsToClose;

#ifdef Q_OS_WIN32
        QWinTaskbarButton *button;
#endif

        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;
        QAction *hideOnCloseAction;
        QAction *closeAction;

        const QString teamLoginUrl = QString("https://%1.slack.com");
        const QString loginUrl = QString("https://slack.com/signin");

        void createActions();
        void createTray();
        void setIcons();
        void readSettings();
        void saveSettings();

private slots:
 	void createNewTab();
	void closeTab(int);
        void createWebView(QString team);
};

#endif // MAINWINDOW_H
