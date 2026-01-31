#ifndef HULLSETTINGSDIALOG_H
#define HULLSETTINGSDIALOG_H

#include "SettingsDialog.h"

class HullSettingsDialog : public SettingsDialog {
    Q_OBJECT

public:
    explicit HullSettingsDialog(QWidget *parent = nullptr);

    int getVertexCount() const { return vertexCountSpin_->value(); }
    bool useAndrewAlgorithm() const { return algorithmCombo_->currentText() == tr("Андрюс"); }

private:
    QSpinBox *vertexCountSpin_;
    QComboBox *algorithmCombo_;
};

#endif
