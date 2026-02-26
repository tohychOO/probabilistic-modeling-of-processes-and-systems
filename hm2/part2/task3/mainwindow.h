#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void runSimulationA();
    void runSimulationB();

private:
    QSpinBox *spinN;
    QSpinBox *spinR;
    QSpinBox *spinNgroup;
    QSpinBox *spinK;
    QTextEdit *output;
};

#endif