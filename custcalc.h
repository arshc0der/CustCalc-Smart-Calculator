#ifndef CUSTCALC_H
#define CUSTCALC_H

#include <QMainWindow>
#include "ConfigurationManager.h"
#include "settingsdashboard.h"
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class CustCalc;
}
QT_END_NAMESPACE

class CustCalc : public QMainWindow
{
    Q_OBJECT

public:
    CustCalc(QWidget *parent = nullptr);
    ~CustCalc();

private slots:
    void on_settings_btn_clicked();

    void on_ac_btn_clicked();

    void on_neg_positive_btn_clicked();

    void on_percent_btn_clicked();

    void on_divide_btn_clicked();

    void on_multiply_btn_clicked();

    void on_minus_btn_clicked();

    void on_plus_btn_clicked();

    void on_equalto_btn_clicked();

    void on_zero_btn_clicked();

    void on_dot_btn_clicked();

    void on_one_btn_clicked();

    void on_two_btn_clicked();

    void on_three_btn_clicked();

    void on_four_btn_clicked();

    void on_five_btn_clicked();

    void on_six_btn_clicked();

    void on_seven_btn_clicked();

    void on_eight_btn_clicked();

    void on_nine_btn_clicked();

    void on_backspace_btn_clicked();

    void on_calulator_history_btn_clicked();

    void on_back_btn_clicked();

    void on_clear_history_btn_clicked();

    void onItemClicked(QListWidgetItem *item);

    void on_calulator_btn_clicked();

    void on_converter_dashboard_btn_clicked();

    void on_length_distance_unit_btn_clicked();

    void on_convert_btn_clicked();

    void on_mass_weight_unit_btn_clicked();

    void on_Temperature_unit_btn_clicked();

    void on_Time_unit_btn_clicked();

    void on_Area_unit_btn_clicked();

    void on_Volume_unit_btn_clicked();

    void on_speed_velocity_unit_btn_clicked();

    void on_Pressure_unit_btn_clicked();

    void on_Energy_unit_btn_clicked();

    void on_Power_unit_btn_clicked();

    void on_data_storage_unit_btn_clicked();

    void on_Angle_unit_btn_clicked();

private:
    Ui::CustCalc *ui;
    ConfigurationManager *configManager;  // The config manager
    void applySettings();  // Function to apply settings from ConfigurationManager
    SettingsDashboard *Settings_Dashboard;
    void saveHistoryToFile(const QString& calculation);
    void appendDigit(const QString& digit);
    void on_operator_btn_clicked();
    void on_digit_btn_clicked();
    void unit_converter_theme_manager();
    QString scroll_var_style(QString scroll_bar_color);
    void switch_to_another_page(int page_index,QString light_icon_path,QString dark_icon_path);
    void ready_conversion_page(QString page_label,QStringList units);
};
#endif // CUSTCALC_H
