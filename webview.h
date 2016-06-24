#ifndef WEBVIEW_H
#define WEBVIEW_H


#include <QWebView>
#include "cookiejar.h"


class WebView : public QWebView
{
        Q_OBJECT

    public:
        explicit WebView(QWidget *parent = 0, CookieJar *cookieJar = 0);
        ~WebView();

    protected:
        QString storagePath;

    private:
        void setStoragePath();
        void setCookies();

        CookieJar *cookieJar;
        QUrl lastUrl;

    private slots:
	void onUrlChanged(QUrl);
	void featureRequest(QWebFrame *frame, QWebPage::Feature feature);
};

#endif // WEBVIEW_H
