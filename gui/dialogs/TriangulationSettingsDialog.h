#ifndef TRIANGULATIONSETTINGSDIALOG_H
#define TRIANGULATIONSETTINGSDIALOG_H

#include "SettingsDialog.h"

class TriangulationSettingsDialog : public SettingsDialog {
    Q_OBJECT

public:
    explicit TriangulationSettingsDialog(QWidget *parent = nullptr);

    int getVertexCount() const { return vertexCountSpin_->value(); }
    bool shouldLoadFromJSON() const { return methodCombo_->currentText() == tr("Загрузить JSON"); }

private:
    QSpinBox *vertexCountSpin_;
    QComboBox *methodCombo_;
};

#endif
