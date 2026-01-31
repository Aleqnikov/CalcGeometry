#include "TriangulationSettingsDialog.h"
#include <QVBoxLayout>

TriangulationSettingsDialog::TriangulationSettingsDialog(QWidget *parent)
    : SettingsDialog(tr("Триангуляция полигона"), parent)
{
    QGroupBox *paramGroup = createParameterGroup();
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    vertexCountSpin_ = addSpinBox(groupLayout, tr("Количество вершин:"), 3, 10000, 12);

    methodCombo_ = addComboBox(groupLayout, tr("Метод или источник:"), {
                                                                           tr("Сгенерировать"),
                                                                           tr("Загрузить JSON")
                                                                       });

    addConfirmButton();
}
