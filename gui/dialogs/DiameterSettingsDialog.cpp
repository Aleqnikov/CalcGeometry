#include "DiameterSettingsDialog.h"
#include <QVBoxLayout>

DiameterSettingsDialog::DiameterSettingsDialog(QWidget *parent)
    : SettingsDialog(tr("Нахождение диаметра множества"), parent)
{
    QGroupBox *paramGroup = createParameterGroup();
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    pointCountSpin_ = addSpinBox(groupLayout, tr("Количество точек:"), 2, 10000, 200);

    algorithmCombo_ = addComboBox(groupLayout, tr("Алгоритм:"), {
                                                                    tr("Наивный (O(n^2))"),
                                                                    tr("Rotating Calipers"),
                                                                    tr("Сравнение (оба)")
                                                                });

    addConfirmButton();
}

DiameterController::Mode DiameterSettingsDialog::getMode() const
{
    const QString choice = algorithmCombo_->currentText();

    if (choice == tr("Наивный (O(n^2))")) return DiameterController::Naive;
    if (choice == tr("Rotating Calipers")) return DiameterController::Calipers;
    return DiameterController::CompareBoth;
}
