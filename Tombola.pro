TEMPLATE = subdirs

SUBDIRS += \
    src \
    UnitTests

UnitTests.depends = src

TRANSLATIONS = tombola_hu.ts tombola_sk.ts # EN is the default language, so no need for 'tombola_en.ts'
