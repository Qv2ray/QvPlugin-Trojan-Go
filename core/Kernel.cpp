#include "Kernel.hpp"

#include "TrojanGoPlugin.hpp"

#include <QFile>

QvTrojanGoPluginKernel::QvTrojanGoPluginKernel() : Qv2rayPlugin::PluginKernel()
{
    connect(&process, &QProcess::channelReadyRead, this, &QvTrojanGoPluginKernel::OnProcessOutputReadyRead);
    connect(&process, &QProcess::stateChanged, this, &QvTrojanGoPluginKernel::OnProcessClosed);
}

bool QvTrojanGoPluginKernel::StartKernel()
{
    const auto executablePath = QvTrojanGoPluginInstance->GetSettngs()["kernelPath"].toString();
    if (!QFile::exists(executablePath))
    {
        QvTrojanGoPluginInstance->PluginErrorMessageBox(tr("Stupid Configuration?"),
                                                        tr("We cannot find your Trojan-Go kernel. Please configure it in the plugin settings."));
        return false;
    }

    process.setProgram(executablePath);
    process.setArguments({ "-url", url, "-url-option",
                           QString(mux ? "mux=true;listen=" : "mux=false;listen=") + listenAddress + ":" + QString::number(listenPort) });
    process.start();
    process.waitForStarted();
    isStarted = true;
    return true;
}

void QvTrojanGoPluginKernel::SetConnectionSettings(const QMap<Qv2rayPlugin::KernelOptionFlags, QVariant> &settings,
                                                   const QJsonObject &connectionInfo)
{
    listenAddress = settings[KERNEL_LISTEN_ADDRESS].toString();
    listenPort = settings[KERNEL_HTTP_ENABLED].toBool() ? settings[KERNEL_HTTP_PORT].toInt() : settings[KERNEL_SOCKS_PORT].toInt();
    url = TrojanGoSerializer().SerializeOutbound("trojan-go", "", "", connectionInfo, {});
    const auto obj = TrojanGoShareLinkObject::fromJson(connectionInfo);
    mux = obj.mux;
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
