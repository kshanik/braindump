
// NB: You can use math functions/operators on ImVec2 if you #define IMGUI_DEFINE_MATH_OPERATORS and #include "imgui_internal.h"
// Here we only declare simple +/- operators so others don't leak into the demo code.
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

enum NodeType
{
    MainTopic,
    SubTopic,
};

struct Node;
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

    Node    *parent;
    std::vector<Node*> children;

    Node(int id, const char* name, const ImVec2& pos, float value, const ImVec4& color, int inputs_count, int outputs_count, NodeType type, Node *p)
    {
        ID = id;
        strcpy(Name, name);
        Pos = pos;
        Value = value;
        Color = color;
        InputsCount = inputs_count;
        OutputsCount = outputs_count;
        Type = type;
        parent = p;
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
    std::vector<Node*> nodes;
    std::vector<NodeLink*> links;
    bool about_open;
    bool show_grid;
    bool show_nodes;
    bool quit = false;
    ImVec2 scrolling = ImVec2(0.0f, 0.0f);
    int node_selected = -1;
} global_state;

global_state g_state = {};
