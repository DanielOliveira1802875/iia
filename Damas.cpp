#include "Damas.h"

#include <iostream>

void Damas::resetState()
{
    while (tabuleiro.size() != 0)
        tabuleiro.pop_back();
}

void Damas::genSuccessors(DLList<Node*>& successors)
{
    if (tabuleiro.size() == edge)
        return;
    for(int i = 0; i < edge; i++)
    {
        Damas* successor = (Damas*)getClone();        
        successor->tabuleiro.push_back(i);

        // Este filho tem posição invalida, é descartado
        if(!successor->isValid())
        {
            delete successor;
            continue;
        }
        successors.addToHead(successor);
    }
}

bool Damas::isSolution()
{
    if(isValid() && tabuleiro.size() == edge)
        return true;
    return false;
}

bool Damas::isValid()
{
    std::vector<int> elementosPorColuna;
    std::vector<int> elementosPorDiagonal1; // dialgonal "\"
    std::vector<int> elementosPorDiagonal2; // dialgonal "/"

    //inicializa vetor das colunas
    for (int i = 0; i < edge; ++i)    
        elementosPorColuna.push_back(0);    
    
    const int totalDeDiagonais = edge * 2 - 1;
    // inicializa vetor das diagonais
    for (int i = 0; i < totalDeDiagonais; ++i)
    {
        elementosPorDiagonal1.push_back(0);
        elementosPorDiagonal2.push_back(0);
    }

    int linha = 0;

    for (std::vector<int>::iterator it = tabuleiro.begin(); it != tabuleiro.end(); ++it)
    {
        elementosPorColuna[*it]++;

        int diagnal1 = edge-1 + *it - linha;
        elementosPorDiagonal1[diagnal1]++;

        int diagnal2 = edge -1 - (edge - 1 - *it) + linha;
        elementosPorDiagonal2[diagnal2]++;

        if (elementosPorColuna[*it] > 1 ||
            elementosPorDiagonal1[diagnal1] > 1 ||
            elementosPorDiagonal2[diagnal2] > 1)
            return false;
        linha++;
    }
    return true;
}

Node* Damas::getClone()
{
    Damas* newDamas = new Damas();

    // copia o tabuleiro para o novo objeto Damas
    for (std::vector<int>::iterator it = tabuleiro.begin(); it != tabuleiro.end(); ++it)
        newDamas->tabuleiro.push_back(*it);
    return newDamas;
}

std::string Damas::toString()
{
    std::string str = "\n";
    for (int j = 0; j < edge; j++)
        str += "----";
    str += "\n";
    for (int i = 0; i < tabuleiro.size(); ++i)
    {        
        for (int j = 0; j < edge; j++)
            if (tabuleiro[i] != j)
                str += "|   ";
            else
                str += "| X ";
        str += "|\n";
        for (int j = 0; j < edge; j++)
            str += "----";
        str += "|\n";
    }
    return str;
}

bool Damas::operator==(Node& node)
{
    const bool numDeDamasDiferente = this->tabuleiro.size() != ((Damas&)node).tabuleiro.size();
    if (numDeDamasDiferente)
        return false;

    for (size_t i = 0; i < tabuleiro.size(); ++i)
        if (tabuleiro[i] != ((Damas&)node).tabuleiro[i])
            return false;

    return true;
}
