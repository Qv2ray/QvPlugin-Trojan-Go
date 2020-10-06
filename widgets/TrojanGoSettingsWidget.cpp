#include "TrojanGoSettingsWidget.hpp"

#include "TrojanGoPlugin.hpp"

#include <QFileDialog>
#include <QMessageBox>

TrojanGoSettingsWidget::TrojanGoSettingsWidget(QWidget *parent) : Qv2rayPlugin::QvPluginSettingsWidget(parent)
{
    setupUi(this);
}

void TrojanGoSettingsWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange: retranslateUi(this); break;
        default: break;
    }
}

std::tuple<KernelPathCheckerResult, std::optional<QString>> TrojanGoSettingsWidget::preliminaryKernelPathChecker(const QString &pathToKernel)
{
    // some random extensions that doesn't seem to be a Trojan-Go kernel.
    const QStringList nonKernelExtensions = { ".so", ".dll", ".dylib", ".zip", ".rar", ".gz", ".tar", ".xz" };
    if (std::any_of(nonKernelExtensions.cbegin(), nonKernelExtensions.cend(),
                    [&pathToKernel](const QString &extension) -> bool { return pathToKernel.endsWith(extension, Qt::CaseInsensitive); }))
        return { KernelPathCheckerResult::KPCR_WARNING, tr("Suffix `%1` does not seem to be a Trojan-Go Core.\r\n"
                                                           "It should be like `trojan-go` or `trojan-go.exe`.") };

    // check the existance, read permission and file size issues.
    if (QFile kernelFile(pathToKernel); !kernelFile.exists())
        return { KernelPathCheckerResult::KPCR_FAILURE, tr("Kernel file does not exist.") };
    else if (!kernelFile.open(QIODevice::ReadOnly))
        return { KernelPathCheckerResult::KPCR_FAILURE, tr("Kernel file is not accessible.") };
    else if (kernelFile.atEnd())
        return { KernelPathCheckerResult::KPCR_FAILURE, tr("Kernel file is empty.") };

    // so far so good.
    return { KernelPathCheckerResult::KPCR_VERY_OK, std::nullopt };
}

void TrojanGoSettingsWidget::on_selectKernelBtn_clicked()
{

#if defined(Q_OS_MAC)
    const static QString platformDefaultKernelDir = "/usr/local/bin";
    const static QString platformKernelPathFilter = "Trojan-Go Core(trojan-go*);;All Files(*)";
#elif defined(Q_OS_UNIX)
    const static QString platformDefaultKernelDir = "/usr/bin";
    const static QString platformKernelPathFilter = "Trojan-Go Core(trojan-go*);;All Files(*)";
#elif defined(Q_OS_WIN)
    const static QString platformDefaultKernelDir = "";
    const static QString platformKernelPathFilter = "Trojan-Go Core(trojan-go.exe);;All Files(*)";
#else
    const static QString platformDefaultKernelDir = "";
    const static QString platformKernelPathFilter = "";
#endif

    const auto path = QFileDialog::getOpenFileName(this, tr("Select Trojan-Go Core"), platformDefaultKernelDir, platformKernelPathFilter);
    if (path.isEmpty())
        return;

    const auto [result, someErrMsg] = this->preliminaryKernelPathChecker(path);
    switch (result)
    {
        case KernelPathCheckerResult::KPCR_VERY_OK: break;
        case KernelPathCheckerResult::KPCR_FAILURE:
        {
            QMessageBox::critical(this, tr("Trojan-Go Core Path Check Failed"),
                                  tr("Path `%1` did not pass kernel path checker:\r\n%2").arg(path, *someErrMsg));
            return;
        }
        case KernelPathCheckerResult::KPCR_WARNING:
        {
            const auto choice = QMessageBox::question(this, tr("Trojan-Go Core Path Warning"),
                                                      tr("Path `%1` might not be the correct Trojan-Go kernel path:\r\n%2").arg(path, *someErrMsg),
                                                      QMessageBox::Ignore | QMessageBox::Abort, QMessageBox::Abort);
            if (choice == QMessageBox::Abort)
                return;
        }
    }

    settings.insert("kernelPath", path);
    kernelPathTxt->setText(path);
}

void TrojanGoSettingsWidget::on_kernelPathTxt_textEdited(const QString &arg1)
{
    settings.insert("kernelPath", arg1);
}
