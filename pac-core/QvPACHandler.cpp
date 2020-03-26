#include "QvPACHandler.hpp"

static QString pacContent = "";

PACServer::PACServer(QObject *parent) : QThread(parent)
{
    server = new httplib::Server();
    connect(this, &QThread::finished, this, &QThread::deleteLater);
}
PACServer::~PACServer()
{
    wait();
    delete server;
}
void PACServer::stopServer()
{
    if (server && server->is_running())
        server->stop();
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
    server->Get("/pac", pacRequestHandler);
    auto result = server->listen(address.toStdString().c_str(), static_cast<ushort>(port));
    if (!result)
    {
        pluginInstance->PluginErrorMessageBox(tr("Failed to listen PAC request on this port, please verify the permissions"));
    }
}

void PACServer::pacRequestHandler(const httplib::Request &req, httplib::Response &rsp)
{
    rsp.set_header("Server", "Qv2ray/Plugin/PAC_Handler");
    if (req.method == "GET")
    {
        if (req.path == "/pac")
        {
            rsp.status = 200;
            rsp.set_content(pacContent.toStdString(), "application/javascript; charset=utf-8");
        }
        else
        {
            rsp.status = 404;
            rsp.set_content("NOT FOUND", "text/plain; charset=utf-8");
        }
    }
    else
    {
        rsp.status = 405;
        rsp.set_content("PAC ONLY SUPPORT GET", "text/plain; charset=utf-8");
    }
}
