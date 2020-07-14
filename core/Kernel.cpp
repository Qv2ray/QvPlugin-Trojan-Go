#include "Kernel.hpp"

QvTrojanGoPluginKernel::QvTrojanGoPluginKernel(QObject *parent) : Qv2rayPlugin::QvPluginKernel(parent)
{
}
bool QvTrojanGoPluginKernel::StartKernel()
{
    return true;
}
void QvTrojanGoPluginKernel::SetConnectionSettings(const QMap<KernelSetting, QVariant> &settings, const QJsonObject &connectionInfo)
{
    Q_UNUSED(settings)
    Q_UNUSED(connectionInfo)
}
bool QvTrojanGoPluginKernel::StopKernel()
{
    return true;
}
