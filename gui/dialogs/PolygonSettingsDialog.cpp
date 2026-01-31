#include "PolygonSettingsDialog.h"
#include <QVBoxLayout>

PolygonSettingsDialog::PolygonSettingsDialog(QWidget *parent)
    : SettingsDialog(tr("Точка внутри многоугольника"), parent)
{
    QGroupBox *paramGroup = createParameterGroup();
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    vertexCountSpin_ = addSpinBox(groupLayout, tr("Количество вершин:"), 3, 10000, 10);

    typeCombo_ = addComboBox(groupLayout, tr("Тип многоугольника:"), {
                                                                         tr("Выпуклый"),
                                                                         tr("Звёздчатый"),
                                                                         tr("Обычный")
                                                                     });

    addConfirmButton();
}
