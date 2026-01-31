#include "RectangleSettingsDialog.h"
#include <QVBoxLayout>

RectangleSettingsDialog::RectangleSettingsDialog(QWidget *parent)
    : SettingsDialog(tr("Минимальный ограничивающий прямоугольник"), parent)
{
    QGroupBox *paramGroup = createParameterGroup();
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    pointCountSpin_ = addSpinBox(groupLayout, tr("Количество точек:"), 3, 10000, 100);

    addConfirmButton();
}
