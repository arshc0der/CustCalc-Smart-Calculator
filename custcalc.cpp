#include "custcalc.h"
#include "ui_custcalc.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QPixmap>
#include <QTimer>

double firstNum = 0;
double secondNum = 0;
QString currentOperation;
QString currentExpression = ""; // Track the current expression
bool isDecimal = false;
QString historyFilePath;
double previous_result=0;
QListWidget *listWidget;

CustCalc::CustCalc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CustCalc)
    , configManager(new ConfigurationManager("config.ini", this))
{
    ui->setupUi(this);

    // In CustCalc constructor, after `ui->setupUi(this);`
    QTimer::singleShot(500, this, [this]() {  // Preload after 500ms
        for (int i = 0; i < ui->stackedWidget->count(); ++i) {
            ui->stackedWidget->widget(i)->setVisible(true);
        }
    });

    configManager->loadSettings();  // Load settings from INI file
    applySettings();

    ui->stackedWidget->setCurrentIndex(0);
    ui->calulator_btn->setStyleSheet(
        "border-top: none; "
        "border-right: none; "
        "border-bottom: 2px solid "+configManager->symbolButtonBgColor().name()+";"
                                                        "border-left: none;"
        );
    ui->calulator_history_btn->setStyleSheet("border: none;");


    //list widget
    listWidget = new QListWidget(this);
    ui->verticalLayout_history->addWidget(listWidget);
    connect(listWidget, &QListWidget::itemClicked, this, &CustCalc::onItemClicked);

    if(configManager->isDarkTheme()){
        QIcon buttonIcon(":/resources/icons/settings-white.svg");
        ui->settings_btn->setIcon(buttonIcon);
        ui->expression_label->setStyleSheet("color: white;");
        ui->display_label->setStyleSheet("color: "+configManager->textColor().name()+";");
        QIcon backSpaceIcon(":/resources/icons/backspace-white.svg");
        ui->backspace_btn->setIcon(backSpaceIcon);

        QIcon history_icon(":/resources/icons/history-white.svg");
        ui->calulator_history_btn->setIcon(history_icon);

        //dashboard
        QIcon Convert_dashboard_icon(":/resources/icons/dashboard-white.svg");
        ui->converter_dashboard_btn->setIcon(Convert_dashboard_icon);

        //history page
        QIcon backIcon(":/resources/icons/back-white.svg");
        ui->back_btn->setIcon(backIcon);

        QIcon CalulatorIcon(":/resources/icons/calculate-white.svg");
        ui->calulator_btn->setIcon(CalulatorIcon);

        //ui->history_container_frame->setStyle("");
        ui->history_label->setStyleSheet("color: white;");
        ui->history_content_label->setStyleSheet("color: white;");
        ui->history_container_frame->setStyleSheet(
            "QListWidget::item {"
            "    color: white;"
            "    border: none;"
            "}"

            "QListWidget::item:hover {"
            "    color: red;"
            "    background: white;"
            "    border: none;"
            "}"+
            scroll_var_style("red")
            );


    }else{
        QIcon buttonIcon(":/resources/icons/settings-black.svg");
        ui->settings_btn->setIcon(buttonIcon);
        ui->expression_label->setStyleSheet("color: black;");
        ui->display_label->setStyleSheet("color: "+configManager->textColor().name()+";");
        QIcon backSpaceIcon(":/resources/icons/backspace-black.svg");
        ui->backspace_btn->setIcon(backSpaceIcon);

        QIcon history_icon(":/resources/icons/history-black.svg");
        ui->calulator_history_btn->setIcon(history_icon);

        QIcon CalulatorIcon(":/resources/icons/calculate-black.svg");
        ui->calulator_btn->setIcon(CalulatorIcon);

        //dashboard
        QIcon Convert_dashboard_icon(":/resources/icons/dashboard-black.svg");
        ui->converter_dashboard_btn->setIcon(Convert_dashboard_icon);

        //history page
        QIcon backIcon(":/resources/icons/back-black.svg");
        ui->back_btn->setIcon(backIcon);


        ui->history_label->setStyleSheet("color: black;");
        ui->history_content_label->setStyleSheet("color: black;");
        ui->history_container_frame->setStyleSheet(
            "QListWidget::item {"
            "    color: black;"
            "    border: none;"
            "}"

            "QListWidget::item:hover {"
            "    color: red;"
            "    background: white;"
            "    border: none;"
            "}"+
            scroll_var_style("red")
            );

    }
    unit_converter_theme_manager();

    ui->display_label->setText(QString::number(firstNum));

    // Define the history file path using AppDataLocation
    historyFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/history.txt";

    // Check if the history file exists, create if not
    if (!QFile::exists(historyFilePath)) {
        QFile historyFile(historyFilePath);
        if (historyFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&historyFile);
            out << "";  // Initialize with an empty file
            historyFile.close();
        }
    }
}

CustCalc::~CustCalc()
{
    delete ui;
}

void CustCalc::on_settings_btn_clicked()
{
    Settings_Dashboard = new SettingsDashboard();
    Settings_Dashboard->exec();

    // In CustCalc constructor, after `ui->setupUi(this);` preload all pages
    QTimer::singleShot(500, this, [this]() {  // Preload after 500ms
        for (int i = 0; i < ui->stackedWidget->count(); ++i) {
            ui->stackedWidget->widget(i)->setVisible(true);
        }
    });


    applySettings();
     ui->stackedWidget->setCurrentIndex(0);
    ui->calulator_btn->setStyleSheet(
        "border-top: none; "
        "border-right: none; "
        "border-bottom: 2px solid "+configManager->symbolButtonBgColor().name()+";"
                                                        "border-left: none;"
        );
    ui->calulator_history_btn->setStyleSheet("border: none;");
    ui->converter_dashboard_btn->setStyleSheet("border: none;");
}

void CustCalc::applySettings()
{
    // Apply settings using ConfigurationManager
    QPalette palette;
    palette.setColor(QPalette::WindowText, configManager->textColor());
    this->setPalette(palette);

    this->setStyleSheet("background-color: " + configManager->bgColor().name() + ";");
    ui->display_label->setStyleSheet("color: " + configManager->textColor().name() + ";");

    QString Numberbtn = "background-color: " + configManager->numberButtonBgColor().name() +
                        "; color: " + configManager->numberButtonColor().name() +
                        "; border-radius: " + QString::number(configManager->roundnessOfButton()) + "px;";

    QString Symbolbtn = "background-color: " + configManager->symbolButtonBgColor().name() +
                        "; color: " + configManager->symbolButtonColor().name() +
                        "; border-radius: " + QString::number(configManager->roundnessOfButton()) + "px;";

    // Apply styles for buttons
    ui->ac_btn->setStyleSheet(Symbolbtn);
    ui->neg_positive_btn->setStyleSheet(Symbolbtn);
    ui->percent_btn->setStyleSheet(Symbolbtn);
    ui->divide_btn->setStyleSheet(Symbolbtn);
    ui->multiply_btn->setStyleSheet(Symbolbtn);
    ui->minus_btn->setStyleSheet(Symbolbtn);
    ui->plus_btn->setStyleSheet(Symbolbtn);
    ui->equalto_btn->setStyleSheet(Symbolbtn);

    ui->zero_btn->setStyleSheet(Numberbtn);
    ui->one_btn->setStyleSheet(Numberbtn);
    ui->two_btn->setStyleSheet(Numberbtn);
    ui->three_btn->setStyleSheet(Numberbtn);
    ui->four_btn->setStyleSheet(Numberbtn);
    ui->five_btn->setStyleSheet(Numberbtn);
    ui->six_btn->setStyleSheet(Numberbtn);
    ui->seven_btn->setStyleSheet(Numberbtn);
    ui->eight_btn->setStyleSheet(Numberbtn);
    ui->nine_btn->setStyleSheet(Numberbtn);
    ui->dot_btn->setStyleSheet(Numberbtn);

    if(configManager->isDarkTheme()){
        QIcon buttonIcon(":/resources/icons/settings-white.svg");
        ui->settings_btn->setIcon(buttonIcon);
        ui->expression_label->setStyleSheet("color: white;");
        ui->display_label->setStyleSheet("color: "+configManager->textColor().name()+";");
        QIcon backSpaceIcon(":/resources/icons/backspace-white.svg");
        ui->backspace_btn->setIcon(backSpaceIcon);

        QIcon history_icon(":/resources/icons/history-white.svg");
        ui->calulator_history_btn->setIcon(history_icon);

        //dashboard
        QIcon Convert_dashboard_icon(":/resources/icons/dashboard-white.svg");
        ui->converter_dashboard_btn->setIcon(Convert_dashboard_icon);

        //history page
        QIcon backIcon(":/resources/icons/back-white.svg");
        ui->back_btn->setIcon(backIcon);

        QIcon CalulatorIcon(":/resources/icons/calculate-white.svg");
        ui->calulator_btn->setIcon(CalulatorIcon);

        //ui->history_container_frame->setStyle("");
        ui->history_label->setStyleSheet("color: white;");
        ui->history_content_label->setStyleSheet("color: white;");
        ui->history_container_frame->setStyleSheet(
            "QListWidget::item {"
            "    color: white;"
            "    border: none;"
            "}"

            "QListWidget::item:hover {"
            "    color: red;"
            "    background: white;"
            "    border: none;"
            "}"+
            scroll_var_style("red")
            );


    }else{
        QIcon buttonIcon(":/resources/icons/settings-black.svg");
        ui->settings_btn->setIcon(buttonIcon);
        ui->expression_label->setStyleSheet("color: black;");
        ui->display_label->setStyleSheet("color: "+configManager->textColor().name()+";");
        QIcon backSpaceIcon(":/resources/icons/backspace-black.svg");
        ui->backspace_btn->setIcon(backSpaceIcon);

        QIcon history_icon(":/resources/icons/history-black.svg");
        ui->calulator_history_btn->setIcon(history_icon);

        QIcon CalulatorIcon(":/resources/icons/calculate-black.svg");
        ui->calulator_btn->setIcon(CalulatorIcon);

        //dashboard
        QIcon Convert_dashboard_icon(":/resources/icons/dashboard-black.svg");
        ui->converter_dashboard_btn->setIcon(Convert_dashboard_icon);

        //history page
        QIcon backIcon(":/resources/icons/back-black.svg");
        ui->back_btn->setIcon(backIcon);


        ui->history_label->setStyleSheet("color: black;");
        ui->history_content_label->setStyleSheet("color: black;");
        ui->history_container_frame->setStyleSheet(
            "QListWidget::item {"
            "    color: black;"
            "    border: none;"
            "}"

            "QListWidget::item:hover {"
            "    color: red;"
            "    background: white;"
            "    border: none;"
            "}"+
            scroll_var_style("red")
            );

    }
    unit_converter_theme_manager();
}

void CustCalc::on_ac_btn_clicked()
{
    ui->display_label->clear();
    ui->expression_label->clear();  // Clear the expression
    currentExpression.clear();
    firstNum = 0;
    secondNum = 0;
    currentOperation.clear();
    isDecimal = false;
    previous_result =0;
    ui->display_label->setText(QString::number(firstNum));
}

void CustCalc::on_neg_positive_btn_clicked()
{
    QString currentText = ui->display_label->text();
    if (!currentText.isEmpty()) {
        double value = currentText.toDouble();
        value = -value;
        ui->display_label->setText(QString::number(value));

        // Update expression label
        currentExpression = currentExpression.startsWith('-') ? currentExpression.mid(1) : "-" + currentExpression;
        ui->expression_label->setText(currentExpression);
    }
}

void CustCalc::on_percent_btn_clicked()
{
    QString currentText = ui->display_label->text();
    if (!currentText.isEmpty()) {
        double value = currentText.toDouble();
        value /= 100;
        ui->display_label->setText(QString::number(value));

        // Update expression label
        currentExpression += "%";
        ui->expression_label->setText(currentExpression);
    }
}

void CustCalc::on_divide_btn_clicked()
{
    // If there's any number in the display, store it as firstNum and proceed with the operation
    if (previous_result!=0) {
        firstNum = previous_result;
        currentOperation = "/";
        //ui->display_label->clear();

        if(currentExpression.isEmpty()){
            currentExpression += QString::number(firstNum) + "/";
        }else{
            currentExpression += "/";
        }

        ui->expression_label->setText(currentExpression);
    }else{
        firstNum = ui->display_label->text().toDouble();
        currentOperation = "/";
        //ui->display_label->clear();
        currentExpression += "/";
        ui->expression_label->setText(currentExpression);
    }

}

void CustCalc::on_multiply_btn_clicked()
{
    // If there's any number in the display, store it as firstNum and proceed with the operation
    if (previous_result!=0) {
        firstNum = previous_result;
        currentOperation = "*";
        //ui->display_label->clear();

        if(currentExpression.isEmpty()){
            currentExpression += QString::number(firstNum) + "*";
        }else{
            currentExpression += "*";
        }

        ui->expression_label->setText(currentExpression);
    }else{
        firstNum = ui->display_label->text().toDouble();
        currentOperation = "*";
        //ui->display_label->clear();
        currentExpression += "*";
        ui->expression_label->setText(currentExpression);
    }
}

void CustCalc::on_minus_btn_clicked()
{

    // If there's any number in the display, store it as firstNum and proceed with the operation
    if (previous_result!=0) {
        firstNum = previous_result;
        currentOperation = "-";
        //ui->display_label->clear();

        if(currentExpression.isEmpty()){
            currentExpression += QString::number(firstNum) + "-";
        }else{
            currentExpression += "-";
        }

        ui->expression_label->setText(currentExpression);
    }else{
        firstNum = ui->display_label->text().toDouble();
        currentOperation = "-";
        //ui->display_label->clear();
        currentExpression += "-";
        ui->expression_label->setText(currentExpression);
    }
}

void CustCalc::on_plus_btn_clicked()
{
    // If there's any number in the display, store it as firstNum and proceed with the operation
    if (previous_result!=0) {
        firstNum = previous_result;
        currentOperation = "+";
        //ui->display_label->clear();

        if(currentExpression.isEmpty()){
            currentExpression += QString::number(firstNum) + "+";
        }else{
            currentExpression += "+";
        }

        ui->expression_label->setText(currentExpression);
    }else{
        firstNum = ui->display_label->text().toDouble();
        currentOperation = "+";
        //ui->display_label->clear();
        currentExpression += "+";
        ui->expression_label->setText(currentExpression);
    }
}

void CustCalc::on_equalto_btn_clicked()
{
    secondNum = ui->display_label->text().toDouble();
    double result = 0;

    if (!currentOperation.isEmpty()) {
        if (currentOperation == "+") {
            result = firstNum + secondNum;
        } else if (currentOperation == "-") {
            result = firstNum - secondNum;
        } else if (currentOperation == "*") {
            result = firstNum * secondNum;
        } else if (currentOperation == "/") {
            if (secondNum != 0) {
                result = firstNum / secondNum;
            } else {
                ui->display_label->setText("Error");
                return;
            }
        }

        ui->display_label->setText(QString::number(result));
        currentExpression += "=" + QString::number(result);
        ui->expression_label->setText(currentExpression);

        saveHistoryToFile(currentExpression);
        listWidget->addItem(currentExpression);

        if (listWidget && listWidget->count() > 0) {
            ui->history_content_label->setVisible(false);  // Hide the label if the list is not empty
        }

        firstNum = result;
        previous_result = result;
        currentExpression.clear();
        currentOperation.clear();
        isDecimal = false;
    } else {
        ui->display_label->setText(QString::number(secondNum));
    }
}

void CustCalc::on_zero_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];

    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("0");
    }else if(lastChar.isDigit()){
        appendDigit("0");
    }else if(lastChar == '.'){
        appendDigit("0");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("0");
    }

}

void CustCalc::on_dot_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit(".");
    }else if(lastChar.isDigit()){
        appendDigit(".");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit(".");
    }
}

void CustCalc::on_one_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("1");
    }else if(lastChar.isDigit()){
        appendDigit("1");
    }else if(lastChar == '.'){
        appendDigit("1");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("1");
    }
}

void CustCalc::on_two_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("2");
    }else if(lastChar.isDigit()){
        appendDigit("2");
    }else if(lastChar == '.'){
        appendDigit("2");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("2");
    }

}

void CustCalc::on_three_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("3");
    }else if(lastChar.isDigit()){
        appendDigit("3");
    }else if(lastChar == '.'){
        appendDigit("3");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("3");
    }
}

void CustCalc::on_four_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("4");
    }else if(lastChar.isDigit()){
        appendDigit("4");
    }else if(lastChar == '.'){
        appendDigit("4");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("4");
    }
}

void CustCalc::on_five_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("5");
    }else if(lastChar.isDigit()){
        appendDigit("5");
    }else if(lastChar == '.'){
        appendDigit("5");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("5");
    }
}

void CustCalc::on_six_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("6");
    }else if(lastChar.isDigit()){
        appendDigit("6");
    }else if(lastChar == '.'){
        appendDigit("6");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("6");
    }
}

void CustCalc::on_seven_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("7");
    }else if(lastChar.isDigit()){
        appendDigit("7");
    }else if(lastChar == '.'){
        appendDigit("7");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("7");
    }
}

void CustCalc::on_eight_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("8");
    }else if(lastChar.isDigit()){
        appendDigit("8");
    }else if(lastChar == '.'){
        appendDigit("8");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("8");
    }
}

void CustCalc::on_nine_btn_clicked()
{
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        ui->display_label->clear();
        appendDigit("9");
    }else if(lastChar.isDigit()){
        appendDigit("9");
    }else if(lastChar == '.'){
        appendDigit("9");
    }else {
        if(previous_result!=0){
            currentExpression=QString::number(previous_result);
            previous_result=0;
        }
        appendDigit("9");
    }
}

void CustCalc::appendDigit(const QString& digit)
{
    QString currentText = ui->display_label->text();
    currentText.append(digit);
    ui->display_label->setText(currentText);
    currentExpression.append(digit);
    ui->expression_label->setText(currentExpression);
}

void CustCalc::on_backspace_btn_clicked()
{
    QString currentText = ui->display_label->text();
    QChar lastChar = currentExpression.isEmpty() ? QChar() : currentExpression[currentExpression.length() - 1];
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        currentExpression.chop(1);
        ui->expression_label->setText(currentExpression);
    }else if (!currentText.isEmpty()) {

        currentText.chop(1); // Remove last character
        ui->display_label->setText(currentText);

        // Update expression label
        currentExpression.chop(1);
        ui->expression_label->setText(currentExpression);

        if (currentText.isEmpty()) {
            ui->display_label->setText("0");
        }

        if (currentExpression.isEmpty()) {
            ui->expression_label->setText("0");
            previous_result=0;
        }
    }
}

void CustCalc::saveHistoryToFile(const QString& calculation)
{
    QFile historyFile(historyFilePath);
    if (historyFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&historyFile);
        out << calculation << "\n";
        historyFile.close();
    }
}

void CustCalc::on_calulator_history_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->calulator_history_btn->setStyleSheet(
        "border-top: none; "
        "border-right: none; "
        "border-bottom: 2px solid "+configManager->symbolButtonBgColor().name()+";"
        "border-left: none;"
        );
    ui->calulator_btn->setStyleSheet("border: none;");
    ui->converter_dashboard_btn->setStyleSheet("border: none;");
    if(configManager->isDarkTheme()){
        QIcon dashboard_icon(":/resources/icons/dashboard-white.svg");
        ui->converter_dashboard_btn->setIcon(dashboard_icon);
    }else{
        QIcon dashboard_icon(":/resources/icons/dashboard-black.svg");
        ui->converter_dashboard_btn->setIcon(dashboard_icon);
    }
}


void CustCalc::on_back_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void CustCalc::on_clear_history_btn_clicked()
{
    listWidget->clear();
    ui->history_content_label->setVisible(true);
}

void CustCalc::onItemClicked(QListWidgetItem *item){
    currentExpression.clear();
    currentExpression.append(item->text());
    ui->stackedWidget->setCurrentIndex(0);
    ui->expression_label->setText(currentExpression);

    int equalIndex = currentExpression.indexOf('=');

    if (equalIndex != -1) {
        // Extract the substring after '='
        QString result = currentExpression.mid(equalIndex + 1);  // Starts after '='

        // Convert result to double or integer if needed
        bool ok;
        double value = result.toDouble(&ok);  // Converts to double

        if (ok) {
            qDebug() << "Result after '=' is:" << value;
            ui->display_label->setText(QString::number(value));

            firstNum = value;
            previous_result = value;
            currentExpression.clear();
            currentOperation.clear();
            isDecimal = false;
        } else {
            qDebug() << "Failed to convert result to a number.";
        }
    }
}


void CustCalc::on_calulator_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->calulator_btn->setStyleSheet(
        "border-top: none; "
        "border-right: none; "
        "border-bottom: 2px solid "+configManager->symbolButtonBgColor().name()+";"
        "border-left: none;"
        );
    ui->calulator_history_btn->setStyleSheet("border: none;");
    ui->converter_dashboard_btn->setStyleSheet("border: none;");
    if(configManager->isDarkTheme()){
        QIcon dashboard_icon(":/resources/icons/dashboard-white.svg");
        ui->converter_dashboard_btn->setIcon(dashboard_icon);
    }else{
        QIcon dashboard_icon(":/resources/icons/dashboard-black.svg");
        ui->converter_dashboard_btn->setIcon(dashboard_icon);
    }
}


void CustCalc::on_converter_dashboard_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->converter_dashboard_btn->setStyleSheet(
        "border-top: none; "
        "border-right: none; "
        "border-bottom: 2px solid "+configManager->symbolButtonBgColor().name()+";"
                                                        "border-left: none;"
        );
    ui->calulator_history_btn->setStyleSheet("border: none;");
    ui->calulator_btn->setStyleSheet("border: none;");
    if(configManager->isDarkTheme()){
        QIcon dashboard_icon(":/resources/icons/dashboard-white.svg");
        ui->converter_dashboard_btn->setIcon(dashboard_icon);
    }else{
        QIcon dashboard_icon(":/resources/icons/dashboard-black.svg");
        ui->converter_dashboard_btn->setIcon(dashboard_icon);
    }
}

void CustCalc::unit_converter_theme_manager(){

    QString light_theme_stylesheet="color: black;";
    QString dark_theme_stylesheet="color: white;";

    if(configManager->isDarkTheme()){
        ui->unit_converter_label->setStyleSheet(dark_theme_stylesheet);
        ui->length_distance_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->mass_weight_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->Temperature_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->Time_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->Area_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->Volume_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->speed_velocity_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->Pressure_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->Energy_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->Power_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->data_storage_unit_label->setStyleSheet(dark_theme_stylesheet);
        ui->Angle_unit_label->setStyleSheet(dark_theme_stylesheet);

        QIcon length(":/resources/units_icons/ruler-white.svg");
        ui->length_distance_unit_btn->setIcon(length);

        QIcon weight(":/resources/units_icons/Weight-white.svg");
        ui->mass_weight_unit_btn->setIcon(weight);

        QIcon temprature(":/resources/units_icons/thermometer-white.svg");
        ui->Temperature_unit_btn->setIcon(temprature);

        QIcon clock(":/resources/units_icons/clock-white.svg");
        ui->Time_unit_btn->setIcon(clock);

        QIcon square(":/resources/units_icons/square-white.svg");
        ui->Area_unit_btn->setIcon(square);

        QIcon bottle(":/resources/units_icons/bottle-white.svg");
        ui->Volume_unit_btn->setIcon(bottle);

        QIcon speed(":/resources/units_icons/speedometer-white.svg");
        ui->speed_velocity_unit_btn->setIcon(speed);

        QIcon pressure(":/resources/units_icons/Pressure-white.svg");
        ui->Pressure_unit_btn->setIcon(pressure);

        QIcon energy(":/resources/units_icons/Energy-white.svg");
        ui->Energy_unit_btn->setIcon(energy);

        QIcon power(":/resources/units_icons/plug-white.svg");
        ui->Power_unit_btn->setIcon(power);

        QIcon hdd_data(":/resources/units_icons/hdd-white.svg");
        ui->data_storage_unit_btn->setIcon(hdd_data);

        QIcon angle(":/resources/units_icons/Angle-white.svg");
        ui->Angle_unit_btn->setIcon(angle);


    }else{
        ui->unit_converter_label->setStyleSheet(light_theme_stylesheet);
        ui->length_distance_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->mass_weight_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->Temperature_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->Time_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->Area_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->Volume_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->speed_velocity_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->Pressure_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->Energy_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->Power_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->data_storage_unit_label->setStyleSheet(light_theme_stylesheet);
        ui->Angle_unit_label->setStyleSheet(light_theme_stylesheet);

        QIcon length(":/resources/units_icons/ruler-black.svg");
        ui->length_distance_unit_btn->setIcon(length);

        QIcon weight(":/resources/units_icons/Weight-black.svg");
        ui->mass_weight_unit_btn->setIcon(weight);

        QIcon temprature(":/resources/units_icons/thermometer-black.svg");
        ui->Temperature_unit_btn->setIcon(temprature);

        QIcon clock(":/resources/units_icons/clock-black.svg");
        ui->Time_unit_btn->setIcon(clock);

        QIcon square(":/resources/units_icons/square-black.svg");
        ui->Area_unit_btn->setIcon(square);

        QIcon bottle(":/resources/units_icons/bottle-black.svg");
        ui->Volume_unit_btn->setIcon(bottle);

        QIcon speed(":/resources/units_icons/speedometer-black.svg");
        ui->speed_velocity_unit_btn->setIcon(speed);

        QIcon pressure(":/resources/units_icons/Pressure-black.svg");
        ui->Pressure_unit_btn->setIcon(pressure);

        QIcon energy(":/resources/units_icons/Energy-black.svg");
        ui->Energy_unit_btn->setIcon(energy);

        QIcon power(":/resources/units_icons/plug-black.svg");
        ui->Power_unit_btn->setIcon(power);

        QIcon hdd_data(":/resources/units_icons/hdd-black.svg");
        ui->data_storage_unit_btn->setIcon(hdd_data);

        QIcon angle(":/resources/units_icons/Angle-black.svg");
        ui->Angle_unit_btn->setIcon(angle);
    }

    QString scroll_bar_style = scroll_var_style(configManager->symbolButtonBgColor().name());
    QString buttonBgColor = configManager->symbolButtonBgColor().name();
    QString hoverBorderColor = configManager->numberButtonBgColor().name();

    ui->unit_conatiner_frame->setStyleSheet(
        "QPushButton { border: none; "
        "background-color: " + buttonBgColor + "; "
                          "border-radius: 10px; "
                          "padding: 10px; } "
                          "QPushButton:hover { border: 2px solid " + hoverBorderColor + "; }"
        );
    ui->unit_scrollArea->setStyleSheet(
        "QScrollArea{border:none;}"+
        scroll_bar_style);

    ui->conversion_page_label->setStyleSheet("color: " + buttonBgColor + "; ");

    if(configManager->isDarkTheme()){
        QString light_combobox_theme="QComboBox {"
                                       "background-color: #333;"
                                       "color: white;"
                                       "border: 1px solid #555;"
                                       "padding: 5px;"
                                       "border-radius: 5px;"
                                       "}"

                                       "QComboBox QAbstractItemView {"
                                       "background: #222;"
                                       "color: white;"
                                       "selection-background-color: blue;"
                                       "}"

                                       "QComboBox QAbstractItemView::item:selected {"
                                       "background-color: "+buttonBgColor+";"
                                       "color: white;"
                                       "}"

                                       "QComboBox::drop-down {"
                                       "border: 0px;"
                                       "}"

                                       "QComboBox::down-arrow {"
                                       "image: url(:/resources/icons/down-white.png);"
                                       "width: 10px;"
                                       "height: 10px;"
                                       "}";
        ui->from_comboBox->setStyleSheet(light_combobox_theme);
        ui->to_comboBox->setStyleSheet(light_combobox_theme);
        ui->input_doubleSpinBox->setStyleSheet(
                            "QDoubleSpinBox {"
                            "    color: white;"
                            "    border-radius: 10px;"
                            "    padding: 4px;"
                            "    border: 1px solid " + buttonBgColor + ";}"
                            "QDoubleSpinBox::up-button, "
                            "QDoubleSpinBox::down-button {"
                            "    width: 0px;"
                            "    height: 0px;"
                            "    border: none;"
                            "}"
            );
        ui->output_label->setStyleSheet("color: white;"
                                        "border-radius: 10px;"
                                        "padding: 4px;"
                                        "border: 1px solid "+buttonBgColor+";");
        ui->from_label->setStyleSheet("color: white;");
        ui->to_label->setStyleSheet("color: white;");
    }else{
        QString dark_combobox_theme="QComboBox {"
                                      "background-color: white;"
                                      "color: black;"
                                      "border: 1px solid gray;"
                                      "padding: 5px;"
                                      "border-radius: 5px;"
                                      "}"

                                      "QComboBox QAbstractItemView {"
                                      "background: white;"
                                      "color: black;"
                                      "selection-background-color: lightblue;"
                                      "}"

                                      "QComboBox QAbstractItemView::item:selected {"
                                      "background-color: "+buttonBgColor+";"
                                      "color: white;"
                                      "}"

                                      "QComboBox::drop-down {"
                                      "border: 0px;"
                                      "}"

                                      "QComboBox::down-arrow {"
                                      "image: url(:/resources/icons/down-black.png);"
                                      "width: 10px;"
                                      "height: 10px;"
                                      "}";
        ui->from_comboBox->setStyleSheet(dark_combobox_theme);
        ui->to_comboBox->setStyleSheet(dark_combobox_theme);
        ui->input_doubleSpinBox->setStyleSheet(
            "QDoubleSpinBox {"
            "    color: black;"
            "    border-radius: 10px;"
            "    padding: 4px;"
            "    border: 1px solid " + buttonBgColor + ";}"
            "QDoubleSpinBox::up-button, "
            "QDoubleSpinBox::down-button {"
            "    width: 0px;"
            "    height: 0px;"
            "    border: none;"
            "}");
        ui->output_label->setStyleSheet("color: black;"
                                        "border-radius: 10px;"
                                        "padding: 4px;"
                                        "border: 1px solid "+buttonBgColor+";");
        ui->from_label->setStyleSheet("color: black;");
        ui->to_label->setStyleSheet("color: black;");
    }

    ui->convert_btn->setStyleSheet("QPushButton{"
                                   "background-color: "+buttonBgColor+";"
                                   "color: white;}"
                                   "QPushButton:hover{"
                                   "background-color: "+configManager->numberButtonBgColor().name()+";"
                                   "color: white;"
                                   "border: 2px solid "+buttonBgColor+";"
                                   "border-radius: 10px;"
                                   "}");
}

QString CustCalc::scroll_var_style(QString scroll_bar_color) {
    QString style =
        "QScrollBar:vertical {"
        "    background-color: #2A2929;"
        "    width: 15px;"
        "    margin: 15px 3px 15px 3px;"
        "    border: 1px transparent #2A2929;"
        "    border-radius: 4px;"
        "}"

        "QScrollBar::handle:vertical {"
        "    background-color: " + scroll_bar_color + ";"
                             "    min-height: 5px;"
                             "    border-radius: 4px;"
                             "}"

                             "QScrollBar::sub-line:vertical {"
                             "    margin: 3px 0px 3px 0px;"
                             "    border-image: url(:/qss_icons/rc/up_arrow_disabled.png);"
                             "    height: 10px;"
                             "    width: 10px;"
                             "    subcontrol-position: top;"
                             "    subcontrol-origin: margin;"
                             "}"

                             "QScrollBar::add-line:vertical {"
                             "    margin: 3px 0px 3px 0px;"
                             "    border-image: url(:/qss_icons/rc/down_arrow_disabled.png);"
                             "    height: 10px;"
                             "    width: 10px;"
                             "    subcontrol-position: bottom;"
                             "    subcontrol-origin: margin;"
                             "}"

                             "QScrollBar::sub-line:vertical:hover, QScrollBar::sub-line:vertical:on {"
                             "    border-image: url(:/resources/icons/up-black.svg);"
                             "    height: 10px;"
                             "    width: 10px;"
                             "    subcontrol-position: top;"
                             "    subcontrol-origin: margin;"
                             "}"

                             "QScrollBar::add-line:vertical:hover, QScrollBar::add-line:vertical:on {"
                             "    border-image: url(:/resources/icons/down-black.svg);"
                             "    height: 10px;"
                             "    width: 10px;"
                             "    subcontrol-position: bottom;"
                             "    subcontrol-origin: margin;"
                             "}"

                             "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                             "    background: none;"
                             "}"

                             "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                             "    background: none;"
                             "}";

    return style;
}

void CustCalc::switch_to_another_page(int page_index,QString light_icon_path,QString dark_icon_path){
    ui->stackedWidget->setCurrentIndex(page_index);
    if(configManager->isDarkTheme()){
        QIcon lenght_icon(light_icon_path);
        ui->converter_dashboard_btn->setIcon(lenght_icon);
    }else{
        QIcon lenght_icon(dark_icon_path);
        ui->converter_dashboard_btn->setIcon(lenght_icon);
    }

    ui->converter_dashboard_btn->setStyleSheet(
        "border-top: none; "
        "border-right: none; "
        "border-bottom: 2px solid "+configManager->symbolButtonBgColor().name()+";"
                                                        "border-left: none;"
        );
    ui->calulator_history_btn->setStyleSheet("border: none;");
    ui->calulator_btn->setStyleSheet("border: none;");
}

void CustCalc::ready_conversion_page(QString page_label,QStringList units){
    ui->conversion_page_label->setText(page_label);
    ui->from_comboBox->clear();
    ui->to_comboBox->clear();
    ui->from_comboBox->addItems(units);
    ui->to_comboBox->addItems(units);
    ui->input_doubleSpinBox->clear();
    ui->input_doubleSpinBox->setValue(0.00);
    ui->output_label->clear();
}

void CustCalc::on_convert_btn_clicked()
{
    double input = ui->input_doubleSpinBox->value();
    QString from = ui->from_comboBox->currentText();
    QString to = ui->to_comboBox->currentText();
    double result = input;

    QMap<QString, double> factors;
    QString category = ui->conversion_page_label->text();

    if (category == "Length") {
        factors = { {"Meters", 1.0}, {"Kilometers", 1000.0}, {"Miles", 1609.34}, {"Yards", 0.9144},
                   {"Feet", 0.3048}, {"Centimeters", 0.01}, {"Millimeters", 0.001}, {"Nanometers", 1e-9},
                   {"Inches", 0.0254}, {"Light years", 9.461e+15}, {"Astronomical units", 1.496e+11} };
    }
    else if (category == "Mass") {
        factors = { {"Kilograms", 1.0}, {"Grams", 0.001}, {"Milligrams", 1e-6}, {"Micrograms", 1e-9},
                   {"Pounds", 0.453592}, {"Ounces", 0.0283495}, {"Stones", 6.35029}, {"Tons", 1000.0} };
    }
    else if (category == "Temperature") {
        if (from == "Celsius" && to == "Fahrenheit") result = (input * 9/5) + 32;
        else if (from == "Fahrenheit" && to == "Celsius") result = (input - 32) * 5/9;
        else if (from == "Celsius" && to == "Kelvin") result = input + 273.15;
        else if (from == "Kelvin" && to == "Celsius") result = input - 273.15;
        else if (from == "Fahrenheit" && to == "Kelvin") result = (input - 32) * 5/9 + 273.15;
        else if (from == "Kelvin" && to == "Fahrenheit") result = (input - 273.15) * 9/5 + 32;
        else if (from == "Rankine" && to == "Celsius") result = (input - 491.67) * 5/9;
        else if (from == "Celsius" && to == "Rankine") result = (input + 273.15) * 9/5;
        else if (from == "Fahrenheit" && to == "Rankine") result = input + 459.67;
        else if (from == "Rankine" && to == "Fahrenheit") result = input - 459.67;
        else if (from == "Kelvin" && to == "Rankine") result = input * 9/5;
        else if (from == "Rankine" && to == "Kelvin") result = input * 5/9;
    }
    else if (category == "Area") {
        factors = { {"Square meters", 1.0}, {"Square kilometers", 1e6}, {"Square miles", 2.59e6},
                   {"Acres", 4046.86}, {"Hectares", 10000.0}, {"Square yards", 0.836127},
                   {"Square feet", 0.092903}, {"Square inches", 0.00064516} };
    }
    else if (category == "Time") {
        factors = { {"Seconds", 1.0}, {"Minutes", 60.0}, {"Hours", 3600.0}, {"Days", 86400.0},
                   {"Weeks", 604800.0}, {"Months", 2.628e+6}, {"Years", 3.154e+7} };
    }
    else if (category == "Volume") {
        factors = { {"Cubic meters", 1.0}, {"Liters", 0.001}, {"Milliliters", 1e-6}, {"Gallons", 0.00378541},
                   {"Fluid ounces", 2.957e-5}, {"Quarts", 0.000946353}, {"Pints", 0.000473176},
                   {"Cubic inches", 1.63871e-5}, {"Cubic feet", 0.0283168} };
    }
    else if (category == "Speed") {
        factors = { {"Meters per second", 1.0}, {"Kilometers per hour", 0.277778}, {"Miles per hour", 0.44704},
                   {"Feet per second", 0.3048}, {"Knots", 0.514444} };
    }
    else if (category == "Pressure") {
        factors = { {"Pascals", 1.0}, {"Kilopascals", 1000.0}, {"Bar", 100000.0}, {"Atmospheres", 101325.0},
                   {"Pounds per square inch", 6894.76}, {"Torr", 133.322}, {"Millimeters of mercury", 133.322} };
    }
    else if (category == "Energy") {
        factors = { {"Joules", 1.0}, {"Kilojoules", 1000.0}, {"Calories", 4.184}, {"Kilocalories", 4184.0},
                   {"Electronvolts", 1.602e-19}, {"Watt-hours", 3600.0}, {"British Thermal Units", 1055.06} };
    }
    else if (category == "Power") {
        factors = { {"Watts", 1.0}, {"Kilowatts", 1000.0}, {"Horsepower", 745.7},
                   {"BTU per hour", 0.293071}, {"Kilocalories per hour", 1.163} };
    }
    else if (category == "Data Storage") {
        factors = { {"Bytes", 1.0}, {"Kilobytes", 1024.0}, {"Megabytes", 1048576.0}, {"Gigabytes", 1073741824.0},
                   {"Terabytes", 1.0995e+12}, {"Petabytes", 1.1259e+15}, {"Exabytes", 1.1529e+18},
                   {"Zettabytes", 1.1806e+21}, {"Yottabytes", 1.2089e+24} };
    }
    else if (category == "Angle") {
        factors = { {"Degrees", 1.0}, {"Radians", 57.2958}, {"Gradians", 0.9} };
    }

    if (factors.contains(from) && factors.contains(to)) {
        result = (input * factors[from]) / factors[to];
    }

    ui->output_label->setText("Result: " + QString::number(result));
}


void CustCalc::on_length_distance_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/ruler-white.svg",":/resources/units_icons/ruler-black.svg");
    ready_conversion_page("Length",{"Meters", "Kilometers", "Miles", "Yards", "Feet", "Centimeters", "Millimeters", "Inches"});
}

void CustCalc::on_mass_weight_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/Weight-white.svg",":/resources/units_icons/Weight-black.svg");
    ready_conversion_page("Mass", {"Kilograms", "Grams", "Milligrams", "Pounds", "Ounces", "Stones", "Tons", "Micrograms"});
}


void CustCalc::on_Temperature_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/thermometer-white.svg",":/resources/units_icons/thermometer-black.svg");
    ready_conversion_page("Temperature", {"Celsius", "Fahrenheit", "Kelvin", "Rankine"});
}


void CustCalc::on_Time_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/clock-white.svg",":/resources/units_icons/clock-black.svg");
    ready_conversion_page("Time", {"Seconds", "Minutes", "Hours", "Days", "Weeks", "Months", "Years"});
}


void CustCalc::on_Area_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/square-white.svg",":/resources/units_icons/square-black.svg");
    ready_conversion_page("Area", {"Square meters", "Square kilometers", "Square miles", "Acres", "Hectares", "Square yards", "Square feet", "Square inches"});
}


void CustCalc::on_Volume_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/bottle-white.svg",":/resources/units_icons/bottle-black.svg");
    ready_conversion_page("Volume", {"Cubic meters", "Liters", "Milliliters", "Gallons", "Fluid ounces", "Quarts", "Pints", "Cubic inches", "Cubic feet"});
}


void CustCalc::on_speed_velocity_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/speedometer-white.svg",":/resources/units_icons/speedometer-black.svg");
    ready_conversion_page("Speed", {"Meters per second", "Kilometers per hour", "Miles per hour", "Feet per second", "Knots"});
}


void CustCalc::on_Pressure_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/Pressure-white.svg",":/resources/units_icons/Pressure-black.svg");
    ready_conversion_page("Pressure", {"Pascals", "Kilopascals", "Bar", "Atmospheres", "Pounds per square inch", "Torr", "Millimeters of mercury"});
}


void CustCalc::on_Energy_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/Energy-white.svg",":/resources/units_icons/Energy-black.svg");
    ready_conversion_page("Energy", {"Joules", "Kilojoules", "Calories", "Kilocalories", "Electronvolts", "Watt-hours", "British Thermal Units"});
}


void CustCalc::on_Power_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/plug-white.svg",":/resources/units_icons/plug-black.svg");
    ready_conversion_page("Power", {"Watts", "Kilowatts", "Horsepower", "BTU per hour", "Kilocalories per hour"});
}


void CustCalc::on_data_storage_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/hdd-white.svg",":/resources/units_icons/hdd-black.svg");
    ready_conversion_page("Data Storage", {"Bytes", "Kilobytes", "Megabytes", "Gigabytes", "Terabytes", "Petabytes", "Exabytes", "Zettabytes", "Yottabytes"});
}


void CustCalc::on_Angle_unit_btn_clicked()
{
    switch_to_another_page(3,":/resources/units_icons/Angle-white.svg",":/resources/units_icons/Angle-black.svg");
    ready_conversion_page("Angle", {"Degrees", "Radians", "Gradians"});
}

