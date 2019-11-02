SET bat_file_path=%~dp0

rem debug
cl /Zi %bat_file_path%/src/main.cpp /I %bat_file_path%/3rdparty/sdl/include  /link %bat_file_path%/3rdparty/bin/x64/Debug/SDL2.lib %bat_file_path%/3rdparty/bin/x64/Debug/SDL2main.lib OpenGL32.lib  gdi32.lib user32.lib shell32.lib  comdlg32.lib ole32.lib oleAut32.lib winmm.lib advapi32.lib version.lib Imm32.lib Setupapi.lib /SUBSYSTEM:CONSOLE

rem release
rem cl %bat_file_path%/src/main.cpp /I %bat_file_path%/3rdparty/sdl/include  /link %bat_file_path%/3rdparty/bin/x64/Release/SDL2.lib %bat_file_path%/3rdparty/bin/x64/Release/SDL2main.lib OpenGL32.lib  gdi32.lib user32.lib shell32.lib  comdlg32.lib ole32.lib oleAut32.lib winmm.lib advapi32.lib version.lib Imm32.lib Setupapi.lib /SUBSYSTEM:CONSOLE
