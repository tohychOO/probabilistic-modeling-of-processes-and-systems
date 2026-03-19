#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QChartView>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <random>
#include <vector>
#include <map>
#include <memory>
#include "graphwalk.h"

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDistChanged(int index);
    void onRunClicked();
    void onSaveConfig();
    void onLoadConfig();
    void onShowLog();
    void onClearLog();

private:
    QDoubleSpinBox *hSpin, *ySpin;
    QSpinBox *nSpin, *kSpin, *lSpin;
    QComboBox *distCombo;
    
    QDoubleSpinBox *pSpin, *geomProbSpin, *aSpin, *bSpin;
    QSpinBox *modeSpin, *mSpin;
    QGroupBox *uniformBox, *binomialBox, *geometricBox, *triangularBox;
    
    QPushButton *runBtn, *saveBtn, *loadBtn, *logBtn, *clearLogBtn;
    QTextEdit *output;
    QListWidget *logList;
    QChartView *chartView;
    
    std::vector<Trajectory> log;
    int maxLogSize;
    
    int totalExperiments;
    std::map<int, int> totalHits;
    
    std::mt19937 rng;
    
    void setupUI();
    void updateDistParamsVisibility();
    std::unique_ptr<Distribution> createDistribution();
    void plotTrajectory(const Trajectory& traj);
    void addToLog(const Trajectory& traj);
    void estimateProbability(int l);
};

#endif // MAINWINDOW_H