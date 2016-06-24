#include "webview.h"
#include "webpage.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QWebFrame>

WebView::WebView(QWidget *parent, CookieJar *cookieJar) :
    QWebView(parent),
    cookieJar(cookieJar)
{
    WebPage *page = new WebPage();
    page->setView(this);
    connect(page, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
    connect(page, SIGNAL(featurePermissionRequested(QWebFrame *, QWebPage::Feature)),
            this, SLOT(featureRequest(QWebFrame *, QWebPage::Feature)));
    this->setPage(page);
    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);
    settings()->setAttribute(QWebSettings::NotificationsEnabled, true);
    settings()->setThirdPartyCookiePolicy(QWebSettings::ThirdPartyCookiePolicy::AlwaysAllowThirdPartyCookies);
    settings()->setFontFamily(QWebSettings::StandardFont, "Segoe UI");
    settings()->setFontSize(QWebSettings::DefaultFontSize, 16);
    setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
    setStoragePath();
    setCookies();
}

WebView::~WebView()
{
}

void WebView::setCookies()
{
    if (!cookieJar)
    {
        cookieJar = new CookieJar(this);
    }

    page()->networkAccessManager()->setCookieJar(cookieJar);
}

void WebView::setStoragePath()
{
    QString path(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    qDebug() << "Cache path" << path;
    storagePath = path;
    page()->settings()->enablePersistentStorage(storagePath);
}

void WebView::onUrlChanged(QUrl url)
{
    qDebug() << url.host();

    if (url.host().endsWith(".slack.com", Qt::CaseSensitive))
    {
        this->page()->setFeaturePermission(this->page()->mainFrame(), QWebPage::Feature::Notifications,
                                              QWebPage::PermissionPolicy::PermissionGrantedByUser);
    }
}

void WebView::featureRequest(QWebFrame *frame, QWebPage::Feature feature)
{
    qDebug() << frame->url();

    if (feature == QWebPage::Feature::Notifications)
    {
        int result = QMessageBox::question(this,
                                           QString("Notification permission"),
                                           QString("%1\nasks for notifications persmission. Should I allow?").arg(frame->url().toString()),
                                           QMessageBox::StandardButton::Ok, QMessageBox::Cancel);

        if (result == QMessageBox::StandardButton::Ok)
        {
            this->page()->setFeaturePermission(frame, feature,
                                                  QWebPage::PermissionPolicy::PermissionGrantedByUser);
        }
    }
}
