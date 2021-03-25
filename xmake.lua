add_rules("mode.debug", "mode.release")

target("BingBackground")
    add_rules("qt.console")
    add_frameworks("QtNetwork")
    add_files("src/*.cpp")
    add_files("src/WallpaperInfo.h")
    add_files("src/WallpaperDown.h")
