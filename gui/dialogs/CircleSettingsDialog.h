#ifndef CIRCLESETTINGSDIALOG_H
#define CIRCLESETTINGSDIALOG_H

#include "SettingsDialog.h"
#include "../controllers/CircleController.h"

class CircleSettingsDialog : public SettingsDialog {
    Q_OBJECT

public:
    explicit CircleSettingsDialog(QWidget *parent = nullptr);

    int getPointCount() const { return pointCountSpin_->value(); }
    CircleController::Mode getMode() const;

private:
    QSpinBox *pointCountSpin_;
    QComboBox *modeCombo_;
};

#endif
