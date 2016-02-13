# Directories
# INCLUDEPATH += $$TOP_PWD/src/libs
SRC_DIR = $$TOP_PWD

# Enable C++11 support (but on MSVC not needed - it generates warnings)
QMAKE_CXXFLAGS += -std=c++0x
