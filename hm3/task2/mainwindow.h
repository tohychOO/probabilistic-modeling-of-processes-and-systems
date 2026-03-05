#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <random>
#include <vector>

struct Node {
    int id;
    double stayProb;
    std::vector<int> children;
};

struct PathInfo {
    std::vector<int> nodes;
    int leafIndex;
    bool stuck;
};

class Tree {
public:
    int M;
    int height;
    std::vector<Node> nodes;
    std::vector<double> leafHits;
    std::vector<int> pathLengths;
    std::vector<PathInfo> allPaths;

    Tree(int m, int h);
    int runSimulation(std::mt19937& rng);
    void resetStats();
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void runSimulation();

private:
    QSpinBox *spinM, *spinHeight, *spinL, *spinLeaf;
    QDoubleSpinBox *spinP;
    QComboBox *choiceBox;
    QPushButton *btnRun;
    QTextEdit *output;
    QGraphicsScene *scene;
    QGraphicsView *view;

    Tree tree;
    std::mt19937 rng;
};

#endif