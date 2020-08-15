#pragma once
#include "QvPluginProcessor.hpp"

class SimpleEventHandler : public Qv2rayPlugin::PluginEventHandler
{
  public:
    SimpleEventHandler();
    QvPlugin_EventHandler_Decl(SystemProxy);
    QvPlugin_EventHandler_Decl(Connectivity);
    QvPlugin_EventHandler_Decl(ConnectionStats);
    QvPlugin_EventHandler_Decl(ConnectionEntry);
};
