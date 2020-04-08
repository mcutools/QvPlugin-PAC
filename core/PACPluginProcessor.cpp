#include "PACPluginProcessor.hpp"

#include "PACPlugin.hpp"
#include "PACSystemProxyConfigurator.hpp"

PACPluginProcessor::PACPluginProcessor(QObject *parent) : Qv2rayPlugin::QvPluginEventHandler(parent), server(this)
{
}
void PACPluginProcessor::ProcessEvent_SystemProxy(const ::Qv2rayPlugin::QvSystemProxyEventObject &event)
{
    switch (event.systemProxyState)
    {
        case Qv2rayPlugin::SystemProxyState_SetProxy:
        {
            const auto &settings = pluginInstance->GetSettngs();
            if (isPACStarted)
            {
                // If we use PAC and socks/http are properly configured for PAC
                pluginInstance->PluginLog("Setting up system proxy for PAC.");
                auto proxyAddress = "http://127.0.0.1:" + QString::number(settings["port"].toInt(8990)) + "/pac";
                // Set System Proxy
                SetSystemProxy(proxyAddress);
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
        case Qv2rayPlugin::SystemProxyState_ClearProxy:
        {
            break;
        }
    }
}

void PACPluginProcessor::ProcessEvent_Connectivity(const ::Qv2rayPlugin::QvConnectivityEventObject &event)
{
    switch (event.eventType)
    {
        case Qv2rayPlugin::QvConnecticity_Connected:
        {
            const auto &settings = pluginInstance->GetSettngs();
            if (!settings["setSystemProxy"].toBool(false))
            {
                break;
            }
            bool pacUseSocks = settings["useSocksProxy"].toBool();
            bool httpEnabled = event.inboundPorts.contains("http");
            bool socksEnabled = event.inboundPorts.contains("socks");
            QString pacProxyString; // Something like: SOCKS5 127.0.0.1:1080; SOCKS 127.0.0.1:1080; DIRECT; http://proxy:8080
            auto pacProxyServerIP = settings["localIP"].toString("127.0.0.1");

            if (pacUseSocks)
            {
                if (socksEnabled)
                {
                    pacProxyString = "SOCKS5 " + pacProxyServerIP + ":" + QString::number(event.inboundPorts["socks"]);
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
                    pacProxyString = "PROXY " + pacProxyServerIP + ":" + QString::number(event.inboundPorts["http"]);
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
        case Qv2rayPlugin::QvConnecticity_Disconnected:
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
