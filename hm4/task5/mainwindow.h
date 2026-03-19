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
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <random>
#include <memory>
#include <vector>
#include "distribution.h"

QT_CHARTS_USE_NAMESPACE

struct Staircase {
    std::vector<double> x;
    std::vector<double> y;
    bool isIncreasing;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onGenerateClicked();
    void onSaveConfig();
    void onLoadConfig();
    void onPrevPage();
    void onNextPage();

private:
    QDoubleSpinBox *mSpin, *hSpin;
    QSpinBox *nSpin, *segmentsSpin, *pageSpin;
    QComboBox *distCombo;
    QPushButton *generateBtn, *saveBtn, *loadBtn, *prevBtn, *nextBtn;
    QTextEdit *output;
    QListWidget *listWidget;
    QChartView *chartView;
    QLineSeries *series;

    QDoubleSpinBox *pSpin, *geomPSpin;
    QSpinBox *trialsSpin, *geomMaxSpin, *triangularASpin, *triangularBSpin, *triangularCSpin;
    QGroupBox *binomBox, *geomBox, *triangularBox;

    std::mt19937 rng;
    std::vector<Staircase> staircases;
    int currentPage;
    int itemsPerPage;
    int totalExperiments;
    int increasingCount;

    void setupUI();
    void updateDistParamsVisibility(int index);
    std::unique_ptr<Distribution> createDistribution();
    void plotStaircase(const Staircase& s);
    void updateList();
    void loadDistParams(const QJsonObject& obj);
    void saveDistParams(QJsonObject& obj);
    bool isStrictlyIncreasing(const std::vector<double>& y);
};

#endif // MAINWINDOW_H