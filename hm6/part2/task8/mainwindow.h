#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <random>
#include <cmath>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStart();
    void onNext();
    void onAuto();

private:
    void setupUI();
    void drawLines();
    void drawNeedle(double x, double phi);
    void updateStats();
    
    QGraphicsScene *scene;
    QGraphicsView *view;
    QLineEdit *dEdit, *LEdit, *trialsEdit;
    QPushButton *startBtn, *nextBtn, *autoBtn;
    QLabel *infoLabel;
    
    double d, L;
    int totalTrials, currentTrial, intersections;
    std::mt19937 gen;
    QTimer *timer;
    bool isRunning;
};

#endif