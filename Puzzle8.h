#pragma once
#include <vector>

#include "Node.h"
class Puzzle8 : public Node
{
    std::vector<int> puzzle;
    int zero;
public:

    void resetState() override;
    void Trocar(int a, int b);
    void genSuccessors(DLList<Node*>& successors) override;
    bool isSolution() override;
    Node* getClone() override;
    std::string toString() override;
    bool operator==(Node& node) override;    
};

