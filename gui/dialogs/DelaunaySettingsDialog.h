#ifndef DELAUNAYSETTINGSDIALOG_H
#define DELAUNAYSETTINGSDIALOG_H

#include "SettingsDialog.h"

class DelaunaySettingsDialog : public SettingsDialog {
	Q_OBJECT

public:
	explicit DelaunaySettingsDialog(QWidget *parent = nullptr);

	int getVertexCount() const { return vertexCountSpin_->value(); }
	bool shouldLoadFromJSON() const { return methodCombo_->currentText() == tr("Загрузить JSON"); }

private:
	QSpinBox *vertexCountSpin_;
	QComboBox *methodCombo_;
};

#endif // DELAUNAYSETTINGSDIALOG_H