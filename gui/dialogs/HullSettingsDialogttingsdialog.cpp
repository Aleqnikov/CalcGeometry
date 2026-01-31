#include "HullSettingsDialog.h"
#include <QVBoxLayout>

HullSettingsDialog::HullSettingsDialog(QWidget *parent)
    : SettingsDialog(tr("Построение выпуклой оболочки"), parent)
{
    QGroupBox *paramGroup = createParameterGroup();
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    vertexCountSpin_ = addSpinBox(groupLayout, tr("Количество вершин:"), 3, 10000, 10);

    algorithmCombo_ = addComboBox(groupLayout, tr("Алгоритм"), {
                                                                   tr("Джарвис"),
                                                                   tr("Андрюс")
                                                               });

    addConfirmButton();
}
