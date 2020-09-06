#include "TrojanGoPlugin.hpp"

#include "ui/GUIInterface.hpp"

#include <QDateTime>
#include <QLabel>
#include <QMetaEnum>

bool QvTrojanGoPlugin::InitializePlugin(const QString &, const QJsonObject &)
{
    emit PluginLog("Initialize plugin.");
    outboundHandler = std::make_shared<TrojanGoSerializer>();
    eventHandler = std::make_shared<SimpleEventHandler>();
    kernelInterface = std::make_unique<TrojanGoPluginKernelInterface>();
    guiInterface = new TrojanGoGUIInterface();
    return true;
}
