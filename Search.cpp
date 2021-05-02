#include "Search.h"
#include <iostream>

Search::Search(Node* root)
{
    this->root = root;
    totGenerations = totExpansions = limit =  0;
    root->cost = 0;
    root->parent = nullptr;
    root->resetState();
    knownStates = nullptr;
    withDuplicates = reconstructPath =  useHashTable = false;    
    startSearch();
}

void Search::startSearch()
{
    limit = -1;
    reconstructPath = false;
    withDuplicates = false;
    useHashTable = false;


    if (!bestFS())
        printStats();

    // DFS
    /*if (!dFS())
        printStats();*/

    // BFS
    /*if (!bFS())
        printStats();*/


    ///////////////////////////////
    //// Input de dados manual ////
    ///////////////////////////////
    
    /*char answer = 's';
    int searchType = -1;
    std::cout << "\nEstado inicial: \n";
    do
    {  
        std::cout << root->toString() << "\nGerar novo estado? (s/n): ";
        std::cin >> answer;
        if (answer == 's')
            root->resetState();
    } while (answer == 's');    
    std::cout << "\nExiste ciclos entre os estados? (s/n): ";
    std::cin >> answer;
    if (answer == 's' || answer == 'S')    
        withDuplicates = true;    
    else
        withDuplicates = false;
    
    std::cout << "\nReconstruir o caminho da solucao? (s/n): ";
    std::cin >> answer;
    if (answer == 's')
        reconstructPath = true;
    else
        reconstructPath = false;
    std::cout << "\n1 - DFS || 2 - BFS || 3 - DFS iterativo\nTipo de Procura: ";
    std::cin >> searchType;    

    if (searchType == 1)
    {
        std::cout << "\nProfundidade maxima do DFS  (-1 sem limite): ";
        std::cin >> limit;
    }
    else if (searchType == 3)
        limit = 0;
    if (withDuplicates && (searchType == 1 && limit == -1) || searchType == 2)
    {
        std::cout << "\nUsar HashTable para localizar os duplicados entre ramos? (s/n): ";
        std::cin >> answer;
        if (answer == 's')
            useHashTable = true;
        else
            useHashTable = false;
    }
    if (searchType == 1 || searchType == 3)
    {
        if (!dFS())
            printStats();
    }
    else if (searchType == 2)
    {
        if (!bFS())
            printStats();
    }*/
}

Search::~Search()
{
}

// Atualiza o atributo pai e custo total de cada sucessor
void Search::updateNodeParent(DLList<Node*>& successors, Node* parent)
{
    successors.setIteratorToHead();
    while (successors.isIteratorValid())
    { 
        Node* tmp = successors.getIteratorValue();
        if(withDuplicates || reconstructPath)
            tmp->parent = parent;
        successors.iteratorNext();
    }
}

void Search::printPath(Node* currentNode)
{
    std::cout << "------------------";
    while (currentNode != nullptr)
    {
        std::cout << currentNode->toString() << "------------------";
        currentNode = currentNode->parent;
    }
}

void Search::printStats(Node* node)
{
    if(node != nullptr && node->isSolution())
    {        
        if (reconstructPath) 
            printPath(node);
        else 
            std::cout << node->toString();
        std::cout << "\nSolucao encontrada\n";
        std::cout << "Custo da Solucao:    " << node->cost << std::endl;
    }
    else    
        std::cout << "\nSolucao nao encontrada\n";       
    std::cout << "Numero de expancoes: " << totExpansions << std::endl;
    std::cout << "Numero de geracoes:  " << totGenerations << std::endl;
}


void Search::initializeKnownStates(Node* rootCopy)
{
    if (knownStates != nullptr)
        knownStates->clear();
    knownStates = new HashTable<Node*>(9887);
    knownStates->add(rootCopy->toString(), nullptr);
}

bool Search::dFS()
{
    
    if(limit == 0) // DFS iterativo
    {
        limit++;
        while (dFS() == false)        
            limit++;
        clearLists();
        return true;
    }
    Node* rootCopy = root->getClone();
    open.addToHead(rootCopy);
    if(withDuplicates && useHashTable)    
        initializeKnownStates(rootCopy);    
    while (!open.isEmpty())
    {
        Node* currentNode = open.deleteFromHead();
        std::cout << currentNode->toString();
        if (currentNode->isSolution())
        {            
            printStats(currentNode);
            clearLists();
            return true;
        }
        // Se existe um limite no custo e este foi alcancado, nao expande este no.
        if(limit > 0 && currentNode->cost >= limit)
        {
            if (withDuplicates || reconstructPath)
                closed.addToHead(currentNode);
            else
                delete currentNode;
            continue;
        }
        ++totExpansions;
        DLList<Node*> newNodes;
        currentNode->genSuccessors(newNodes);
        updateNodeParent(newNodes, currentNode);
        if (withDuplicates)  
            removeDuplicates(newNodes);
        totGenerations += newNodes.getSize();
        open.addToHead(newNodes);
        if (withDuplicates || reconstructPath)
            closed.addToHead(currentNode);
        else
            delete currentNode;
    }
    clearLists();    
    return false;
}

bool Search::bFS()
{
    Node* rootCopy = root->getClone();
    open.addToHead(rootCopy);
    if (useHashTable)
        initializeKnownStates(rootCopy);
    while (!open.isEmpty())
    {
        Node* currentNode = open.deleteFromHead();
        std::cout << currentNode->toString();
        if (currentNode->isSolution())
        {
            printStats(currentNode);
            clearLists();
            return true;
        }
        ++totExpansions;
        DLList<Node*> newNodes;
        currentNode->genSuccessors(newNodes);
        updateNodeParent(newNodes, currentNode); // atualiza pais e custos
        removeDuplicates(newNodes);
        totGenerations += newNodes.getSize();
        open.addToTail(newNodes);
        if (withDuplicates || reconstructPath)
            closed.addToHead(currentNode);
        else
            delete currentNode;
    }
    clearLists();
    printStats();
    return false;
}

bool Search::bestFS()
{   
    Node* rootCopy = root->getClone();
    priorityOpen.addValue(rootCopy);
    if (withDuplicates && useHashTable)
        initializeKnownStates(rootCopy);
    while (!priorityOpen.isEmpty())
    {
        Node* currentNode = priorityOpen.removeMin();
        //std::cout << currentNode->toString();
        //std::cout << currentNode->heuristic<< " ";
        if (currentNode->isSolution())
        {
            printStats(currentNode);
            clearLists();
            return true;
        }
        ++totExpansions;
        DLList<Node*> newNodes;
        currentNode->genSuccessors(newNodes);
        updateNodeParent(newNodes, currentNode);
        if (withDuplicates)
            removeDuplicates(newNodes);
        totGenerations += newNodes.getSize();
        while (!newNodes.isEmpty())
        {
            Node* successor = newNodes.deleteFromHead();
            priorityOpen.addValue(successor);
        }
        if (withDuplicates || reconstructPath)
            closed.addToHead(currentNode);
        else
            delete currentNode;
    }
    clearLists();
    return false;
}

void Search::removeDuplicates(DLList<Node*>& successors)
{
    if (!withDuplicates)
        return;
    // Verifica na hastable
    if(useHashTable && knownStates != nullptr)
    {
        successors.setIteratorToHead();
        while (successors.isIteratorValid() && !successors.isEmpty())
        {
            Node* discardNode = nullptr;
            Node* tmp = successors.getIteratorValue();
            std::string key = tmp->toString();
            if (knownStates->isInList(key))
            {
                if (successors.deleteIterator(discardNode))
                    delete discardNode;
            }
            else
            {
                knownStates->add(key, nullptr); // passo nullptr porque já tenho referencia para ele em open ou closed
                successors.iteratorNext();
            }
        }
        return;
    }

    // verifica só no proprio ramo (até à root)
    successors.setIteratorToHead();
    while (successors.isIteratorValid())
    {
        Node* discardNode = nullptr;
        Node* tmp = successors.getIteratorValue()->parent;
        while(tmp != nullptr)
        {            
            if(*tmp == *(successors.getIteratorValue()))
            {
                if (successors.deleteIterator(discardNode))
                    delete discardNode;
                break;
            }
            tmp = tmp->parent;
        }
        successors.iteratorNext();
    }

    // Verifica o open e closed por repetidos (dempra mt tempo)
    /*successors.setIteratorToHead();
    open.setIteratorToHead();
    closed.setIteratorToHead();
    while  (open.isIteratorValid())
    {
        if (successors.isInList(open.getIteratorValue()))
            successors.deleteNode(open.getIteratorValue());
        open.iteratorNext();
    }
    while (closed.isIteratorValid())
    {
        if (successors.isInList(closed.getIteratorValue()))
            successors.deleteNode(closed.getIteratorValue());
        closed.iteratorNext();
    }*/
}

void Search::clearLists()
{
    while (!open.isEmpty())
        delete open.deleteFromHead();
    while (!closed.isEmpty())
        delete closed.deleteFromHead();
    while (!priorityOpen.isEmpty())
        delete priorityOpen.removeMin();
    if (knownStates != nullptr)
    {   // NOTA: Porque passei nullptr no valor do no. Caso contrario, era necessário desalocar a memoria (aqui OU em open e close).
        knownStates->clear();        
        delete knownStates;
        knownStates = nullptr;
    }   
}
