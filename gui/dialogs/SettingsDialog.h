#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QComboBox>

class SettingsDialog : public QWidget {
    Q_OBJECT

public:
    explicit SettingsDialog(const QString& title, QWidget *parent = nullptr);
    virtual ~SettingsDialog() = default;

protected:
    // Вспомогательные методы для создания UI
    QGroupBox* createParameterGroup(const QString& title = tr("Параметры"));
    QSpinBox* addSpinBox(QVBoxLayout* layout, const QString& label,
                         int min, int max, int defaultValue);
    QComboBox* addComboBox(QVBoxLayout* layout, const QString& label,
                           const QStringList& items);
    void addConfirmButton();

    QVBoxLayout* mainLayout_;

signals:
    void confirmed();
};

#endif
