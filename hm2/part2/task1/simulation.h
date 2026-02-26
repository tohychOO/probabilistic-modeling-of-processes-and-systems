#ifndef SIMULATION_H
#define SIMULATION_H

#include <QVector>
#include <QSet>
#include <random>

enum State { Healthy, Infected, Recovered };

struct SimPerson {
    QString name;
    State state;
    QVector<int> friends;
};

class Simulation {
private:
    QVector<SimPerson> people;
    std::mt19937 gen;
    double p_infect;
    double p_recover;

public:
    Simulation() : gen(std::time(nullptr)) {}

    void setData(const QVector<SimPerson>& data) {
        people = data;
    }

    void setParams(double p1, double p2) {
        p_infect = p1;
        p_recover = p2;
    }

    void run() {
        // сначала все здоровы
        for (auto& p : people) {
            p.state = Healthy;
        }

        // случайный первый заражённый
        std::uniform_int_distribution<int> randPerson(0, people.size() - 1);
        int first = randPerson(gen);
        people[first].state = Infected;

        std::bernoulli_distribution infectDist(p_infect);
        std::bernoulli_distribution recoverDist(p_recover);

        bool changed;
        do {
            changed = false;

            // 1. Лечение
            for (auto& p : people) {
                if (p.state == Infected && recoverDist(gen)) {
                    p.state = Recovered;
                    changed = true;
                }
            }

            // 2. Заражение
            QVector<int> newInfected;
            for (int i = 0; i < people.size(); ++i) {
                if (people[i].state == Infected) {
                    for (int fIdx : people[i].friends) {
                        if (people[fIdx].state == Healthy && infectDist(gen)) {
                            newInfected.append(fIdx);
                        }
                    }
                }
            }

            for (int idx : newInfected) {
                if (people[idx].state == Healthy) {
                    people[idx].state = Infected;
                    changed = true;
                }
            }

        } while (changed);
    }

    QVector<SimPerson> getPeople() const {
        return people;
    }
};

#endif // SIMULATION_H