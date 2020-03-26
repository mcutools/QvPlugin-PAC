#include "PACPlugin.hpp"

#include "pac-core/QvPACHandler.hpp"
#include "ui/QvPACPluginSettingsWidget.hpp"

#include <QDateTime>
#include <QLabel>
#include <QMetaEnum>

PACPlugin *pluginInstance = nullptr;

// Helper template function to convert an enum to QString.
template<typename T>
QString EnumToString(const T &t)
{
    return QMetaEnum::fromType<T>().key(t);
}

QString PACPlugin::Name() const
{
    return "Qv2ray PAC Plugin";
}

QString PACPlugin::Author() const
{
    return "Qv2ray Workgroup";
}

QString PACPlugin::InternalName() const
{
    return "pac_plugin";
}

QString PACPlugin::Description() const
{
    return tr("This plugin allows you to use PAC as a proxy method.");
}

QStringList PACPlugin::OutboundTypes() const
{
    emit PluginLog("Getting outbound types.");
    return {};
}

Qv2ray::Qv2rayKernelPluginObject *PACPlugin::GetKernelInstance()
{
    return nullptr;
}

bool PACPlugin::UpdatePluginSettings(const QJsonObject &conf)
{
    settings = conf;
    return true;
}

bool PACPlugin::InitializePlugin(const QString &confPath, const QJsonObject &settings)
{
    emit PluginLog("Initialize PAC plugin.");
    pluginInstance = this;
    this->configPath = confPath;
    this->settings = settings;
    return true;
}

const QIcon PACPlugin::Icon() const
{
    emit PluginLog("Getting icon.");
    return QIcon(":/qv2ray.png");
}

const QJsonObject PACPlugin::GetPluginSettngs()
{
    return settings;
}

QObject *PACPlugin::GetQObject()
{
    return this;
}

QWidget *PACPlugin::GetSettingsWidget()
{
    return new QvPACPluginSettingsWidget();
}

Qv2ray::Qv2rayPluginEditorWidget *PACPlugin::GetEditorWidget(Qv2ray::QV2RAY_PLUGIN_UI_TYPE type)
{
    // PAC does not have an editor widget.
    Q_UNUSED(type)
    return nullptr;
}

void PACPlugin::ProcessHook(Qv2ray::QV2RAY_PLUGIN_HOOK_TYPE event, Qv2ray::QV2RAY_PLUGIN_HOOK_SUBTYPE subtype, QVariant *data)
{
    Q_UNUSED(data)
    emit PluginLog("Processing Plugin Hook.");
    emit PluginLog("Data: " + EnumToString(event) + ", subtype: " + EnumToString(subtype));
    // if (usePAC)
    //    {
    //        if ((httpEnabled && !pacUseSocks) || (socksEnabled && pacUseSocks))
    //        {
    //            // If we use PAC and socks/http are properly configured for PAC
    //            LOG(MODULE_PROXY, "System proxy uses PAC")
    //            proxyAddress = "http://" + GlobalConfig.inboundConfig.listenip + ":" + QSTRN(GlobalConfig.inboundConfig.pacConfig.port) +
    //            "/pac";
    //        }
    //        else
    //        {
    //            // Not properly configured
    //            LOG(MODULE_PROXY, "Failed to process pac due to following reasons:")
    //            LOG(MODULE_PROXY, " --> PAC is configured to use socks but socks is not enabled.")
    //            LOG(MODULE_PROXY, " --> PAC is configuted to use http but http is not enabled.")
    //            QvMessageBoxWarn(this, tr("PAC Processing Failed"),
    //                             tr("HTTP or SOCKS inbound is not properly configured for PAC") + NEWLINE +
    //                                 tr("Qv2ray will continue, but will not set system proxy."));
    //            canSetSystemProxy = false;
    //        }
    //    }
    //
    //    bool httpEnabled = GlobalConfig.inboundConfig.useHTTP;
    //    bool socksEnabled = GlobalConfig.inboundConfig.useSocks;
    //    if (usePAC)
    //    {
    //        bool canStartPAC = true;
    //        QString pacProxyString; // Something like this --> SOCKS5 127.0.0.1:1080; SOCKS
    //                                // 127.0.0.1:1080; DIRECT; http://proxy:8080
    //        auto pacIP = GlobalConfig.inboundConfig.pacConfig.localIP;

    //        if (pacIP.isEmpty())
    //        {
    //            LOG(MODULE_PROXY, "PAC Local IP is empty, default to 127.0.0.1")
    //            pacIP = "127.0.0.1";
    //        }

    //        if (pacUseSocks)
    //        {
    //            if (socksEnabled)
    //            {
    //                pacProxyString = "SOCKS5 " + pacIP + ":" + QSTRN(GlobalConfig.inboundConfig.socks_port);
    //            }
    //            else
    //            {
    //                LOG(MODULE_UI, "PAC is using SOCKS, but it is not enabled")
    //                QvMessageBoxWarn(this, tr("Configuring PAC"),
    //                                 tr("Could not start PAC server as it is configured to use SOCKS, but it is not enabled"));
    //                canStartPAC = false;
    //            }
    //        }
    //        else
    //        {
    //            if (httpEnabled)
    //            {
    //                pacProxyString = "PROXY " + pacIP + ":" + QSTRN(GlobalConfig.inboundConfig.http_port);
    //            }
    //            else
    //            {
    //                LOG(MODULE_UI, "PAC is using HTTP, but it is not enabled")
    //                QvMessageBoxWarn(this, tr("Configuring PAC"),
    //                                 tr("Could not start PAC server as it is configured to use HTTP, but it is not enabled"));
    //                canStartPAC = false;
    //            }
    //        }

    //        if (canStartPAC)
    //        {
    //            pacServer = new PACServer(this);
    //            pacServer->setPACProxyString(pacProxyString);
    //            pacServer->start();
    //        }
    //        else
    //        {
    //            LOG(MODULE_PROXY, "Not starting PAC due to previous error.")
    //        }
    //    }

    //
    //

    if (event == Qv2ray::HOOK_TYPE_STATE_EVENTS)
    {
        if (subtype == Qv2ray::HOOK_STYPE_POST_CONNECTED)
        {
            server = new PACServer(this);
            server->start();
        }
        if (subtype == Qv2ray::HOOK_STYPE_POST_DISCONNECTED)
        {
            if (server && server->isRunning())
            {
                server->stopServer();
            }
        }
    }
}

Qv2ray::QV2RAY_PLUGIN_HOOK_TYPE_FLAGS PACPlugin::PluginHooks() const
{
    return { Qv2ray::HOOK_TYPE_STATE_EVENTS };
}

Qv2ray::QV2RAY_SPECIAL_PLUGIN_TYPE_FLAGS PACPlugin::SpecialPluginType() const
{
    return { Qv2ray::SPECIAL_TYPE_NONE };
}
