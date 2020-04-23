#pragma once
#include "PACConfig.hpp"
#include "PACPluginProcessor.hpp"
#include "QvPluginInterface.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMetaEnum>
#include <QObject>
#include <QTextCodec>
#include <QtPlugin>

class PACServer;
class QLabel;
using namespace Qv2rayPlugin;

class Qv2rayPACPlugin
    : public QObject
    , public Qv2rayInterface
{
    Q_INTERFACES(Qv2rayPlugin::Qv2rayInterface)
    Q_PLUGIN_METADATA(IID Qv2rayInterface_IID)
    Q_OBJECT
  public:
    //
    // Basic metainfo of this plugin
    const QvPluginMetadata GetMetadata() const override
    {
        return QvPluginMetadata{
            "Qv2ray PAC Plugin",                                        //
            "Qv2ray Workgroup",                                         //
            "pac_plugin",                                               //
            tr("This plugin allows you to use PAC as a proxy method."), //
            QIcon(":/qv2ray.png"),                                      //
            { CAPABILITY_SYSTEM_PROXY, CAPABILITY_CONNECTIVITY },       //
            {}                                                          //
        };
    }
    //
    std::unique_ptr<QWidget> GetSettingsWidget() override;
    std::unique_ptr<QvPluginEditor> GetEditorWidget(Qv2rayPlugin::UI_TYPE) override;
    std::shared_ptr<QvPluginKernel> GetKernel() override;
    std::shared_ptr<QvPluginEventHandler> GetEventHandler() override;
    std::shared_ptr<QvPluginSerializer> GetSerializer() override
    {
        return nullptr;
    };
    //
    bool UpdateSettings(const QJsonObject &) override;
    bool Initialize(const QString &, const QJsonObject &) override;
    const QJsonObject GetSettngs() override;
    const PACSettings Settings() const;

  signals:
    void PluginLog(const QString &) const override;
    void PluginErrorMessageBox(const QString &) const override;

  public:
    const QString GetConfigPath() const
    {
        return configPath;
    }

  private:
    std::shared_ptr<PACPluginProcessor> processor;
    QString configPath;
    PACSettings settings;
};

extern Qv2rayPACPlugin *pluginInstance;

inline bool StringToFile(const QString &text, QString &path)
{
    QFile targetFile(path);
    QFileInfo info(targetFile);
    if (!info.dir().exists())
    {
        info.dir().mkpath(info.dir().path());
    }
    bool override = targetFile.exists();
    targetFile.open(QFile::WriteOnly);
    targetFile.write(text.toUtf8());
    targetFile.close();
    return override;
}

inline QString StringFromFile(QString &sourcePath)
{
    QFile source(sourcePath);
    bool wasOpened = source.isOpen();
    if (!wasOpened)
        source.open(QFile::ReadOnly);
    auto byteArray = source.readAll();
    if (!wasOpened)
        source.close();
    //
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    const QString text = codec->toUnicode(byteArray.constData(), byteArray.size(), &state);
    return (state.invalidChars > 0) ? byteArray : text;
}
