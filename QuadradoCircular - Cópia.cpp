#include "QuadradoCircular.h"

#include <iostream>

void QuadradoCircular::resetState()
{
    
    for (int i = 0; i < tamanho; ++i)
        posicaoNumeros.push_back(i + 1);

    int numTrocas = 200;

    for (int i = 0; i < numTrocas; i++)
    {
        bool isLine = rand() % 2;               // escolha entre linha ou coluna
        int direcao = rand() % 2 == 0 ? -1 : 1; // escolha da direção
        int pocicao = rand() % aresta;          // escolha da linha ou coluna
        if (isLine)
            moveLinha(pocicao, direcao);
        else
            moveColuna(pocicao, direcao);
    }
}

void QuadradoCircular::genSuccessors(DLList<Node*>& successors)
{
    for (int i = 0; i < aresta; i++)
    {
        QuadradoCircular* sucessorColunaMenos = (QuadradoCircular*)getClone();
        sucessorColunaMenos->moveColuna(i, -1);
        successors.addToHead(sucessorColunaMenos);

        QuadradoCircular* sucessorColunaMais = (QuadradoCircular*)getClone();
        sucessorColunaMais->moveColuna(i, 1);
        successors.addToHead(sucessorColunaMais);

        QuadradoCircular* sucessorLinhaMenos = (QuadradoCircular*)getClone();
        sucessorLinhaMenos->moveLinha(i, -1);
        successors.addToHead(sucessorLinhaMenos);

        QuadradoCircular* sucessorLinhaMais = (QuadradoCircular*)getClone();
        sucessorLinhaMais->moveLinha(i, 1);
        successors.addToHead(sucessorLinhaMais);
    }
}

bool QuadradoCircular::isSolution()
{
    for (int i = 0; i < 9; i++)
    {
        if (posicaoNumeros[i] != (i + 1))
            return false;
    }
    return true;
}

Node* QuadradoCircular::getClone()
{
    QuadradoCircular* clone = new QuadradoCircular();
    clone->posicaoNumeros = this->posicaoNumeros;
    return clone;
}

std::string QuadradoCircular::toString()
{
    std::string str = "\n";
    for (int i = 0; i < aresta; i++) {
        for (int j = 0; j < aresta; j++)
            str += std::to_string(posicaoNumeros[i * aresta + j]) + " ";
        str += "\n";
    }
    return str;
}

bool QuadradoCircular::operator==(Node& node)
{
    for (int i = 0; i < tamanho; i++) {
        if (((QuadradoCircular&)node).posicaoNumeros[i] != this->posicaoNumeros[i])
            return false;
    }
    return true;
}

inline void QuadradoCircular::moveColuna(int coluna, int direction)
{
    if (!(direction == -1 || direction == 1) || !(coluna >= 0 && coluna < aresta))             // valida os parametros
        return;

    if (direction == -1) // cima
    {
        //std::cout << " coluna cima " + toString();
        for (int i = 0; i < (aresta - 1); i++)
        {
            int tmp = posicaoNumeros[coluna + i * aresta];
            posicaoNumeros[coluna + i * aresta] = posicaoNumeros[(coluna + i * aresta) + aresta];
            posicaoNumeros[(coluna + i * aresta) + aresta] = tmp;
        }
        //std::cout << toString();
    }
    else if (direction == 1) // baixo
    {
        //std::cout << " coluna baixo " + toString();
        for (int i = (aresta - 1); i > 0; i--)
        {
            int tmp = posicaoNumeros[coluna + i * aresta];
            posicaoNumeros[coluna + i * aresta] = posicaoNumeros[(coluna + i * aresta) - aresta];
            posicaoNumeros[(coluna + i * aresta) - aresta] = tmp;
        }
        //std::cout << toString();
    }
}

inline void QuadradoCircular::moveLinha(int linha, int direction)
{
    if (!(direction == -1 || direction == 1) || !(linha >= 0 && linha < aresta))             // valida os parametros
        return;

    if (direction == -1) // esquerda
    {
        //std::cout << "linha esquerda " + toString();
        for (int i = 0; i < (aresta - 1); i++)
        {
            int tmp = posicaoNumeros[linha * aresta +i];
            posicaoNumeros[linha * aresta +i ] = posicaoNumeros[(linha * aresta +i) + 1];
            posicaoNumeros[(linha * aresta +i) + 1] = tmp;
        }
        //std::cout << toString();
    }
    else if (direction == 1) // direita
    {
        //std::cout << " linha direita " + toString();
        for (int i = (aresta - 1); i > 0; i--)
        {
            int tmp = posicaoNumeros[linha * aresta + i];
            posicaoNumeros[linha * aresta + i] = posicaoNumeros[linha * aresta + i - 1];
            posicaoNumeros[linha * aresta + i - 1] = tmp;
        }
        //std::cout << toString();
    }
}