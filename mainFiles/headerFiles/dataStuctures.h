//Operations

typedef double elem_t;

enum TYPE
{
    VOID,
    CONST,
    VARIABLE,
    OPERATION,
};

enum OPERATIONS
{
    ADD_OP,
    SUB_OP,
    
    MUL_OP,
    DIV_OP,
    POW_OP,

    SIN_OP,
    COS_OP,
    TAN_OP,

    LOG_OP,
    EXP_OP,
};

struct Node
{
    Node* right;
    Node* left;
    Node* parent;

    elem_t value;
    TYPE type;
};


struct Tree
{
    Node* root;
};