#include "mainwindow.h"
#include <QChart>
#include <QValueAxis>
#include <QDateTime>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), rng(std::time(nullptr)), currentPage(0), itemsPerPage(10),
      totalExperiments(0), increasingCount(0)
{
    setupUI();
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Основные параметры
    QGroupBox *paramBox = new QGroupBox("Параметры");
    QGridLayout *paramLayout = new QGridLayout(paramBox);

    paramLayout->addWidget(new QLabel("M (длина отрезка):"), 0, 0);
    mSpin = new QDoubleSpinBox();
    mSpin->setRange(1.0, 100.0);
    mSpin->setValue(10.0);
    paramLayout->addWidget(mSpin, 0, 1);

    paramLayout->addWidget(new QLabel("h (шаг):"), 1, 0);
    hSpin = new QDoubleSpinBox();
    hSpin->setRange(0.1, 10.0);
    hSpin->setValue(1.0);
    hSpin->setSingleStep(0.1);
    paramLayout->addWidget(hSpin, 1, 1);

    paramLayout->addWidget(new QLabel("τ (шаг значений):"), 2, 0);
    nSpin = new QSpinBox();
    nSpin->setRange(1, 20);
    nSpin->setValue(5);
    paramLayout->addWidget(nSpin, 2, 1);

    paramLayout->addWidget(new QLabel("Сегментов:"), 3, 0);
    segmentsSpin = new QSpinBox();
    segmentsSpin->setRange(2, 50);
    segmentsSpin->setValue(10);
    paramLayout->addWidget(segmentsSpin, 3, 1);

    paramLayout->addWidget(new QLabel("Распределение ξ:"), 4, 0);
    distCombo = new QComboBox();
    distCombo->addItem("Равномерное");
    distCombo->addItem("Биномиальное");
    distCombo->addItem("Геометрическое");
    distCombo->addItem("Треугольное");
    paramLayout->addWidget(distCombo, 4, 1);

    layout->addWidget(paramBox);

    // Дополнительные параметры для распределений
    QHBoxLayout *distParamLayout = new QHBoxLayout();

    // Биномиальное
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

    // Геометрическое
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

    // Треугольное
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

    // Кнопки генерации и конфига
    QHBoxLayout *btnLayout = new QHBoxLayout();
    generateBtn = new QPushButton("Сгенерировать N фигур");
    saveBtn = new QPushButton("Сохранить конфиг");
    loadBtn = new QPushButton("Загрузить конфиг");
    btnLayout->addWidget(generateBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(loadBtn);
    layout->addLayout(btnLayout);

    // Список фигур и пагинация
    QHBoxLayout *listLayout = new QHBoxLayout();
    listWidget = new QListWidget();
    listWidget->setMaximumWidth(200);
    listLayout->addWidget(listWidget);

    QVBoxLayout *pageLayout = new QVBoxLayout();
    QHBoxLayout *navLayout = new QHBoxLayout();
    prevBtn = new QPushButton("<");
    nextBtn = new QPushButton(">");
    pageSpin = new QSpinBox();
    pageSpin->setRange(1, 1);
    pageSpin->setValue(1);
    navLayout->addWidget(prevBtn);
    navLayout->addWidget(pageSpin);
    navLayout->addWidget(nextBtn);
    pageLayout->addLayout(navLayout);

    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    pageLayout->addWidget(chartView);
    listLayout->addLayout(pageLayout);
    layout->addLayout(listLayout);

    // Вывод статистики
    output = new QTextEdit();
    output->setReadOnly(true);
    output->setMaximumHeight(100);
    layout->addWidget(output);

    setCentralWidget(central);
    resize(1200, 800);

    // Подключения
    connect(distCombo, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::updateDistParamsVisibility);
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerateClicked);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveConfig);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadConfig);
    connect(prevBtn, &QPushButton::clicked, this, &MainWindow::onPrevPage);
    connect(nextBtn, &QPushButton::clicked, this, &MainWindow::onNextPage);
    connect(listWidget, &QListWidget::currentRowChanged, [this](int row) {
        if (row >= 0) plotStaircase(staircases[row]);
    });
    connect(pageSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int page) {
        currentPage = page - 1;
        updateList();
    });

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
    int type = distCombo->currentIndex();

    switch (type) {
        case 0: // Равномерное
            return std::make_unique<UniformIntDistribution>(0, nSpin->value());
        case 1: // Биномиальное
            return std::make_unique<BinomialDistribution>(trialsSpin->value(), pSpin->value());
        case 2: // Геометрическое
            return std::make_unique<GeometricDistribution>(geomPSpin->value(), geomMaxSpin->value());
        case 3: // Треугольное
            return std::make_unique<TriangularDistribution>(
                triangularASpin->value(),
                triangularBSpin->value(),
                triangularCSpin->value());
        default:
            return std::make_unique<UniformIntDistribution>(0, nSpin->value());
    }
}

bool MainWindow::isStrictlyIncreasing(const std::vector<double>& y)
{
    for (size_t i = 1; i < y.size(); ++i) {
        if (y[i] <= y[i-1]) return false;
    }
    return true;
}

void MainWindow::onGenerateClicked()
{
    int N = segmentsSpin->value();  // число сегментов
    int count = 100;  // генерируем 100 фигур (можно добавить поле ввода)
    double M = mSpin->value();
    double h = hSpin->value();
    double tau = nSpin->value();  // шаг значений

    auto dist = createDistribution();

    staircases.clear();
    increasingCount = 0;

    for (int i = 0; i < count; ++i) {
        Staircase s;
        s.x.resize(N + 1);
        s.y.resize(N + 1);
        s.x[0] = 0;
        s.y[0] = 0;

        for (int j = 0; j < N; ++j) {
            s.x[j + 1] = s.x[j] + h;
            int xi = dist->sample(rng);
            s.y[j + 1] = s.y[j] + xi * tau;
        }

        s.isIncreasing = isStrictlyIncreasing(s.y);
        if (s.isIncreasing) increasingCount++;

        staircases.push_back(s);
    }

    totalExperiments += count;
    currentPage = 0;
    pageSpin->setMaximum((staircases.size() + itemsPerPage - 1) / itemsPerPage);
    pageSpin->setValue(1);
    updateList();

    if (!staircases.empty()) plotStaircase(staircases[0]);

    output->setText(QString("Всего фигур: %1\nСтрого возрастающих: %2 (%3%)\n")
                    .arg(totalExperiments)
                    .arg(increasingCount)
                    .arg(100.0 * increasingCount / totalExperiments, 0, 'f', 2));
}

void MainWindow::updateList()
{
    listWidget->clear();
    int start = currentPage * itemsPerPage;
    int end = std::min(start + itemsPerPage, (int)staircases.size());

    for (int i = start; i < end; ++i) {
        QString itemText = QString("Фигура %1: %2")
                              .arg(i + 1)
                              .arg(staircases[i].isIncreasing ? "↑" : "→");
        listWidget->addItem(itemText);
    }
}

void MainWindow::plotStaircase(const Staircase& s)
{
    QChart *chart = new QChart();
    chart->setTitle("Ступенчатая фигура");

    QLineSeries *series = new QLineSeries();
    for (size_t i = 0; i < s.x.size(); ++i) {
        series->append(s.x[i], s.y[i]);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("x");
    chart->axes(Qt::Vertical).first()->setTitleText("y");

    delete chartView->chart();
    chartView->setChart(chart);
}

void MainWindow::onPrevPage()
{
    if (currentPage > 0) {
        currentPage--;
        pageSpin->setValue(currentPage + 1);
        updateList();
    }
}

void MainWindow::onNextPage()
{
    if ((currentPage + 1) * itemsPerPage < (int)staircases.size()) {
        currentPage++;
        pageSpin->setValue(currentPage + 1);
        updateList();
    }
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
    obj["h"] = hSpin->value();
    obj["tau"] = nSpin->value();
    obj["segments"] = segmentsSpin->value();
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

    mSpin->setValue(obj["M"].toDouble(10.0));
    hSpin->setValue(obj["h"].toDouble(1.0));
    nSpin->setValue(obj["tau"].toInt(5));
    segmentsSpin->setValue(obj["segments"].toInt(10));
    loadDistParams(obj);

    QMessageBox::information(this, "Успех", "Конфиг загружен");
}