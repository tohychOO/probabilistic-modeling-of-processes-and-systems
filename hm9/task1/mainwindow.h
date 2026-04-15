#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QtCharts/QChartView>
#include "discrete_random_variable.h"

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCreateVariable();      // создать СВ из таблицы
    void onSaveToFile();          // сохранить в файл
    void onLoadFromFile();        // загрузить из файла
    void onMultiplyScalar();      // умножить на скаляр
    void onAddVariable();         // сложить с другой СВ
    void onMultiplyVariable();    // умножить на другую СВ
    void onUpdatePlots();         // обновить все графики

private:
    void setupUI();               // создать интерфейс
    void updateInfo();            // обновить таблицу и статистику
    void plotProbabilityMass();   // полилайн
    void plotCumulative();        // функция распределения
    
    DiscreteRandomVariable m_currentRV;
    
    QTableWidget* m_inputTable;   // таблица для ввода значений
    QTextEdit* m_statsText;       // вывод статистики
    QLineEdit* m_scalarEdit;      // поле для ввода скаляра
    QLabel* m_statusLabel;        // строка статуса
    
    QTabWidget* m_tabWidget;      // ыкладки для графиков
    QChartView* m_pmfChartView;   // график полилайна
    QChartView* m_cdfChartView;   // график функции распределения
    
    QPushButton* m_createBtn;
    QPushButton* m_saveBtn;
    QPushButton* m_loadBtn;
    QPushButton* m_mulScalarBtn;
    QPushButton* m_addBtn;
    QPushButton* m_mulBtn;
};

#endif // MAINWINDOW_H