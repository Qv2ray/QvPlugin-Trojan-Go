#pragma once

#include "interface/QvGUIPluginInterface.hpp"
#include "ui_TrojanGoSettingsWidget.h"

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

  private slots:
    void on_selectKernelBtn_clicked();
    void on_kernelPathTxt_textEdited(const QString &arg1);

  private:
    QJsonObject settings;

  private:
    /**
     * @brief Priliminary Kernel Path Checker
     * @param pathToKernel the kernel path to be checked.
     * @return if the kernel path check passes, and if not, the detailed error message.
     */
    std::tuple<KernelPathCheckerResult, std::optional<QString>> preliminaryKernelPathChecker(const QString &pathToKernel);
};
