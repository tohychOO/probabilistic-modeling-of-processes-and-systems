#ifndef GRAPHLOADER_H
#define GRAPHLOADER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QTextStream>
#include <QFile>

struct Person {
    QString name;
    QVector<int> friends;
};

class GraphLoader {
public:
    static bool load(const QString& filename, QVector<Person>& people, QMap<QString, int>& nameToIndex) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

        QTextStream in(&file);
        int n;
        in >> n;
        people.resize(n);

        for (int i = 0; i < n; ++i) {
            in >> people[i].name;
            nameToIndex[people[i].name] = i;
        }

        int m;
        in >> m;
        for (int i = 0; i < m; ++i) {
            QString name1, name2;
            in >> name1 >> name2;
            int idx1 = nameToIndex[name1];
            int idx2 = nameToIndex[name2];
            people[idx1].friends.append(idx2);
            people[idx2].friends.append(idx1);
        }
        return true;
    }
};

#endif // GRAPHLOADER_H