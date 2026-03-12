#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QChartView>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <random>
#include <complex>
#include <memory>
#include <vector>
#include "distribution.h"

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onRunClicked();
    void onSaveConfig();
    void onLoadConfig();

private:
    QSpinBox *mSpin, *kSpin, *nSpin;
    QDoubleSpinBox *rhoSpin;
    QComboBox *distCombo;
    QPushButton *runBtn, *saveBtn, *loadBtn;
    QTextEdit *output;
    QChartView *chartView;
    QLineSeries *series;

    QDoubleSpinBox *pSpin, *geomPSpin;
    QSpinBox *trialsSpin, *geomMaxSpin, *triangularASpin, *triangularBSpin, *triangularCSpin;
    QGroupBox *binomBox, *geomBox, *triangularBox;

    std::mt19937 rng;
    int totalExperiments;
    int returnedCount;

    void setupUI();
    void updateDistParamsVisibility(int index);
    std::unique_ptr<Distribution> createDistribution();
    void plotTrajectory(const std::vector<std::complex<double>>& traj);
    void loadDistParams(const QJsonObject& obj);
    void saveDistParams(QJsonObject& obj);
};

#endif // MAINWINDOW_H