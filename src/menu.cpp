void draw_menu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
            }
            if (ImGui::MenuItem("Open"))
            {
            }
            if (ImGui::MenuItem("Exit"))
            {
                g_state.quit = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Insert"))
        {
            if (ImGui::MenuItem("Main Topic"))
            {
            }
            if (ImGui::MenuItem("Sub Topic"))
            {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Show Grid", NULL, g_state.show_grid))
            {
                g_state.show_grid = !g_state.show_grid;
            }
            if (ImGui::MenuItem("Show Nodes", NULL, g_state.show_nodes))
            {
                g_state.show_nodes = !g_state.show_nodes;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            g_state.about_open = false;
            if (ImGui::MenuItem("About"))
            {
                g_state.about_open = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (g_state.about_open)
    {
        ImGui::Begin("About Dear ImGui", &g_state.about_open, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Braindump is free as in beer! \n\nDeveloper: https://github.com/oxffffffff");
        ImGui::End();
    }
}
