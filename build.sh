export bat_file_path=$(dirname "$0")

g++ -g  ${bat_file_path}/src/main.cpp ${bat_file_path}/3rdparty/imgui/imgui.cpp ${bat_file_path}/3rdparty/imgui/imgui_demo.cpp ${bat_file_path}/3rdparty/imgui/imgui_widgets.cpp ${bat_file_path}/3rdparty/imgui/imgui_draw.cpp ${bat_file_path}/3rdparty/imgui/examples/imgui_impl_sdl.cpp  ${bat_file_path}/3rdparty/imgui/examples/imgui_impl_opengl2.cpp -I ${bat_file_path}/3rdparty/imgui/examples -I ${bat_file_path}/3rdparty/sdl/include -I ${bat_file_path}/3rdparty/imgui -L${bat_file_path}/3rdparty/bin/linux/ -lSDL2main -lSDL2 -ldl -lm -lGL -lpthread
