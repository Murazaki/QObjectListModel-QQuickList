QObjectListModel-QQuickList
===========================

This is a fork of the code you can find in this page : http://stackoverflow.com/questions/19324641/qml-2-0-tableview-with-qabstractitemmodel-and-context-menu .

Why ?
-----

I was annoyed by the lack of integration in QML for QList & QQmlListProperty elements, and the fact that you need to subclass QAbstractListModel for every QObject I needed to show in lists.

How it works
------------

I modified the QObjectListModel a bit, but the big feature here is the QQuickList class : it's a template subclass of QList<T*> that takes QObject pointers as list elements. It integrates a QObjectListModel, that is updated when the QList is modified. You can then pass QObjectListModel to QML to show the changes made in the list.

Example
-------

By running the code, you can see an example of the possibilities provided by the QQuickList.

Feedback
--------

If you have any feedback, please don't hesitate to contact me.