#pragma once
#include "interface/QvGUIPluginInterface.hpp"
#include "widgets/TrojanGoOutboundWidget.hpp"
#include "widgets/TrojanGoSettingsWidget.hpp"

using namespace Qv2rayPlugin;
class TrojanGoGUIInterface : public PluginGUIInterface
{
    QIcon Icon() const override
    {
        return QIcon(":/assets/qv2ray.png");
    }

    QList<PluginGuiComponentType> GetComponents() const override
    {
        return { GUI_COMPONENT_SETTINGS, GUI_COMPONENT_OUTBOUND_EDITOR };
    }

    std::unique_ptr<QvPluginSettingsWidget> createSettingsWidgets() const override
    {
        return std::make_unique<TrojanGoSettingsWidget>();
    }

    QList<typed_plugin_editor> createInboundEditors() const override
    {
        return {};
    }

    QList<typed_plugin_editor> createOutboundEditors() const override
    {
        return { MakeEditorInfoPair<TrojanGoOutboundWidget>("trojan-go", "Trojan Go") };
    }

    std::unique_ptr<QvPluginMainWindowWidget> createMainWindowWidget() const override
    {
        return nullptr;
    }
};
