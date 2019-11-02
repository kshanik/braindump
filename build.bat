SET bat_file_path=%~dp0

rem debug
cl /Zi /Febraindump.exe %bat_file_path%/src/main.cpp %bat_file_path%/3rdparty/imgui/imgui.cpp %bat_file_path%/3rdparty/imgui/imgui_demo.cpp %bat_file_path%/3rdparty/imgui/imgui_widgets.cpp %bat_file_path%/3rdparty/imgui/imgui_draw.cpp %bat_file_path%/3rdparty/imgui/examples/imgui_impl_sdl.cpp  %bat_file_path%/3rdparty/imgui/examples/imgui_impl_opengl2.cpp %bat_file_path%/3rdparty/imgui/examples/imgui_impl_opengl2.cpp /I %bat_file_path%/3rdparty/imgui/examples /I %bat_file_path%/3rdparty/sdl/include /I3rdparty/imgui /link %bat_file_path%/3rdparty/bin/x64/Debug/SDL2.lib %bat_file_path%/3rdparty/bin/x64/Debug/SDL2main.lib OpenGL32.lib  gdi32.lib user32.lib shell32.lib  comdlg32.lib ole32.lib oleAut32.lib winmm.lib advapi32.lib version.lib Imm32.lib Setupapi.lib /SUBSYSTEM:CONSOLE

rem release
rem cl %bat_file_path%/src/main.cpp /I %bat_file_path%/3rdparty/sdl/include  /link %bat_file_path%/3rdparty/bin/x64/Release/SDL2.lib %bat_file_path%/3rdparty/bin/x64/Release/SDL2main.lib OpenGL32.lib  gdi32.lib user32.lib shell32.lib  comdlg32.lib ole32.lib oleAut32.lib winmm.lib advapi32.lib version.lib Imm32.lib Setupapi.lib /SUBSYSTEM:CONSOLE
