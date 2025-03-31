#include "settingsdashboard.h"
#include "ui_settingsdashboard.h"
#include <QColorDialog>
#include <QMessageBox>

QColor background_color;
QColor Text_color;
QColor symbol_btn_bg_color;
QColor symbol_btn_text_color;
QColor number_btn_bg_color;
QColor number_btn_text_color;
int roundness_btn;
bool is_Dark_Theme;
QString Theme_Name;

SettingsDashboard::SettingsDashboard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDashboard)
    , configManager(new ConfigurationManager("config.ini", this))
{
    ui->setupUi(this);

    Theme_Name=configManager->theme();
    ui->theme_comboBox->setCurrentText(Theme_Name);
    if(Theme_Name!="Custom"){
        disable_customization();
    }else if(Theme_Name=="Custom"){
        ui->deafult_settings_btn->setEnabled(false);
    }

    ui->sample_symbol_btn->setStyleSheet("border-radius : "+QString::number(configManager->roundnessOfButton())+"px;"
                                        "background-color: "+configManager->symbolButtonBgColor().name()+";"
                                        "color: "+configManager->symbolButtonColor().name()+";"
                                         );
    ui->sample_number_btn->setStyleSheet("border-radius : "+QString::number(configManager->roundnessOfButton())+"px;"
                                        "background-color: "+configManager->numberButtonBgColor().name()+";"
                                        "color: "+configManager->numberButtonColor().name()+";"
                                         );
    ui->roundness_slider->setSliderPosition(configManager->roundnessOfButton());

    is_Dark_Theme = configManager->isDarkTheme();

    if(configManager->isDarkTheme()){
        ui->isDarkTheme_comboBox->setCurrentText("Yes");
    }else{
        ui->isDarkTheme_comboBox->setCurrentText("No");
    }

    background_color=configManager->bgColor().name();
    Text_color=configManager->textColor().name();
    symbol_btn_bg_color=configManager->symbolButtonBgColor().name();
    symbol_btn_text_color=configManager->symbolButtonColor().name();
    number_btn_bg_color=configManager->numberButtonBgColor().name();
    number_btn_text_color=configManager->numberButtonColor().name();
    roundness_btn = configManager->roundnessOfButton();

    ui->bg_color_label->setText(configManager->bgColor().name());
    ui->text_color_label->setText(configManager->textColor().name());

    ui->symbol_btn_bg_label->setText(configManager->symbolButtonBgColor().name());
    ui->symbol_btn_text_color_label->setText(configManager->symbolButtonColor().name());
    ui->number_btn_bg_label->setText(configManager->numberButtonBgColor().name());
    ui->number_btn_text_color_label->setText(configManager->numberButtonColor().name());

    ui->display_frame->setStyleSheet("background-color: "+configManager->bgColor().name()+";");
    ui->display_label->setStyleSheet("color: "+configManager->textColor().name()+";");

    ui->symbol_btn_bg_label->setStyleSheet("background-color: "+configManager->symbolButtonBgColor().name()+";");
    ui->symbol_btn_text_color_label->setStyleSheet("background-color: "+configManager->symbolButtonColor().name()+";");
    ui->number_btn_bg_label->setStyleSheet("background-color: "+configManager->numberButtonBgColor().name()+";");
    ui->number_btn_text_color_label->setStyleSheet("background-color: "+configManager->numberButtonColor().name()+";");
    ui->bg_color_label->setStyleSheet("background-color: "+configManager->bgColor().name()+";");
    ui->text_color_label->setStyleSheet("background-color: "+configManager->textColor().name()+";");

    ui->sample_symbol_btn->setStyleSheet(
        "border-radius : "+QString::number(roundness_btn)+"px;"
        "background-color: "+symbol_btn_bg_color.name()+";"
        "color: "+symbol_btn_text_color.name()+";"
         );

    ui->sample_number_btn->setStyleSheet(
        "border-radius : "+QString::number(roundness_btn)+"px;"
        "background-color: "+number_btn_bg_color.name()+";"
        "color: "+number_btn_text_color.name()+";"
        );

}

SettingsDashboard::~SettingsDashboard()
{
    delete ui;
}

void SettingsDashboard::on_roundness_slider_valueChanged(int value)
{
    ui->sample_symbol_btn->setStyleSheet("border-radius : "+QString::number(roundness_btn)+"px;"
                                        "background-color: "+symbol_btn_bg_color.name()+";"
                                        "color: "+symbol_btn_text_color.name()+";"
                                         );
    ui->sample_number_btn->setStyleSheet("border-radius : "+QString::number(roundness_btn)+"px;"
                                        "background-color: "+number_btn_bg_color.name()+";"
                                        "color: "+number_btn_text_color.name()+";"
                                         );
    roundness_btn = value;
}


void SettingsDashboard::on_deafult_settings_btn_clicked()
{
    int reply = QMessageBox::question(this, "Confirm", "Do you want to apply default settings?",QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        if(ui->theme_comboBox->currentText()=="Light"){
            default_light_theme();
            configManager->setTheme("Light");
        }else if(ui->theme_comboBox->currentText()=="Dark"){
            default_dark_theme();
             configManager->setTheme("Dark");
        }else if(ui->theme_comboBox->currentText()=="Orange Light"){
            orange_light_theme();
             configManager->setTheme("Orange Light");
        }else if(ui->theme_comboBox->currentText()=="Orange Dark"){
            orange_dark_theme();
             configManager->setTheme("Orange Dark");
        }else if(ui->theme_comboBox->currentText()=="Green Dark"){
            green_dark_theme();
             configManager->setTheme("Green Dark");
        }

        this->close();

    } else {
        // User clicked No
    }


}


void SettingsDashboard::on_apply_btn_clicked()
{
    //void setZeroButtonRoundness(int roundness);

    configManager->setBgColor(background_color);
    configManager->setTextColor(Text_color);
    configManager->setSymbolButtonBgColor(symbol_btn_bg_color);
    configManager->setSymbolButtonColor(symbol_btn_text_color);
    configManager->setNumberButtonBgColor(number_btn_bg_color);
    configManager->setNumberButtonColor(number_btn_text_color);
    configManager->setRoundnessOfButton(roundness_btn);
    configManager->setDarkTheme(is_Dark_Theme);
    configManager->setTheme(Theme_Name);

    int reply = QMessageBox::question(this, "Confirm", "Do you want to apply new settings?",QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        this->close();
    } else {
        // User clicked No
    }
}


void SettingsDashboard::on_symbol_btn_bg_btn_clicked()
{
    symbol_btn_bg_color = QColorDialog::getColor();
    ui->symbol_btn_bg_label->setText(symbol_btn_bg_color.name());
    ui->symbol_btn_bg_label->setStyleSheet("background-color: "+symbol_btn_bg_color.name()+";");
    ui->sample_symbol_btn->setStyleSheet(
        "border-radius : "+QString::number(roundness_btn)+"px;"
        "background-color: "+symbol_btn_bg_color.name()+";"
        "color: "+symbol_btn_text_color.name()+";"
        );

}


void SettingsDashboard::on_symbol_btn_text_color_btn_clicked()
{
    symbol_btn_text_color = QColorDialog::getColor();
    ui->symbol_btn_text_color_label->setText(symbol_btn_text_color.name());
    ui->symbol_btn_text_color_label->setStyleSheet("background-color: "+symbol_btn_text_color.name()+";");
    ui->sample_symbol_btn->setStyleSheet(
        "border-radius : "+QString::number(roundness_btn)+"px;"
        "background-color: "+symbol_btn_bg_color.name()+";"
        "color: "+symbol_btn_text_color.name()+";"
        );
}


void SettingsDashboard::on_number_btn_bg_btn_clicked()
{
    number_btn_bg_color = QColorDialog::getColor();
    ui->number_btn_bg_label->setText(number_btn_bg_color.name());
    ui->number_btn_bg_label->setStyleSheet("background-color: "+number_btn_bg_color.name()+";");
    ui->sample_number_btn->setStyleSheet(
        "border-radius : "+QString::number(roundness_btn)+"px;"
        "background-color: "+number_btn_bg_color.name()+";"
        "color: "+number_btn_text_color.name()+";"
        );
}


void SettingsDashboard::on_number_btn_text_color_btn_clicked()
{
    number_btn_text_color = QColorDialog::getColor();
    ui->number_btn_text_color_label->setText(number_btn_text_color.name());
    ui->number_btn_text_color_label->setStyleSheet("background-color: "+number_btn_text_color.name()+";");
    ui->sample_number_btn->setStyleSheet(
        "border-radius : "+QString::number(roundness_btn)+"px;"
        "background-color: "+number_btn_bg_color.name()+";"
        "color: "+number_btn_text_color.name()+";"
        );
}

void SettingsDashboard::default_light_theme(){

    configManager->setBgColor("#ECF5FF");
    configManager->setTextColor("#0D2A4B");

    configManager->setSymbolButtonBgColor("#A3CFF9");
    configManager->setSymbolButtonColor("#5487BA");
    configManager->setNumberButtonBgColor("#1D1D1D");
    configManager->setNumberButtonColor("#CFCFCF");
    configManager->setRoundnessOfButton(25);
    configManager->setDarkTheme(false);

};
void SettingsDashboard::default_dark_theme(){
    configManager->setBgColor("#030b16");
    configManager->setTextColor("#ccc9df");

    configManager->setSymbolButtonBgColor("#8faedc");
    configManager->setSymbolButtonColor("#27282d");
    configManager->setNumberButtonBgColor("#1a202c");
    configManager->setNumberButtonColor("#a5a5b7");
    configManager->setRoundnessOfButton(25);
    configManager->setDarkTheme(true);
};

void SettingsDashboard::orange_light_theme(){

    configManager->setBgColor("#f5f5f5");
    configManager->setTextColor("#010101");

    configManager->setSymbolButtonBgColor("#ff9500");
    configManager->setSymbolButtonColor("#efca67");
    configManager->setNumberButtonBgColor("#505050");
    configManager->setNumberButtonColor("#dbdbdb");
    configManager->setRoundnessOfButton(25);
    configManager->setDarkTheme(false);

};
void SettingsDashboard::orange_dark_theme(){
    configManager->setBgColor("#232d36");
    configManager->setTextColor("#f7fafc");

    configManager->setSymbolButtonBgColor("#ff9500");
    configManager->setSymbolButtonColor("#efca67");
    configManager->setNumberButtonBgColor("#4f5150");
    configManager->setNumberButtonColor("#dbdbdb");
    configManager->setRoundnessOfButton(25);
    configManager->setDarkTheme(true);
};

void SettingsDashboard::green_dark_theme(){
    configManager->setBgColor("#000000");
    configManager->setTextColor("#f6f6f6");

    configManager->setSymbolButtonBgColor("#31d811");
    configManager->setSymbolButtonColor("#362c23");
    configManager->setNumberButtonBgColor("#292323");
    configManager->setNumberButtonColor("#31d811");
    configManager->setRoundnessOfButton(25);
    configManager->setDarkTheme(true);
};

void SettingsDashboard::on_bg_color_btn_clicked()
{
    background_color = QColorDialog::getColor();
    ui->bg_color_label->setText(background_color.name());
    ui->display_frame->setStyleSheet("background-color: "+background_color.name()+";");
    ui->bg_color_label->setStyleSheet("background-color: "+background_color.name()+";");

}


void SettingsDashboard::on_text_color_btn_clicked()
{
    Text_color = QColorDialog::getColor();
    ui->text_color_label->setText(Text_color.name());
    ui->display_label->setStyleSheet("color: "+Text_color.name()+";");
    ui->text_color_label->setStyleSheet("background-color: "+Text_color.name()+";");
}


void SettingsDashboard::on_isDarkTheme_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Yes"){
        is_Dark_Theme=true;
    }else{
        is_Dark_Theme=false;
    }
}

void SettingsDashboard::current_color_theme_apply(QString BgColor,QString TextColor,QString SymbolButtonBgColor,QString SymbolButtonColor,QString NumberButtonBgColor,QString NumberButtonColor,int RoundnessOfButton,bool DarkTheme)
{
    ui->display_frame->setStyleSheet("background-color: "+BgColor+";");
    ui->display_label->setStyleSheet("color: "+TextColor+";");

    ui->symbol_btn_bg_label->setStyleSheet("background-color: "+SymbolButtonBgColor+";");
    ui->symbol_btn_bg_label->setText(SymbolButtonBgColor);

    ui->symbol_btn_text_color_label->setStyleSheet("background-color: "+SymbolButtonColor+";");
    ui->symbol_btn_text_color_label->setText(SymbolButtonColor);

    ui->number_btn_bg_label->setStyleSheet("background-color: "+NumberButtonBgColor+";");
    ui->number_btn_bg_label->setText(NumberButtonBgColor);

    ui->number_btn_text_color_label->setStyleSheet("background-color: "+NumberButtonColor+";");
    ui->number_btn_text_color_label->setText(NumberButtonColor);

    ui->bg_color_label->setStyleSheet("background-color: "+BgColor+";");
    ui->bg_color_label->setText(BgColor);

    ui->text_color_label->setStyleSheet("background-color: "+TextColor+";");
    ui->text_color_label->setText(TextColor);

    ui->roundness_slider->setValue(RoundnessOfButton);
    ui->isDarkTheme_comboBox->setCurrentText(DarkTheme?"Yes":"No");

    ui->sample_symbol_btn->setStyleSheet(
        "border-radius : "+QString::number(RoundnessOfButton)+"px;"
        "background-color: "+SymbolButtonBgColor+";"
        "color: "+SymbolButtonColor+";"
        );

    ui->sample_number_btn->setStyleSheet(
        "border-radius : "+QString::number(RoundnessOfButton)+"px;"
        "background-color: "+NumberButtonBgColor+";"
        "color: "+NumberButtonColor+";"
        );
}

void SettingsDashboard::disable_customization(){
    ui->bg_color_btn->setEnabled(false);
    ui->text_color_btn->setEnabled(false);
    ui->symbol_btn_bg_btn->setEnabled(false);
    ui->symbol_btn_text_color_btn->setEnabled(false);
    ui->number_btn_bg_btn->setEnabled(false);
    ui->number_btn_text_color_btn->setEnabled(false);
    ui->roundness_slider->setEnabled(false);
    ui->isDarkTheme_comboBox->setEnabled(false);
    ui->apply_btn->setEnabled(false);
    ui->deafult_settings_btn->setEnabled(true);
}
void SettingsDashboard::on_theme_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1=="Custom"){
        ui->bg_color_btn->setEnabled(true);
        ui->text_color_btn->setEnabled(true);
        ui->symbol_btn_bg_btn->setEnabled(true);
        ui->symbol_btn_text_color_btn->setEnabled(true);
        ui->number_btn_bg_btn->setEnabled(true);
        ui->number_btn_text_color_btn->setEnabled(true);
        ui->roundness_slider->setEnabled(true);
        ui->isDarkTheme_comboBox->setEnabled(true);
        ui->apply_btn->setEnabled(true);
        ui->deafult_settings_btn->setEnabled(false);
        current_color_theme_apply(background_color.name(),Text_color.name(),symbol_btn_bg_color.name(),symbol_btn_text_color.name(),number_btn_bg_color.name(),number_btn_text_color.name(),roundness_btn,is_Dark_Theme);
        Theme_Name="Custom";

    }else if(arg1=="Light"){
        disable_customization();
        current_color_theme_apply("#ECF5FF","#0D2A4B","#A3CFF9","#5487BA","#1D1D1D","#CFCFCF",25,false);
        Theme_Name="Light";
    }else if(arg1=="Dark"){
        disable_customization();
        current_color_theme_apply("#030b16","#ccc9df","#8faedc","#27282d","#1a202c","#a5a5b7",25,true);
        Theme_Name="Dark";
    }else if(arg1=="Orange Light"){
        disable_customization();
        current_color_theme_apply("#f5f5f5","#010101","#ff9500","#efca67","#505050","#dbdbdb",25,false);
        Theme_Name="Orange Light";
    }else if(arg1=="Orange Dark"){
        disable_customization();
        current_color_theme_apply("#232d36","#f7fafc","#ff9500","#efca67","#4f5150","#dbdbdb",25,true);
        Theme_Name="Orange Dark";
    }else if(arg1=="Green Dark"){
        disable_customization();
        current_color_theme_apply("#000000","#f6f6f6","#31d811","#362c23","#292323","#31d811",25,true);
        Theme_Name="Green Dark";
    }
}



