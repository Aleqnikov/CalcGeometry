#include "SettingsDialog.h"
#include <QHBoxLayout>

SettingsDialog::SettingsDialog(const QString& title, QWidget *parent)
    : QWidget(parent)
{
    mainLayout_ = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel(title, this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout_->addWidget(titleLabel);
    mainLayout_->addStretch();
}

QGroupBox* SettingsDialog::createParameterGroup(const QString& title)
{
    QGroupBox *group = new QGroupBox(title, this);
    mainLayout_->addWidget(group);
    mainLayout_->addStretch();
    return group;
}

QSpinBox* SettingsDialog::addSpinBox(QVBoxLayout* layout, const QString& label,
                                     int min, int max, int defaultValue)
{
    layout->addWidget(new QLabel(label));

    QSpinBox *spin = new QSpinBox();
    spin->setRange(min, max);
    spin->setValue(defaultValue);
    layout->addWidget(spin);

    return spin;
}

QComboBox* SettingsDialog::addComboBox(QVBoxLayout* layout, const QString& label,
                                       const QStringList& items)
{
    layout->addWidget(new QLabel(label));

    QComboBox *combo = new QComboBox();
    combo->addItems(items);
    layout->addWidget(combo);

    return combo;
}

void SettingsDialog::addConfirmButton()
{
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *confirm = new QPushButton(tr("Создать"), this);
    confirm->setCursor(Qt::PointingHandCursor);

    buttonLayout->addStretch();
    buttonLayout->addWidget(confirm);
    mainLayout_->addLayout(buttonLayout);

    connect(confirm, &QPushButton::clicked, this, &SettingsDialog::confirmed);
}
