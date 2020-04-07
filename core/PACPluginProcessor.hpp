#pragma once

#include "Qv2rayPluginProcessor.hpp"
#include "QvPACHandler.hpp"

class PACPluginProcessor : public Qv2rayPlugin::QvPluginEventHandler
{
  public:
    explicit PACPluginProcessor(QObject *parent = nullptr);
    QVPlUGIN_DECL_EVENT_HANDLER_OVERRIDE(SystemProxy);
    QVPlUGIN_DECL_EVENT_HANDLER_OVERRIDE(Connectivity);

  private:
    PACServer *server;
    bool canStartPAC;
};
