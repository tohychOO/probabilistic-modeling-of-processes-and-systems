#include "mainwindow.h"
#include "graphloader.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Строка загрузки файла
    QHBoxLayout *fileLayout = new QHBoxLayout();
    fileLayout->addWidget(new QLabel("Файл:"));
    filePathEdit = new QLineEdit();
    fileLayout->addWidget(filePathEdit);
    loadBtn = new QPushButton("Обзор...");
    fileLayout->addWidget(loadBtn);
    layout->addLayout(fileLayout);

    // Параметры
    QHBoxLayout *paramLayout = new QHBoxLayout();
    paramLayout->addWidget(new QLabel("p1 (заражение):"));
    p1Spin = new QDoubleSpinBox();
    p1Spin->setRange(0.0, 1.0);
    p1Spin->setValue(0.3);
    p1Spin->setSingleStep(0.05);
    paramLayout->addWidget(p1Spin);

    paramLayout->addWidget(new QLabel("p2 (излечение):"));
    p2Spin = new QDoubleSpinBox();
    p2Spin->setRange(0.0, 1.0);
    p2Spin->setValue(0.1);
    p2Spin->setSingleStep(0.05);
    paramLayout->addWidget(p2Spin);

    runBtn = new QPushButton("Запустить симуляцию");
    paramLayout->addWidget(runBtn);
    layout->addLayout(paramLayout);

    // Кнопки поиска
    QHBoxLayout *searchLayout = new QHBoxLayout();
    btn1 = new QPushButton("Не заразившиеся");
    btn2 = new QPushButton("Исцелившиеся");
    btn3 = new QPushButton("Исцел. + окруж. не исцел.");
    btn4 = new QPushButton("Здоров, все соседи больны");
    searchLayout->addWidget(btn1);
    searchLayout->addWidget(btn2);
    searchLayout->addWidget(btn3);
    searchLayout->addWidget(btn4);
    layout->addLayout(searchLayout);

    // Вывод
    output = new QTextEdit();
    output->setReadOnly(true);
    layout->addWidget(output);

    setCentralWidget(central);
    resize(700, 500);

    // Подключения
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadFile);
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::runSimulation);
    connect(btn1, &QPushButton::clicked, this, &MainWindow::showNeverInfected);
    connect(btn2, &QPushButton::clicked, this, &MainWindow::showRecovered);
    connect(btn3, &QPushButton::clicked, this, &MainWindow::showRecoveredWithNonRecoveredNeighbors);
    connect(btn4, &QPushButton::clicked, this, &MainWindow::showHealthyWithAllInfectedNeighbors);
}

void MainWindow::loadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл с данными");
    if (!fileName.isEmpty()) {
        filePathEdit->setText(fileName);

        QVector<Person> people;
        QMap<QString, int> nameToIndex;
        if (GraphLoader::load(fileName, people, nameToIndex)) {
            QVector<SimPerson> simPeople;
            for (const auto& p : people) {
                SimPerson sp;
                sp.name = p.name;
                sp.friends = p.friends;
                sp.state = Healthy;
                simPeople.append(sp);
            }
            sim.setData(simPeople);
            output->append("Файл загружен: " + fileName);
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить файл");
        }
    }
}

void MainWindow::runSimulation() {
    sim.setParams(p1Spin->value(), p2Spin->value());
    sim.run();
    lastResult = sim.getPeople();
    output->append("Симуляция завершена");
}

void MainWindow::showNeverInfected() {
    output->append("\n--- Не заразившиеся ---");
    for (const auto& p : lastResult) {
        if (p.state == Healthy)
            output->append(p.name);
    }
}

void MainWindow::showRecovered() {
    output->append("\n--- Исцелившиеся ---");
    for (const auto& p : lastResult) {
        if (p.state == Recovered)
            output->append(p.name);
    }
}

void MainWindow::showRecoveredWithNonRecoveredNeighbors() {
    output->append("\n--- Исцелившиеся, чьё окружение не исцелилось ---");
    for (const auto& p : lastResult) {
        if (p.state != Recovered) continue;
        bool hasNonRecovered = false;
        for (int fIdx : p.friends) {
            if (lastResult[fIdx].state != Recovered) {
                hasNonRecovered = true;
                break;
            }
        }
        if (hasNonRecovered)
            output->append(p.name);
    }
}

void MainWindow::showHealthyWithAllInfectedNeighbors() {
    output->append("\n--- Не заразились, хотя все соседи болели ---");
    for (const auto& p : lastResult) {
        if (p.state != Healthy) continue;
        bool allInfectedOrRecovered = true;
        for (int fIdx : p.friends) {
            if (lastResult[fIdx].state == Healthy) {
                allInfectedOrRecovered = false;
                break;
            }
        }
        if (allInfectedOrRecovered && !p.friends.isEmpty())
            output->append(p.name);
    }
}