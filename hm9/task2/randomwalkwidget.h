#ifndef RANDOMWALKWIDGET_H
#define RANDOMWALKWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <vector>
#include "discrete_random_variable.h"

class RandomWalkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RandomWalkWidget(QWidget *parent = nullptr);
    
    void setStartPosition(double pos);
    void setSteps(int steps);
    void setStepDistribution(const DiscreteRandomVariable& dist);
    
    void startWalk();
    void stopWalk();
    void reset();
    
    DiscreteRandomVariable getResultDistribution() const;
    
    bool isRunning() const { return m_isRunning; }

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onTimerTick();

private:
    void calculateResultDistribution();  // вычисляет ДСВ конечных положений
    void performSingleStep();            // выполняет один шаг
    
    // параметры блуждания
    double m_startPosition;
    int m_totalSteps;
    DiscreteRandomVariable m_stepDistribution;
    
    // состояние блуждания
    double m_currentPosition;
    int m_currentStep;
    double m_currentTargetPosition;
    QTimer* m_animationTimer;
    QTimer* m_stepTimer;
    bool m_isRunning;
    
    // результаты
    std::vector<double> m_positions;
    std::map<double, int> m_positionCount;
};

#endif // RANDOMWALKWIDGET_H