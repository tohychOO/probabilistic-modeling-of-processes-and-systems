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
    void onCreateVariable();      // Создать СВ из таблицы
    void onSaveToFile();          // Сохранить в файл
    void onLoadFromFile();        // Загрузить из файла
    void onMultiplyScalar();      // Умножить на скаляр
    void onAddVariable();         // Сложить с другой СВ
    void onMultiplyVariable();    // Умножить на другую СВ
    void onUpdatePlots();         // Обновить все графики

private:
    void setupUI();               // Создать интерфейс
    void updateInfo();            // Обновить таблицу и статистику
    void plotProbabilityMass();   // Полилайн (график вероятностей)
    void plotCumulative();        // Функция распределения
    
    // Данные
    DiscreteRandomVariable m_currentRV;
    
    // UI элементы
    QTableWidget* m_inputTable;   // Таблица для ввода значений
    QTextEdit* m_statsText;       // Вывод статистики
    QLineEdit* m_scalarEdit;      // Поле для ввода скаляра
    QLabel* m_statusLabel;        // Строка статуса
    
    QTabWidget* m_tabWidget;      // Вкладки для графиков
    QChartView* m_pmfChartView;   // График полилайна
    QChartView* m_cdfChartView;   // График функции распределения
    
    QPushButton* m_createBtn;
    QPushButton* m_saveBtn;
    QPushButton* m_loadBtn;
    QPushButton* m_mulScalarBtn;
    QPushButton* m_addBtn;
    QPushButton* m_mulBtn;
};

#endif // MAINWINDOW_H