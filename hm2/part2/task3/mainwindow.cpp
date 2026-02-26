#include "mainwindow.h"
#include <random>
#include <vector>
#include <set>
#include <QProgressBar>
#include <QHBoxLayout>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Ввод параметров
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("n (жителей без источника):"));
    spinN = new QSpinBox();
    spinN->setRange(1, 100);
    spinN->setValue(10);
    inputLayout->addWidget(spinN);

    inputLayout->addWidget(new QLabel("r (число передач):"));
    spinR = new QSpinBox();
    spinR->setRange(1, 100);
    spinR->setValue(5);
    inputLayout->addWidget(spinR);

    inputLayout->addWidget(new QLabel("N (размер группы):"));
    spinNgroup = new QSpinBox();
    spinNgroup->setRange(1, 10);
    spinNgroup->setValue(1);
    inputLayout->addWidget(spinNgroup);

    inputLayout->addWidget(new QLabel("K (экспериментов):"));
    spinK = new QSpinBox();
    spinK->setRange(1000, 10000000);
    spinK->setValue(10000);
    inputLayout->addWidget(spinK);

    layout->addLayout(inputLayout);

    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *btnA = new QPushButton("Запуск (условие а)");
    QPushButton *btnB = new QPushButton("Запуск (условие б)");
    buttonLayout->addWidget(btnA);
    buttonLayout->addWidget(btnB);
    layout->addLayout(buttonLayout);

    // Поле вывода
    output = new QTextEdit();
    output->setReadOnly(true);
    layout->addWidget(output);

    setCentralWidget(central);
    resize(600, 400);

    // Подключаем кнопки
    connect(btnA, &QPushButton::clicked, this, &MainWindow::runSimulationA);
    connect(btnB, &QPushButton::clicked, this, &MainWindow::runSimulationB);
}

void MainWindow::runSimulationA() {
    int n = spinN->value();
    int r = spinR->value();
    int N = spinNgroup->value();
    int K = spinK->value();

    mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(0, n);

    int success = 0;

    for (int exp = 0; exp < K; ++exp) {
        int current = 0;
        int count = 0;
        bool ok = true;

        for (int step = 0; step < r; ++step) {
            set<int> stepReceivers;
            for (int j = 0; j < N; ++j) {
                int next;
                do {
                    next = dist(gen);
                } while (next == current || stepReceivers.count(next));
                stepReceivers.insert(next);
            }

            if (stepReceivers.count(0)) {
                ok = false;
                break;
            }

            current = *stepReceivers.begin();
            count++;
        }

        if (ok && count == r) 
            success++;
    }

    double prob = static_cast<double>(success) / K;
    output->append(QString("Условие а: вероятность = %1 (%2/%3)")
                   .arg(prob, 0, 'f', 6)
                   .arg(success)
                   .arg(K));
}

void MainWindow::runSimulationB() {
    int n = spinN->value();
    int r = spinR->value();
    int N = spinNgroup->value();
    int K = spinK->value();

    mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(1, n);

    int success = 0;

    for (int exp = 0; exp < K; ++exp) {
        set<int> allReceivers;
        int current = 0;
        int count = 0;
        bool ok = true;

        allReceivers.insert(0);

        for (int step = 0; step < r; ++step) {
            set<int> stepReceivers;

            for (int j = 0; j < N; ++j) {
                int next;
                do {
                    next = dist(gen);
                } while (stepReceivers.count(next));
                stepReceivers.insert(next);
            }

            for (int rec : stepReceivers) {
                if (allReceivers.count(rec)) {
                    ok = false;
                    break;
                }
            }

            if (!ok)
                break;

            for (int rec : stepReceivers) {
                allReceivers.insert(rec);
            }

            current = *stepReceivers.begin();
            count++;
        }

        if (ok && count == r) 
            success++;
    }

    double prob = static_cast<double>(success) / K;
    output->append(QString("Условие б: вероятность = %1 (%2/%3)")
                   .arg(prob, 0, 'f', 6)
                   .arg(success)
                   .arg(K));
}