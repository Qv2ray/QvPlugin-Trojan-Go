#include "TrojanGoSettingsWidget.hpp"

#include "TrojanGoPlugin.hpp"

#include <QDrag>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>

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
    const QStringList nonKernelExtensions = { ".so", ".dll", ".dylib", ".zip", ".rar", ".gz", ".tar", ".xz", ".txt" };
    if (const auto it =
            std::find_if(nonKernelExtensions.cbegin(), nonKernelExtensions.cend(),
                         [&pathToKernel](const QString &extension) -> bool { return pathToKernel.endsWith(extension, Qt::CaseInsensitive); });
        it != nonKernelExtensions.cend())
        return { KernelPathCheckerResult::KPCR_WARNING, tr("The suffix `%1` does not seem to be of a Trojan-Go Core.\r\n"
                                                           "Its filename should have been like `trojan-go` or `trojan-go.exe`.")
                                                            .arg(*it) };

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
    const auto path = QFileDialog::getOpenFileName(this, tr("Select Trojan-Go Core"), platformDefaultKernelDir, platformKernelPathFilter);
    if (path.isEmpty())
        return;

    // debounce: disabling this button for now
    selectKernelBtn->setEnabled(false);

    if (handleKernelPathCheck(path))
    {
        settings.insert("kernelPath", path);
        kernelPathTxt->setText(path);
    }

    // debounce: recover in 800ms
    debounceTimer.singleShot(800, this, &TrojanGoSettingsWidget::debounceUnfreeze);
}

void TrojanGoSettingsWidget::debounceUnfreeze()
{
    selectKernelBtn->setEnabled(true);
}

bool TrojanGoSettingsWidget::handleKernelPathCheck(const QString &pathToCheck)
{
    const auto [result, someErrMsg] = this->preliminaryKernelPathChecker(pathToCheck);
    switch (result)
    {
        case KernelPathCheckerResult::KPCR_VERY_OK: break;
        case KernelPathCheckerResult::KPCR_FAILURE:
        {
            QMessageBox::critical(this, tr("Trojan-Go Core Path Check Failed"),
                                  tr("Path `%1` did not pass kernel path checker:\r\n%2").arg(pathToCheck, *someErrMsg));
            return false;
        }
        case KernelPathCheckerResult::KPCR_WARNING:
        {
            const auto choice =
                QMessageBox::question(this, tr("Trojan-Go Core Path Warning"),
                                      tr("Path `%1` might not be the correct Trojan-Go kernel path:\r\n%2").arg(pathToCheck, *someErrMsg),
                                      QMessageBox::Ignore | QMessageBox::Abort, QMessageBox::Abort);
            if (choice == QMessageBox::Abort)
                return false;
        }
    }
    return true;
}

void TrojanGoSettingsWidget::dragEnterEvent(QDragEnterEvent *event)
{
    // accept only file-like drops
    const auto mimeData = event->mimeData();
    if (!mimeData->hasUrls())
    {
        event->ignore();
        return;
    }

    // accept only single file drop
    if (const auto &urlList = mimeData->urls(); urlList.isEmpty() || urlList.length() != 1)
    {
        event->ignore();
        return;
    }

    // else: let the drag pass
    event->acceptProposedAction();
}

void TrojanGoSettingsWidget::dropEvent(QDropEvent *event)
{
    const auto mimeData = event->mimeData();
    if (!mimeData->hasUrls())
    {
        return;
    }

    const auto path = mimeData->urls().first().toLocalFile();
    if (path.isEmpty())
    {
        return;
    }

    if (handleKernelPathCheck(path))
    {
        settings.insert("kernelPath", path);
        kernelPathTxt->setText(path);
    }
}

void TrojanGoSettingsWidget::on_testKernelBtn_clicked()
{
    const auto path = settings["kernelPath"].toString();
    if (path.isEmpty())
    {
        QMessageBox::critical(this, tr("Nothing to Test"), tr("Kernel Path is empty."));
        return;
    }

    QProcess process;
#ifdef Q_OS_WIN32
    process.setProgram(path);
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.setNativeArguments("--version");
    process.start();
#else
    process.start(path, { "--version" });
#endif
    process.waitForStarted();
    process.waitForFinished();

    const auto exitCode = process.exitCode();
    if (exitCode != 0)
    {
        QMessageBox::critical(this, tr("Trojan-Go Core Test Failed"),
                              tr("Trojan-Go Core failed with exit code %1.\r\nReason: %2").arg(exitCode).arg(process.errorString()));
        return;
    }

    QString output = process.readAllStandardOutput();
    QMessageBox::information(this, tr("Trojan-Go Test Result"), output);
}
