# Directories
# INCLUDEPATH += $$TOP_PWD/src/libs
SRC_DIR = $$TOP_PWD

# Enable C++11 support (but on MSVC not needed - it generates warnings)
unix:QMAKE_CXXFLAGS += -std=c++0x

# When deploying to Windows XP, prevent error message "not a Win32 application"
# http://www.tripleboot.org/?p=423
#win32:CONFIG(release, debug|release): QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

# Generate .PDB files for Windows release builds
# http://stackoverflow.com/questions/9234337/qt-no-map-pdb-files-generated-for-windows-release-builds
# http://stackoverflow.com/questions/6993061/build-qt-in-release-with-debug-info-mode
# !! Beware that forcing debug info into release will make windeployqt.exe think it should deploy Debug version DLLs !!
# CONFIG += release force_debug_info
#win32:CONFIG(release, debug|release): CONFIG += release force_debug_info
