QT += core network serialport
QT -= gui

TARGET = TargetTrackingGPU
CONFIG += console link_pkgconfig
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    globalsettings.cpp \
    serialPortService/dataparser4payload.cpp \
    serialPortService/dataparserbase.cpp \
    serialPortService/serviceserialpayload.cpp \
    udpService/serviceudpvideo.cpp \
    datamanager.cpp \
    videoProcService/videoencodeh264.cpp \
    serialPortService/serviceserialfirectrl.cpp \
    serialPortService/dataparser4firectrl.cpp \
    videoProcService/trackalgorithm.cpp \
    udpService/udpsendservice.cpp

HEADERS += \
    globalsettings.h \
    serialPortService/dataparser4payload.h \
    serialPortService/dataparserbase.h \
    serialPortService/serviceserialpayload.h \
    udpService/serviceudpvideo.h \
    datamanager.h \
    videoProcService/videoencodeh264.h \
    serialPortService/serviceserialfirectrl.h \
    serialPortService/dataparser4firectrl.h \
    videoProcService/trackalgorithm.h \
    algorithm/include/tracker.h \
    algorithm/include/calibrateinformationmeasure_global.h \
    algorithm/include/calibrateinformationmeasure.h \
    algorithm/include/cvfeatureextractmatch_global.h \
    algorithm/include/cvfeatureextractmatch.h \
    algorithm/include/ffttools.hpp \
    algorithm/include/fhog.hpp \
    algorithm/include/kcftracker_global.h \
    algorithm/include/kcftracker.h \
    algorithm/include/labdata.hpp \
    algorithm/include/recti_global.h \
    algorithm/include/recti.h \
    algorithm/include/recttools.hpp \
    udpService/udpsendservice.h

INCLUDEPATH += ./serialPortService ./udpService ./videoProcService ./algorithm/include

INCLUDEPATH += /usr/local/include/opencv310 /usr/local/include/ffmpeg \
                /usr/include/log4cplus

LIBS += -L/usr/local/lib/opencv310 -lopencv_core -lopencv_imgproc   -lopencv_videoio \
    -lopencv_highgui  -lopencv_video  -lopencv_features2d -lopencv_calib3d  -lopencv_imgcodecs \
    -lopencv_shape -lopencv_cudaobjdetect -lopencv_cudawarping -lopencv_cudaimgproc
LIBS += -L/usr/local/lib -lavcodec-ffmpeg -lavutil-ffmpeg
LIBS += -L../algorithm/lib -lcvFeatureExtractMatch -lKCFTracker -lCalibrateInformationMeasure

OBJECTS_DIR += ./temp
MOC_DIR += ./temp
