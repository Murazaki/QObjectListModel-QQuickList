#include "person.h"

int Person::m_itemNumber = 0;

Person::Person(QObject * parent) : QObject(parent) {
    setRandomName();
}

Person::Person(QString name, QObject * parent) :
    QObject(parent), m_name(name) {

}

void Person::setRandomName() {
    static const QString names = "Badger,Shopkeeper,Pepperpots,Gumbys,Colonel";
    static const QStringList nameList = names.split(',');
    QString newName = QString::number(m_itemNumber) +" - "+ nameList.at(qrand() % nameList.length());
    if (newName != m_name) {
        m_name = newName;
        emit nameChanged(m_name);
    }
    m_itemNumber++;
}
