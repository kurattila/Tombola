rem ============================================
rem Customize these paths to your own locations:
rem ============================================
set QtBinPath=D:\Qt\Qt5.6\5.6\msvc2015\bin
set ReleaseBuildDir=D:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release



set path=%path%;%QtBinPath%
windeployqt --qmldir src\TombolaApp %ReleaseBuildDir%\src\TombolaApp\release\TombolaApp.exe

rem Copy over .DLL file and also its .PDB file
copy /Y %ReleaseBuildDir%\src\TombolaLib\release\TombolaLib.dll %ReleaseBuildDir%\src\TombolaApp\release\
copy /Y %ReleaseBuildDir%\src\TombolaLib\release\TombolaLib.pdb %ReleaseBuildDir%\src\TombolaApp\release\

remo Copy over localization *.qm files
copy /Y %ReleaseBuildDir%\src\TombolaApp\*.qm %ReleaseBuildDir%\src\TombolaApp\release\

rem strnlen missing warning on XP
rem https://bugreports.qt.io/browse/QTBUG-44985
echo !!! Ship with d3dcompiler_43.dll when targeting XP !!!

pause