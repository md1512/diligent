#include "webpage.h"
#include <QDesktopServices>
#include <QWebFrame>
#include <QNetworkRequest>

WebPage::WebPage(QObject *parent) : QWebPage(parent)
{
  //Always allow desktop notifications by default
  this->setFeaturePermission(this->mainFrame(), QWebPage::Feature::Notifications,
                                              QWebPage::PermissionPolicy::PermissionGrantedByUser);
}

WebPage::~WebPage()
{
}

bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type)
{
  //qDebug() << "Got navigation request:" << request.url();
    if (!frame){
        QDesktopServices::openUrl(request.url());
        return false;
    }else{
      return QWebPage::acceptNavigationRequest(frame, request, type);
    }
}
