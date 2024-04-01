#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_XTRemoteControl.h"

class XTRemoteControl : public QMainWindow
{
    Q_OBJECT

public:
    XTRemoteControl(QWidget *parent = nullptr);
    ~XTRemoteControl();

private:
    Ui::XTRemoteControlClass ui;
};
