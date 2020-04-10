#include "QvPACPluginSettingsWidget.hpp"

#include "HTTPRequestHelper.hpp"
#include "PACPlugin.hpp"

#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>

QvPACPluginSettingsWidget::QvPACPluginSettingsWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    this->settings = pluginInstance->GetSettngs();
    pacGroupBox->setChecked(settings["enablePAC"].toBool(true));
    pacPortSB->setValue(settings["port"].toInt(0));
    pacProxyTxt->setText(settings["localIP"].toString());
    pacProxyCB->setCurrentIndex(settings["useSocksProxy"].toBool() ? 1 : 0);
    pacListenAddrLabel->setText("http://" + (pacProxyTxt->text().isEmpty() ? "127.0.0.1" : pacProxyTxt->text()) + ":" +
                                QString::number(pacPortSB->value()) + "/pac");
    pacSystemProxyCB->setChecked(settings["setSystemProxy"].toBool());
}

QvPACPluginSettingsWidget::~QvPACPluginSettingsWidget()
{
    pluginInstance->UpdateSettings(settings);
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

void QvPACPluginSettingsWidget::on_pacListenAddrTxt_textEdited(const QString &arg1)
{
    settings["listenip"] = arg1;
}

void QvPACPluginSettingsWidget::on_openPACDirBtn_clicked()
{
    QDesktopServices::openUrl(QUrl::fromUserInput(pluginInstance->GetConfigPath()));
}

void QvPACPluginSettingsWidget::on_pacSystemProxyCB_stateChanged(int arg1)
{
    settings["setSystemProxy"] = arg1 == Qt::Checked;
}
