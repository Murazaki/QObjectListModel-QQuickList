# Add more folders to ship with the application, here
folder_01.source = qml/ObjectListModel
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    qobjectlistmodel.cpp \
    person.cpp \
    testobject.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    main.qrc

HEADERS += \
    qobjectlistmodel.h \
    person.h \
    testobject.h \
    qquicklist.h

CONFIG += c++11
