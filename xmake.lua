add_rules("mode.debug", "mode.release")

target("BingW")
    add_rules("qt.widgetapp")
    add_frameworks("QtCore", "QtWidgets", "QtGui","QtNetwork")
    add_files("resources/resources.qrc")
    add_files("resources/*.ui")
    add_files("src/*.cpp")
    add_files("src/*.h")
    if(is_os("windows"))then 
        add_files("src/app.rc")
    end 
