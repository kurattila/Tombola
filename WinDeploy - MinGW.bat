rem ============================================
rem Customize these paths to your own locations:
rem ============================================
set QtBinPath=D:\Qt\Qt5.6\5.6\mingw49_32\bin
set QtToolsPath=D:\Qt\Qt5.6\Tools\mingw492_32\bin
set ReleaseBuildDir=D:\git-repos\build-Tombola-Desktop_Qt_5_6_0-Release



set path=%QtBinPath%;%QtToolsPath%;%path%
windeployqt --qmldir src\TombolaApp %ReleaseBuildDir%\src\TombolaApp\release\TombolaApp.exe

rem Copy over our application's .DLL file
copy /Y %ReleaseBuildDir%\src\TombolaLib\release\TombolaLib.dll %ReleaseBuildDir%\src\TombolaApp\release\

rem Copy over Qt .DLLs needed which 'windeployqt' did not discover
rem copy /Y %QtToolsPath%\libgcc_s_dw2-1.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtToolsPath%\libstdc++-6.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtToolsPath%\libwinpthread-1.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtBinPath%\Qt5Core.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtBinPath%\Qt5Gui.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtBinPath%\Qt5Network.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtBinPath%\Qt5Qml.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtBinPath%\Qt5Quick.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtBinPath%\Qt5Svg.dll %ReleaseBuildDir%\src\TombolaApp\release\
rem copy /Y %QtBinPath%\Qt5Widgets.dll %ReleaseBuildDir%\src\TombolaApp\release\

rem Copy over localization *.qm files
copy /Y %ReleaseBuildDir%\src\TombolaApp\*.qm %ReleaseBuildDir%\src\TombolaApp\release\

rem strnlen missing warning on XP
rem https://bugreports.qt.io/browse/QTBUG-44985
rem echo !!! Ship with d3dcompiler_43.dll when targeting XP !!!

rem Clean up "garbage"
del %ReleaseBuildDir%\src\TombolaApp\release\*.o
del %ReleaseBuildDir%\src\TombolaApp\release\*.cpp

echo .
echo ==============================================================
echo Deployed in folder: %ReleaseBuildDir%\src\TombolaApp\release
echo ==============================================================

%SystemRoot%\explorer.exe %ReleaseBuildDir%\src\TombolaApp\release

pause