#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , gen(std::time(nullptr))
    , isRunning(false)
{
    setupUI();
    drawLines();
    
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onNext);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    
    QHBoxLayout *paramLayout = new QHBoxLayout();
    
    dEdit = new QLineEdit("100");
    LEdit = new QLineEdit("80");
    trialsEdit = new QLineEdit("100");
    startBtn = new QPushButton("Старт");
    nextBtn = new QPushButton("Бросок");
    autoBtn = new QPushButton("Авто");
    
    paramLayout->addWidget(new QLabel("d:"));
    paramLayout->addWidget(dEdit);
    paramLayout->addWidget(new QLabel("L:"));
    paramLayout->addWidget(LEdit);
    paramLayout->addWidget(new QLabel("Бросков:"));
    paramLayout->addWidget(trialsEdit);
    paramLayout->addWidget(startBtn);
    paramLayout->addWidget(nextBtn);
    paramLayout->addWidget(autoBtn);
    
    layout->addLayout(paramLayout);
    
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setFixedSize(800, 600);
    layout->addWidget(view);
    
    infoLabel = new QLabel("Готов");
    layout->addWidget(infoLabel);
    
    setCentralWidget(central);
    resize(900, 700);
    
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStart);
    connect(nextBtn, &QPushButton::clicked, this, &MainWindow::onNext);
    connect(autoBtn, &QPushButton::clicked, this, &MainWindow::onAuto);
    
    nextBtn->setEnabled(false);
    autoBtn->setEnabled(false);
}

void MainWindow::drawLines()
{
    scene->clear();
    double dd = dEdit->text().toDouble();
    if (dd < 1) dd = 100;
    for (int y = 0; y <= 600; y += (int)dd) {
        scene->addLine(0, y, 800, y, QPen(Qt::black, 2));
    }
}

void MainWindow::drawNeedle(double x, double phi)
{
    // удаляем старую иглу
    for (auto item : scene->items()) {
        if (auto line = dynamic_cast<QGraphicsLineItem*>(item)) {
            if (line->pen().color() == Qt::red) {
                scene->removeItem(line);
                delete line;
            }
        }
        if (auto ell = dynamic_cast<QGraphicsEllipseItem*>(item)) {
            if (ell->brush().color() == Qt::blue) {
                scene->removeItem(ell);
                delete ell;
            }
        }
    }
    
    double cx = 400;
    double cy = x;
    double hl = L / 2.0;
    double x1 = cx - hl * cos(phi);
    double y1 = cy - hl * sin(phi);
    double x2 = cx + hl * cos(phi);
    double y2 = cy + hl * sin(phi);
    
    auto needle = scene->addLine(x1, y1, x2, y2, QPen(Qt::red, 3));
    auto center = scene->addEllipse(cx - 3, cy - 3, 6, 6, QPen(), QBrush(Qt::blue));
}

void MainWindow::updateStats()
{
    if (currentTrial == 0) return;
    double emp = (double)intersections / currentTrial;
    double anal = (2.0 * L) / (d * M_PI);
    double piEst = (2.0 * L) / (d * emp);
    infoLabel->setText(QString("Бросок %1/%2\nПересечений: %3\nPэмп=%4\nPанал=%5\nπ≈%6")
        .arg(currentTrial).arg(totalTrials).arg(intersections)
        .arg(emp, 0, 'f', 5).arg(anal, 0, 'f', 5).arg(piEst, 0, 'f', 5));
}

void MainWindow::onStart()
{
    d = dEdit->text().toDouble();
    L = LEdit->text().toDouble();
    totalTrials = trialsEdit->text().toInt();
    
    if (L > d) {
        QMessageBox::warning(this, "Ошибка", "L <= d");
        return;
    }
    
    drawLines();
    currentTrial = 0;
    intersections = 0;
    isRunning = true;
    
    startBtn->setEnabled(false);
    nextBtn->setEnabled(true);
    autoBtn->setEnabled(true);
    timer->stop();
    
    infoLabel->setText("Нажат Старт");
    drawNeedle(0, 0);
}

void MainWindow::onNext()
{
    if (!isRunning) return;
    if (currentTrial >= totalTrials) {
        isRunning = false;
        startBtn->setEnabled(true);
        nextBtn->setEnabled(false);
        autoBtn->setEnabled(false);
        infoLabel->setText("Готово");
        return;
    }
    
    std::uniform_real_distribution<> distX(0.0, d / 2.0);
    std::uniform_real_distribution<> distPhi(0.0, M_PI / 2.0);
    
    double x = distX(gen);
    double phi = distPhi(gen);
    
    if (x <= (L / 2.0) * sin(phi)) intersections++;
    
    currentTrial++;
    drawNeedle(x, phi);
    updateStats();
}

void MainWindow::onAuto()
{
    if (!isRunning) return;
    autoBtn->setEnabled(false);
    nextBtn->setEnabled(false);
    timer->start(30);
}