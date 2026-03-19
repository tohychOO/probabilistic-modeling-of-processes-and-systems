#include "mainwindow.h"
#include <QChart>
#include <QValueAxis>
#include <QDateTime>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), rng(std::time(nullptr)), totalExperiments(0), returnedCount(0)
{
    setupUI();
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QGroupBox *paramBox = new QGroupBox("Параметры");
    QGridLayout *paramLayout = new QGridLayout(paramBox);

    paramLayout->addWidget(new QLabel("M (запусков):"), 0, 0);
    mSpin = new QSpinBox();
    mSpin->setRange(1, 1000000);
    mSpin->setValue(10000);
    paramLayout->addWidget(mSpin, 0, 1);

    paramLayout->addWidget(new QLabel("K (шагов):"), 1, 0);
    kSpin = new QSpinBox();
    kSpin->setRange(1, 10000);
    kSpin->setValue(1000);
    paramLayout->addWidget(kSpin, 1, 1);

    paramLayout->addWidget(new QLabel("ρ:"), 2, 0);
    rhoSpin = new QDoubleSpinBox();
    rhoSpin->setRange(0.1, 10.0);
    rhoSpin->setValue(1.0);
    paramLayout->addWidget(rhoSpin, 2, 1);

    paramLayout->addWidget(new QLabel("n (≥4):"), 3, 0);
    nSpin = new QSpinBox();
    nSpin->setRange(4, 100);
    nSpin->setValue(6);
    paramLayout->addWidget(nSpin, 3, 1);

    paramLayout->addWidget(new QLabel("Распределение ξ:"), 4, 0);
    distCombo = new QComboBox();
    distCombo->addItem("Равномерное (0..n-1)");
    distCombo->addItem("Биномиальное");
    distCombo->addItem("Геометрическое");
    distCombo->addItem("Треугольное");
    paramLayout->addWidget(distCombo, 4, 1);

    layout->addWidget(paramBox);

    QHBoxLayout *distParamLayout = new QHBoxLayout();

    binomBox = new QGroupBox("Биномиальное");
    QVBoxLayout *binomLayout = new QVBoxLayout(binomBox);
    QHBoxLayout *trialsLayout = new QHBoxLayout();
    trialsLayout->addWidget(new QLabel("Испытаний:"));
    trialsSpin = new QSpinBox();
    trialsSpin->setRange(1, 100);
    trialsSpin->setValue(5);
    trialsLayout->addWidget(trialsSpin);
    binomLayout->addLayout(trialsLayout);

    QHBoxLayout *pLayout = new QHBoxLayout();
    pLayout->addWidget(new QLabel("p:"));
    pSpin = new QDoubleSpinBox();
    pSpin->setRange(0.0, 1.0);
    pSpin->setValue(0.5);
    pSpin->setSingleStep(0.05);
    pLayout->addWidget(pSpin);
    binomLayout->addLayout(pLayout);
    distParamLayout->addWidget(binomBox);

    geomBox = new QGroupBox("Геометрическое");
    QVBoxLayout *geomLayout = new QVBoxLayout(geomBox);
    QHBoxLayout *geomPLayout = new QHBoxLayout();
    geomPLayout->addWidget(new QLabel("p:"));
    geomPSpin = new QDoubleSpinBox();
    geomPSpin->setRange(0.01, 0.99);
    geomPSpin->setValue(0.3);
    geomPSpin->setSingleStep(0.05);
    geomPLayout->addWidget(geomPSpin);
    geomLayout->addLayout(geomPLayout);

    QHBoxLayout *geomMaxLayout = new QHBoxLayout();
    geomMaxLayout->addWidget(new QLabel("Макс:"));
    geomMaxSpin = new QSpinBox();
    geomMaxSpin->setRange(1, 100);
    geomMaxSpin->setValue(10);
    geomMaxLayout->addWidget(geomMaxSpin);
    geomLayout->addLayout(geomMaxLayout);
    distParamLayout->addWidget(geomBox);

    triangularBox = new QGroupBox("Треугольное");
    QVBoxLayout *triangularLayout = new QVBoxLayout(triangularBox);
    QHBoxLayout *aTriLayout = new QHBoxLayout();
    aTriLayout->addWidget(new QLabel("a:"));
    triangularASpin = new QSpinBox();
    triangularASpin->setRange(0, 100);
    triangularASpin->setValue(0);
    aTriLayout->addWidget(triangularASpin);
    triangularLayout->addLayout(aTriLayout);

    QHBoxLayout *bTriLayout = new QHBoxLayout();
    bTriLayout->addWidget(new QLabel("b:"));
    triangularBSpin = new QSpinBox();
    triangularBSpin->setRange(0, 100);
    triangularBSpin->setValue(5);
    bTriLayout->addWidget(triangularBSpin);
    triangularLayout->addLayout(bTriLayout);

    QHBoxLayout *cTriLayout = new QHBoxLayout();
    cTriLayout->addWidget(new QLabel("мода:"));
    triangularCSpin = new QSpinBox();
    triangularCSpin->setRange(0, 100);
    triangularCSpin->setValue(2);
    cTriLayout->addWidget(triangularCSpin);
    triangularLayout->addLayout(cTriLayout);
    distParamLayout->addWidget(triangularBox);

    layout->addLayout(distParamLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    runBtn = new QPushButton("Запустить");
    saveBtn = new QPushButton("Сохранить конфиг");
    loadBtn = new QPushButton("Загрузить конфиг");
    btnLayout->addWidget(runBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(loadBtn);
    layout->addLayout(btnLayout);

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setMaximumHeight(100);
    layout->addWidget(output);

    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

    setCentralWidget(central);
    resize(1000, 800);

    connect(distCombo, &QComboBox::activated, this, &MainWindow::updateDistParamsVisibility);
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::onRunClicked);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveConfig);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadConfig);

    updateDistParamsVisibility(0);
}

void MainWindow::updateDistParamsVisibility(int index)
{
    binomBox->setVisible(index == 1);
    geomBox->setVisible(index == 2);
    triangularBox->setVisible(index == 3);
}

std::unique_ptr<Distribution> MainWindow::createDistribution()
{
    int n = nSpin->value();
    int type = distCombo->currentIndex();

    switch (type) {
        case 0:
            return std::make_unique<UniformIntDistribution>(0, n-1);
        case 1:
            return std::make_unique<BinomialDistribution>(trialsSpin->value(), pSpin->value());
        case 2:
            return std::make_unique<GeometricDistribution>(geomPSpin->value(), geomMaxSpin->value());
        case 3:
            return std::make_unique<TriangularDistribution>(
                triangularASpin->value(),
                triangularBSpin->value(),
                triangularCSpin->value());
        default:
            return std::make_unique<UniformIntDistribution>(0, n-1);
    }
}

void MainWindow::onRunClicked()
{
    int M = mSpin->value();
    int K = kSpin->value();
    double rho = rhoSpin->value();

    auto dist = createDistribution();

    int returned = 0;
    std::vector<std::complex<double>> lastTraj;

    for (int m = 0; m < M; ++m) {
        std::vector<std::complex<double>> traj;
        traj.push_back(std::complex<double>(0, 0));

        for (int step = 1; step <= K; ++step) {
            int xi = dist->sample(rng);
            double angle = 2.0 * M_PI * xi / nSpin->value();
            std::complex<double> z = traj.back() + std::polar(rho, angle);
            traj.push_back(z);

            if (std::abs(z) < 1e-9) {
                returned++;
                break;
            }
        }

        if (m == M-1) lastTraj = traj;
    }

    totalExperiments += M;
    returnedCount += returned;

    output->setText(QString("Всего экспериментов: %1\nВернулись в 0: %2 (%3%)\n")
                    .arg(totalExperiments)
                    .arg(returnedCount)
                    .arg(100.0 * returnedCount / totalExperiments, 0, 'f', 2));

    plotTrajectory(lastTraj);
}

void MainWindow::plotTrajectory(const std::vector<std::complex<double>>& traj)
{
    QChart *chart = new QChart();
    chart->setTitle("Последняя траектория");

    QLineSeries *series = new QLineSeries();

    for (const auto& z : traj) {
        series->append(z.real(), z.imag());
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("Re");
    chart->axes(Qt::Vertical).first()->setTitleText("Im");

    delete chartView->chart();
    chartView->setChart(chart);
}

void MainWindow::saveDistParams(QJsonObject& obj)
{
    int type = distCombo->currentIndex();
    obj["dist_type"] = type;

    QJsonObject distObj;
    distObj["name"] = distCombo->currentText();

    if (type == 1) {
        distObj["trials"] = trialsSpin->value();
        distObj["p"] = pSpin->value();
    } else if (type == 2) {
        distObj["p"] = geomPSpin->value();
        distObj["max"] = geomMaxSpin->value();
    } else if (type == 3) {
        distObj["a"] = triangularASpin->value();
        distObj["b"] = triangularBSpin->value();
        distObj["c"] = triangularCSpin->value();
    }

    obj["distribution"] = distObj;
}

void MainWindow::loadDistParams(const QJsonObject& obj)
{
    int type = obj["dist_type"].toInt();
    distCombo->setCurrentIndex(type);
    updateDistParamsVisibility(type);

    QJsonObject distObj = obj["distribution"].toObject();

    if (type == 1) {
        trialsSpin->setValue(distObj["trials"].toInt(5));
        pSpin->setValue(distObj["p"].toDouble(0.5));
    } else if (type == 2) {
        geomPSpin->setValue(distObj["p"].toDouble(0.3));
        geomMaxSpin->setValue(distObj["max"].toInt(10));
    } else if (type == 3) {
        triangularASpin->setValue(distObj["a"].toInt(0));
        triangularBSpin->setValue(distObj["b"].toInt(5));
        triangularCSpin->setValue(distObj["c"].toInt(2));
    }
}

void MainWindow::onSaveConfig()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить конфиг", "", "JSON (*.json)");
    if (fileName.isEmpty()) return;

    QJsonObject obj;
    obj["M"] = mSpin->value();
    obj["K"] = kSpin->value();
    obj["rho"] = rhoSpin->value();
    obj["n"] = nSpin->value();
    saveDistParams(obj);

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

    mSpin->setValue(obj["M"].toInt(10000));
    kSpin->setValue(obj["K"].toInt(1000));
    rhoSpin->setValue(obj["rho"].toDouble(1.0));
    nSpin->setValue(obj["n"].toInt(6));
    loadDistParams(obj);

    QMessageBox::information(this, "Успех", "Конфиг загружен");
}