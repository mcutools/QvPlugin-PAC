#pragma once

#include "PACConfig.hpp"
#include "ui_QvPACPluginSettingsWidget.h"

#include <QJsonObject>
#include <QWidget>

class QvPACPluginSettingsWidget
    : public QWidget
    , private Ui::QvPACPluginSettingsWidget
{
    Q_OBJECT

  public:
    explicit QvPACPluginSettingsWidget(QWidget *parent = nullptr);
    ~QvPACPluginSettingsWidget();

  private slots:
    void on_pacGoBtn_clicked();
    void on_pacPortSB_valueChanged(int arg1);
    void on_pacProxyCB_currentIndexChanged(int index);
    void on_pacProxyTxt_textEdited(const QString &arg1);
    void on_pacProxyTxt_textChanged(const QString &arg1);
    void on_pacGroupBox_clicked(bool checked);
    void on_pacListenAddrTxt_textEdited(const QString &arg1);
    void on_openPACDirBtn_clicked();
    void on_pacSystemProxyCB_stateChanged(int arg1);

  private:
    PACSettings settings;
};
