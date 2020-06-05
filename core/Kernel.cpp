#include "Kernel.hpp"

SimpleKernel::SimpleKernel(QObject *parent) : Qv2rayPlugin::QvPluginKernel(parent)
{
}
bool SimpleKernel::StartKernel()
{
    return true;
}
void SimpleKernel::SetConnectionSettings(const QMap<KernelSetting, QVariant> &settings, const QJsonObject &connectionInfo)
{
    Q_UNUSED(settings)
    Q_UNUSED(connectionInfo)
}
bool SimpleKernel::StopKernel()
{
    return true;
}
