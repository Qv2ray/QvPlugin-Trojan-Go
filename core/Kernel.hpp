#pragma once
#include "QvPluginProcessor.hpp"

#include <QProcess>

class QvTrojanGoPluginKernel : public Qv2rayPlugin::PluginKernel
{
    Q_OBJECT
  public:
    explicit QvTrojanGoPluginKernel();
    /// Kernel related operations
    void SetConnectionSettings(const QMap<Qv2rayPlugin::KernelOptionFlags, QVariant> &settings, const QJsonObject &connectionInfo) override;
    bool StartKernel() override;
    bool StopKernel() override;
    QString GetKernelName() const override
    {
        return "TrojanGo";
    }

  private slots:
    void OnProcessOutputReadyRead(int);
    void OnProcessClosed();

  private:
    QString url;
    QString listenAddress;
    QProcess process;
    bool mux;
    int listenPort;
    bool isStarted;
};

class TrojanGoPluginKernelInterface : public Qv2rayPlugin::PluginKernelInterface
{
  public:
    std::unique_ptr<Qv2rayPlugin::PluginKernel> CreateKernel() const override
    {
        return std::make_unique<QvTrojanGoPluginKernel>();
    }
    QList<QString> GetKernelProtocols() const override
    {
        return { "trojan-go" };
    }
};
