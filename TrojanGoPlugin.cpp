#include "TrojanGoPlugin.hpp"

#include "ui/GUIInterface.hpp"

#include <QDateTime>
#include <QLabel>
#include <QMetaEnum>

bool QvTrojanGoPlugin::InitializePlugin(const QString &, const QJsonObject &_settings)
{
    emit PluginLog("Initialize plugin.");
    this->settings = _settings;
    QvTrojanGoPluginInstance = this;
    outboundHandler = std::make_shared<TrojanGoSerializer>();
    eventHandler = std::make_shared<SimpleEventHandler>();
    kernelInterface = std::make_unique<TrojanGoPluginKernelInterface>();
    guiInterface = new TrojanGoGUIInterface();
    return true;
}
