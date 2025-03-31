#ifndef SETTINGSDASHBOARD_H
#define SETTINGSDASHBOARD_H

#include <QDialog>
#include "configurationmanager.h"

namespace Ui {
class SettingsDashboard;
}

class SettingsDashboard : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDashboard(QWidget *parent = nullptr);
    ~SettingsDashboard();

private slots:
    void on_roundness_slider_valueChanged(int value);

    void on_deafult_settings_btn_clicked();

    void on_apply_btn_clicked();

    void on_symbol_btn_bg_btn_clicked();

    void on_symbol_btn_text_color_btn_clicked();

    void on_number_btn_bg_btn_clicked();

    void on_number_btn_text_color_btn_clicked();

    void on_bg_color_btn_clicked();

    void on_text_color_btn_clicked();

    void on_isDarkTheme_comboBox_currentTextChanged(const QString &arg1);

    void on_theme_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::SettingsDashboard *ui;
    ConfigurationManager *configManager;  // The config manager

    void default_light_theme();
    void default_dark_theme();
    void orange_light_theme();
    void orange_dark_theme();
    void green_dark_theme();
    void disable_customization();
    void current_color_theme_apply(QString BgColor,QString TextColor,QString SymbolButtonBgColor,QString SymbolButtonColor,QString NumberButtonBgColor,QString NumberButtonColor,int RoundnessOfButton,bool DarkTheme);
};

#endif // SETTINGSDASHBOARD_H
