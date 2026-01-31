#include "CircleSettingsDialog.h"
#include <QVBoxLayout>

CircleSettingsDialog::CircleSettingsDialog(QWidget *parent)
    : SettingsDialog(tr("Минимальная ограничивающая окружность"), parent)
{
    QGroupBox *paramGroup = createParameterGroup();
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);
    groupLayout->setSpacing(18);

    pointCountSpin_ = addSpinBox(groupLayout, tr("Количество точек:"), 1, 10000, 200);

    modeCombo_ = addComboBox(groupLayout, tr("Алгоритм:"), {
                                                               tr("Наивный (перебор)"),
                                                               tr("Welzl (рандомизированный)"),
                                                               tr("Сравнить оба")
                                                           });

    addConfirmButton();
}

CircleController::Mode CircleSettingsDialog::getMode() const
{
    const QString choice = modeCombo_->currentText();

    if (choice.contains("Наивный")) return CircleController::Naive;
    if (choice.contains("Welzl")) return CircleController::Welzl;
    return CircleController::CompareBoth;
}
