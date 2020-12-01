#include "headerFiles/funcsPrototypes.h"

#ifdef dbg
#define dbg printf("(func: %s | line: %d)\n", __func__, __LINE__)
#else
#define dbg {}
#endif

// #undef dbg
// #define dbg {};

const double EPS = 1e-6;
const char OPEN_BRACKET = '(';
const char CLOSE_BRACKET = ')';
const size_t AMOUNT_OPES = 10;
const char* OPERS[AMOUNT_OPES] = 
{
    "+",
    "-",
    "*",
    "/",
    "^",

    "sin",
    "cos",
    "tan",

    "log",
    "exp",
};

const char* TEX_EQUIVALENTS[]
{
    "+", 
    "-",
    "\\cdot",
    "\\frac",
    "^",
    "\\sin",
    "\\cos",
    "\\tan",
    "\\log",
    "\\exp"
};

const char* VARIABLES = "xyz";
const double VAR_BUFFER[31] = {};
const char VALID_SYMBOLS[] = ".-";
const char* DOT_COMMAND = "dot -Tpng ";
const char* TEX_COMMAND = "pdflatex ";

//Service funcs
void systemCall(const char* filename, const char* graph_filename)
{
    char command[96] = {};

    size_t i = 0;
    while (DOT_COMMAND[i] != '\0')
    {
        command[i] = DOT_COMMAND[i];
        ++i;
    }
    size_t j = 0;
    while (filename[j] != '\0')
    {
        command[i] = filename[j++];
        ++i;
    }
    command[i++] = ' ';
    command[i++] = '-';
    command[i++] = 'o';
    command[i++] = ' ';
    j = 0;
    while (graph_filename[j] != '\0')
    {
        command[i] = graph_filename[j++];
        ++i;
    }
    
    system(command);

}

void systemCall(const char* tex_filename)
{
    char command[96] = {};

    strcpy(command, TEX_COMMAND);
    strcat(command, tex_filename);

    system(command);
    
}

inline bool isOpenBracket(char* symbol)
{
    return (*symbol == OPEN_BRACKET);
}

inline bool isCloseBracket(char* symbol)
{
    return (*symbol == CLOSE_BRACKET);
}

inline bool isVariable(char* symbol)
{
    return (strchr(VARIABLES, *symbol) != nullptr);
}

inline bool isValidSymbol(char symbol)
{
    return (strchr(VALID_SYMBOLS, symbol) != NULL);
}

bool isNumber(char* string)
{
    if (string == NULL)
        return 0;

    while(*string != '\0' && *string != ']')
    {
        if (!isdigit(*string) && !isValidSymbol(*string))
        {
            return 0;
        }
        ++string;
    }
    return 1;
}

int getOperationNum(char* string)
{
    bool is_equal = false;
    size_t j = 0;
    for (size_t i = 0; i < AMOUNT_OPES; ++i)
    {   
        is_equal = true;
        while (OPERS[i][j] != '\0')
        {   
            if (OPERS[i][j] != string[j])
            {   
                is_equal = false;
                break;
            }
            ++j;
        }
        if (is_equal) return i;
        j = 0;
    }
    return -1;
}

int processNumber(char* string, double* number)
{
    int offset = 0;

    if (sscanf(string, "%lg%n", number, &offset) != 1)
    {
        return 0;
    }
    return offset;    
}

bool isOperation(char* string)
{
    return (getOperationNum(string) > 0);
}

bool isStringInArray(const char* array[], size_t size, char* string)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (strcmp(array[i], string) == 0)
        {
            return true;
        }
    }
    return false;
}

bool isRight(Node* node)
{   
    return (node->parent != nullptr && node->parent->right == node);
};

bool hasVariable(Node* node, char variable)
{   
    dbg;
    if (node == nullptr) return false; 

    if (node->type == VARIABLE && node->value == 'x' - variable) return true;

    return (hasVariable(node->left, variable) || hasVariable(node->right, variable));
}

int approxLength(const char* filename)
{
	assert(filename);
	
	struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

	stat(filename, buff);

	return buff->st_size;
}

char* readFile(const char* filename, size_t* buffer_size)
{
    FILE* file = fopen(filename, "r");

    assert(file);
    
    size_t apr_size = approxLength(filename);

    char* buffer = (char*)calloc(apr_size, sizeof(char));
    
    apr_size =  fread(buffer, sizeof(char), apr_size, file);
    if (buffer_size != nullptr) *buffer_size = apr_size;

    if (buffer != nullptr)      buffer[apr_size] = '\0';

    return buffer;
}

inline OPERATIONS getOperationCode(Node* node)
{
    return (OPERATIONS)node->value;
}

inline bool isHas2Args(Node* node)
{
    return (node->value < SIN_OP);
}

inline bool isConst(Node* node)
{
    return (node->type == CONST);
}

inline bool isZero(double value)
{
    return (fabs(value) < EPS);
}

inline bool isOne(double value)
{
    return isZero(value - 1);
}

#define L node->left
#define R node->right
#define vL node->left->value
#define vR node->right->value

elem_t getResultOperation(Node* node)
{
    assert(L && R);
    switch (getOperationCode(node))
    {
        case ADD_OP:
            return (vL + vR);

        case SUB_OP:
            return (vL - vR);

        case MUL_OP:
            return (vL * vR);

        case DIV_OP:
            return (vL / vR);

        case POW_OP:
            return pow(vL, vR);

        case SIN_OP:
            return sin(vL);

        case COS_OP:
            return cos(vL);

        case TAN_OP:
            return tan(vL);

        case LOG_OP:
            return log(vL);

        case EXP_OP:
            return exp(vL);

        default:
            return VOID;
    }
}
#undef L 
#undef R 
#undef vL 
#undef vR 

//Node
void constructNode(Node* node)
{
    node->left = nullptr;
    node->right = nullptr;
    node->right = nullptr;
    node->value = 0;
    node->type = VOID;
}

void constructNode(Node* node, elem_t value, TYPE type)
{
    constructNode(node);

    node->value = value;
    node->type = type;
}

void constructChild(Node* node, Node* parent)
{
    constructNode(node);

    node->parent = parent;
}

void constructChild(Node* node, Node* parent, elem_t value, TYPE type)
{
    constructChild(node, parent);
    
    node->value = value;
    node->type = type;
}

void initNode(Node* node, elem_t value, TYPE type)
{
    node->value = value;
    node->type = type;
}

void initNode(Node* node, Node* left, Node* right, elem_t value, TYPE type)
{
    assert(node);
    initNode(node, value, type);
    node->left = left;
    node->right = right;
}

void copyNode(Node* copy_node, Node* node)
{
    initNode(copy_node, node->left, node->right, node->value, node->type);
}

void destructNode(Node* node)
{
    free(node);
}

void destructNodes(Node* node)
{   
    dbg;
    if (node == nullptr)
    {
        return;
    }
    if (node->left != nullptr)
    {
        destructNodes(node->left);
    }
    if (node->right != nullptr)
    {
        destructNodes(node->right);
    }
    destructNode(node);
}

void processChild(Node** node, Node** child, char** current)
{
    *child = newChild(*node);
    *node = *child;
    *current += 1;
}

void showNode(Node* node)
{
    printf("[\n");
    printf("NODE: (type = %d | value = %lg)\n", node->type, node->value);
    printf("parent: ");
    if (node->parent != nullptr)
    {
        printf("NODE: (type = %d | value = %lg)\n", node->parent->type, node->parent->value);
    }
    else
    {
        printf("nullptr\n");
    }
    
    printf("left: ");
    if (node->left != nullptr)
    {
        printf("NODE: type = %d | value = %lg)\n", node->left->type, node->left->value);
    }
    else
    {
        printf("nullptr\n");
    }
    printf("right: ");
    if (node->right != nullptr)
    {
        printf("NODE: type = %d | value = %lg)\n", node->right->type, node->right->value);
    }
    else
    {
        printf("nullptr\n");
    }
    
    printf("]\n");
}

void printNodeValue(FILE* file, Node* node)
{
    switch (node->type)
    {
    case CONST:
        fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"seagreen1\", label=\"{%lg}\"]\n ", node, node->value); 
        break;

    case VARIABLE:
        fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"lightpink\", label=\"{%c}\"]\n ", node, VARIABLES[(int)node->value]); 
        break;
    
    case OPERATION:
        fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"skyblue1\", label=\"{%s}\"]\n ", node, OPERS[(int)node->value]);
        break;
    case VOID:
        fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"seashell1\", label=\"{void}\"]\n ", node);
    }
}

void reduceConsts(Node* node)
{
    if (node->left->type != CONST || node->right->type != CONST)
    {
        return;
    }
    initNode(node, getResultOperation(node), CONST);
    node->left = deleteNodes(node->left);
    node->right = deleteNodes(node->right);
}

void relinkChilds(Node* copy_node, Node* node)
{   
    dbg;
    if (node->right != nullptr)
    {
        node->right->parent = copy_node;
    }
    if (node->left != nullptr)
    {
        node->left->parent = copy_node;
    }
}

void updateNode(Node* destructable_child, Node* relinkable_child, Node* updatable_node)
{
    dbg;
    destructNode(destructable_child);
    copyNode(updatable_node, relinkable_child);
    relinkChilds(updatable_node, relinkable_child);
    destructNode(relinkable_child);
}

void nodeToConst(Node* node, double value)
{
    initNode(node, deleteNodes(node->left), deleteNodes(node->right), value, CONST);
}

inline bool possibleSkipBrackets(Node* node)
{
    return (getOperationCode(node) > 1 && getOperationCode(node) < 5);
}

Node* newNode(Node* left_node, Node* right_node, elem_t value, TYPE type)
{
    Node* new_node = newNode(value, type);

    new_node->parent = nullptr;
    new_node->left = left_node;
    new_node->right = right_node;

    if (left_node != nullptr) left_node->parent = new_node;
    if (right_node != nullptr) right_node->parent = new_node;

    return new_node;
}

Node* getCopyNode(Node* node)
{
    return newNode(node->value, node->type);
}

Node* copyNodes(Node* node)
{   
    if (node == nullptr)
    {
        return nullptr;
    }

    return newNode(copyNodes(node->left), copyNodes(node->right), node->value, node->type);
}

Node* loadNodes(FILE* file, Node* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->left != nullptr)
    {
        fprintf(file, "\"%p\":sw->\"%p\"\n", loadNodes(file, node->left)->parent, node->left);  // link parent and left child
    }
    if (node->right != nullptr)
    {
        fprintf(file, "\"%p\":se->\"%p\"\n", loadNodes(file, node->right)->parent, node->right);  // link parent and right child
    }
    printNodeValue(file, node);
    
    return node;
}

Node* newNode()
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    constructNode(new_node);

    return new_node;
}

Node* newNode(elem_t value, TYPE type)
{
    Node* new_node = newNode();

    constructNode(new_node, value, type);

    return new_node;
}

Node* newChild(Node* parent)
{
    Node* new_child = newNode();

    constructChild(new_child, parent);

    return new_child;
}

Node* newChild(Node* parent, elem_t value, TYPE type)
{
    Node* new_child = newChild(parent);
    
    new_child->value = value;
    new_child->type = type;

    return new_child;
}

Node* deleteNode(Node* node)
{
    destructNode(node);

    return nullptr;
}

Node* deleteNodes(Node* node)
{
    destructNodes(node);

    return nullptr;
}

//Tree
void constructTree(Tree* tree)
{
    tree->root = newNode();    
}

int processTree(Node* cur_node, char* buffer, size_t size)
{
    char* current = buffer;
    int idx = 0;
    double number = 0;
    while (current - buffer < size)
    {   
        if (isOpenBracket(current))
        {
            if (cur_node->left == nullptr)
            {
                processChild(&cur_node, &(cur_node->left), &current);
            }
            else if (cur_node->right == nullptr)
            {
                processChild(&cur_node, &(cur_node->right), &current);
            }
            else return 1;
            
        }
        else if (isCloseBracket(current))
        {
            cur_node = cur_node->parent;
            ++current;
        }
        else if ((idx = getOperationNum(current)) > -1)
        {
            initNode(cur_node, idx, OPERATION);
            current += strlen(OPERS[idx]);
        }
        else if (isVariable(current))
        {
            initNode(cur_node, *current - 'x', VARIABLE);
            ++current;
        }
        else if ((idx = processNumber(current, &number)) > 0)
        {
            initNode(cur_node, number, CONST);
            current += idx;
        }
        else 
        {
            printf("SYNTAX ERROR!!!\n");
            return 1;
        }
        number = 0;
        idx = 0;
    }
    return 0;
}

void constructTree(Tree* tree, char* buffer, size_t size)
{
    constructTree(tree);

    if (processTree(tree->root, buffer, size) != 0)
    {
        printf("Perhaps you down ;( sorry\n");
        destructTree(tree);
    }
}

void destructTree(Tree* tree)
{
    destructNodes(tree->root);
}

void drawGraph(Node* node, const char* filename)
{
    if (node == nullptr) 
    {
        return;
    }
    FILE* file = fopen("graphPlot.txt", "w");

    fprintf(file, "digraph G{\n");
    fprintf(file, "node [shape=\"record\", style=\"solid\", color=\"blue\"];\n");

    loadNodes(file, node);

    fprintf(file, "}");

    fclose(file);

    systemCall("graphPlot.txt", filename);
}

void drawGraph(Tree* tree, const char* filename)
{
    if (tree->root == nullptr) return;

    drawGraph(tree->root, filename);
}

void graphRepoter(Node* node, const char* reason)
{
    static size_t number_of_calls = 0;
    printf("====\nGRAPH REPORTER CALL #%d\n======\n", number_of_calls);

    char filename[128];
    
    snprintf(filename, 128, "Nodes/%s%d.png", reason, number_of_calls);
    drawGraph(node, filename);

    ++number_of_calls;
}

void Graphviz(Tree* tree)
{
    if (tree->root == nullptr) 
    {
        return;
    }
    FILE* file = fopen("graphPlot.txt", "w");

    fprintf(file, "digraph G{\n");
    fprintf(file, "node [shape=\"record\", style=\"solid\", color=\"blue\"];\n");

    loadNodes(file, tree->root);

    fprintf(file, "}");

    fclose(file);

    system("dot -Tjpg  graphPlot.txt -o Graph.jpg");
}

Tree* newTree()
{
    Tree* new_tree = (Tree*)calloc(1, sizeof(Tree));

    constructTree(new_tree);

    return new_tree;
}

Tree* newTree(char* buffer, size_t buffer_size)
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));

    constructTree(tree, buffer, buffer_size);

    return tree;
}

Tree* getTree(const char* filename)
{
    size_t buffer_size = 0;
    char* buffer = readFile(filename, &buffer_size);

    return newTree(buffer, buffer_size);
}

Tree* newCopyTree(Tree* tree)
{   
    Tree* copy_tree = (Tree*)calloc(1, sizeof(Tree));

    copy_tree->root = copyNodes(tree->root);

    return copy_tree;
}

//Derivative
#define L node->left
#define R node->right

#define dL getDerivative(node->left)
#define dR getDerivative(node->right)

#define cL copyNodes(node->left)
#define cR copyNodes(node->right)

#define OP(left, right, type)   \
    newNode(left, right, type, OPERATION)

#define NUM(value)              \
    newNode(nullptr, nullptr, value, CONST)

#define ADD(left, right) OP(left, right, ADD_OP)
#define SUB(left, right) OP(left, right, SUB_OP)
#define MUL(left, right) OP(left, right, MUL_OP)
#define DIV(left, right) OP(left, right, DIV_OP)
#define POW(left, right) OP(left, right, POW_OP)

#define SIN(left) OP(left, nullptr, SIN_OP)
#define COS(left) OP(left, nullptr, COS_OP)
#define TAN(left) OP(left, nullptr, TAN_OP)
#define LOG(left) OP(left, nullptr, LOG_OP)
#define EXP(left) OP(left, nullptr, EXP_OP)
 
 Node* processOperation(Node* node)
 {
    assert(node);
    switch (getOperationCode(node))
    {
        case ADD_OP:
            return ADD(dL, dR);

        case SUB_OP:
            return SUB(dL, dR);

        case MUL_OP:
            return ADD(MUL(dL, cR), MUL(cL, dR));

        case DIV_OP:
            return DIV(SUB(MUL(dL, cR), MUL(cL, dR)), POW(cR, NUM(2)));

        case POW_OP:
            if (!hasVariable(R, 'x')) return MUL(dL, MUL(cR, POW(cL, SUB(cR, NUM(1.0)))));

            //look up wolfram alpha if you dont trust me
            return MUL(POW(cL, cR), ADD(MUL(dR, LOG(cL)), MUL(DIV(cR, cL), dL))); 

        case SIN_OP:
            return MUL(dL, COS(cL));

        case COS_OP:
            return MUL(NUM(-1), MUL(dL, COS(cL)));

        case TAN_OP:
            return MUL(dL, DIV(NUM(1), POW(COS(cL), NUM(2))));

        case LOG_OP:
            return DIV(dL, cL);
        
        case EXP_OP:
            return MUL(dL, EXP(cL));

        default:
            return nullptr;
    }
 }

Node* getDerivative(Node* node)
{
    assert(node);

    switch (node->type)
    {   
        case CONST:
            return NUM(0);

        case VARIABLE:
            return NUM((int)node->value == 0);

        case OPERATION:
            return processOperation(node);

        default:
            printf("void\n");
            return nullptr;
    }
}

Tree* Derivative(Tree* tree)
{
    Tree* diff_tree = (Tree*)calloc(1, sizeof(Tree));

    diff_tree->root = getDerivative(tree->root);

    return diff_tree;
}

#undef dL
#undef dR
#undef cL
#undef cR
#undef OP
#undef NUM
#undef ADD
#undef SUB
#undef MUL
#undef DIV
#undef DIV
#undef POW
#undef SIN
#undef COS
#undef COS
#undef TAN
#undef LOG
#undef EXP

#define P node->parent
#define vL node->left->value
#define vR node->right->value

bool simplifyAdd(Node* node)
{
    assert(node);
    dbg;

    if (isConst(L) && isZero(vL))
    {   
        updateNode(L, R, node);
    }
    else if (isConst(R) && isZero(vR))
    {
        updateNode(R, L, node);
    }
    else
    {
        return false;
    }
    return simplifyNeutral(node);    
}

bool simplifySub(Node* node)
{   
    assert(node);
    dbg;
    if (isConst(R) && isZero(vR))
    {
        updateNode(R, L, node);
    }    
    else
    {
        return false;
    }
    return simplifyNeutral(node);
}

bool simplifyMul(Node* node)
{   
    assert(node);
    dbg;

    if ((isConst(L) && isZero(vL)) || (isConst(R) && isZero(vR)))
    {
        nodeToConst(node, 0.0);
    }
    else if (isConst(L) && isOne(vL))
    {   
        updateNode(L, R, node);
    }
    else if (isConst(R) && isOne(vR))
    {
        updateNode(R, L, node);
    }
    else
    {
        return false;
    }
    return simplifyNeutral(node);
}

bool simplifyDiv(Node* node)
{   
    assert(node);
    dbg;
    if (isConst(L) && isZero(vL))
    {
        nodeToConst(node, 0.0);
    }
    else if (isConst(R) && isOne(vR))
    {
        updateNode(R, L, node);
    }
    else 
    {
        return false;
    }
    return simplifyNeutral(node);
}

bool simplifyPow(Node* node)
{   
    assert(node);
    dbg;
    if (isConst(L) && isOne(vL))
    {
        nodeToConst(node, 1.0);
    }
    else if (isConst(R) && isOne(vR))
    {
        updateNode(R, L, node);
    }
    else if (isConst(node) && isZero(vR))
    {
        nodeToConst(node, 1.0);
    }
    else
    {
        return false;
    }
    return simplifyNeutral(node);
}

bool simplifyNeutral(Node* node)
{   
    dbg;

    if (node == nullptr) return false;
    
    bool result = simplifyNeutral(L) || simplifyNeutral(R);

    if (node->type != OPERATION) return result;

    switch (getOperationCode(node))
    {
        case ADD_OP:
            return simplifyAdd(node);

        case SUB_OP:
            return simplifySub(node);

        case MUL_OP:
            return simplifyMul(node);

        case DIV_OP:
            return simplifyDiv(node);

        case POW_OP:
            return simplifyPow(node);
        
        default:
            return result;
    }
    
}

bool simplifyConsts(Node* node)
{   
    dbg;
    if (node == nullptr) return false; 

    bool result = (simplifyConsts(L) || simplifyConsts(R));

    if (node->type != OPERATION) return result;
    
    if (isHas2Args(node))
    {
        if (isConst(L) && isConst(R))
        {
            reduceConsts(node);
            result = true;
        }
    }

    return result;    
}

#undef L 
#undef R 
#undef vL 
#undef vR 
#undef P

void Simplify(Tree* tree)
{   
    while (simplifyNeutral(tree->root) || simplifyConsts(tree->root));
}

void writeTexBegin(FILE* file)
{
    fprintf(file, "\\documentclass{article}\n");

    fprintf(file, "\\usepackage[russian]{babel}\n"
                  "\\usepackage[utf8]{inputenc}\n"
                  "\\usepackage{amsmath}\n");


    fprintf(file, "\n\\begin{document}\n");
}

void writeTexEnd(FILE* file)
{
    fprintf(file, "\\end{document}");
}

void writeEquationTex(Node* node, const char* tex_filename)
{
    FILE* file = fopen(tex_filename, "w");

    writeTexEnd(file);

    fprintf(file, "\n$$\n");
    writeEquation(file, node);
    fprintf(file, "\n$$\n");

    writeTexEnd(file);

    fclose(file);

    systemCall(tex_filename);
}

void writeFuncAndHisDiff(Tree* tree, Tree* diff_tree, const char* tex_filename)
{
    FILE* file = fopen(tex_filename, "w");

    writeTexBegin(file);

    fprintf(file, "\n$$\n");
    writeEquation(file, tree->root);
    fprintf(file, "\n$$\n");

    fprintf(file, "\n$$\n");
    writeEquation(file, diff_tree->root);
    fprintf(file, "\n$$\n");

    writeTexEnd(file);

    fclose(file);

    systemCall(tex_filename);
}

void writeNodeToTexAndGoNext(FILE* file, Node* node, const char* open, const char* close)
{
    if (getOperationCode(node) == DIV_OP)
    {
        fprintf(file, "\\frac");
        fprintf(file, "%s", open);
        writeEquation(file, node->left);
        fprintf(file, "%s%s", close, open);
        writeEquation(file, node->right);
        fprintf(file, "%s", close);
    }
    else if (isHas2Args(node))
    {
        fprintf(file, "%s", open);
        writeEquation(file, node->left);
        fprintf(file, "}%s{", TEX_EQUIVALENTS[getOperationCode(node)]);
        writeEquation(file, node->right);
        fprintf(file, "%s", close);
    }
    else
    {
        fprintf(file, "%s", TEX_EQUIVALENTS[getOperationCode(node)]);
        fprintf(file, "%s", open);
        writeEquation(file, node->left);
        fprintf(file, "%s", close);
    }
}

void writeEquation(FILE* file, Node* node)
{   
    if (node == nullptr) return;
    if (node->type != OPERATION)
    {   
        fprintf(file, "{");
        if (node->type == CONST) fprintf(file, "%lg", node->value);
        else                     fprintf(file, "%c", 'x' + (int)node->value);
        fprintf(file, "}");
    }
    else
    {
        if (possibleSkipBrackets(node))
        {   
            writeNodeToTexAndGoNext(file, node, "{", "}");
        }
        else
        {
            writeNodeToTexAndGoNext(file, node, "{(", ")}");
        }
    }
}
