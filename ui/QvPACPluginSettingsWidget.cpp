#include "QvPACPluginSettingsWidget.hpp"

#include "PACPlugin.hpp"
#include "common/HTTPRequestHelper.hpp"

#include <QFileDialog>
#include <QMessageBox>

QvPACPluginSettingsWidget::QvPACPluginSettingsWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    this->settings = pluginInstance->GetPluginSettngs();
    bool pacEnabled = settings["enablePAC"].toBool(true);
    pacGroupBox->setChecked(pacEnabled);
    //
    // PAC
    pacPortSB->setValue(settings["port"].toInt(0));
    pacProxyTxt->setText(settings["localIP"].toString());
    pacProxyCB->setCurrentIndex(settings["useSocksProxy"].toBool() ? 1 : 0);
    pacListenAddrLabel->setText("http://" + (pacProxyTxt->text().isEmpty() ? "127.0.0.1" : pacProxyTxt->text()) + ":" +
                                QString::number(pacPortSB->value()) + "/pac");
}

QvPACPluginSettingsWidget::~QvPACPluginSettingsWidget()
{
}

void QvPACPluginSettingsWidget::on_pacProxyTxt_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

    pacListenAddrLabel->setText("http://" + (pacProxyTxt->text().isEmpty() ? "127.0.0.1" : pacProxyTxt->text()) + ":" +
                                QString::number(pacPortSB->value()) + "/pac");
}

void QvPACPluginSettingsWidget::on_pacProxyCB_currentIndexChanged(int index)
{
    // 0 -> http
    // 1 -> socks
    settings["useSocksProxy"] = index == 1;
}

void QvPACPluginSettingsWidget::on_pacGroupBox_clicked(bool checked)
{
    settings["enablePAC"] = checked;
    if (checked)
    {
        QMessageBox::warning(
            this, QObject::tr("Deprecated"),
            QObject::tr("PAC is now deprecated and is not encouraged to be used anymore.") + "\r\n" +
                QObject::tr("It will be removed or be provided as a plugin in the future.") + "\r\n\r\n" +
                QObject::tr("PAC will still work currently, but please switch to the V2ray built-in routing as soon as possible."));
    }
}

void QvPACPluginSettingsWidget::on_pacProxyTxt_textEdited(const QString &arg1)
{
    settings["localIP"] = arg1;
}

void QvPACPluginSettingsWidget::on_pacGoBtn_clicked()
{
    QString gfwLocation;
    QString fileContent;
    pacGoBtn->setEnabled(false);
    gfwListCB->setEnabled(false);
    bool withProxy = getGFWListWithProxyCB->isChecked();
    QvHttpRequestHelper request;
    emit pluginInstance->PluginLog("Downloading GFWList file.");

    switch (gfwListCB->currentIndex())
    {
        case 0:
            gfwLocation = "https://gitlab.com/gfwlist/gfwlist/raw/master/gfwlist.txt";
            fileContent = QString::fromUtf8(request.syncget(gfwLocation, withProxy));
            break;

        case 1:
            gfwLocation = "https://pagure.io/gfwlist/raw/master/f/gfwlist.txt";
            fileContent = QString::fromUtf8(request.syncget(gfwLocation, withProxy));
            break;

        case 2:
            gfwLocation = "http://repo.or.cz/gfwlist.git/blob_plain/HEAD:/gfwlist.txt";
            fileContent = QString::fromUtf8(request.syncget(gfwLocation, withProxy));
            break;

        case 3:
            gfwLocation = "https://bitbucket.org/gfwlist/gfwlist/raw/HEAD/gfwlist.txt";
            fileContent = QString::fromUtf8(request.syncget(gfwLocation, withProxy));
            break;

        case 4:
            gfwLocation = "https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt";
            fileContent = QString::fromUtf8(request.syncget(gfwLocation, withProxy));
            break;

        case 5:
            gfwLocation = "https://git.tuxfamily.org/gfwlist/gfwlist.git/plain/gfwlist.txt";
            fileContent = QString::fromUtf8(request.syncget(gfwLocation, withProxy));
            break;

        case 6:
            auto file = QFileDialog::getOpenFileName(this, tr("Select GFWList in base64"));

            if (file.isEmpty())
            {
                QMessageBox::warning(this, tr("Download GFWList"), tr("Operation is cancelled."));
                return;
            }

            fileContent = StringFromFile(file);
            break;
    }

    QMessageBox::warning(this, tr("Download GFWList"), tr("Successfully downloaded GFWList."));
    pacGoBtn->setEnabled(true);
    gfwListCB->setEnabled(true);

    auto configPath = pluginInstance->GetConfigPath();
    if (!QDir(configPath).exists())
    {
        QDir(configPath).mkpath(configPath);
    }
    auto path = configPath + "/gfwList.txt";
    StringToFile(fileContent, path);
}

void QvPACPluginSettingsWidget::on_pacPortSB_valueChanged(int arg1)
{
    settings["port"] = arg1;
    pacListenAddrLabel->setText("http://" + (pacProxyTxt->text().isEmpty() ? "127.0.0.1" : pacProxyTxt->text()) + ":" +
                                QString::number(pacPortSB->value()) + "/pac");
}

void QvPACPluginSettingsWidget::on_buttonBox_accepted()
{
    pluginInstance->UpdatePluginSettings(settings);
}

void QvPACPluginSettingsWidget::on_pacListenAddrTxt_textEdited(const QString &arg1)
{
    settings["listenip"] = arg1;
}
