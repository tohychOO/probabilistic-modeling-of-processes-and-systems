#include "mainwindow.h"
#include <QChart>
#include <QValueAxis>
#include <QCategoryAxis>
#include <cmath>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), rng(time(nullptr))
{
    setupUI();
    createChart();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QHBoxLayout *paramLayout = new QHBoxLayout();

    paramLayout->addWidget(new QLabel("p (вперёд):"));
    pSpin = new QDoubleSpinBox();
    pSpin->setRange(0.0, 1.0);
    pSpin->setValue(0.4);
    pSpin->setSingleStep(0.05);
    paramLayout->addWidget(pSpin);

    paramLayout->addWidget(new QLabel("q (назад):"));
    qSpin = new QDoubleSpinBox();
    qSpin->setRange(0.0, 1.0);
    qSpin->setValue(0.6);
    qSpin->setSingleStep(0.05);
    paramLayout->addWidget(qSpin);

    paramLayout->addWidget(new QLabel("Макс B:"));
    maxBSpin = new QSpinBox();
    maxBSpin->setRange(1, 1000);
    maxBSpin->setValue(20);
    paramLayout->addWidget(maxBSpin);

    paramLayout->addWidget(new QLabel("Экспериментов:"));
    experimentsSpin = new QSpinBox();
    experimentsSpin->setRange(1000, 1000000);
    experimentsSpin->setValue(10000);
    paramLayout->addWidget(experimentsSpin);

    runButton = new QPushButton("Запустить");
    paramLayout->addWidget(runButton);

    layout->addLayout(paramLayout);

    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setMaximumHeight(150);
    layout->addWidget(output);

    setCentralWidget(central);
    resize(800, 600);

    connect(runButton, &QPushButton::clicked, this, &MainWindow::runSimulation);
}

void MainWindow::createChart()
{
    QChart *chart = new QChart();
    chart->setTitle("Вероятность упасть в обрыв и вернуться в кафе");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    seriesFall = new QLineSeries();
    seriesFall->setName("Упасть в обрыв");

    seriesReturn = new QLineSeries();
    seriesReturn->setName("Вернуться в кафе");

    chart->addSeries(seriesFall);
    chart->addSeries(seriesReturn);

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("Начальная позиция B");
    chart->axes(Qt::Vertical).first()->setTitleText("Вероятность");

    chartView->setChart(chart);
}

void MainWindow::runSimulation()
{
    double p = pSpin->value();
    double q = qSpin->value();

    if (abs(p + q - 1.0) > 1e-6) {
        output->setText("Ошибка: p + q должно быть равно 1");
        return;
    }

    int maxB = maxBSpin->value();
    int experiments = experimentsSpin->value();

    seriesFall->clear();
    seriesReturn->clear();

    QString results = "Результаты для p = " + QString::number(p) + ", q = " + QString::number(q) + "\n\n";
    results += "B\tУпасть\tВернуться\n";

    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int B = 1; B <= maxB; ++B) {
        int fallCount = 0;
        int returnCount = 0;

        for (int exp = 0; exp < experiments; ++exp) {
            int pos = B;

            while (pos > 0) {
                double r = dist(rng);
                if (r < p) {
                    pos++;
                } else {
                    pos--;
                }
            }

            if (pos == 0) {
                fallCount++;
            }
        }

        fallCount = 0;
        returnCount = 0;

        for (int exp = 0; exp < experiments; ++exp) {
            int pos = B;
            bool returned = false;

            while (pos > 0 && !returned) {
                double r = dist(rng);
                if (r < p) {
                    pos++;
                } else {
                    pos--;
                }

                if (pos == B) {
                    returned = true;
                }
            }

            if (pos == 0) {
                fallCount++;
            } else if (returned) {
                returnCount++;
            }
        }

        double fallProb = static_cast<double>(fallCount) / experiments;
        double returnProb = static_cast<double>(returnCount) / experiments;

        seriesFall->append(B, fallProb);
        seriesReturn->append(B, returnProb);

        results += QString::number(B) + "\t" +
                   QString::number(fallProb, 'f', 4) + "\t" +
                   QString::number(returnProb, 'f', 4) + "\n";
    }

    output->setText(results);

    chartView->chart()->axes(Qt::Horizontal).first()->setRange(1, maxB);
    chartView->chart()->axes(Qt::Vertical).first()->setRange(0, 1);
}