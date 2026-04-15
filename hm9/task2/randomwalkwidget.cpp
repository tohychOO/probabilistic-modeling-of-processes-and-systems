#include "randomwalkwidget.h"
#include <QPainter>
#include <QPen>
#include <cmath>
#include <algorithm>

RandomWalkWidget::RandomWalkWidget(QWidget *parent)
    : QWidget(parent)
    , m_startPosition(0.0)
    , m_totalSteps(0)
    , m_currentPosition(0.0)
    , m_currentStep(0)
    , m_currentTargetPosition(0.0)
    , m_isRunning(false)
{
    setMinimumHeight(200);
    setMinimumWidth(600);
    
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(20);
    
    m_stepTimer = new QTimer(this);
    m_stepTimer->setInterval(1000);
    
    connect(m_animationTimer, &QTimer::timeout, this, &RandomWalkWidget::onTimerTick);
    connect(m_stepTimer, &QTimer::timeout, this, &RandomWalkWidget::performSingleStep);
}

void RandomWalkWidget::setStartPosition(double pos)
{
    m_startPosition = pos;
    m_currentPosition = pos;
    update();
}

void RandomWalkWidget::setSteps(int steps)
{
    m_totalSteps = steps;
}

void RandomWalkWidget::setStepDistribution(const DiscreteRandomVariable& dist)
{
    m_stepDistribution = dist;
}

void RandomWalkWidget::startWalk()
{
    if (m_stepDistribution.isEmpty()) {
        return;
    }
    
    if (m_totalSteps <= 0) {
        return;
    }
    
    m_currentPosition = m_startPosition;
    m_currentStep = 0;
    m_positions.clear();
    m_positionCount.clear();
    m_positions.push_back(m_currentPosition);
    
    // Запуск
    m_isRunning = true;
    m_animationTimer->start();
    m_stepTimer->start();
    
    update();
}

void RandomWalkWidget::stopWalk()
{
    m_isRunning = false;
    m_animationTimer->stop();
    m_stepTimer->stop();
    update();
}

void RandomWalkWidget::reset()
{
    stopWalk();
    m_currentPosition = m_startPosition;
    m_currentStep = 0;
    m_positions.clear();
    m_positionCount.clear();
    m_positions.push_back(m_currentPosition);
    update();
}

void RandomWalkWidget::performSingleStep()
{
    if (m_currentStep >= m_totalSteps) {
        stopWalk();
        calculateResultDistribution();
        return;
    }
    
    auto table = m_stepDistribution.getTable();
    if (table.empty()) return;
    
    double randVal = static_cast<double>(rand()) / RAND_MAX;
    double cumProb = 0.0;
    double step = 0.0;
    
    for (const auto& [val, prob] : table) {
        cumProb += prob;
        if (randVal <= cumProb) {
            step = val;
            break;
        }
    }
    
    m_currentTargetPosition = m_currentPosition + step;
    
    m_currentStep++;
}

void RandomWalkWidget::onTimerTick()
{
    if (!m_isRunning) return;
    
    double diff = m_currentTargetPosition - m_currentPosition;
    if (std::abs(diff) > 0.01) {
        double speed = std::abs(diff) * 0.05;
        if (speed < 0.01) speed = 0.01;
        
        if (diff > 0) {
            m_currentPosition += speed;
            if (m_currentPosition > m_currentTargetPosition) {
                m_currentPosition = m_currentTargetPosition;
            }
        } else {
            m_currentPosition -= speed;
            if (m_currentPosition < m_currentTargetPosition) {
                m_currentPosition = m_currentTargetPosition;
            }
        }
    }
    
    if (std::abs(m_currentPosition - m_currentTargetPosition) < 0.001) {
        if (m_currentStep > 0 && m_positions.back() != m_currentPosition) {
            m_positions.push_back(m_currentPosition);
            m_positionCount[m_currentPosition]++;
        }
    }
    
    update();
}

void RandomWalkWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.setPen(QPen(Qt::black, 2));
    int yCenter = height() / 2;
    painter.drawLine(30, yCenter, width() - 30, yCenter);
    
    painter.drawLine(width() - 30, yCenter, width() - 40, yCenter - 5);
    painter.drawLine(width() - 30, yCenter, width() - 40, yCenter + 5);
    
    painter.drawText(width() - 25, yCenter - 5, "x");
    
    double minX = m_startPosition;
    double maxX = m_startPosition;
    for (double pos : m_positions) {
        minX = std::min(minX, pos);
        maxX = std::max(maxX, pos);
    }
    
    double range = maxX - minX;
    if (range < 1.0) range = 1.0;
    minX -= range * 0.2;
    maxX += range * 0.2;
    
    auto toScreenX = [&](double x) -> int {
        return 30 + static_cast<int>((x - minX) / (maxX - minX) * (width() - 60));
    };
    
    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    for (int i = static_cast<int>(minX); i <= static_cast<int>(maxX); i++) {
        int x = toScreenX(i);
        painter.drawLine(x, yCenter - 10, x, yCenter + 10);
        painter.drawText(x - 5, yCenter + 20, QString::number(i));
    }
    
    int xPos = toScreenX(m_currentPosition);
    painter.setBrush(Qt::red);
    painter.setPen(QPen(Qt::red, 2));
    painter.drawEllipse(xPos - 8, yCenter - 8, 16, 16);
    
    painter.setPen(Qt::black);
    painter.drawText(10, 20, QString("Шаг: %1 / %2").arg(m_currentStep).arg(m_totalSteps));
    painter.drawText(10, 40, QString("Позиция: %1").arg(m_currentPosition, 0, 'f', 3));
}

void RandomWalkWidget::calculateResultDistribution()
{   
    if (m_positions.empty()) return;
    
    double finalPos = m_positions.back();
    
    DiscreteRandomVariable result = m_stepDistribution;
    for (int i = 1; i < m_totalSteps; i++) {
        result = result + m_stepDistribution;
    }
    
    auto table = result.getTable();
    std::vector<std::pair<double, double>> shiftedTable;
    for (const auto& [val, prob] : table) {
        shiftedTable.emplace_back(val + m_startPosition, prob);
    }
    
    result = DiscreteRandomVariable(shiftedTable);
}