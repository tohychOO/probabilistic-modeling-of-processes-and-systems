#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    setWindowTitle("Дискретная случайная величина");
    resize(1300, 800);
    
    QWidget* central = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    
    // ==================== ЛЕВАЯ ПАНЕЛЬ ====================
    QWidget* leftPanel = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    
    // 1. Таблица для ввода
    QGroupBox* inputGroup = new QGroupBox("Закон распределения");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);
    
    m_inputTable = new QTableWidget(4, 2);
    m_inputTable->setHorizontalHeaderLabels({"Значение (x)", "Вероятность P(x)"});
    m_inputTable->horizontalHeader()->setStretchLastSection(true);
    inputLayout->addWidget(m_inputTable);
    
    // Пример заполнения для демонстрации
    m_inputTable->setItem(0, 0, new QTableWidgetItem("1"));
    m_inputTable->setItem(0, 1, new QTableWidgetItem("0.2"));
    m_inputTable->setItem(1, 0, new QTableWidgetItem("2"));
    m_inputTable->setItem(1, 1, new QTableWidgetItem("0.3"));
    m_inputTable->setItem(2, 0, new QTableWidgetItem("3"));
    m_inputTable->setItem(2, 1, new QTableWidgetItem("0.5"));
    
    QPushButton* addRowBtn = new QPushButton("+ Добавить строку");
    connect(addRowBtn, &QPushButton::clicked, [this]() {
        int row = m_inputTable->rowCount();
        m_inputTable->insertRow(row);
    });
    inputLayout->addWidget(addRowBtn);
    
    leftLayout->addWidget(inputGroup);
    
    // 2. Кнопки операций
    QGroupBox* operationsGroup = new QGroupBox("Операции");
    QVBoxLayout* opsLayout = new QVBoxLayout(operationsGroup);
    
    m_createBtn = new QPushButton("Создать СВ");
    m_saveBtn = new QPushButton("Сохранить в файл");
    m_loadBtn = new QPushButton("Загрузить из файла");
    
    QHBoxLayout* scalarLayout = new QHBoxLayout();
    m_scalarEdit = new QLineEdit();
    m_scalarEdit->setPlaceholderText("Множитель");
    m_mulScalarBtn = new QPushButton("Умножить на скаляр");
    scalarLayout->addWidget(m_scalarEdit);
    scalarLayout->addWidget(m_mulScalarBtn);
    
    m_addBtn = new QPushButton("Сложить с другой СВ");
    m_mulBtn = new QPushButton("Умножить на другую СВ");
    
    opsLayout->addWidget(m_createBtn);
    opsLayout->addLayout(scalarLayout);
    opsLayout->addWidget(m_addBtn);
    opsLayout->addWidget(m_mulBtn);
    opsLayout->addWidget(m_saveBtn);
    opsLayout->addWidget(m_loadBtn);
    
    leftLayout->addWidget(operationsGroup);
    
    // 3. Статистика
    QGroupBox* statsGroup = new QGroupBox("Характеристики");
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    m_statsText = new QTextEdit();
    m_statsText->setReadOnly(true);
    m_statsText->setMaximumHeight(200);
    statsLayout->addWidget(m_statsText);
    leftLayout->addWidget(statsGroup);
    
    // 4. Статус
    m_statusLabel = new QLabel("Готов");
    leftLayout->addWidget(m_statusLabel);
    
    leftLayout->addStretch();
    leftPanel->setMaximumWidth(450);
    
    // ==================== ПРАВАЯ ПАНЕЛЬ (ГРАФИКИ) ====================
    m_tabWidget = new QTabWidget();
    m_pmfChartView = new QChartView();
    m_cdfChartView = new QChartView();
    
    m_tabWidget->addTab(m_pmfChartView, "Полилайн (PMF)");
    m_tabWidget->addTab(m_cdfChartView, "Функция распределения (CDF)");
    
    // ==================== Собираем всё ====================
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(m_tabWidget, 1);
    central->setLayout(mainLayout);
    setCentralWidget(central);
    
    // ==================== СВЯЗЬ СИГНАЛОВ ====================
    connect(m_createBtn, &QPushButton::clicked, this, &MainWindow::onCreateVariable);
    connect(m_saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveToFile);
    connect(m_loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadFromFile);
    connect(m_mulScalarBtn, &QPushButton::clicked, this, &MainWindow::onMultiplyScalar);
    connect(m_addBtn, &QPushButton::clicked, this, &MainWindow::onAddVariable);
    connect(m_mulBtn, &QPushButton::clicked, this, &MainWindow::onMultiplyVariable);
}

void MainWindow::onCreateVariable()
{
    try {
        vector<pair<double, double>> table;
        for (int row = 0; row < m_inputTable->rowCount(); ++row) {
            QTableWidgetItem* valItem = m_inputTable->item(row, 0);
            QTableWidgetItem* probItem = m_inputTable->item(row, 1);
            
            if (!valItem || !probItem) continue;
            
            bool valOk, probOk;
            double val = valItem->text().toDouble(&valOk);
            double prob = probItem->text().toDouble(&probOk);
            
            if (valOk && probOk && prob >= 0 && prob <= 1) {
                table.emplace_back(val, prob);
            }
        }
        
        if (table.empty()) {
            QMessageBox::warning(this, "Ошибка", "Нет корректных данных");
            return;
        }
        
        m_currentRV = DiscreteRandomVariable(table);
        updateInfo();
        onUpdatePlots();
        m_statusLabel->setText("СВ создана");
        
    } catch (const exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::updateInfo()
{
    if (m_currentRV.isEmpty()) {
        m_statsText->setText("Нет данных");
        return;
    }
    
    QString text;
    text += "    Закон распределения\n";
    text += QString::fromStdString(m_currentRV.distributionTable());
    text += "\n    Числовые характеристики\n";
    text += QString("Математическое ожидание: %1\n").arg(m_currentRV.expectation(), 0, 'f', 6);
    text += QString("Дисперсия: %1\n").arg(m_currentRV.variance(), 0, 'f', 6);
    text += QString("Асимметрия: %1\n").arg(m_currentRV.skewness(), 0, 'f', 6);
    text += QString("Эксцесс: %1\n").arg(m_currentRV.kurtosis(), 0, 'f', 6);
    
    m_statsText->setText(text);
}

void MainWindow::plotProbabilityMass()
{
    if (m_currentRV.isEmpty()) {
        m_pmfChartView->setChart(new QChart());
        return;
    }
    
    QLineSeries* series = new QLineSeries();
    for (const auto& [val, prob] : m_currentRV.getTable()) {
        series->append(val, prob);
    }
    
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Полилайн (график значений и вероятностей)");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("Значение");
    chart->axes(Qt::Vertical).first()->setTitleText("Вероятность");
    
    m_pmfChartView->setChart(chart);
}

void MainWindow::plotCumulative()
{
    if (m_currentRV.isEmpty()) {
        m_cdfChartView->setChart(new QChart());
        return;
    }
    
    QLineSeries* series = new QLineSeries();
    double cumProb = 0.0;
    for (const auto& [val, prob] : m_currentRV.getTable()) {
        cumProb += prob;
        series->append(val, cumProb);
    }
    
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Функция распределения (CDF)");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("x");
    chart->axes(Qt::Vertical).first()->setTitleText("F(x)");
    
    m_cdfChartView->setChart(chart);
}

void MainWindow::onUpdatePlots()
{
    plotProbabilityMass();
    plotCumulative();
}

void MainWindow::onSaveToFile()
{
    if (m_currentRV.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет СВ для сохранения");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить СВ", "", "Текстовые файлы (*.txt)");
    if (!filename.isEmpty()) {
        try {
            m_currentRV.saveToFile(filename.toStdString());
            m_statusLabel->setText("Сохранено: " + filename);
        } catch (const exception& e) {
            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }
}

void MainWindow::onLoadFromFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Загрузить СВ", "", "Текстовые файлы (*.txt)");
    if (!filename.isEmpty()) {
        try {
            m_currentRV = DiscreteRandomVariable::loadFromFile(filename.toStdString());
            updateInfo();
            onUpdatePlots();
            m_statusLabel->setText("Загружено: " + filename);
        } catch (const exception& e) {
            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }
}

void MainWindow::onMultiplyScalar()
{
    if (m_currentRV.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет СВ для умножения");
        return;
    }
    
    bool ok;
    double scalar = m_scalarEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите число");
        return;
    }
    
    try {
        m_currentRV = m_currentRV * scalar;
        updateInfo();
        onUpdatePlots();
        m_statusLabel->setText(QString("Умножено на %1").arg(scalar));
    } catch (const exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::onAddVariable()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите СВ для сложения", "", "Текстовые файлы (*.txt)");
    if (filename.isEmpty()) return;
    
    try {
        DiscreteRandomVariable other = DiscreteRandomVariable::loadFromFile(filename.toStdString());
        m_currentRV = m_currentRV + other;
        updateInfo();
        onUpdatePlots();
        m_statusLabel->setText("Сложение выполнено");
    } catch (const exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::onMultiplyVariable()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите СВ для умножения", "", "Текстовые файлы (*.txt)");
    if (filename.isEmpty()) return;
    
    try {
        DiscreteRandomVariable other = DiscreteRandomVariable::loadFromFile(filename.toStdString());
        m_currentRV = m_currentRV * other;
        updateInfo();
        onUpdatePlots();
        m_statusLabel->setText("Умножение выполнено");
    } catch (const exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}