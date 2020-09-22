#pragma once

#include "QvPluginInterface.hpp"
#include "core/EventHandler.hpp"
#include "core/Kernel.hpp"
#include "core/Serializer.hpp"

#include <QObject>
#include <QtPlugin>

using namespace Qv2rayPlugin;

class QvTrojanGoPlugin
    : public QObject
    , public Qv2rayInterface
{
    Q_INTERFACES(Qv2rayPlugin::Qv2rayInterface)
    Q_PLUGIN_METADATA(IID Qv2rayInterface_IID)
    Q_OBJECT
  public:
    // Basic metainfo of this plugin
    const QvPluginMetadata GetMetadata() const override
    {
        return QvPluginMetadata{
            "Trojan-Go Plugin",                                 //
            "Qv2ray Workgroup",                                 //
            "qvtrojango_plugin",                                //
            "Connect to Trojan-GFW/Trojan-Go server in Qv2ray", //
            "v3.0.0",                                           //
            "Qv2ray/QvPlugin-Command",                          //
            {
                COMPONENT_GUI,             //
                COMPONENT_KERNEL,          //
                COMPONENT_OUTBOUND_HANDLER //
            },
            UPDATE_GITHUB_RELEASE //
        };
    }
    bool InitializePlugin(const QString &, const QJsonObject &) override;

  signals:
    void PluginLog(const QString &) const override;
    void PluginErrorMessageBox(const QString &, const QString &) const override;
};

DECLARE_PLUGIN_INSTANCE(QvTrojanGoPlugin);
