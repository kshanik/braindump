SET bat_file_path=%~dp0

cl /Zi %bat_file_path%/src/main.cpp /I %bat_file_path%/3rdparty/sdl/include  /link %bat_file_path%/3rdparty/sdl/VisualC/x64/Debug/SDL2.lib OpenGL32.lib /SUBSYSTEM:CONSOLE