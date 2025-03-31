#include "ConfigurationManager.h"
#include <QColor>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

ConfigurationManager::ConfigurationManager(const QString &configFile, QObject *parent)
    : QObject(parent)
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(appDataPath)) {
        if (dir.mkpath(appDataPath)) {
            qDebug() << "App folder created at:" << appDataPath;
        } else {
            qDebug() << "Failed to create app folder.";
        }
    }
    QString configFilePath = appDataPath + "/config.ini";
    settings = new QSettings(configFilePath, QSettings::IniFormat, this);
    qDebug() << "Config file path: " << settings->fileName();
}

ConfigurationManager::~ConfigurationManager()
{
    delete settings;
}

void ConfigurationManager::loadSettings()
{
    // Load values from INI file
    QString textColor = settings->value("Appearance/textcolor", "#0D2A4B").toString();
    QString bgColor = settings->value("Appearance/bgcolor", "#ECF5FF").toString();
    QString symbolButtonBgColor = settings->value("Appearance/symbolbuttonbg", "#A3CFF9").toString();
    QString symbolButtonColor = settings->value("Appearance/symbolbuttoncolor", "#5487BA").toString();
    QString numberButtonBgColor = settings->value("Appearance/numerbtnbg", "#1D1D1D").toString();
    QString numberButtonColor = settings->value("Appearance/numberbtncolor", "#CFCFCF").toString();
    QString themeName = settings->value("Appearance/theme", "Light").toString();
    bool isDarkTheme = settings->value("Appearance/isDarkTheme", false).toBool();

    int roundnessOfButton = settings->value("Appearance/roundnessofbutton", 25).toInt();
    int zeroButtonRoundness = settings->value("Appearance/0buttonroundness", 25).toInt();

    // Apply these values (you can directly access these or use getters to retrieve them)

    // Save these default values to the INI file if they weren't found
    settings->setValue("Appearance/textcolor", textColor);
    settings->setValue("Appearance/bgcolor", bgColor);
    settings->setValue("Appearance/symbolbuttonbg", symbolButtonBgColor);
    settings->setValue("Appearance/symbolbuttoncolor", symbolButtonColor);
    settings->setValue("Appearance/numerbtnbg", numberButtonBgColor);
    settings->setValue("Appearance/numberbtncolor", numberButtonColor);
    settings->setValue("Appearance/theme", themeName);
    settings->setValue("Appearance/roundnessofbutton", roundnessOfButton);
    settings->setValue("Appearance/0buttonroundness", zeroButtonRoundness);
    settings->setValue("Appearance/isDarkTheme", isDarkTheme);
}

void ConfigurationManager::saveSettings()
{
    // Save settings to the INI file
    settings->setValue("Appearance/textcolor", textColor().name());
    settings->setValue("Appearance/bgcolor", bgColor().name());
    settings->setValue("Appearance/symbolbuttonbg", symbolButtonBgColor().name());
    settings->setValue("Appearance/symbolbuttoncolor", symbolButtonColor().name());
    settings->setValue("Appearance/numerbtnbg", numberButtonBgColor().name());
    settings->setValue("Appearance/numberbtncolor", numberButtonColor().name());
    settings->setValue("Appearance/theme", theme());
    settings->setValue("Appearance/roundnessofbutton", roundnessOfButton());
    settings->setValue("Appearance/0buttonroundness", zeroButtonRoundness());
    settings->setValue("Appearance/isDarkTheme", isDarkTheme());
}

QString ConfigurationManager::theme() const
{
    return  settings->value("Appearance/theme", "Light").toString();
}

bool ConfigurationManager::isDarkTheme() const
{
    return settings->value("Appearance/isDarkTheme", false).toBool();
}

QColor ConfigurationManager::textColor() const
{
    return QColor(settings->value("Appearance/textcolor", "#0D2A4B").toString());
}

QColor ConfigurationManager::bgColor() const
{
    return QColor(settings->value("Appearance/bgcolor", "#ECF5FF").toString());
}

QColor ConfigurationManager::symbolButtonBgColor() const
{
    return QColor(settings->value("Appearance/symbolbuttonbg", "#A3CFF9").toString());
}

QColor ConfigurationManager::symbolButtonColor() const
{
    return QColor(settings->value("Appearance/symbolbuttoncolor", "#5487BA").toString());
}

QColor ConfigurationManager::numberButtonBgColor() const
{
    return QColor(settings->value("Appearance/numerbtnbg", "#1D1D1D").toString());
}

QColor ConfigurationManager::numberButtonColor() const
{
    return QColor(settings->value("Appearance/numberbtncolor", "#CFCFCF").toString());
}

int ConfigurationManager::roundnessOfButton() const
{
    return settings->value("Appearance/roundnessofbutton", 25).toInt();
}

int ConfigurationManager::zeroButtonRoundness() const
{
    return settings->value("Appearance/0buttonroundness", 25).toInt();
}

void ConfigurationManager::setTextColor(const QColor &color)
{
    settings->setValue("Appearance/textcolor", color.name());
}

void ConfigurationManager::setBgColor(const QColor &color)
{
    settings->setValue("Appearance/bgcolor", color.name());
}

void ConfigurationManager::setSymbolButtonBgColor(const QColor &color)
{
    settings->setValue("Appearance/symbolbuttonbg", color.name());
}

void ConfigurationManager::setSymbolButtonColor(const QColor &color)
{
    settings->setValue("Appearance/symbolbuttoncolor", color.name());
}

void ConfigurationManager::setNumberButtonBgColor(const QColor &color)
{
    settings->setValue("Appearance/numerbtnbg", color.name());
}

void ConfigurationManager::setNumberButtonColor(const QColor &color)
{
    settings->setValue("Appearance/numberbtncolor", color.name());
}

void ConfigurationManager::setRoundnessOfButton(int roundness)
{
    settings->setValue("Appearance/roundnessofbutton", roundness);
}

void ConfigurationManager::setZeroButtonRoundness(int roundness)
{
    settings->setValue("Appearance/0buttonroundness", roundness);
}

void ConfigurationManager::setDarkTheme(bool value)
{
    settings->setValue("Appearance/isDarkTheme", value);
}

void ConfigurationManager::setTheme(QString themeName){
    settings->setValue("Appearance/theme", themeName);
}
