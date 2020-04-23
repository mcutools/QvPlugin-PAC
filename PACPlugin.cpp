#include "PACPlugin.hpp"

#include "QvPACHandler.hpp"
#include "ui/QvPACPluginSettingsWidget.hpp"

#include <QDateTime>
#include <QLabel>

Qv2rayPACPlugin *pluginInstance = nullptr;

std::shared_ptr<QvPluginKernel> Qv2rayPACPlugin::GetKernel()
{
    return nullptr;
}

bool Qv2rayPACPlugin::UpdateSettings(const QJsonObject &conf)
{
    settings.loadJson(conf);
    return true;
}

const PACSettings Qv2rayPACPlugin::Settings() const
{
    return settings;
}

bool Qv2rayPACPlugin::Initialize(const QString &confPath, const QJsonObject &settings)
{
    emit PluginLog("Initialize PAC plugin.");
    pluginInstance = this;
    processor = std::make_shared<PACPluginProcessor>(this);
    this->configPath = confPath;
    this->settings.loadJson(settings);
    return true;
}

const QJsonObject Qv2rayPACPlugin::GetSettngs()
{
    return settings.toJson();
}

std::unique_ptr<QWidget> Qv2rayPACPlugin::GetSettingsWidget()
{
    return std::make_unique<QvPACPluginSettingsWidget>();
}

std::unique_ptr<Qv2rayPlugin::QvPluginEditor> Qv2rayPACPlugin::GetEditorWidget(Qv2rayPlugin::UI_TYPE)
{
    // PAC does not have an editor widget.
    return nullptr;
}

std::shared_ptr<QvPluginEventHandler> Qv2rayPACPlugin::GetEventHandler()
{
    return processor;
}
