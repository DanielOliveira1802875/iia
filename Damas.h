#pragma once
#include <vector>
#include "Node.h"

class Damas : public Node
{
    std::vector<int> tabuleiro;

    // tamanho do tabuleiro (neste caso 8x8)
    static const int edge = 8;

public:
    ~Damas() override{}
    void resetState() override;
    void genSuccessors(DLList<Node*>& successors) override;
    bool isSolution() override;
    bool isValid();
    Node* getClone()  override;
    std::string toString()  override;
    bool operator==(Node& node)  override;
};

