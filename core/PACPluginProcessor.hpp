#pragma once

#include "QvPACHandler.hpp"
#include "QvPluginProcessor.hpp"

class PACPluginProcessor : public Qv2rayPlugin::QvPluginEventHandler
{
  public:
    explicit PACPluginProcessor(QObject *parent = nullptr);
    QvPlugin_EventHandler_Decl(SystemProxy);
    QvPlugin_EventHandler_Decl(Connectivity);

  private:
    PACServer server;
    bool isPACStarted;
};
