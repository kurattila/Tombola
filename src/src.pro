TEMPLATE = subdirs

SUBDIRS += \
    TombolaApp \
    TombolaLib

TombolaApp.depends = TombolaLib
