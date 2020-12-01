#include "headerFiles/funcsPrototypes.h"

int main()
{
    Tree* tree = getTree("input.txt");

    drawGraph(tree, "Graphs/Graph.png");

    Tree* diff_tree = Derivative(tree);

    Simplify(tree);

    drawGraph(tree, "Graphs/Graph_simple.png");

    drawGraph(diff_tree, "Graphs/Diff.png");

    Simplify(diff_tree);

    drawGraph(diff_tree, "Graphs/Diff_simple.png");

    writeFuncAndHisDiff(tree, diff_tree, "tex.txt");

    destructTree(tree);
    destructTree(diff_tree);

    return 0;
}