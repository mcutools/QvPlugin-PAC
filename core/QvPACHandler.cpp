#include "QvPACHandler.hpp"

#include "PACPlugin.hpp"

static QString pacContent = "";

PACServer::PACServer(QObject *parent) : QObject(parent)
{
}
PACServer::~PACServer()
{
}
void PACServer::stopServer()
{
    if (server)
    {
        server->close();
    }
    delete server;
    server = nullptr;
}
bool PACServer::startServer()
{
    if (isStarted)
    {
        // This line should rarely be touched.
        stopServer();
    }
    server = new QHttpServer(this);
    connect(server, &QHttpServer::newRequest, this, &PACServer::PACRequestHandler);
    //
    const auto &settings = pluginInstance->GetSettngs();
    //
    auto fPath = pluginInstance->GetConfigPath() + "/gfwList.txt";
    //
    QFile pacFile(fPath);
    if (!pacFile.exists())
    {
        pluginInstance->PluginErrorMessageBox(tr("Cannot find GFWList file: ") + fPath);
        return false;
    }
    //
    pacFile.open(QFile::ReadOnly);
    pacContent = ConvertGFWToPAC(pacFile.readAll(), proxyString);
    pacFile.close();
    //
    isStarted = server->listen(QHostAddress(settings["listenip"].toString()), settings["port"].toInt());
    if (!isStarted)
    {
        pluginInstance->PluginErrorMessageBox(tr("Failed to listen PAC request on this port, please verify the permissions"));
    }
    return isStarted;
}

void PACServer::PACRequestHandler(QHttpRequest *req, QHttpResponse *rsp)
{
    rsp->setHeader("Server", "Qv2ray/QvPACPlugin");
    if (req->method() == QHttpRequest::HTTP_GET)
    {
        if (req->path() == "/pac")
        {
            rsp->writeHead(200);
            rsp->setHeader("Content-Type", "application/javascript; charset=utf-8");
            rsp->end(pacContent.toUtf8());
        }
        else
        {
            rsp->writeHead(404);
            rsp->setHeader("Content-Type", "text/plain; charset=utf-8");
            rsp->end("NOT FOUND");
        }
    }
    else
    {
        rsp->writeHead(405);
        rsp->setHeader("Content-Type", "text/plain; charset=utf-8");
        rsp->end("PAC ONLY SUPPORT GET");
    }
}
