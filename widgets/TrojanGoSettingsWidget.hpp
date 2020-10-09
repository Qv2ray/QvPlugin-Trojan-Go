#pragma once

#include "interface/QvGUIPluginInterface.hpp"
#include "ui_TrojanGoSettingsWidget.h"

#include <QTimer>
#include <optional>

/**
 * @brief Kernel Path Checker Result
 */
enum KernelPathCheckerResult : int
{
    /**
     * @brief VERY_OK: So far so good.
     */
    KPCR_VERY_OK = 0,
    /**
     * @brief WARNING: This does not smell good.
     */
    KPCR_WARNING = 1,
    /**
     * @brief FAILURE: Totally failure.
     */
    KPCR_FAILURE = 2,
};

class TrojanGoSettingsWidget
    : public Qv2rayPlugin::QvPluginSettingsWidget
    , private Ui::TrojanGoSettingsWidget
{
    Q_OBJECT

  public:
    explicit TrojanGoSettingsWidget(QWidget *parent = nullptr);
    void SetSettings(const QJsonObject &s) override
    {
        settings = s;
        kernelPathTxt->setText(settings["kernelPath"].toString());
    }
    QJsonObject GetSettings() override
    {
        return settings;
    }

  protected:
    void changeEvent(QEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

  private slots:
    void debounceUnfreeze();
    void on_selectKernelBtn_clicked();
    void on_testKernelBtn_clicked();

  private:
    QJsonObject settings;
    QTimer debounceTimer;

  private:
    // platform-dependent options.
#if defined(Q_OS_MAC)
    const inline static char *platformDefaultKernelDir = "/usr/local/bin";
    const inline static char *platformKernelPathFilter = "Trojan-Go Core(trojan-go*);;All Files(*)";
#elif defined(Q_OS_UNIX)
    const inline static char *platformDefaultKernelDir = "/usr/bin";
    const inline static char *platformKernelPathFilter = "Trojan-Go Core(trojan-go*);;All Files(*)";
#elif defined(Q_OS_WIN)
    const inline static char *platformDefaultKernelDir = "";
    const inline static char *platformKernelPathFilter = "Trojan-Go Core(trojan-go.exe);;All Files(*)";
#else
    const inline static char *platformDefaultKernelDir = "";
    const inline static char *platformKernelPathFilter = "";
#endif

  private:
    /**
     * @brief Priliminary Kernel Path Checker
     * @param pathToKernel the kernel path to be checked.
     * @return if the kernel path check passes, and if not, the detailed error message.
     */
    std::tuple<KernelPathCheckerResult, std::optional<QString>> preliminaryKernelPathChecker(const QString &pathToKernel);

    /**
     * @brief The UI routine to handle kernel path check.
     *        To be reused by multiple part of this program.
     * @param pathToCheck the path to be checked.
     * @return if the backend should accept the user input.
     */
    bool handleKernelPathCheck(const QString &pathToCheck);
};
