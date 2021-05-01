#pragma once
#include <string>
#include "DLList.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// A classe que usar os algoritmos implementados em Search.h deve herdar desta classe abstrata,
// implementar as funções virtuais, assim como, as propriedades/metodos que lhe sejam
// necessarios para definir e alterar o seu estado.
////////////////////////////////////////////////////////////////////////////////////////////////

class Node
{    
    // variveis que definem o estado deste nó    


public:

    // Custo total desde a root (custo dos nós ascendentes mais o custo deste nó)
    // IMPORTANTE: devemos colocar apenas o custo desde o pai,
    // o algoritmo irá posteriormente somar o custo dos ascendestes.
    // Caso o custo seja utitário, podemos ignorar esta variavel.
    int cost;
    
    // Apontador para o pai desta Node
    // Não é necessário definir, o algoritmo encarrega-se disso.
    Node* parent = nullptr;

    Node() { cost = 1; }
    virtual ~Node(){}

    ////////////////////////////////////////////////
    // Os proximos metodos deveram ser        
    // implementados pela classe que herdar desta 
    ////////////////////////////////////////////////

    // Gera um estado inicial, aleatório ou não.
    virtual void resetState() = 0;

    // Gera e devolve uma lista de estados sucessores
    // IMPORTANTE: caso o custo seja diferente de 1, e necessario defini-lo,
    // (variavel cost) com custo DESDE O PAI ATE AO SUCESSOR.
    virtual void genSuccessors(DLList<Node*>& successors) = 0;

    // Verifica se este estado é uma solução
    virtual bool isSolution() = 0;

    // Devolve um clone deste estado
    virtual Node* getClone() = 0;

    // Devolve uma representação visual deste estado.
    virtual std::string toString() = 0;

    // Compara este estado com outro
    virtual bool operator==(Node& node) = 0;
};

