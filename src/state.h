enum NodeType
{
    MainTopic,
    SubTopic,
};

struct Node
{
    int      ID;
    char     Name[256];
    NodeType Type;
    ImVec2   Pos, Size;
    float    Value;
    ImVec4   Color;
    int      InputsCount, OutputsCount;
    bool     Focused;

    Node(int id, const char* name, const ImVec2& pos, float value, const ImVec4& color, int inputs_count, int outputs_count, NodeType type)
    {
        ID = id; strncpy(Name, name, 31); Name[31] = 0; Pos = pos; Value = value; Color = color; InputsCount = inputs_count; OutputsCount = outputs_count;
        Type = type;
    }

    ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1)); }
    ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1)); }
};

struct NodeLink
{
    int     InputIdx, InputSlot, OutputIdx, OutputSlot;

    NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) { InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot; }
};

typedef struct
{
    ImVector<Node> nodes;
    ImVector<NodeLink> links;
    bool about_open;
    bool show_grid;
    bool show_nodes;
    bool quit = false;
    ImVec2 scrolling = ImVec2(0.0f, 0.0f);
    int node_selected = -1;
} global_state;

global_state g_state = {};
