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
    pluginInstance->PluginLog("Starting PAC Server.");
    //
    const auto &settings = pluginInstance->Settings();
    //
    auto GFWListsPath = pluginInstance->GetConfigPath() + "/gfwList.txt";
    QFile gfwFile(GFWListsPath);
    auto PACPath = pluginInstance->GetConfigPath() + "/qv2ray.pac";
    QFile pacFile(PACPath);

    if (pacFile.exists())
    {
        pluginInstance->PluginLog("Loaded PAC from user's qv2ray.pac");
        pacContent = StringFromFile(PACPath);
    }
    else if (gfwFile.exists())
    {
        pluginInstance->PluginLog("Loading PAC from GFWLists");
        gfwFile.open(QFile::ReadOnly);
        pacContent = ConvertGFWToPAC(gfwFile.readAll(), proxyString);
        gfwFile.close();
        StringToFile(pacContent, PACPath);
    }
    else
    {
        pluginInstance->PluginLog("Cannot find qv2ray.pac or gfwList.txt");
        pluginInstance->PluginErrorMessageBox(tr("Cannot find GFWLists file or user's PAC file."));
        return false;
    }
    //
    //
    isStarted = server->listen(QHostAddress(settings.listenIP), settings.listenPort);
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
            pluginInstance->PluginLog("Processing PAC request.");
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
