#include "mainwindow.h"
#include <QChart>
#include <QValueAxis>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), rng(std::time(nullptr)), maxLogSize(100)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QHBoxLayout *paramLayout = new QHBoxLayout();
    
    QGroupBox *basicBox = new QGroupBox("Основные параметры");
    QVBoxLayout *basicLayout = new QVBoxLayout(basicBox);
    
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel("h (шаг по x):"));
    hSpin = new QDoubleSpinBox();
    hSpin->setRange(0.1, 10.0);
    hSpin->setValue(1.0);
    hSpin->setSingleStep(0.1);
    hLayout->addWidget(hSpin);
    basicLayout->addLayout(hLayout);
    
    QHBoxLayout *yLayout = new QHBoxLayout();
    yLayout->addWidget(new QLabel("Y (старт):"));
    ySpin = new QDoubleSpinBox();
    ySpin->setRange(-100.0, 100.0);
    ySpin->setValue(5.0);
    yLayout->addWidget(ySpin);
    basicLayout->addLayout(yLayout);
    
    QHBoxLayout *nLayout = new QHBoxLayout();
    nLayout->addWidget(new QLabel("n (шагов):"));
    nSpin = new QSpinBox();
    nSpin->setRange(1, 1000);
    nSpin->setValue(100);
    nLayout->addWidget(nSpin);
    basicLayout->addLayout(nLayout);
    
    QHBoxLayout *kLayout = new QHBoxLayout();
    kLayout->addWidget(new QLabel("K (лог):"));
    kSpin = new QSpinBox();
    kSpin->setRange(1, 500);
    kSpin->setValue(20);
    kLayout->addWidget(kSpin);
    basicLayout->addLayout(kLayout);
    
    QHBoxLayout *lLayout = new QHBoxLayout();
    lLayout->addWidget(new QLabel("l (пересечений):"));
    lSpin = new QSpinBox();
    lSpin->setRange(0, 1000);
    lSpin->setValue(2);
    lLayout->addWidget(lSpin);
    basicLayout->addLayout(lLayout);
    
    paramLayout->addWidget(basicBox);
    
    QGroupBox *distBox = new QGroupBox("Распределение");
    QVBoxLayout *distLayout = new QVBoxLayout(distBox);
    
    QHBoxLayout *comboLayout = new QHBoxLayout();
    comboLayout->addWidget(new QLabel("Тип:"));
    distCombo = new QComboBox();
    distCombo->addItem("Равномерное");
    distCombo->addItem("Биномиальное (0/1)");
    distCombo->addItem("Геометрическое (конечное)");
    distCombo->addItem("Треугольное");
    comboLayout->addWidget(distCombo);
    distLayout->addLayout(comboLayout);
    
    uniformBox = new QGroupBox("Параметры равномерного");
    QVBoxLayout *uniformLayout = new QVBoxLayout(uniformBox);
    
    QHBoxLayout *aLayout = new QHBoxLayout();
    aLayout->addWidget(new QLabel("a:"));
    aSpin = new QDoubleSpinBox();
    aSpin->setRange(-100.0, 100.0);
    aSpin->setValue(-2.0);
    aLayout->addWidget(aSpin);
    uniformLayout->addLayout(aLayout);
    
    QHBoxLayout *bLayout = new QHBoxLayout();
    bLayout->addWidget(new QLabel("b:"));
    bSpin = new QDoubleSpinBox();
    bSpin->setRange(-100.0, 100.0);
    bSpin->setValue(2.0);
    bLayout->addWidget(bSpin);
    uniformLayout->addLayout(bLayout);
    distLayout->addWidget(uniformBox);
    
    binomialBox = new QGroupBox("Параметры биномиального");
    QVBoxLayout *binomialLayout = new QVBoxLayout(binomialBox);
    
    QHBoxLayout *pLayout = new QHBoxLayout();
    pLayout->addWidget(new QLabel("p:"));
    pSpin = new QDoubleSpinBox();
    pSpin->setRange(0.0, 1.0);
    pSpin->setValue(0.5);
    pSpin->setSingleStep(0.05);
    pLayout->addWidget(pSpin);
    binomialLayout->addLayout(pLayout);
    distLayout->addWidget(binomialBox);
    
    geometricBox = new QGroupBox("Параметры геометрического");
    QVBoxLayout *geometricLayout = new QVBoxLayout(geometricBox);
    
    QHBoxLayout *mGeomLayout = new QHBoxLayout();
    mGeomLayout->addWidget(new QLabel("m (макс):"));
    mSpin = new QSpinBox();
    mSpin->setRange(1, 100);
    mSpin->setValue(10);
    mGeomLayout->addWidget(mSpin);
    geometricLayout->addLayout(mGeomLayout);
    
    QHBoxLayout *probGeomLayout = new QHBoxLayout();
    probGeomLayout->addWidget(new QLabel("p:"));
    geomProbSpin = new QDoubleSpinBox();
    geomProbSpin->setRange(0.0, 1.0);
    geomProbSpin->setValue(0.5);
    geomProbSpin->setSingleStep(0.05);
    probGeomLayout->addWidget(geomProbSpin);
    geometricLayout->addLayout(probGeomLayout);
    distLayout->addWidget(geometricBox);
    
    triangularBox = new QGroupBox("Параметры треугольного");
    QVBoxLayout *triangularLayout = new QVBoxLayout(triangularBox);
    
    QHBoxLayout *aTriLayout = new QHBoxLayout();
    aTriLayout->addWidget(new QLabel("a:"));
    aSpin = new QDoubleSpinBox();
    aSpin->setRange(-100.0, 100.0);
    aSpin->setValue(-3.0);
    aTriLayout->addWidget(aSpin);
    triangularLayout->addLayout(aTriLayout);
    
    QHBoxLayout *bTriLayout = new QHBoxLayout();
    bTriLayout->addWidget(new QLabel("b:"));
    bSpin = new QDoubleSpinBox();
    bSpin->setRange(-100.0, 100.0);
    bSpin->setValue(3.0);
    bTriLayout->addWidget(bSpin);
    triangularLayout->addLayout(bTriLayout);
    
    QHBoxLayout *modeTriLayout = new QHBoxLayout();
    modeTriLayout->addWidget(new QLabel("мода:"));
    modeSpin = new QSpinBox();
    modeSpin->setRange(-100, 100);
    modeSpin->setValue(0);
    modeTriLayout->addWidget(modeSpin);
    triangularLayout->addLayout(modeTriLayout);
    distLayout->addWidget(triangularBox);
    
    paramLayout->addWidget(distBox);
    
    QVBoxLayout *btnLayout = new QVBoxLayout();
    runBtn = new QPushButton("Запустить");
    saveBtn = new QPushButton("Сохранить конфиг");
    loadBtn = new QPushButton("Загрузить конфиг");
    logBtn = new QPushButton("Показать лог");
    clearLogBtn = new QPushButton("Очистить лог");
    
    btnLayout->addWidget(runBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(loadBtn);
    btnLayout->addWidget(logBtn);
    btnLayout->addWidget(clearLogBtn);
    btnLayout->addStretch();
    
    paramLayout->addLayout(btnLayout);
    
    mainLayout->addLayout(paramLayout);
    
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView, 2);
    
    QHBoxLayout *logLayout = new QHBoxLayout();
    
    output = new QTextEdit();
    output->setReadOnly(true);
    output->setMaximumHeight(150);
    logLayout->addWidget(output, 1);
    
    logList = new QListWidget();
    logList->setMaximumWidth(200);
    logLayout->addWidget(logList);
    
    mainLayout->addLayout(logLayout);
    
    setCentralWidget(central);
    resize(1200, 800);
    
    connect(distCombo, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::onDistChanged);
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::onRunClicked);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveConfig);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadConfig);
    connect(logBtn, &QPushButton::clicked, this, &MainWindow::onShowLog);
    connect(clearLogBtn, &QPushButton::clicked, this, &MainWindow::onClearLog);
    
    onDistChanged(0);
}

void MainWindow::onDistChanged(int index)
{
    uniformBox->setVisible(index == 0);
    binomialBox->setVisible(index == 1);
    geometricBox->setVisible(index == 2);
    triangularBox->setVisible(index == 3);
}

std::unique_ptr<Distribution> MainWindow::createDistribution()
{
    int type = distCombo->currentIndex();
    
    switch (type) {
        case 0:
            return std::make_unique<UniformDistribution>(aSpin->value(), bSpin->value());
        case 1:
            return std::make_unique<BinomialDistribution>(pSpin->value());
        case 2:
            return std::make_unique<GeometricDistribution>(mSpin->value(), geomProbSpin->value());
        case 3:
            return std::make_unique<TriangularDistribution>(
                static_cast<int>(aSpin->value()),
                static_cast<int>(bSpin->value()),
                modeSpin->value()
            );
        default:
            return std::make_unique<UniformDistribution>(-1, 1);
    }
}

void MainWindow::onRunClicked() 
{
    double h = hSpin->value();
    int n = nSpin->value();
    double y0 = ySpin->value();
    int experiments = 10000;
    maxLogSize = kSpin->value();

    totalExperiments = 0;
    totalHits.clear();
    log.clear();
    logList->clear();
    
    for (int exp = 0; exp < experiments; ++exp) {
        auto dist = createDistribution();
        RandomWalk walk(h, n, y0, std::move(dist));
        Trajectory traj = walk.run(rng);
        
        totalExperiments++;
        totalHits[traj.crossings]++;
        
        addToLog(traj);
    }
    
    if (!log.empty())
        plotTrajectory(log.back());
    
    estimateProbability(lSpin->value());
}

void MainWindow::plotTrajectory(const Trajectory& traj)
{
    QChart *chart = new QChart();
    chart->setTitle("Траектория случайного блуждания");
    
    QLineSeries *series = new QLineSeries();
    series->setName("y(x)");
    
    for (size_t i = 0; i < traj.x.size(); ++i) {
        series->append(traj.x[i], traj.y[i]);
    }
    
    chart->addSeries(series);
    chart->createDefaultAxes();
    
    QLineSeries *zeroLine = new QLineSeries();
    zeroLine->setName("y=0");
    zeroLine->append(traj.x.front(), 0);
    zeroLine->append(traj.x.back(), 0);
    QPen pen(Qt::gray);
    pen.setStyle(Qt::DashLine);
    zeroLine->setPen(pen);
    chart->addSeries(zeroLine);
    
    chart->axes(Qt::Horizontal).first()->setTitleText("x");
    chart->axes(Qt::Vertical).first()->setTitleText("y");
    
    delete chartView->chart();
    chartView->setChart(chart);
}

void MainWindow::addToLog(const Trajectory& traj)
{
    log.push_back(traj);
    
    if (log.size() > static_cast<size_t>(maxLogSize)) {
        log.erase(log.begin());
    }
    
    logList->clear();
    for (size_t i = 0; i < log.size(); ++i) {
        QString item = QString("Траектория %1: %2 пересеч.")
                            .arg(i + 1)
                            .arg(log[i].crossings);
        logList->addItem(item);
    }
}

void MainWindow::estimateProbability(int l)
{
    if (totalExperiments == 0) {
        output->append("Нет данных для оценки вероятности");
        return;
    }
    
    int count = totalHits[l];
    double prob = static_cast<double>(count) / totalExperiments;
    
    output->append(QString("Вероятность l=%1: %2 (%3/%4)")
                        .arg(l)
                        .arg(prob, 0, 'f', 4)
                        .arg(count)
                        .arg(totalExperiments));
}

void MainWindow::onSaveConfig()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить конфиг", "", "JSON (*.json)");
    if (fileName.isEmpty()) return;
    
    QJsonObject obj;
    obj["h"] = hSpin->value();
    obj["n"] = nSpin->value();
    obj["Y"] = ySpin->value();
    obj["K"] = kSpin->value();
    obj["l"] = lSpin->value();
    obj["dist_type"] = distCombo->currentIndex();
    
    auto dist = createDistribution();
    obj["distribution"] = dist->toJson();
    
    QJsonDocument doc(obj);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        QMessageBox::information(this, "Успех", "Конфиг сохранён");
    }
}

void MainWindow::onLoadConfig()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Загрузить конфиг", "", "JSON (*.json)");
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат JSON");
        return;
    }
    
    QJsonObject obj = doc.object();
    
    hSpin->setValue(obj["h"].toDouble(1.0));
    nSpin->setValue(obj["n"].toInt(100));
    ySpin->setValue(obj["Y"].toDouble(5.0));
    kSpin->setValue(obj["K"].toInt(20));
    lSpin->setValue(obj["l"].toInt(2));
    
    int distType = obj["dist_type"].toInt(0);
    distCombo->setCurrentIndex(distType);
    onDistChanged(distType);
    
    if (obj.contains("distribution")) {
        QJsonObject distObj = obj["distribution"].toObject();
        QString type = distObj["type"].toString();
        
        if (type == "uniform") {
            aSpin->setValue(distObj["a"].toDouble(-2.0));
            bSpin->setValue(distObj["b"].toDouble(2.0));
        } else if (type == "binomial") {
            pSpin->setValue(distObj["p"].toDouble(0.5));
        } else if (type == "geometric") {
            mSpin->setValue(distObj["m"].toInt(10));
            geomProbSpin->setValue(distObj["prob"].toDouble(0.5));
        } else if (type == "triangular") {
            aSpin->setValue(distObj["a"].toDouble(-3.0));
            bSpin->setValue(distObj["b"].toDouble(3.0));
            modeSpin->setValue(distObj["mode"].toInt(0));
        }
    }
    
    QMessageBox::information(this, "Успех", "Конфиг загружен");
}

void MainWindow::onShowLog()
{
    QString logText = "Лог последних траекторий:\n\n";
    for (size_t i = 0; i < log.size(); ++i) {
        logText += QString("Траектория %1: %2 пересечений\n")
                        .arg(i + 1)
                        .arg(log[i].crossings);
    }
    output->setText(logText);
}

void MainWindow::onClearLog()
{
    log.clear();
    logList->clear();
    output->setText("Лог очищен");
}