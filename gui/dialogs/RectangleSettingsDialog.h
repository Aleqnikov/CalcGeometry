#ifndef RECTANGLESETTINGSDIALOG_H
#define RECTANGLESETTINGSDIALOG_H

#include "SettingsDialog.h"

class RectangleSettingsDialog : public SettingsDialog {
    Q_OBJECT

public:
    explicit RectangleSettingsDialog(QWidget *parent = nullptr);
    int getPointCount() const { return pointCountSpin_->value(); }

private:
    QSpinBox *pointCountSpin_;
};

#endif
