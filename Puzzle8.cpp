#include "Puzzle8.h"

void Puzzle8::resetState()
{
	// colocar a posicao final
	puzzle.resize(9);
	for (int i = 0; i < 9; i++)
		puzzle[i] = i;
	zero = 8;

	puzzle[0] = 1;
	puzzle[1] = 2;
	puzzle[2] = 5;
	puzzle[3] = 3;
	puzzle[4] = 7;
	puzzle[5] = 8;
	puzzle[6] = 4;
	puzzle[7] = 6;
	puzzle[8] = 0;


	// efectuar trocas ao acaso
	/*for (int i = 0; i < 50; i++) 
	{
		int move = rand() % 4;
		if (move == 0 && zero >= 3) 			
			Trocar(zero, zero - 3);
		else if(move == 1 && zero < 6)
			Trocar(zero, zero + 3);
		else if (move == 2 && zero % 3 > 0)
			Trocar(zero, zero - 1);
		else if (move == 3 && zero % 3 < 2)
			Trocar(zero, zero + 1);
	}*/
}

void Puzzle8::Trocar(int a, int b) {
	if (puzzle[a] == 0) {
		puzzle[a] = puzzle[b];
		puzzle[b] = 0;
		zero = b;
	}
	else {
		puzzle[a] = puzzle[b];
		puzzle[b] = 0;
		zero = b;
	}
}

void Puzzle8::genSuccessors(DLList<Node*>& successors)
{
	if (zero >= 3) { // subir
		Puzzle8* sucessor = (Puzzle8*)getClone();
		sucessor->Trocar(zero, zero - 3);
		successors.addToHead(sucessor);
	}
	if (zero < 6) { // baixar
		Puzzle8* sucessor = (Puzzle8*)getClone();
		sucessor->Trocar(zero, zero + 3);
		successors.addToHead(sucessor);
	}
	if (zero % 3 > 0) { // esquerda
		Puzzle8* sucessor = (Puzzle8*)getClone();
		sucessor->Trocar(zero, zero - 1);
		successors.addToHead(sucessor);
	}
	if (zero % 3 < 2) { // direita
		Puzzle8* sucessor = (Puzzle8*)getClone();
		sucessor->Trocar(zero, zero + 1);
		successors.addToHead(sucessor);
	}
}

bool Puzzle8::isSolution()
{
	for (int i = 0; i < 9; i++)
		if (puzzle[i] != i)
			return false;
	return true;
}

Node* Puzzle8::getClone()
{
	Puzzle8* clone = new Puzzle8();
	clone->puzzle = this->puzzle;
	clone->zero = this->zero;
	return clone;
}

std::string Puzzle8::toString()
{
	std::string str;
	for (int i = 0; i < 3; i++) {
		str += "\n";
		for (int j = 0; j < 3; j++)
			if (puzzle[i * 3 + j] == 0)
				str += " . ";
			else str += " " + std::to_string(puzzle[i * 3 + j]) + " ";
	}
	str += "\n";
	return str;
}

bool Puzzle8::operator==(Node& node)
{
	if (this->zero != ((Puzzle8&)node).zero)
		return false;
	for (int i = 0; i < 9; i++)
		if (this->puzzle[i] != ((Puzzle8&)node).puzzle[i])
			return false;
	return true;
}

