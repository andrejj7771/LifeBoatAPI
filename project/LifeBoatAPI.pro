TEMPLATE = lib
CONFIG += c++11

DEFINES += LIFEBOAT_API_EXPORTS

TARGET = LifeBoatAPI
DESTDIR += $$PWD/../bin

!contains(QMAKE_TARGET.arch, x86_64) {
    TARGET = $$join(TARGET, "", "", "_x86")
} else {
    TARGET = $$join(TARGET, "", "", "_x64")
}

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET, "", "", "d")
}

INCLUDEPATH += $$PWD/../include

SOURCES += $$PWD/../src/GameObjects/GameObject.cpp
SOURCES += $$PWD/../src/GameObjects/Character.cpp
SOURCES += $$PWD/../src/GameObjects/NavigationCard.cpp
SOURCES += $$PWD/../src/GameObjects/ProvisionCard.cpp

SOURCES += $$PWD/../src/Controllers/CardDistributionController.cpp
SOURCES += $$PWD/../src/Controllers/IterationTotalController.cpp
SOURCES += $$PWD/../src/Controllers/ActionController.cpp
SOURCES += $$PWD/../src/Controllers/RowingController.cpp
SOURCES += $$PWD/../src/Controllers/FightController.cpp
SOURCES += $$PWD/../src/Controllers/LogicController.cpp
SOURCES += $$PWD/../src/Controllers/StartController.cpp
SOURCES += $$PWD/../src/Controllers/TotalController.cpp
SOURCES += $$PWD/../src/Controllers/CardController.cpp

HEADERS += $$PWD/../include/LifeboatAPI_global.h

HEADERS += $$PWD/../include/Utils/Callback.h

HEADERS += $$PWD/../include/GameObjects/GameObject.h
HEADERS += $$PWD/../include/GameObjects/Character.h
HEADERS += $$PWD/../include/GameObjects/NavigationCard.h
HEADERS += $$PWD/../include/GameObjects/ProvisionCard.h

HEADERS += $$PWD/../include/Controllers/CardDistributionController.h
HEADERS += $$PWD/../include/Controllers/IterationTotalController.h
HEADERS += $$PWD/../include/Controllers/ActionController.h
HEADERS += $$PWD/../include/Controllers/RowingController.h
HEADERS += $$PWD/../include/Controllers/FightController.h
HEADERS += $$PWD/../include/Controllers/LogicController.h
HEADERS += $$PWD/../include/Controllers/StartController.h
HEADERS += $$PWD/../include/Controllers/TotalController.h
HEADERS += $$PWD/../include/Controllers/CardController.h
