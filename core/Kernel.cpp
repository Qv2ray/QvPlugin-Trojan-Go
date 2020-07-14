#include "Kernel.hpp"

#include "TrojanGoPlugin.hpp"

QvTrojanGoPluginKernel::QvTrojanGoPluginKernel(QObject *parent) : Qv2rayPlugin::QvPluginKernel(parent)
{
    connect(&process, &QProcess::channelReadyRead, this, &QvTrojanGoPluginKernel::OnProcessOutputReadyRead);
    connect(&process, &QProcess::stateChanged, this, &QvTrojanGoPluginKernel::OnProcessClosed);
}

bool QvTrojanGoPluginKernel::StartKernel()
{
    process.setProgram(PluginInstance->GetSettngs()["kernelPath"].toString());
    process.setArguments({ "-url", url, "-url-option",
                           QString(mux ? "mux=true;listen=" : "mux=false;listen=") + listenAddress + ":" + QString::number(listenPort) });
    process.start();
    process.waitForStarted();
    isStarted = true;
    return true;
}

void QvTrojanGoPluginKernel::SetConnectionSettings(const QMap<KernelSetting, QVariant> &settings, const QJsonObject &connectionInfo)
{
    mux = true;
    listenAddress = settings[KERNEL_LISTEN_ADDRESS].toString();
    listenPort = settings[KERNEL_HTTP_ENABLED].toBool() ? settings[KERNEL_HTTP_PORT].toInt() : settings[KERNEL_SOCKS_PORT].toInt();
    url = TrojanGoSerializer().SerializeOutbound("trojan-go", "", "", connectionInfo);
}

void QvTrojanGoPluginKernel::OnProcessClosed()
{
    if (isStarted)
    {
        if (process.state() == QProcess::NotRunning)
            emit OnKernelCrashed(tr("Trojan-Go Kernel Crashed."));
    }
}

void QvTrojanGoPluginKernel::OnProcessOutputReadyRead(int)
{
    emit OnKernelLogAvailable(process.readAll());
}

bool QvTrojanGoPluginKernel::StopKernel()
{
    isStarted = false;
    process.close();
    process.waitForFinished();
    return true;
}
