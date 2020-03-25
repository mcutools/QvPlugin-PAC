#include "PACPlugin.hpp"

#include <QDateTime>
#include <QLabel>
#include <QMetaEnum>

// Helper template function to convert an enum to QString.
template<typename T>
QString EnumToString(const T &t)
{
    return QMetaEnum::fromType<T>().key(t);
}

QString PACPlugin::Name() const
{
    return "Simple Plugin";
}

QString PACPlugin::Author() const
{
    return "Example Author";
}

QString PACPlugin::InternalName() const
{
    return "sample_plugin";
}

QString PACPlugin::Description() const
{
    return "Sample plugin description";
}

QStringList PACPlugin::OutboundTypes() const
{
    emit PluginLog("Getting outbound types.");
    return {};
}

Qv2ray::Qv2rayKernelPluginObject *PACPlugin::GetKernelInstance()
{
    emit PluginLog("Getting kernel instance.");
    return nullptr;
}

bool PACPlugin::UpdatePluginSettings(const QJsonObject &conf)
{
    settings = conf;
    return true;
}

bool PACPlugin::InitializePlugin(const QJsonObject &settings)
{
    emit PluginLog("Initialize plugin.");
    this->settings = settings;
    pluginWidget = new QLabel;
    return true;
}

const QIcon PACPlugin::Icon() const
{
    emit PluginLog("Getting icon.");
    return QIcon(":/qv2ray.png");
}

const QJsonObject PACPlugin::GetPluginSettngs()
{
    emit PluginLog("Getting plugin settings.");
    return settings;
}

QObject *PACPlugin::GetQObject()
{
    emit PluginLog("Getting qobject.");
    return this;
}

QWidget *PACPlugin::GetSettingsWidget()
{
    pluginWidget->setText("From Settings: " + settings["msg"].toString());
    return pluginWidget;
}

Qv2ray::Qv2rayPluginEditorWidget *PACPlugin::GetEditorWidget(Qv2ray::QV2RAY_PLUGIN_UI_TYPE type)
{
    // PAC does not have an editor widget.
    Q_UNUSED(type)
    return nullptr;
}

void PACPlugin::ProcessHook(Qv2ray::QV2RAY_PLUGIN_HOOK_TYPE event, Qv2ray::QV2RAY_PLUGIN_HOOK_SUBTYPE subtype, QVariant *data)
{
    emit PluginLog("Processing Plugin Hook.");
    emit PluginLog("Data: " + EnumToString(event) + ", subtype: " + EnumToString(subtype));
}

Qv2ray::QV2RAY_PLUGIN_HOOK_TYPE_FLAGS PACPlugin::PluginHooks() const
{
    return { Qv2ray::HOOK_TYPE_STATE_EVENTS };
}

Qv2ray::QV2RAY_SPECIAL_PLUGIN_TYPE_FLAGS PACPlugin::SpecialPluginType() const
{
    return { Qv2ray::SPECIAL_TYPE_NONE };
}
