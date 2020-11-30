#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <sys/stat.h>
#include "dataStuctures.h"

//Service funcs
void        systemCall          (const char* filename, const char* graph_filename);
inline bool isVariable          (char* symbol);
inline bool isValidSymbol       (char symbol);
inline bool isOpenBracket       (char* symbol);
inline bool isCloseBracket      (char* symbol);
inline bool possibleSkipBrackets(Node* node);
bool        isRight             (Node* node);
bool        isNumber            (char* string);
bool        isOperation         (char* string);
bool        isStringInArray     (const char* array[], size_t size, char* string);
bool        hasVariable         (Node* node, char variable);
int         getOperationNum     (char* string);
int         approxLength        (const char* filename);
int         processNumber       (char* string, double* number);
char*       readFile            (const char* filename, size_t* buffer_size);
elem_t      getResultOperation  (Node* node);


//Node
void        constructNode           (Node* node);
void        constructNode           (Node* node, elem_t value, TYPE type);
void        constructChild          (Node* node, Node* parent);
void        constructChild          (Node* node, Node* parent, elem_t value, TYPE type);
void        destructNode            (Node* node);
void        destructNodes           (Node* node);
void        initNode                (Node* node, elem_t value, TYPE type);
void        processChild            (Node** node, Node** child, char** current);
void        printNodeValue          (FILE* file, Node* node);
void        reduceConsts            (Node* node);
void        relinkChilds            (Node* copy_node, Node* node);
void        updateNode              (Node* destructable_child, Node* relinkable_child, Node* current_node);
void        nodeToConst             (Node* node, double value);
void        copyNode                (Node* copy_node, Node* node);
void        showNode                (Node* node);
void        drawGraph               (Node* node, const char* filename);
void        graphRepoter            (Node* node, const char* reason);
void        writeNodeToTexAndGoNext (FILE* file, Node* node, const char* open, const char* close);
Node*       copyNodes               (Node* node);
Node*       newNode                 (elem_t value, TYPE type);
Node*       newNode                 (Node* left_node, Node* right_node, elem_t value, TYPE type);
Node*       newChild                (Node* parent);
Node*       newChild                (Node* parent, elem_t value, TYPE type);
Node*       loadNodes               (FILE* file, Node* node);
Node*       newNode                 ();
Node*       deleteNode              (Node* node);
Node*       deleteNodes             (Node* node);



//Tree
void        constructTree   (Tree* tree);
void        constructTree   (Tree* tree, char* buffer, size_t size);
void        destructTree    (Tree* tree);
void        drawGraph       (Tree* tree, const char* filename);
Tree*       newTree         (char* buffer, size_t size);
Tree*       getTree         (const char* filename);
Tree*       newCopyTree     (Tree* tree);
int         processTree     (Node* cur_node, char* buffer, size_t size);

//Simplify
void        Simplify        (Tree* tree);
bool        simplifyAdd     (Node* node);
bool        simplifySub     (Node* node);
bool        simplifyMul     (Node* node);
bool        simplifyDiv     (Node* node);
bool        simplifyPow     (Node* node);
bool        simplifyNeutral (Node* node);
bool        simplifyConsts  (Node* node);

//Derivative
Tree*       Derivative      (Tree* tree);
Node*       processOperation(Node* node);
Node*       getDerivative   (Node* node);

//Texify
void        writeEquationTex(Node* node);
void        writeEquation   (FILE* file, Node* node);
