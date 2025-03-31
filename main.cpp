#include "custcalc.h"

#include <QApplication>

#include <QStandardPaths>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString folderPath = appDataPath + "/CustCalc";
    QDir dir;
    if (!dir.exists(folderPath)) {
        if (dir.mkpath(folderPath)) {
            qDebug() << "Folder created at:" << folderPath;
        } else {
            qDebug() << "Failed to create folder.";
        }
    } else {
        qDebug() << "Folder already exists at:" << folderPath;
    }

    QApplication a(argc, argv);
    CustCalc w;
    w.show();
    return a.exec();
}
