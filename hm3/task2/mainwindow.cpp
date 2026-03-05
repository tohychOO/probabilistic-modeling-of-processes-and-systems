#include "mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <cmath>

Tree::Tree(int m, int h) : M(m), height(h) {
    int totalNodes = (pow(M, height) - 1) / (M - 1);
    nodes.resize(totalNodes);
    leafHits.resize(pow(M, height - 1), 0.0);
    pathLengths.clear();
    allPaths.clear();

    for (int i = 0; i < totalNodes; i++) {
        nodes[i].id = i;
        nodes[i].stayProb = 0.1;
        for (int j = 0; j < M; j++) {
            int child = i * M + j + 1;
            if (child < totalNodes) nodes[i].children.push_back(child);
        }
    }
}

int Tree::runSimulation(std::mt19937& rng) {
    int currentNode = 0;
    int steps = 0;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    PathInfo path;
    path.nodes.push_back(currentNode);

    while (true) {
        steps++;
        if (dist(rng) < nodes[currentNode].stayProb) {
            path.stuck = true;
            path.leafIndex = -1;
            pathLengths.push_back(steps);
            allPaths.push_back(path);
            return -1;
        }

        if (nodes[currentNode].children.empty()) {
            int leafIndex = currentNode - (pow(M, height - 1) - 1) / (M - 1);
            leafHits[leafIndex]++;
            pathLengths.push_back(steps);
            path.stuck = false;
            path.leafIndex = leafIndex;
            allPaths.push_back(path);
            return leafIndex;
        }

        int childIndex = dist(rng) * nodes[currentNode].children.size();
        currentNode = nodes[currentNode].children[childIndex];
        path.nodes.push_back(currentNode);
    }
}

void Tree::resetStats() {
    for (int i = 0; i < leafHits.size(); i++) leafHits[i] = 0.0;
    pathLengths.clear();
    allPaths.clear();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), rng(std::time(nullptr)), tree(2, 3) {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QHBoxLayout *paramLayout = new QHBoxLayout();

    paramLayout->addWidget(new QLabel("M:"));
    spinM = new QSpinBox();
    spinM->setValue(2);
    paramLayout->addWidget(spinM);

    paramLayout->addWidget(new QLabel("Высота:"));
    spinHeight = new QSpinBox();
    spinHeight->setValue(3);
    paramLayout->addWidget(spinHeight);

    paramLayout->addWidget(new QLabel("p (застрять):"));
    spinP = new QDoubleSpinBox();
    spinP->setRange(0.0, 1.0);
    spinP->setValue(0.1);
    spinP->setSingleStep(0.05);
    paramLayout->addWidget(spinP);

    paramLayout->addWidget(new QLabel("Режим:"));
    choiceBox = new QComboBox();
    choiceBox->addItem("По длине пути (l)");
    choiceBox->addItem("По номеру листа");
    paramLayout->addWidget(choiceBox);

    paramLayout->addWidget(new QLabel("l / № листа:"));
    spinL = new QSpinBox();
    spinL->setRange(1, spinHeight->value());
    spinL->setValue(1);
    paramLayout->addWidget(spinL);

    spinLeaf = new QSpinBox();
    spinLeaf->setRange(1, pow(spinM->value(), spinHeight->value() - 1));
    spinLeaf->setValue(1);
    spinLeaf->setVisible(false);
    paramLayout->addWidget(spinLeaf);

    connect(choiceBox, QOverload<int>::of(&QComboBox::activated), [this](int index) {
        spinL->setVisible(index == 0);
        spinLeaf->setVisible(index == 1);
    });

    connect(spinHeight, QOverload<int>::of(&QSpinBox::valueChanged), [this](int newHeight) {
        spinL->setMaximum(newHeight);
        spinLeaf->setMaximum(pow(spinM->value(), newHeight - 1));
    });

    connect(spinM, QOverload<int>::of(&QSpinBox::valueChanged), [this](int newM) {
        spinLeaf->setMaximum(pow(newM, spinHeight->value() - 1));
    });

    btnRun = new QPushButton("Запустить");
    paramLayout->addWidget(btnRun);
    layout->addLayout(paramLayout);

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setMaximumHeight(100);
    layout->addWidget(output);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(view);

    setCentralWidget(central);
    resize(800, 600);

    connect(btnRun, &QPushButton::clicked, this, &MainWindow::runSimulation);
}

void MainWindow::runSimulation() {
    int M = spinM->value();
    int h = spinHeight->value();
    double p = spinP->value();

    if (M < 2) {
        output->setText("M должно быть >= 2");
        return;
    }
    if (h < 2) {
        output->setText("Высота должна быть >= 2");
        return;
    }

    long long totalNodes = (pow(M, h) - 1) / (M - 1);
    if (totalNodes > 10000) {
        output->setText("Слишком много узлов (>10000), моделирование отменено");
        return;
    }

    tree = Tree(M, h);
    for (auto& node : tree.nodes) node.stayProb = p;
    tree.resetStats();

    int experiments = 10000;
    for (int i = 0; i < experiments; i++) {
        tree.runSimulation(rng);
    }

    QString stats = QString("Экспериментов: %1\n").arg(experiments);
    stats += "Попадания в листья:\n";
    for (int i = 0; i < tree.leafHits.size(); i++) {
        stats += QString("  Лист %1: %2 (%3%)\n")
                    .arg(i + 1)
                    .arg(tree.leafHits[i])
                    .arg(tree.leafHits[i] / experiments * 100, 0, 'f', 2);
    }

    std::vector<int> pathDist(tree.height + 1, 0);
    for (int len : tree.pathLengths) {
        if (len <= tree.height) pathDist[len]++;
    }
    stats += "Длины путей:\n";
    for (int i = 1; i <= tree.height; i++) {
        stats += QString("  %1 шаг: %2 (%3%)\n")
                    .arg(i)
                    .arg(pathDist[i])
                    .arg(pathDist[i] / (double)experiments * 100, 0, 'f', 2);
    }

    int leafNumber = spinLeaf->value() - 1;
    if (leafNumber >= 0 && leafNumber < tree.leafHits.size()) {
        double leafProb = tree.leafHits[leafNumber] / experiments;
        stats += QString("\nВероятность попадания в лист %1: %2 (%3%)\n")
                    .arg(leafNumber + 1)
                    .arg(leafProb, 0, 'f', 6)
                    .arg(leafProb * 100, 0, 'f', 2);
    }

    output->setText(stats);

    scene->clear();

    double scaleX = view->width() - 100;
    double scaleY = view->height() - 100;
    double levelHeight = scaleY / (h + 1);
    double levelWidth = scaleX * 0.8;

    for (int i = 0; i < tree.nodes.size(); i++) {
        int level = 0;
        int temp = i;
        while (temp > 0) {
            temp = (temp - 1) / M;
            level++;
        }
        double y = level * levelHeight + 50;

        int nodesOnLevel = pow(M, level);
        int posInLevel = i - (pow(M, level) - 1) / (M - 1);
        double x = 50 + posInLevel * (levelWidth / (nodesOnLevel + 1));

        QGraphicsEllipseItem *ellipse = scene->addEllipse(x-10, y-10, 20, 20);
        if (tree.nodes[i].children.empty()) {
            ellipse->setBrush(QBrush(Qt::green));
        } else {
            ellipse->setBrush(QBrush(Qt::yellow));
        }

        for (int child : tree.nodes[i].children) {
            int childLevel = 0;
            int t = child;
            while (t > 0) { t = (t - 1) / M; childLevel++; }
            double childY = childLevel * levelHeight + 50;
            int childNodesOnLevel = pow(M, childLevel);
            int childPos = child - (pow(M, childLevel) - 1) / (M - 1);
            double childX = 50 + childPos * (levelWidth / (childNodesOnLevel + 1));
            scene->addLine(x, y, childX, childY, QPen(Qt::black));
        }
    }

    int mode = choiceBox->currentIndex();
    PathInfo examplePath;
    bool found = false;

    if (mode == 0) {
        int l = spinL->value();
        for (const auto& path : tree.allPaths) {
            if (path.nodes.size() == l) {
                examplePath = path;
                found = true;
                break;
            }
        }
        if (!found) {
            scene->addText("Нет пути длины " + QString::number(l));
        }
    } else {
        int leafNum = spinLeaf->value() - 1;
        for (const auto& path : tree.allPaths) {
            if (!path.stuck && path.leafIndex == leafNum) {
                examplePath = path;
                found = true;
                break;
            }
        }
        if (!found) {
            scene->addText("Нет пути к листу " + QString::number(leafNum + 1));
        }
    }

    if (found) {
        QPen redPen(Qt::red);
        redPen.setWidth(3);
        
        for (size_t j = 0; j < examplePath.nodes.size(); j++) {
            int nodeIdx = examplePath.nodes[j];
            
            int level = 0;
            int temp = nodeIdx;
            while (temp > 0) {
                temp = (temp - 1) / M;
                level++;
            }
            double y = level * levelHeight + 50;
            int nodesOnLevel = pow(M, level);
            int posInLevel = nodeIdx - (pow(M, level) - 1) / (M - 1);
            double x = 50 + posInLevel * (levelWidth / (nodesOnLevel + 1));
            
            scene->addEllipse(x-15, y-15, 30, 30, redPen);
            
            if (j < examplePath.nodes.size() - 1) {
                int nextIdx = examplePath.nodes[j+1];
                int nextLevel = 0;
                temp = nextIdx;
                while (temp > 0) {
                    temp = (temp - 1) / M;
                    nextLevel++;
                }
                double nextY = nextLevel * levelHeight + 50;
                int nextNodesOnLevel = pow(M, nextLevel);
                int nextPos = nextIdx - (pow(M, nextLevel) - 1) / (M - 1);
                double nextX = 50 + nextPos * (levelWidth / (nextNodesOnLevel + 1));
                scene->addLine(x, y, nextX, nextY, redPen);
            }
        }
    }
}