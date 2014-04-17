#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QStringList>

class Person : public QObject {

    Q_OBJECT

    Q_PROPERTY(QString name NOTIFY nameChanged MEMBER m_name)

    QString m_name;

    static int m_itemNumber;

public:
    Q_INVOKABLE Person(QObject * parent = 0);

    Q_INVOKABLE Person(QString name, QObject * parent = 0);

    Q_SIGNAL void nameChanged(const QString &);

    Q_INVOKABLE void setRandomName();
};

#endif // PERSON_H
