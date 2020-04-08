#include "PACPlugin.hpp"

#include "QvPACHandler.hpp"
#include "ui/QvPACPluginSettingsWidget.hpp"

#include <QDateTime>
#include <QLabel>

Qv2rayPACPlugin *pluginInstance = nullptr;

QvPluginKernel *Qv2rayPACPlugin::GetKernel()
{
    return nullptr;
}

bool Qv2rayPACPlugin::UpdateSettings(const QJsonObject &conf)
{
    settings = conf;
    return true;
}

bool Qv2rayPACPlugin::Initialize(const QString &confPath, const QJsonObject &settings)
{
    emit PluginLog("Initialize PAC plugin.");
    pluginInstance = this;
    processor = new PACPluginProcessor(this);
    this->configPath = confPath;
    this->settings = settings;
    return true;
}

const QJsonObject Qv2rayPACPlugin::GetSettngs()
{
    return settings;
}

QWidget *Qv2rayPACPlugin::GetSettingsWidget()
{
    return new QvPACPluginSettingsWidget();
}

Qv2rayPlugin::QvPluginEditor *Qv2rayPACPlugin::GetEditorWidget(Qv2rayPlugin::UI_TYPE)
{
    // PAC does not have an editor widget.
    return nullptr;
}

QvPluginEventHandler *Qv2rayPACPlugin::GetEventHandler()
{
    return processor;
}
