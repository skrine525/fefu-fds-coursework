QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addappointmentdialog.cpp \
    adddoctordialog.cpp \
    addpatientdialog.cpp \
    appointmentsdebugwidget.cpp \
    doctorsdebugwidget.cpp \
    hashtablecapacitydialog.cpp \
    main.cpp \
    mainwindow.cpp \
    patientsdebugwidget.cpp \
    searchappointmentdialog.cpp \
    searchdoctordialog.cpp \
    searchpatientdialog.cpp \
    table1.cpp \
    table2.cpp \
    table3.cpp

HEADERS += \
    addappointmentdialog.h \
    adddoctordialog.h \
    addpatientdialog.h \
    appointmentsdebugwidget.h \
    doctorsdebugwidget.h \
    hashtablecapacitydialog.h \
    mainwindow.h \
    patientsdebugwidget.h \
    searchappointmentdialog.h \
    searchdoctordialog.h \
    searchpatientdialog.h \
    table1.h \
    table2.h \
    table3.h

FORMS += \
    addappointmentdialog.ui \
    adddoctordialog.ui \
    addpatientdialog.ui \
    appointmentsdebugwidget.ui \
    doctorsdebugwidget.ui \
    hashtablecapacitydialog.ui \
    mainwindow.ui \
    patientsdebugwidget.ui \
    searchappointmentdialog.ui \
    searchdoctordialog.ui \
    searchpatientdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
