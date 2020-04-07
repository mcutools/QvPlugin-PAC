#include "PACPluginProcessor.hpp"

#include "PACPlugin.hpp"

PACPluginProcessor::PACPluginProcessor(QObject *parent) : Qv2rayPlugin::QvPluginEventHandler(parent)
{
}
void PACPluginProcessor::ProcessEvent_SystemProxy(const ::Qv2rayPlugin::QvSystemProxyEventObject &event)
{
    switch (event.systemProxyState)
    {
        case Qv2rayPlugin::SystemProxyState_SetProxy:
        {
            bool canSetSystemProxy = false;
            const auto &settings = pluginInstance->GetSettngs();
            bool pacUseSocks = settings["useSocksProxy"].toBool();
            QString proxyAddress;
            bool httpEnabled = event.systemProxyPortSettings.contains(Qv2rayPlugin::SystemProxy_HTTP);
            bool socksEnabled = event.systemProxyPortSettings.contains(Qv2rayPlugin::SystemProxy_SOCKS);
            if ((httpEnabled && !pacUseSocks) || (socksEnabled && pacUseSocks))
            {
                // If we use PAC and socks/http are properly configured for PAC
                pluginInstance->PluginLog("System proxy uses PAC");
                proxyAddress = "http://" + settings["localIP"].toString() + ":" + QString::number(settings["port"].toInt(8990)) + "/pac";
            }
            else
            {
                // Not properly configured
                // LOG(MODULE_PROXY, "Failed to process pac due to following reasons:")
                // LOG(MODULE_PROXY, " --> PAC is configured to use socks but socks is not enabled.")
                // LOG(MODULE_PROXY, " --> PAC is configuted to use http but http is not enabled.")
                pluginInstance->PluginErrorMessageBox(tr("HTTP or SOCKS inbound is not properly configured for PAC") + "\r\n" +
                                                      tr("Qv2ray will continue, but will not set system proxy."));
                canSetSystemProxy = false;
            }

            QString pacProxyString; // Something like this --> SOCKS5 127.0.0.1:1080; SOCKS
                                    // 127.0.0.1:1080; DIRECT; http://proxy:8080
            auto pacIP = settings["localIP"].toString();

            if (pacIP.isEmpty())
            {
                // LOG(MODULE_PROXY, "PAC Local IP is empty, default to 127.0.0.1")
                pacIP = "127.0.0.1";
            }

            if (pacUseSocks)
            {
                if (socksEnabled)
                {
                    pacProxyString = "SOCKS5 " + pacIP + ":" + QString::number(event.systemProxyPortSettings[Qv2rayPlugin::SystemProxy_SOCKS]);
                }
                else
                {
                    // LOG(MODULE_UI, "PAC is using SOCKS, but it is not enabled")
                    pluginInstance->PluginErrorMessageBox(
                        tr("Could not start PAC server as it is configured to use SOCKS, but it is not enabled"));
                    canStartPAC = false;
                }
            }
            else
            {
                if (httpEnabled)
                {
                    pacProxyString = "PROXY " + pacIP + ":" + QString::number(event.systemProxyPortSettings[Qv2rayPlugin::SystemProxy_HTTP]);
                }
                else
                {
                    // LOG(MODULE_UI, "PAC is using HTTP, but it is not enabled")
                    pluginInstance->PluginErrorMessageBox(
                        tr("Could not start PAC server as it is configured to use HTTP, but it is not enabled"));
                    canStartPAC = false;
                }
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
            server->startServer();
            break;
        }
        case Qv2rayPlugin::QvConnecticity_Disconnected:
        {
            server->stopServer();
            break;
        }
        default: break;
    }
}
