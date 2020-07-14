#pragma once
#include "QvPluginProcessor.hpp"

class QvTrojanGoPluginKernel : public Qv2rayPlugin::QvPluginKernel
{
  public:
    explicit QvTrojanGoPluginKernel(QObject *parent = nullptr);
    /// Kernel related operations
    void SetConnectionSettings(const QMap<KernelSetting, QVariant> &settings, const QJsonObject &connectionInfo) override;
    bool StartKernel() override;
    bool StopKernel() override;
};
