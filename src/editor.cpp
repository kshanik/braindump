#include "imgui_internal.h"
#include <math.h> // fmodf

#define PADDING_BETWEEN_PARENT_CHILD 30

void add_child(Node *node)
{
    g_state.count++;
    ImVec2 pos;
    pos.x = node->Pos.x + node->Size.x + PADDING_BETWEEN_PARENT_CHILD;
    pos.y = node->Pos.y;
    Node *new_node = new Node(g_state.nodes.size(), "Sub Topic", pos, 0.5f, ImColor(100, 100, 200), 2, 2, SubTopic, node);
    node->children.push_back(new_node);
    g_state.nodes.push_back(new_node);
    NodeLink *link = new NodeLink(g_state.node_selected, 0, new_node->ID, 0);
    node->links.push_back(link);
    g_state.node_selected = new_node->ID;
}

void add_sibling(Node *node)
{
    if (node->Type == MainTopic)
        return;

    g_state.count++;
    ImVec2 pos;
    int parent_index = node->parent->ID;
    int selected_node_index = node->ID;
    if (node->parent != 0 && parent_index > -1 && selected_node_index > -1)
    {
        pos.x = node->Pos.x;
        pos.y = node->Pos.y + node->Size.y;
        std::vector<Node*>::iterator it;
        it = std::find(node->parent->children.begin(), node->parent->children.end(), node);
        if (it != node->parent->children.end())
        {
            Node *new_node = new Node(g_state.nodes.size(), "Sub Topic", pos, 0.5f, ImColor(100, 100, 200), 2, 2, SubTopic, node->parent);
            g_state.nodes.push_back(new_node);
            node->parent->children.insert(it, new_node);
            node->parent->links.push_back(new NodeLink(parent_index, 0, new_node->ID, 0));
            g_state.node_selected = new_node->ID;
        }
    }
}

void add_main_topic(ImVec2 scene_pos)
{
    Node *main_topic = new Node(g_state.nodes.size(), "Main Topic", scene_pos, 0.5f, ImColor(100, 100, 200), 2, 2, MainTopic, 0);
    g_state.nodes.push_back(main_topic);
    g_state.main_topics.push_back(main_topic);
    g_state.node_selected = main_topic->ID;
}

// Really dumb data structure provided for the example.
// Note that we storing links are INDICES (not ID) to make example code shorter, obviously a bad idea for any general purpose code.
static void draw_editor(bool* opened)
{
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Mind map", opened))
    {
        ImGui::End();
        return;
    }

    // Draw a list of nodes on the left side
    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;
    if (g_state.show_nodes)
    {
        ImGui::BeginChild("node_list", ImVec2(100, 0));
        ImGui::Text("Nodes");
        ImGui::Separator();
        for (int node_idx = 0; node_idx < g_state.nodes.size(); node_idx++)
        {
            Node* node = g_state.nodes[node_idx];
            ImGui::PushID(node->ID);
            if (ImGui::Selectable(node->Name, node->ID == g_state.node_selected))
                g_state.node_selected = node->ID;
            if (ImGui::IsItemHovered())
            {
                node_hovered_in_list = node->ID;
                open_context_menu |= ImGui::IsMouseClicked(1);
            }
            ImGui::PopID();
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();
    ImGui::BeginGroup();

    const float NODE_SLOT_RADIUS = 4.0f;
    const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

    // Create our child canvas
    ImGui::Text("middle mouse button to scroll (%.2f,%.2f)", g_state.scrolling.x, g_state.scrolling.y);
    //ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);

    ImVec2 offset = ImGui::GetCursorScreenPos() + g_state.scrolling;
    ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // Display grid
    if (g_state.show_grid)
    {
        ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
        float GRID_SZ = 64.0f;
        ImVec2 win_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_sz = ImGui::GetWindowSize();
        for (float x = fmodf(g_state.scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
        for (float y = fmodf(g_state.scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
    }

    // Display links
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(0); // Background

    // Display nodes
    for (int main_topic_idx = 0; main_topic_idx < g_state.main_topics.size(); main_topic_idx++)
    {
        Node *main_topic = g_state.main_topics[main_topic_idx];
        std::vector<Node*> nodes;
        nodes.push_back(main_topic);
        while (nodes.size() > 0)
        {
            std::vector<Node*> new_nodes;
            for (int node_idx = 0; node_idx < nodes.size(); node_idx++)
            {
                Node *node = nodes[node_idx];
                ImGui::PushID(node->ID);
                ImVec2 node_rect_min = offset + node->Pos;

                // Display node contents first
                draw_list->ChannelsSetCurrent(1); // Foreground
                bool old_any_active = ImGui::IsAnyItemActive();
                ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
                ImGui::BeginGroup(); // Lock horizontal position

                struct TextFilters
                {
                    static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) 
                    {
                        if (ImGui::IsKeyPressedMap(ImGuiKey_Tab))
                        {
                            add_child(g_state.nodes[g_state.node_selected]);
                            return 1;
                        }
                        return 0; 
                    }
                };
                if (g_state.node_selected == node->ID)
                {
                    if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
                    {
                        ImGui::SetKeyboardFocusHere(0);
                    }
                    ImVec2 text_size = ImGui::CalcTextSize(node->Name);
                    ImGui::PushItemWidth(text_size.x+10);
                    ImGui::InputText("", node->Name, IM_ARRAYSIZE(node->Name), ImGuiInputTextFlags_CallbackCharFilter|ImGuiInputTextFlags_AllowTabInput, TextFilters::FilterImGuiLetters);
                    ImGui::PopItemWidth();
                }
                else
                {
                    ImVec2 text_size = ImGui::CalcTextSize(node->Name);
                    ImGui::PushItemWidth(text_size.x+10);
                    ImGui::Text(node->Name);
                    ImGui::PopItemWidth();
                }
                ImGui::EndGroup();

                // Save the size of what we have emitted and whether any of the widgets are being used
                bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
                node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
                ImVec2 node_rect_max = node_rect_min + node->Size;

                // Display node box
                draw_list->ChannelsSetCurrent(0); // Background
                ImGui::SetCursorScreenPos(node_rect_min);
                ImGui::InvisibleButton("node", node->Size);
                if (ImGui::IsItemHovered())
                {
                    node_hovered_in_scene = node->ID;
                    open_context_menu |= ImGui::IsMouseClicked(1);
                }
                bool node_moving_active = ImGui::IsItemActive();
                if (node_widgets_active || node_moving_active)
                    g_state.node_selected = node->ID;
                if (node_moving_active && ImGui::IsMouseDragging(0))
                    node->Pos = node->Pos + ImGui::GetIO().MouseDelta;

                ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && g_state.node_selected == node->ID)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
                draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
                draw_list->AddRect(node_rect_min, node_rect_max, node->Type == MainTopic ? IM_COL32(100, 0, 100, 255) : IM_COL32(100, 100, 100, 255), 4.0f, ImDrawCornerFlags_All, node->Type == MainTopic ? 4.0f : 1.0f);
                // for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
                //     draw_list->AddCircleFilled(offset + node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
                // for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
                //     draw_list->AddCircleFilled(offset + node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
                ImGui::PopID();


                for (int link_idx = 0; link_idx < node->links.size(); link_idx++)
                {
                    NodeLink* link = node->links[link_idx];
                    Node* node_inp = g_state.nodes[link->InputIdx];
                    Node* node_out = g_state.nodes[link->OutputIdx];
                    ImVec2 p1 = offset + node_inp->GetOutputSlotPos(link->InputSlot);
                    ImVec2 p2 = offset + node_out->GetInputSlotPos(link->OutputSlot);
                    draw_list->AddBezierCurve(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2, IM_COL32(200, 200, 100, 255), 1.0f);
                }

                for (int child_idx = 0; child_idx < node->children.size(); child_idx++)
                {
                    new_nodes.push_back(node->children[child_idx]);
                }
            }
            nodes = new_nodes;
        }
    }
    draw_list->ChannelsMerge();

    // Open context menu
    if (!ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseClicked(1))
    {
        g_state.node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
        open_context_menu = true;
    }
    if (open_context_menu)
    {
        ImGui::OpenPopup("context_menu");
        if (node_hovered_in_list != -1)
            g_state.node_selected = node_hovered_in_list;
        if (node_hovered_in_scene != -1)
            g_state.node_selected = node_hovered_in_scene;
    }

    /*
    // Draw context menu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("context_menu"))
    {
        Node* node = g_state.node_selected != -1 ? g_state.nodes[g_state.node_selected] : NULL;
        if (node)
        {
            ImGui::Text("Node '%s'", node->Name);
            ImGui::Separator();
            if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
            if (ImGui::MenuItem("Delete", NULL, false, false)) {}
            if (ImGui::MenuItem("Copy", NULL, false, false)) {}
        }
        else
        {
            if (ImGui::MenuItem("Add"))
            {
                Node *main_topic = new Node(g_state.nodes.size(), "Sub Topic", scene_pos, 0.5f, ImColor(100, 100, 200), 2, 2, MainTopic, 0);
                g_state.nodes.push_back(main_topic);
                g_state.main_topics.push_back(main_topic);
            }
            if (ImGui::MenuItem("Paste", NULL, false, false)) {}
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    */


    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive())
    {
        if (ImGui::IsMouseDragging(3, 0.0f) || ImGui::IsMouseDown(3))
        {
            g_state.scrolling = g_state.scrolling + ImGui::GetIO().MouseDelta;
        }
        else if (ImGui::IsMouseDown(0))
        {
            g_state.node_selected = -1;
        }
    }

    if (ImGui::IsMouseDoubleClicked(0))
    {
        add_main_topic(scene_pos-ImVec2(20, 20));
    }

    if (ImGui::IsKeyPressedMap(ImGuiKey_Enter) || ImGui::IsKeyPressedMap(ImGuiKey_KeyPadEnter))
    {
        if (g_state.node_selected > -1)
        {
            add_sibling(g_state.nodes[g_state.node_selected]);
        }
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();

    ImGui::End();
}
