#include "TrojanGoSettingsWidget.hpp"

#include "TrojanGoPlugin.hpp"

#include <QFileDialog>
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

void TrojanGoSettingsWidget::on_selectKernelBtn_clicked()
{
    auto path = QFileDialog::getOpenFileName(this, tr("Open Kernel Path"));
    if (!path.isEmpty())
    {
        settings["kernelPath"] = path;
        kernelPathTxt->setText(path);
    }
}

void TrojanGoSettingsWidget::on_kernelPathTxt_textEdited(const QString &arg1)
{
    settings["kernelPath"] = arg1;
}
