#pragma once

#include "QvPluginInterface.hpp"
#include "core/EventHandler.hpp"
#include "core/Kernel.hpp"
#include "core/Serializer.hpp"

#include <QObject>
#include <QtPlugin>

class QLabel;
using namespace Qv2rayPlugin;

class SimplePlugin
    : public QObject
    , Qv2rayInterface
{
    Q_INTERFACES(Qv2rayPlugin::Qv2rayInterface)
    Q_PLUGIN_METADATA(IID Qv2rayInterface_IID)
    Q_OBJECT
  public:
    //
    // Basic metainfo of this plugin
    const QvPluginMetadata GetMetadata() const override
    {
        QvPluginMetadata meta{
            "Trojan-Go Plugin",                                                                         //
            "Qv2ray Workgroup",                                                                         //
            "qvtrojango_plugin",                                                                        //
            "Connect to Trojan-GFW / Trojan-Go server in Qv2ray, conflicts with the Trojan-GFW plugin", //
            QIcon(":/qv2ray.png"),                                                                      //
            {},                                                                                         //
            { SPECIAL_TYPE_KERNEL,                                                                      //
              SPECIAL_TYPE_SERIALIZOR }                                                                 //
        };
        meta.KernelOutboundCapabilities = { { "Trojan-Go", "trojan-go" } };
        return meta;
    }

    std::unique_ptr<QvPluginKernel> CreateKernel() override;
    std::shared_ptr<QvPluginSerializer> GetSerializer() override;
    std::shared_ptr<QvPluginEventHandler> GetEventHandler() override;
    std::unique_ptr<QvPluginEditor> GetEditorWidget(UI_TYPE) override;
    std::unique_ptr<QWidget> GetSettingsWidget() override;
    //
    bool UpdateSettings(const QJsonObject &) override;
    bool Initialize(const QString &, const QJsonObject &) override;
    const QJsonObject GetSettngs() override;
    //
  signals:
    void PluginLog(const QString &) const override;
    void PluginErrorMessageBox(const QString &) const override;

  private:
    QJsonObject settings;
    std::shared_ptr<QvPluginSerializer> serializer;
    std::shared_ptr<QvPluginEventHandler> eventHandler;
};
