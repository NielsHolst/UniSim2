#-------------------------------------------------
#
# unisim app
#
#-------------------------------------------------
BOXES_APP_NAME = unisim

include("$$_PRO_FILE_PWD_/../../boxes_app.pri")

QT += widgets

# Our project source code
HEADERS +=  \
    main_window.h
SOURCES +=  \
    main.cpp \
    main_window.cpp
    
# Load console library
win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lconsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lconsoled
else:unix:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lconsole
else:unix:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -lconsoled
