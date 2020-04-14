#include "PACPluginProcessor.hpp"

#include "PACPlugin.hpp"
#include "PACSystemProxyConfigurator.hpp"

PACPluginProcessor::PACPluginProcessor(QObject *parent) : Qv2rayPlugin::QvPluginEventHandler(parent), server(this)
{
}
QvPlugin_EventHandler(PACPluginProcessor, SystemProxy)
{
    switch (pluginEvent.systemProxyState)
    {
        case Qv2rayPlugin::Events::SystemProxy::SystemProxyState_SetProxy:
        {
            const auto &settings = pluginInstance->GetSettngs();
            if (isPACStarted)
            {
                if (settings["setSystemProxy"].toBool(false))
                {
                    // If we use PAC and socks/http are properly configured for PAC
                    pluginInstance->PluginLog("Setting up system proxy for PAC.");
                    auto proxyAddress = "http://127.0.0.1:" + QString::number(settings["port"].toInt(8990)) + "/pac";
                    // Set System Proxy
                    SetSystemProxy(proxyAddress);
                }
                else
                {
                    pluginInstance->PluginLog("System proxy has been disabled via settings.");
                }
            }
            else
            {
                // Not properly configured
                pluginInstance->PluginLog("PAC Server is not started, we should not set proxy");
                pluginInstance->PluginErrorMessageBox(tr("PAC Server is not started, not setting proxy."));
                return;
            }
            break;
        }
        case Qv2rayPlugin::Events::SystemProxy::SystemProxyState_ClearProxy:
        {
            break;
        }
    }
}

QvPlugin_EventHandler(PACPluginProcessor, Connectivity)
{
    switch (pluginEvent.eventType)
    {
        case Qv2rayPlugin::Events::Connectivity::QvConnecticity_Connected:
        {
            const auto &settings = pluginInstance->GetSettngs();
            if (!settings["setSystemProxy"].toBool(false))
            {
                break;
            }
            bool pacUseSocks = settings["useSocksProxy"].toBool();
            bool httpEnabled = pluginEvent.inboundPorts.contains("http");
            bool socksEnabled = pluginEvent.inboundPorts.contains("socks");
            QString pacProxyString; // Something like: SOCKS5 127.0.0.1:1080; SOCKS 127.0.0.1:1080; DIRECT; http://proxy:8080
            auto pacProxyServerIP = settings["localIP"].toString("127.0.0.1");

            if (pacUseSocks)
            {
                if (socksEnabled)
                {
                    pacProxyString = "SOCKS5 " + pacProxyServerIP + ":" + QString::number(pluginEvent.inboundPorts["socks"]);
                }
                else
                {
                    pluginInstance->PluginLog("PAC is using SOCKS, but it is not enabled");
                    pluginInstance->PluginErrorMessageBox(tr("Could not start PAC server, SOCKS inbound is not enabled"));
                    break;
                }
            }
            else
            {
                if (httpEnabled)
                {
                    pacProxyString = "PROXY " + pacProxyServerIP + ":" + QString::number(pluginEvent.inboundPorts["http"]);
                }
                else
                {
                    pluginInstance->PluginLog("PAC is using HTTP, but it is not enabled");
                    pluginInstance->PluginErrorMessageBox(tr("Could not start PAC server, HTTP inbound is not enabled"));
                    break;
                }
            }

            server.setPACProxyString(pacProxyString);
            isPACStarted = server.startServer();

            break;
        }
        case Qv2rayPlugin::Events::Connectivity::QvConnecticity_Disconnected:
        {
            if (server.isServerStarted())
            {
                pluginInstance->PluginLog("Stopping PAC Server");
                server.stopServer();
            }
            break;
        }
        default: break;
    }
}
