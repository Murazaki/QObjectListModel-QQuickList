#ifndef QOBJECTLISTMODEL_H
#define QOBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QtQml>
#include <QSet>
#include <QBasicTimer>
#include <functional>

template<typename T> class QQuickList;

class QObjectListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(QObjectListModel)

    //! Whether changes to underlying objects are exposed via `dataChanged` signals
    Q_PROPERTY(bool elementChangeTracking READ elementChangeTracking WRITE setElementChangeTracking NOTIFY elementChangeTrackingChanged)

public:
    //! default constructor
    QObjectListModel(QObject * parent = NULL);

    //! A model that creates instances via a given metaobject
    QObjectListModel(const QMetaObject * mo, QObject * parent = NULL);
    //! A model that creates instances using a factory function
    QObjectListModel(const std::function<QObject*()> & factory, QObject * parent = NULL);

    //! A factory to get a model of QList<T*>
    template<class T> static QObjectListModel * create(QList<T*> list, QObject * parent = NULL) {
        T * assertObject = new T();
        Q_ASSERT_X(qobject_cast<QObject *>(assertObject),"QQuickList<T>::QQuickList","Typename T does not inherit from QObject.");
        delete assertObject;

        QObjectListModel * newModel = new QObjectListModel(&T::staticMetaObject, parent);

        newModel->m_data.clear();

        foreach(T * elt, list) {
            newModel->m_data << elt;
        }

        return newModel;
    }

    ~QObjectListModel();

    bool elementChangeTracking() const;
    void setElementChangeTracking(bool tracking);
    Q_SIGNAL void elementChangeTrackingChanged(bool);
    int rowCount(const QModelIndex &) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    bool insert(QObject * const &item, int row = -1);
    template<class T> bool insert(QList <T*> const &items, int row = -1) {

        T * assertObject = new T();
        Q_ASSERT_X(qobject_cast<QObject *>(assertObject),"QQuickList<T>::QQuickList","Typename T does not inherit from QObject.");
        delete assertObject;

        if(row == -1)
            row = m_data.count();

        if(!items.empty()) {
            beginInsertRows(QModelIndex(), row, row + items.count() - 1);
            for (int i = 0; i < items.count(); ++ i) {
                Q_ASSERT(items[i]);
                m_data.insert(i + row,items[i]);
                updateTracking(items[i]);
                QQmlEngine::setObjectOwnership(items[i], QQmlEngine::CppOwnership);
            }
            endInsertRows();
        }

        return true;
    }
    bool removeAll(QObject * const &item);
    bool removeOne(QObject * const &item);
    bool removeAt(int row);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

protected:
    //! Emits the notifications of changes done on the underlying QObject properties
    void timerEvent(QTimerEvent * ev);

private:
    //! Updates the property tracking connections on given object.
    void updateTracking(QObject* obj);
    //! Receives property notification changes
    Q_SLOT void propertyNotification();

    QObjectList m_data;
    std::function<QObject*()> m_factory;
    bool m_tracking;
    QBasicTimer m_notifyTimer;
    QMap<int, char> m_notifyIndexes;
};

template<typename T> class QQuickList : public QList<T*> {
    QObjectListModel * m_model;

public:
    inline QQuickList() : m_model(new QObjectListModel(&T::staticMetaObject)) {
        T * assertObject = new T();
        Q_ASSERT_X(qobject_cast<QObject *>(assertObject),"QQuickList<T>::QQuickList","Typename T does not inherit from QObject.");
        delete assertObject;
    }

    explicit inline QQuickList(const QList<T*> &list) :
        QList<T*>(list),
        m_model(QObjectListModel::create(list))
    {
        T * assertObject = new T();
        Q_ASSERT_X(qobject_cast<QObject *>(assertObject),"QQuickList<T>::QQuickList","Typename T does not inherit from QObject.");
        delete assertObject;
    }

    inline ~QQuickList() {
        delete m_model;
    }

    inline QObjectListModel * getModel() { return m_model; }

    inline QQuickList<T> &operator=(const QList<T*> &l) { return QQuickList(QList::operator=(l)); }
#ifdef Q_COMPILER_RVALUE_REFS
    inline QQuickList(QList<T*> &&other) : QList(other), m_model(QObjectListModel::create(other))
    {
        T * assertObject = new T();
        Q_ASSERT_X(qobject_cast<QObject *>(assertObject),"QQuickList<T>::QQuickList","Typename T does not inherit from QObject.");
        delete assertObject;
    }
    inline QQuickList &operator=(QList<T*> &&other) { return *(new QQuickList(QList::operator=(other))); }
    inline QQuickList &operator=(QQuickList<T> &&other) { return QQuickList::operator=(QList<T*>(other)); }
#endif

#ifdef Q_COMPILER_INITIALIZER_LISTS
    inline QQuickList(std::initializer_list<T> args)
        : QList(args)
    { }
#endif

    inline void clear() {
        QList::clear();
        m_model->clear();
    }

    inline void append(T *const & t) { QList::append(t); m_model->insert(t); }
    inline void append(const QList<T*> &t) { QList::append(t); m_model->insert(t); }
    inline void prepend(T *const &t) { QList::prepend(t); m_model->insert(t,0); }
    inline void insert(int i, T *const &t) { QList::insert(i,t); m_model->insert(t,i); }
    inline void replace(int i, T *const &t) { QList::replace(i,t); m_model->insert(t,i); m_model->removeAt(i + 1); }
    inline void removeAt(int i) { QList::removeAt(i); m_model->removeAt(i); }
    inline int removeAll(T *const &t) { m_model->removeAll(t); return QList::removeAll(t); }
    inline bool removeOne(T *const &t) { m_model->removeOne(t); return QList::removeOne(t); }
    inline T * takeAt(int i) { m_model->removeAt(i); return QList::takeAt(i); }
    inline T * takeFirst() { m_model->removeAt(0); return QList::takeFirst(); }
    inline T * takeLast() { m_model->removeAt(count() - 1); return QList::takeLast(); }
    inline void move(int from, int to) { QList::move(from,to); m_model->removeAt(from); m_model->insert(this->at(to),to); }
    inline void swap(int i, int j) { QList::swap(i,j); m_model->removeAt(i); m_model->insert(this->at(i),i); m_model->removeAt(j); m_model->insert(this->at(j),j); }

    inline iterator insert(iterator before, T *const &t) {  m_model->insert(t,before-begin()); return QList::insert(before,t); }
    inline iterator erase(iterator pos) {  m_model->removeAt(pos - begin()); return QList::erase(pos); }
    inline iterator erase(iterator first, iterator last) {  m_model->removeRows(first - begin(),last - first); return QList::erase(first,last); }

    inline void removeFirst() { QList::removeFirst(); m_model->removeAt(0); }
    inline void removeLast() { QList::removeLast(); m_model->removeAt(count() - 1); }

    // stl compatibility
    inline void push_back(T *const &t) { append(t); }
    inline void push_front(T *const &t) { prepend(t); }
    inline void pop_front() { removeFirst(); }
    inline void pop_back() { removeLast(); }

    // comfort
    inline QQuickList<T> &operator+=(const QList<T*> &l)
    { m_model->insert(l); QList::operator+=(l); return *this; }
    inline QQuickList<T> &operator+=(const QQuickList<T> &l)
    { return operator+=(QList<T*>(l)); }
    inline QQuickList<T> &operator+(const QList<T*> &l) const // You should not add elements like this (for fluidity purpose)
    { QQuickList<T> * n = new QQuickList<T>(); n->append(*this); n->append(l); return *n; }
//    inline QQuickList<T> operator+(const QQuickList<T> &l)
//    { return QQuickList::operator+(QList<T*>(l)); }
    inline QQuickList<T> &operator+=(T *const &t)
    { append(t); return *this; }
    inline QQuickList<T> &operator<< (T *const &t)
    { append(t); return *this; }
    inline QQuickList<T> &operator<<(const QList<T*> &l)
    { *this += l; return *this; }
    inline QQuickList<T> &operator<<(const QQuickList<T> &l)
    { return operator<<(l); }

    inline operator QList<T*> ()
    { return (QList)*this; }
};

#endif // QOBJECTMODEL_H
