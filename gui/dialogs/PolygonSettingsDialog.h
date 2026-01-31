#ifndef POLYGONSETTINGSDIALOG_H
#define POLYGONSETTINGSDIALOG_H

#include "SettingsDialog.h"

class PolygonSettingsDialog : public SettingsDialog {
    Q_OBJECT

public:
    explicit PolygonSettingsDialog(QWidget *parent = nullptr);

    int getVertexCount() const { return vertexCountSpin_->value(); }
    QString getPolygonType() const { return typeCombo_->currentText(); }

private:
    QSpinBox *vertexCountSpin_;
    QComboBox *typeCombo_;
};

#endif
