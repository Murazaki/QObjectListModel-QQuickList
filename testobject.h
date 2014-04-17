#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QObject>
#include "qobjectlistmodel.h"
#include "person.h"

class TestObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObjectListModel * model READ model NOTIFY modelChanged)

    Q_PROPERTY(QQmlListProperty<Person> listProperty READ listProperty NOTIFY modelChanged)

public:
    explicit TestObject(QObject *parent = 0);

    QObjectListModel * model() { return m_personlist.getModel(); }

    QQmlListProperty<Person> listProperty() { return QQmlListProperty<Person>(this,m_personlist); }

    Q_INVOKABLE void appendOne() {
        m_personlist.append(new Person());

        emit modelChanged(model());
    }

    Q_INVOKABLE void appendList(int num) {
        QQuickList<Person> test;

        for(int i =0; i< num; i++)
            test << new Person();

        m_personlist.append(test);

        emit modelChanged(model());
    }

    Q_INVOKABLE void prependOne() {
        m_personlist.prepend(new Person());

        emit modelChanged(model());
    }

    Q_INVOKABLE void insertOne() {
        m_personlist.insert(qrand()*(m_personlist.count()-1)/RAND_MAX,new Person());

        emit modelChanged(model());
    }

    Q_INVOKABLE void replace() {
        m_personlist.replace(qrand()*(m_personlist.count()-1)/RAND_MAX,new Person());

        emit modelChanged(model());
    }

    Q_INVOKABLE void removeAt() {
        m_personlist.removeAt(qrand()*(m_personlist.count()-1)/RAND_MAX);

        emit modelChanged(model());
    }

    Q_INVOKABLE void removeAll() {
        m_personlist.removeAll(m_personlist.at(qrand()*(m_personlist.count()-1)/RAND_MAX));

        emit modelChanged(model());
    }

    Q_INVOKABLE void removeOne() {
        m_personlist.removeOne(m_personlist.at(qrand()*(m_personlist.count()-1)/RAND_MAX));

        emit modelChanged(model());
    }

    Q_INVOKABLE void takeAt() {
        m_personlist.takeAt(qrand()*(m_personlist.count()-1)/RAND_MAX);

        emit modelChanged(model());
    }

    Q_INVOKABLE void takeFirst() {
        m_personlist.takeFirst();

        emit modelChanged(model());
    }

    Q_INVOKABLE void takeLast() {
        m_personlist.takeLast();

        emit modelChanged(model());
    }

    Q_INVOKABLE void move() {
        m_personlist.move(qrand()*(m_personlist.count()-1)/RAND_MAX,qrand()*(m_personlist.count()-1)/RAND_MAX);

        emit modelChanged(model());
    }

    Q_INVOKABLE void swap() {
        m_personlist.swap(qrand()*(m_personlist.count()-1)/RAND_MAX,qrand()*(m_personlist.count()-1)/RAND_MAX);

        emit modelChanged(model());
    }

    Q_INVOKABLE void insert() {
        m_personlist.insert(m_personlist.begin()+(qrand()*(m_personlist.count()-1)/RAND_MAX),new Person());

        emit modelChanged(model());
    }

    Q_INVOKABLE void eraseOne() {
        m_personlist.erase(m_personlist.begin()+(qrand()*(m_personlist.count()-1)/RAND_MAX));

        emit modelChanged(model());
    }

    Q_INVOKABLE void eraseList() {
        int firstIndex = qrand()*(m_personlist.count()-1)/RAND_MAX;
        QList<Person*>::iterator first = m_personlist.begin() + firstIndex;
        QList<Person*>::iterator last = first + (qrand()*(m_personlist.count()-firstIndex)/RAND_MAX);

        m_personlist.erase(first,last);

        emit modelChanged(model());
    }

    Q_INVOKABLE void removeFirst() {
        m_personlist.removeFirst();

        emit modelChanged(model());
    }

    Q_INVOKABLE void removeLast() {
        m_personlist.removeLast();

        emit modelChanged(model());
    }

    Q_INVOKABLE void appendPlusEqual(int num) {
        QQuickList<Person> test;

        for(int i =0; i< num; i++)
            test << new Person();

        m_personlist += test;

        emit modelChanged(model());
    }

    Q_INVOKABLE void appendPlus(int num) {
        QQuickList<Person> test;

        for(int i =0; i< num; i++)
            test << new Person();

        m_personlist = m_personlist + test;

        emit modelChanged(model());
    }

signals:

    void modelChanged(QObjectListModel *);

public slots:

private:
    QQuickList<Person> m_personlist;
};

#endif // TESTOBJECT_H
