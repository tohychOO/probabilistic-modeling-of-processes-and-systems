#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "simulation.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void loadFile();
    void runSimulation();
    void showNeverInfected();
    void showRecovered();
    void showRecoveredWithNonRecoveredNeighbors();
    void showHealthyWithAllInfectedNeighbors();

private:
    QTextEdit *output;
    QLineEdit *filePathEdit;
    QDoubleSpinBox *p1Spin, *p2Spin;
    QPushButton *loadBtn, *runBtn;
    QPushButton *btn1, *btn2, *btn3, *btn4;

    Simulation sim;
    QVector<SimPerson> lastResult;
};

#endif // MAINWINDOW_H