#include "TrojanGoPlugin.hpp"

#include "widgets/TrojanGoSettingsWidget.hpp"

#include <QDateTime>
#include <QLabel>
#include <QMetaEnum>

std::unique_ptr<QvPluginKernel> QvTrojanGoPlugin::CreateKernel()
{
    return std::make_unique<QvTrojanGoPluginKernel>(this);
}

bool QvTrojanGoPlugin::UpdateSettings(const QJsonObject &conf)
{
    settings = conf;
    return true;
}

bool QvTrojanGoPlugin::Initialize(const QString &, const QJsonObject &settings)
{
    emit PluginLog("Initialize plugin.");
    PluginInstance = this;
    this->settings = settings;
    serializer = std::make_shared<TrojanGoSerializer>(this);
    eventHandler = std::make_shared<SimpleEventHandler>(this);
    return true;
}

const QJsonObject QvTrojanGoPlugin::GetSettngs()
{
    return settings;
}

std::shared_ptr<QvPluginEventHandler> QvTrojanGoPlugin::GetEventHandler()
{
    return eventHandler;
}

std::unique_ptr<QWidget> QvTrojanGoPlugin::GetSettingsWidget()
{
    return std::make_unique<TrojanGoSettingsWidget>();
}

std::unique_ptr<QvPluginEditor> QvTrojanGoPlugin::GetEditorWidget(UI_TYPE type)
{
    switch (type)
    {
        case UI_TYPE_INBOUND_EDITOR:
        case UI_TYPE_OUTBOUND_EDITOR:
        case UI_TYPE_GROUP_EDITOR: break;
    }
    return nullptr;
}

std::shared_ptr<QvPluginSerializer> QvTrojanGoPlugin::GetSerializer()
{
    return serializer;
}
