#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QColor>

class ConfigurationManager : public QObject
{
    Q_OBJECT

public:
    explicit ConfigurationManager(const QString &configFile, QObject *parent = nullptr);
    ~ConfigurationManager();

    // Method to load settings from the INI file
    void loadSettings();

    // Method to save settings to the INI file
    void saveSettings();

    // Getters for settings
    QColor textColor() const;
    QColor bgColor() const;
    QColor symbolButtonBgColor() const;
    QColor symbolButtonColor() const;
    QColor numberButtonBgColor() const;
    QColor numberButtonColor() const;
    QString theme() const;
    int roundnessOfButton() const;
    int zeroButtonRoundness() const;
    bool isDarkTheme() const;

    // Setters for settings
    void setTextColor(const QColor &color);
    void setBgColor(const QColor &color);
    void setSymbolButtonBgColor(const QColor &color);
    void setSymbolButtonColor(const QColor &color);
    void setNumberButtonBgColor(const QColor &color);
    void setNumberButtonColor(const QColor &color);
    void setRoundnessOfButton(int roundness);
    void setZeroButtonRoundness(int roundness);
    void setTheme(QString themeName);
    void setDarkTheme(bool value);


private:
    QSettings *settings;  // QSettings instance
};

#endif // CONFIGURATIONMANAGER_H
