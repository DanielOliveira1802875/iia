#pragma once
#include <vector>

#include "Node.h"
class QuadradoCircular : public Node
{
    std::vector<int> posicaoNumeros;
    static const int aresta = 3;
    static const int tamanho = aresta * aresta;

public:

    void resetState() override;
    void genSuccessors(DLList<Node*>& successors) override;
    bool isSolution() override;
    Node* getClone() override;
    std::string toString() override;
    bool operator==(Node& node) override;

    void moveColuna(int coluna, int direction);
    void moveLinha(int linha, int direction);
};

