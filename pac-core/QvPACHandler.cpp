#include "QvPACHandler.hpp"

static QString pacContent = "";

PACServer::PACServer(QObject *parent) : QThread(parent)
{
    server = new QHttpServer(this);
    connect(this, &QThread::finished, this, &QThread::deleteLater);
}
PACServer::~PACServer()
{
    wait();
    delete server;
}
void PACServer::stopServer()
{
    server->close();
}
void PACServer::run()
{
    auto settings = pluginInstance->GetPluginSettngs();
    auto address = settings["listenip"].toString();
    auto port = settings["port"].toInt();
    //
    auto fPath = pluginInstance->GetConfigPath() + "/gfwList.txt";
    QFile pacFile(fPath);
    pacFile.open(QFile::ReadOnly);
    auto content = pacFile.readAll();
    pacContent = ConvertGFWToPAC(content, proxyString);
    //
    connect(server, &QHttpServer::newRequest, this, &PACServer::pacRequestHandler);
    auto result = server->listen(QHostAddress(address), static_cast<ushort>(port));
    if (!result)
    {
        pluginInstance->PluginErrorMessageBox(tr("Failed to listen PAC request on this port, please verify the permissions"));
    }
}

void PACServer::pacRequestHandler(QHttpRequest *req, QHttpResponse *rsp)
{
    rsp->setHeader("Server", "Qv2ray/Plugin/PAC_Handler");
    if (req->method() == QHttpRequest::HTTP_GET)
    {
        if (req->path() == "/pac")
        {
            rsp->writeHead(200);
            rsp->setHeader("Content-Type", "application/javascript; charset=utf-8");
            rsp->write(pacContent.toUtf8());
        }
        else
        {
            rsp->writeHead(404);
            rsp->setHeader("Content-Type", "text/plain; charset=utf-8");
            rsp->write("NOT FOUND");
        }
    }
    else
    {
        rsp->writeHead(405);
        rsp->setHeader("Content-Type", "text/plain; charset=utf-8");
        rsp->write("PAC ONLY SUPPORT GET");
    }
}
