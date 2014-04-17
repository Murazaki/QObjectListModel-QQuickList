#include "qobjectlistmodel.h"

QObjectListModel::QObjectListModel(QObject * parent) :
    QAbstractListModel(parent),
    m_factory([this](){
        return QObject::staticMetaObject.newInstance(Q_ARG(QObject*, this));
    }),
    m_tracking(true)
{}

QObjectListModel::QObjectListModel(const QMetaObject * mo, QObject * parent) :
    QAbstractListModel(parent),
    m_factory([mo, this](){
        return mo->newInstance(Q_ARG(QObject*, this));
    }),
    m_tracking(true)
{}

QObjectListModel::QObjectListModel(const std::function<QObject*()> & factory,
                QObject * parent) :
    QAbstractListModel(parent), m_factory(factory), m_tracking(false)
{}

QObjectListModel::~QObjectListModel() {
//    qDeleteAll(m_data);
}

bool QObjectListModel::elementChangeTracking() const { return m_tracking; }

void QObjectListModel::setElementChangeTracking(bool tracking) {
    if (m_tracking == tracking) return;
    foreach (QObject* obj, m_data) updateTracking(obj);
    emit elementChangeTrackingChanged(m_tracking = tracking);
}

int QObjectListModel::rowCount(const QModelIndex &) const {
    return m_data.count();
}

QVariant QObjectListModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return QVariant::fromValue(m_data.at(index.row()));
    }
    return QVariant();
}

bool QObjectListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    Q_UNUSED(role);
    QObject* object = value.value<QObject*>();
    if (!object) return false;
    if (object == m_data.at(index.row())) return true;
    m_data[index.row()] = object;
    emit dataChanged(index, index, QVector<int>(1, role));
    return true;
}

bool QObjectListModel::insert(QObject * const &item, int row) {
    if(row == -1)
        row = m_data.count();

    beginInsertRows(QModelIndex(), row, row);
    Q_ASSERT(item);
    m_data.insert(row,item);
    updateTracking(item);
    QQmlEngine::setObjectOwnership(item, QQmlEngine::CppOwnership);
    endInsertRows();

    return true;
}

bool QObjectListModel::removeAll(QObject * const &item) {
    int i;
    bool out = false;

    while(m_data.contains(item)) {
        i = m_data.indexOf(item);
        beginRemoveRows(QModelIndex(), i, i);
        m_data.takeAt(i);
        endRemoveRows();

        out = true;
    }

    return out;
}

bool QObjectListModel::removeOne(QObject * const &item) {
    int i;
    bool out = false;

    if(m_data.contains(item)) {
        i = m_data.indexOf(item);
        beginRemoveRows(QModelIndex(), i, i);
        m_data.takeAt(i);
        endRemoveRows();

        out = true;
    }

    return out;
}

bool QObjectListModel::removeAt(int row) {
    if((m_data.count() > row) && (row >= 0)) {
        beginRemoveRows(QModelIndex(), row, row);
        m_data.takeAt(row);
        endRemoveRows();

        return true;
    }

    return false;
}

bool QObjectListModel::insertRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
//    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = row; i < row + count; ++ i) {
        beginInsertRows(QModelIndex(), row + i, row + i);
        QObject * object = m_factory();
        Q_ASSERT(object);
        m_data.insert(i, object);
        updateTracking(object);
        QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
        endInsertRows();
    }
//    endInsertRows();
    return true;
}

bool QObjectListModel::removeRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
//    beginRemoveRows(QModelIndex(), row, row + count - 1);
    while (count--) {
        beginRemoveRows(QModelIndex(), row, row);
        m_data.takeAt(row);
        endRemoveRows();
    }
//    endRemoveRows();
    return true;
}

void QObjectListModel::timerEvent(QTimerEvent * ev) {
    if (ev->timerId() != m_notifyTimer.timerId()) return;
    emit dataChanged(index(m_notifyIndexes.begin().key()),
                     index((m_notifyIndexes.end()-1).key()),
                     QVector<int>(1, Qt::DisplayRole));
    m_notifyTimer.stop();
    m_notifyIndexes.clear();
}

void QObjectListModel::updateTracking(QObject* obj) {
    const int nIndex = metaObject()->indexOfSlot("propertyNotification()");
    QMetaMethod const nSlot = metaObject()->method(nIndex);
    const int props = obj->metaObject()->propertyCount();
    if (m_tracking) for (int i = 0; i < props; ++i) {
        const QMetaProperty prop = obj->metaObject()->property(i);
        if (prop.hasNotifySignal()) connect(obj, prop.notifySignal(), this, nSlot);
    } else {
        disconnect(obj, 0, this, 0);
    }
}

void QObjectListModel::propertyNotification() {
    int i = m_data.indexOf(sender());
    if (i >= 0) m_notifyIndexes.insert(i, 0);
    // All of the notifications will be sent as a single signal from the event loop.
    if (!m_notifyTimer.isActive()) m_notifyTimer.start(0, this);
}

