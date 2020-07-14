#pragma once

#include "ui_TrojanGoSettingsWidget.h"

class TrojanGoSettingsWidget
    : public QWidget
    , private Ui::TrojanGoSettingsWidget
{
    Q_OBJECT

  public:
    explicit TrojanGoSettingsWidget(QWidget *parent = nullptr);

  protected:
    void changeEvent(QEvent *e);
  private slots:
    void on_selectKernelBtn_clicked();
    void on_kernelPathTxt_textEdited(const QString &arg1);
};
