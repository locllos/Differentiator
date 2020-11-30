#include "headerFiles/funcsPrototypes.h"

int main()
{
    Tree* tree = getTree("input.txt");

    drawGraph(tree, "Graphs/Graph.png");

    Tree* copy_tree = Derivative(tree);

    Simplify(tree);

    drawGraph(tree, "Graphs/Graph_simple.png");

    drawGraph(copy_tree, "Graphs/Diff.png");

    Simplify(copy_tree);

    drawGraph(copy_tree, "Graphs/Diff_simple.png");

    writeEquationTex(copy_tree->root);

    return 0;
}