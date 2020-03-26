#pragma once

#include "interface/QvPluginInterface.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QTextCodec>
#include <QtPlugin>
#include <memory>

class PACServer;

class QLabel;

class PACPlugin
    : public QObject
    , Qv2ray::Qv2rayInterface
{
    Q_INTERFACES(Qv2ray::Qv2rayInterface)
    Q_PLUGIN_METADATA(IID Qv2rayInterface_IID)
    Q_OBJECT
  public:
    Qv2ray::QV2RAY_PLUGIN_HOOK_TYPE_FLAGS PluginHooks() const override;
    Qv2ray::QV2RAY_SPECIAL_PLUGIN_TYPE_FLAGS SpecialPluginType() const override;
    //
    // Basic metainfo of this plugin
    QString Name() const override;
    QString InternalName() const override;
    QString Author() const override;
    QString Description() const override;
    //
    QStringList OutboundTypes() const override;
    //
    QWidget *GetSettingsWidget() override;
    Qv2ray::Qv2rayPluginEditorWidget *GetEditorWidget(Qv2ray::QV2RAY_PLUGIN_UI_TYPE) override;
    QObject *GetQObject() override;
    Qv2ray::Qv2rayKernelPluginObject *GetKernelInstance() override;
    //
    bool UpdatePluginSettings(const QJsonObject &) override;
    bool InitializePlugin(const QString &, const QJsonObject &) override;
    const QIcon Icon() const override;
    const QJsonObject GetPluginSettngs() override;
    //
    /// The hook function, for SPECIAL_TYPE_NONE
    void ProcessHook(Qv2ray::QV2RAY_PLUGIN_HOOK_TYPE, Qv2ray::QV2RAY_PLUGIN_HOOK_SUBTYPE, QVariant *) override;
  signals:
    void PluginLog(const QString &) const override;
    void PluginErrorMessageBox(const QString &) const override;

  public:
    const QString GetConfigPath() const
    {
        return configPath;
    }

  private:
    QString configPath;
    QJsonObject settings;
    PACServer *server;
};

extern PACPlugin *pluginInstance;

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
