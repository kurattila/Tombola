set path=%path%;d:\Qt\Qt5.6\5.6\msvc2015\bin\
windeployqt --qmldir src\TombolaApp d:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release\src\TombolaApp\release\TombolaApp.exe

rem Copy over .DLL file and also its .PDB file
copy /Y d:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release\src\TombolaLib\release\TombolaLib.dll d:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release\src\TombolaApp\release\
copy /Y d:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release\src\TombolaLib\release\TombolaLib.pdb d:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release\src\TombolaApp\release\

remo Copy over localization *.qm files
copy /Y d:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release\src\TombolaApp\*.qm d:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release\src\TombolaApp\release\

rem strnlen missing warning on XP
rem https://bugreports.qt.io/browse/QTBUG-44985
echo !!! Ship with d3dcompiler_43.dll when targeting XP !!!

pause