# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

# symbian:TARGET.UID3 = 0xE0BDE452

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
# symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

QT += network maemo5 webkit dbus

SOURCES += main.cpp mainwindow.cpp \
    linnowplayingform.cpp \
    linvideodisplayform.cpp \
    linnewsfeedwidgetitem.cpp \
    linrssparser.cpp \
    lingstdatadialog.cpp \
    lindocumentationform.cpp \
    linaboutform.cpp \
    linfilterdialog.cpp \
    linseekdialog.cpp \
    linhtmldisplayform.cpp \
    linflickabletabbar.cpp \
    linmaemo5theme.cpp \
    lindbus.cpp \
    linpreferencesform.cpp \
    lincollectiondialog.cpp \
    lincollectionwidgetitem.cpp \
    lintormanager.cpp \
    linfeedsource.cpp \
    lintoritems.cpp \
    linimageloader.cpp \
    linnativedisplayform.cpp \
    lintordisplayform.cpp \
    linauthenticationdialog.cpp \
    linnativedelegate.cpp \
    linfeedselectordialog.cpp \
    linatomparser.cpp \
    linfeedmanagerform.cpp \
    linwidgetuiform.cpp \
    linwidgetsettingsdialog.cpp \
    qmaemo5homescreenadaptor.cpp
HEADERS += mainwindow.h \
    linnowplayingform.h \
    linnewsfeedwidgetitem.h \
    linvideodisplayform.h \
    linrssparser.h \
    lingstdatadialog.h \
    lindocumentationform.h \
    linaboutform.h \
    linfilterdialog.h \
    linseekdialog.h \
    linhtmldisplayform.h \
    qwebviewselectionsuppressor.h \
    linflickabletabbar.h \
    linfilteritems.h \
    linmaemo5theme.h \
    lindbus.h \
    linpreferencesform.h \
    lincollectiondialog.h \
    lincollectionwidgetitem.h \
    lintormanager.h \
    linfeedsource.h \
    lintoritems.h \
    linimageloader.h \
    linnativedisplayform.h \
    lintordisplayform.h \
    linauthenticationdialog.h \
    linnativedelegate.h \
    linfeedselectordialog.h \
    linatomparser.h \
    linfeedmanagerform.h \
    linwidgetuiform.h \
    linwidgetsettingsdialog.h \
    qmaemo5homescreenadaptor.h
FORMS += mainwindow.ui \
    linnowplayingform.ui \
    linvideodisplayform.ui \
    lingstdatadialog.ui \
    lindocumentationform.ui \
    linaboutform.ui \
    linfilterdialog.ui \
    linseekdialog.ui \
    linhtmldisplayform.ui \
    linpreferencesform.ui \
    lincollectiondialog.ui \
    linauthenticationdialog.ui \
    linfeedselectordialog.ui \
    linfeedmanagerform.ui \
    linwidgetuiform.ui \
    linwidgetsettingsdialog.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    newsfeeds.xml \
    doc/documentation.html \
    doc/about.html \
    README.md \
    LICENSE.md \
    COPYING

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-0.10 gstreamer-interfaces-0.10 QJson
}

RESOURCES += \
    linresource.qrc

# Extra files needed to create a homescreen widget:
desktop.path = /usr/share/applications/hildon-home
desktop.files = linguinewidget.desktop

INSTALLS += desktop
