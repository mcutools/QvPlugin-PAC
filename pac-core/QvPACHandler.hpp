#pragma once

#include "3rdparty/cpp-httplib/httplib.h"
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
    httplib::Server *server;
    static void pacRequestHandler(const httplib::Request &req, httplib::Response &rsp);
};
