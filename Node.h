#pragma once
#include <string>
#include "DLList.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// A classe que usar os algoritmos implementados em Search.h deve herdar desta classe abstrata,
// implementar as funções virtuais, assim como, as propriedades/metodos que lhe sejam
// necessarios para definir e alterar o seu estado.
////////////////////////////////////////////////////////////////////////////////////////////////

enum class Heuristic { none = 0, bestFs, aStar };

class Node
{    
    // variveis que definem o estado deste nó    


public:

    // Custo total desde a root (custo dos nós ascendentes mais o custo deste nó)
    // IMPORTANTE: Devido aos algoritmos informados, este valor deverá ser calculado
    // no momento que se geram os sucessores, assumindo o seguinte valor:
    // [custo do pai] + [custo desde o pai até esta node]
    int cost;

    // Define o tipo de euristica usado.
    // Apenas necessário para algoritmos informados (bestFs, aStar)
    // Por defeito é "none", mas pode tomar o valor de "bestFs" e "aStar"
    static Heuristic heuristicType;

    // IMPORTANTE:
    // Se o algoritmo utilizado for informado (bestFs, aStar), o nó terá que calcular este valor.
    // Dependendo do valor de heuristicType (variavel a cima) a classe que herdar desta
    // deverá implementar um metodo que calcule o valor heuristico, tendo em
    // consideracao o valor de heuristicType:
    // "none" -> ignorar, por defeito é 0.
    // "bestFs" -> custo até a solucao.
    // "aStar" -> custo desde a root + custo até a solucao.
    int heuristic;
    
    // Apontador para o pai desta Node
    // Não é necessário definir, o algoritmo encarrega-se disso.
    Node* parent = nullptr;

    Node()
    {
        cost = 1;        
        heuristic = 0;
    }
    virtual ~Node(){}

    ////////////////////////////////////////////////
    // Os proximos metodos deveram ser        
    // implementados pela classe que herdar desta 
    ////////////////////////////////////////////////

    // Gera um estado inicial, aleatório ou não.
    virtual void resetState() = 0;

    // Gera e devolve uma lista de estados sucessores
    // IMPORTANTE: Devido aos algoritmos informados, o custo (cost)
    // deverá ser calculado neste metodo, assumindo o seguinte valor:
    // [custo do pai] + [custo desde o pai até esta node]
    virtual void genSuccessors(DLList<Node*>& successors) = 0;

    // Verifica se este estado é uma solução
    virtual bool isSolution() = 0;

    // Devolve um clone deste estado
    virtual Node* getClone() = 0;

    // Devolve uma representação visual deste estado.
    virtual std::string toString() = 0;

    // Compara este estado com outro
    virtual bool operator==(Node& node) = 0;

    //////////////////////////////////////////////////////////////
    // Metodos necessarios para procuras informadas (bestFs, aStar).
    //////////////////////////////////////////////////////////////
    
    // IMPORTANTE: Deve comparar o valor heuristico
    virtual bool operator>(Node& node) = 0;
    
    // IMPORTANTE: Deve comparar o valor heuristico
    virtual bool operator<(Node& node) = 0;

    // IMPORTANTE: Deve comparar o valor heuristico
    virtual bool operator>=(Node& node) = 0;
    
    // IMPORTANTE: Deve comparar o valor heuristico
    virtual bool operator<=(Node& node) = 0;

};

