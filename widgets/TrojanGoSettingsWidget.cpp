#include "TrojanGoSettingsWidget.hpp"

#include "TrojanGoPlugin.hpp"

#include <QFileDialog>
TrojanGoSettingsWidget::TrojanGoSettingsWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    auto settings = PluginInstance->GetSettngs();
    kernelPathTxt->setText(settings["kernelPath"].toString());
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

void TrojanGoSettingsWidget::on_selectKernelBtn_clicked()
{
    auto path = QFileDialog::getOpenFileName(this, tr("Open Kernel Path"));
    auto settings = PluginInstance->GetSettngs();
    settings["kernelPath"] = path;
    PluginInstance->UpdateSettings(settings);
    kernelPathTxt->setText(path);
}

void TrojanGoSettingsWidget::on_kernelPathTxt_textEdited(const QString &arg1)
{
    auto settings = PluginInstance->GetSettngs();
    settings["kernelPath"] = arg1;
    PluginInstance->UpdateSettings(settings);
}
