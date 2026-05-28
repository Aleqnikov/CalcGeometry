#include "DelaunaySettingsDialog.h"
#include <QVBoxLayout>

DelaunaySettingsDialog::DelaunaySettingsDialog(QWidget *parent)
	: SettingsDialog(tr("Триангуляция Делоне"), parent)
{
	QGroupBox *paramGroup = createParameterGroup();
	QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

	vertexCountSpin_ = addSpinBox(groupLayout, tr("Количество точек:"), 3, 10000, 12);

	methodCombo_ = addComboBox(groupLayout, tr("Метод или источник:"), {
																		   tr("Сгенерировать"),
																		   tr("Загрузить JSON")
																	   });

	addConfirmButton();
}