#ifndef DIAMETERSETTINGSDIALOG_H
#define DIAMETERSETTINGSDIALOG_H

#include "SettingsDialog.h"
#include "../controllers/DiameterController.h"

class DiameterSettingsDialog : public SettingsDialog {
    Q_OBJECT

public:
    explicit DiameterSettingsDialog(QWidget *parent = nullptr);

    int getPointCount() const { return pointCountSpin_->value(); }
    DiameterController::Mode getMode() const;

private:
    QSpinBox *pointCountSpin_;
    QComboBox *algorithmCombo_;
};

#endif
