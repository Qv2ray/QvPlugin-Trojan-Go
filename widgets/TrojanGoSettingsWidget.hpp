#pragma once

#include "interface/QvGUIPluginInterface.hpp"
#include "ui_TrojanGoSettingsWidget.h"

class TrojanGoSettingsWidget
    : public Qv2rayPlugin::QvPluginSettingsWidget
    , private Ui::TrojanGoSettingsWidget
{
    Q_OBJECT

  public:
    explicit TrojanGoSettingsWidget(QWidget *parent = nullptr);
    void SetSettings(const QJsonObject &s) override
    {
        settings = s;
        kernelPathTxt->setText(settings["kernelPath"].toString());
    }
    QJsonObject GetSettings() override
    {
        return settings;
    }

  protected:
    void changeEvent(QEvent *e) override;

  private slots:
    void on_selectKernelBtn_clicked();
    void on_kernelPathTxt_textEdited(const QString &arg1);

  private:
    QJsonObject settings;
};
