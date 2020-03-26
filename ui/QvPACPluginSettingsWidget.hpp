#pragma once

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

  public slots:
    void on_pacGoBtn_clicked();
    void on_pacPortSB_valueChanged(int arg1);
    void on_pacProxyCB_currentIndexChanged(int index);
    void on_pacProxyTxt_textEdited(const QString &arg1);
    void on_pacProxyTxt_textChanged(const QString &arg1);
    void on_pacGroupBox_clicked(bool checked);

  private slots:
    void on_buttonBox_accepted();

    void on_pacListenAddrTxt_textEdited(const QString &arg1);

  private:
    QJsonObject settings;
};
