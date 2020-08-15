#pragma once

#include "core/Common.hpp"
#include "interface/QvGUIPluginInterface.hpp"
#include "interface/QvPluginProcessor.hpp"
#include "ui_TrojanGoOutboundWidget.h"

class TrojanGoOutboundWidget
    : public Qv2rayPlugin::QvPluginEditor
    , private Ui::TrojanGoOutboundWidget
{
    Q_OBJECT

  public:
    explicit TrojanGoOutboundWidget(QWidget *parent = nullptr);

    void SetHostAddress(const QString &address, int port) override
    {
        config.server = address;
        config.port = port;
    }

    QPair<QString, int> GetHostAddress() const override
    {
        return { config.server, config.port };
    }

    void SetContent(const QJsonObject &o) override
    {
        config.loadJson(o);
        sniTxt->setText(config.sni);
        hostTxt->setText(config.host);
        pathTxt->setText(config.path);
        typeCombo->setCurrentText(TRANSPORT_TYPE_STRING_MAP[config.type]);
        encryptionTxt->setText(config.encryption);
        passwordTxt->setText(config.password);
        muxCB->setChecked(config.mux);
        on_typeCombo_currentIndexChanged(typeCombo->currentIndex());
    }

    const QJsonObject GetContent() const override
    {
        return config.toJson();
    }

  protected:
    TrojanGoShareLinkObject config;
    void changeEvent(QEvent *e) override;
  private slots:
    void on_sniTxt_textEdited(const QString &arg1);
    void on_typeCombo_currentIndexChanged(const QString &arg1);
    void on_hostTxt_textEdited(const QString &arg1);
    void on_pathTxt_textEdited(const QString &arg1);
    void on_encryptionTxt_textEdited(const QString &arg1);
    void on_passwordTxt_textEdited(const QString &arg1);
    void on_typeCombo_currentIndexChanged(int index);
    void on_muxCB_stateChanged(int arg1);
};
