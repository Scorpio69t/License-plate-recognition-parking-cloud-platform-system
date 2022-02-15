#-------------------------------------------------
#
# Project created by QtCreator 2021-09-06T19:37:42
#
#-------------------------------------------------


QT       += core gui
QT       += multimedia
CONFIG += c++11
s
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Car_Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/ffmpeg/include
LIBS+= -L/usr/local/ffmpeg/lib \
       -lavcodec \
       -lavdevice \
       -lavfilter \
       -lavformat \
       -lavutil \
       -lswresample \
       -lswscale

SOURCES += \
        main.cpp \
    ../../opt/EasyPR/src/core/chars_identify.cpp \
    ../../opt/EasyPR/src/core/chars_recognise.cpp \
    ../../opt/EasyPR/src/core/chars_segment.cpp \
    ../../opt/EasyPR/src/core/core_func.cpp \
    ../../opt/EasyPR/src/core/feature.cpp \
    ../../opt/EasyPR/src/core/params.cpp \
    ../../opt/EasyPR/src/core/plate_detect.cpp \
    ../../opt/EasyPR/src/core/plate_judge.cpp \
    ../../opt/EasyPR/src/core/plate_locate.cpp \
    ../../opt/EasyPR/src/core/plate_recognize.cpp \
    ../../opt/EasyPR/src/train/ann_train.cpp \
    ../../opt/EasyPR/src/train/annCh_train.cpp \
    ../../opt/EasyPR/src/train/create_data.cpp \
    ../../opt/EasyPR/src/train/svm_train.cpp \
    ../../opt/EasyPR/src/train/train.cpp \
    ../../opt/EasyPR/src/util/kv.cpp \
    ../../opt/EasyPR/src/util/program_options.cpp \
    ../../opt/EasyPR/src/util/util.cpp \
    ../../opt/EasyPR/thirdparty/LBP/helper.cpp \
    ../../opt/EasyPR/thirdparty/LBP/lbp.cpp \
    ../../opt/EasyPR/thirdparty/mser/mser2.cpp \
    ../../opt/EasyPR/thirdparty/svm/corrected_svm.cpp \
    ../../opt/EasyPR/thirdparty/textDetect/erfilter.cpp \
    ../../opt/EasyPR/thirdparty/xmlParser/xmlParser.cpp \
    Thread/thread.cpp \
    Thread/thread_in.cpp \
    Thread/thread_out.cpp \
    Tool/myitem.cpp \
    Tool/singledb.cpp \
    Tool/singledb_path.cpp \
    Widget/beginwidget.cpp \
    Widget/login.cpp \
    Widget/main_in.cpp \
    Widget/main_inmanage.cpp \
    Widget/main_interface.cpp \
    Widget/main_out.cpp \
    Widget/register.cpp \
    Widget/set.cpp \
    Thread/thread_inmanage.cpp \
    Widget/main_replay.cpp \
    Ffmpeg/ffmpeg_decode.cpp \
    Widget/main_vehicl_equery.cpp \
    Widget/main_playback.cpp \
    Sqlite/singledb_sqlite.cpp \
    Sqlite/car_model.cpp


HEADERS += \
    ../../opt/EasyPR/include/easypr/core/character.hpp \
    ../../opt/EasyPR/include/easypr/core/chars_identify.h \
    ../../opt/EasyPR/include/easypr/core/chars_recognise.h \
    ../../opt/EasyPR/include/easypr/core/chars_segment.h \
    ../../opt/EasyPR/include/easypr/core/core_func.h \
    ../../opt/EasyPR/include/easypr/core/feature.h \
    ../../opt/EasyPR/include/easypr/core/params.h \
    ../../opt/EasyPR/include/easypr/core/plate.hpp \
    ../../opt/EasyPR/include/easypr/core/plate_detect.h \
    ../../opt/EasyPR/include/easypr/core/plate_judge.h \
    ../../opt/EasyPR/include/easypr/core/plate_locate.h \
    ../../opt/EasyPR/include/easypr/core/plate_recognize.h \
    ../../opt/EasyPR/include/easypr/train/ann_train.h \
    ../../opt/EasyPR/include/easypr/train/annCh_train.h \
    ../../opt/EasyPR/include/easypr/train/create_data.h \
    ../../opt/EasyPR/include/easypr/train/svm_train.h \
    ../../opt/EasyPR/include/easypr/train/train.h \
    ../../opt/EasyPR/include/easypr/util/kv.h \
    ../../opt/EasyPR/include/easypr/util/program_options.h \
    ../../opt/EasyPR/include/easypr/util/switch.hpp \
    ../../opt/EasyPR/include/easypr/util/util.h \
    ../../opt/EasyPR/include/easypr/api.hpp \
    ../../opt/EasyPR/include/easypr/config.h \
    ../../opt/EasyPR/include/easypr/version.h \
    ../../opt/EasyPR/include/easypr.h \
    ../../opt/EasyPR/test/accuracy.hpp \
    ../../opt/EasyPR/test/chars.hpp \
    ../../opt/EasyPR/test/config.hpp \
    ../../opt/EasyPR/test/plate.hpp \
    ../../opt/EasyPR/test/result.hpp \
    ../../opt/EasyPR/thirdparty/LBP/helper.hpp \
    ../../opt/EasyPR/thirdparty/LBP/lbp.hpp \
    ../../opt/EasyPR/thirdparty/mser/mser2.hpp \
    ../../opt/EasyPR/thirdparty/svm/precomp.hpp \
    ../../opt/EasyPR/thirdparty/textDetect/erfilter.hpp \
    ../../opt/EasyPR/thirdparty/xmlParser/xmlParser.h \
    Thread/thread.h \
    Thread/thread_in.h \
    Thread/thread_out.h \
    Tool/myitem.h \
    Tool/package.h \
    Tool/singledb.h \
    Tool/singledb_path.h \
    Widget/beginwidget.h \
    Widget/login.h \
    Widget/main_in.h \
    Widget/main_inmanage.h \
    Widget/main_interface.h \
    Widget/main_out.h \
    Widget/register.h \
    Widget/set.h \
    Thread/thread_inmanage.h \
    Widget/main_replay.h \
    Ffmpeg/ffmpeg_decode.h \
    Widget/main_vehicl_equery.h \
    Widget/main_playback.h \
    Sqlite/singledb_sqlite.h \
    Sqlite/car_model.h \
    Widget/widget_manager.h





FORMS += \
    Widget/login.ui \
    Widget/register.ui \
    Widget/set.ui \
    Widget/main_interface.ui \
    Widget/main_in.ui \
    Widget/main_out.ui \
    Widget/main_inmanage.ui \
    Widget/main_replay.ui \
    Widget/main_vehicl_equery.ui \
    Widget/main_playback.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

INCLUDEPATH +=/opt/EasyPR/
INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /opt/EasyPR/include/
LIBS += /usr/local/lib/libopencv_world.so
LIBS += -L "./" -leasypr
LIBS+= /opt/EasyPR/build/libeasypr.a
LIBS+= -lsqlite3

DISTFILES += \
    model/ann.xml \
    model/ann_chinese.xml \
    model/annCh.xml \
    model/svm_hist.xml
