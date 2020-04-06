#pragma once

#include "3rdparty/qhttpserver/src/qhttprequest.h"
#include "3rdparty/qhttpserver/src/qhttpresponse.h"
#include "3rdparty/qhttpserver/src/qhttpserver.h"
#include "PACPlugin.hpp"

#include <QObject>
#include <QThread>
#include <memory>

QString ConvertGFWToPAC(const QByteArray &rawContent, const QString &customProxyString);
class PACServer : public QThread
{
    Q_OBJECT
  public:
    explicit PACServer(QObject *parent = nullptr);
    ~PACServer();
    inline void setPACProxyString(const QString &proxyStr)
    {
        proxyString = proxyStr;
    }
    void stopServer();

  private:
    void run() override;
    QString proxyString;

  private:
    QHttpServer *server;
    static void pacRequestHandler(QHttpRequest *request, QHttpResponse *response);
};
