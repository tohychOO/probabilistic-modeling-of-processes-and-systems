#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QChartView>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <random>

QT_CHARTS_USE_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runSimulation();

private:
    QDoubleSpinBox *pSpin, *qSpin;
    QSpinBox *maxBSpin, *experimentsSpin;
    QPushButton *runButton;
    QTextEdit *output;
    QChartView *chartView;
    QLineSeries *seriesFall, *seriesReturn;

    mt19937 rng;

    void setupUI();
    void createChart();
};

#endif // MAINWINDOW_H