#include "NumberLink.h"
#include <limits>
#include <cstring>
#include <algorithm>

#include "MinHeapPtr.h"


int NumberLink::outOfBoundsPosition = 0;
int NumberLink::qntColumns = 0;
int NumberLink::qntLines = 0;
Letter NumberLink::numbers[26];
const char NumberLink::outOfBoundsChar;
PriorityValue Node::priority;


NumberLink::NumberLink()
{
    numbersRemaining = currentNumber = 0;
    pathHead = pathRoot = outOfBoundsPosition;
    state = nullptr;
}

void NumberLink::maskPathRoot()
{
    if (state[pathRoot] == outOfBoundsChar)
        return;
    state[pathRoot] = maskChar;
}

void NumberLink::unmaskPathRoot()
{
    if (state[pathRoot] == outOfBoundsChar)
        return;
    state[pathRoot] = numbers[currentNumber].upperLetter;
}

void NumberLink::moveTo(int position)
{
    if (position < 0 || position >= outOfBoundsPosition || state[position] != '.')
        throw std::invalid_argument("NumberLink::moveTo() Posicao invalida!");
    pathHead = position;
    state[position] = numbers[currentNumber].lowerLetter;
    setPosAroundPathHead();
}

bool NumberLink::isConnected()
{
    for (int i = 0; i < 4; ++i)
        if (state[aroundPathHead[i]] == numbers[currentNumber].upperLetter)
            return true;
    return false;
}

const char* NumberLink::look(Direction direction, int startPosition, const char* someState, int& endPosition)
{
    bool isPositionValid = false;
    int line = startPosition / qntColumns;
    endPosition = startPosition;
    switch (direction)
    {
    case Direction::up:
        endPosition -= qntColumns;
        isPositionValid = endPosition > -1 && endPosition < outOfBoundsPosition;
        break;
    case Direction::down:
        endPosition += qntColumns;
        isPositionValid = endPosition > -1 && endPosition < outOfBoundsPosition;
        break;
    case Direction::left:
        endPosition -= 1;
        isPositionValid = endPosition / qntColumns == line && endPosition > -1 && endPosition < outOfBoundsPosition;
        break;
    case Direction::right:
        endPosition += 1;
        isPositionValid = endPosition / qntColumns == line && endPosition > -1 && endPosition < outOfBoundsPosition;
        break;
    }
    if (isPositionValid)
        return &(someState[endPosition]);
    endPosition = outOfBoundsPosition;
    return &outOfBoundsChar;
}


void NumberLink::setPosAroundPathHead()
{
    int upPos, downPos, leftPos, rightPos;
    look(Direction::up, pathHead, state, upPos);
    look(Direction::down, pathHead, state, downPos);
    look(Direction::left, pathHead, state, leftPos);
    look(Direction::right, pathHead, state, rightPos);
    aroundPathHead[0] = upPos;
    aroundPathHead[1] = leftPos;
    aroundPathHead[2] = rightPos;
    aroundPathHead[3] = downPos;
}

// Devolve um clone deste estado
Node* NumberLink::getClone()
{
    NumberLink* newState = new NumberLink();
    newState->state = new char[static_cast<size_t>(outOfBoundsPosition) + 2];
    memcpy(newState->state, state, static_cast<size_t>(outOfBoundsPosition) + 2);
    memcpy(newState->aroundPathHead, aroundPathHead, sizeof(int) * 4);
    memcpy(newState->connected, connected, sizeof(bool) * 26);
    newState->pathHead = pathHead;
    newState->numbersRemaining = numbersRemaining;
    newState->currentNumber = currentNumber;
    newState->pathRoot = pathRoot;
    return static_cast<Node*>(newState);
}

void NumberLink::setNextNumber()
{
    unmaskPathRoot();
    for (; currentNumber < totalNumbers; currentNumber++)
        if (!connected[currentNumber])
        {
            pathRoot = pathHead = numbers[currentNumber].position2;
            maskPathRoot();
            setPosAroundPathHead();
            return;
        }
    pathRoot = pathHead = outOfBoundsPosition;
}


// Inicia todos os atributos e prepara-se para conetar a letra A
NumberLink::NumberLink(char* state, int qntLines_, int qntColumns_, PriorityValue _heuristicType)
{
    if (outOfBoundsPosition != static_cast<int>(strlen(state)))
    {
        qntColumns = qntColumns_;
        qntLines = qntLines_;
        outOfBoundsPosition = qntColumns * qntLines;
    }
    if (outOfBoundsPosition != static_cast<int>(strlen(state)))
        throw std::invalid_argument(
            "NumberLink::NumberLink() O tamanho da string nao corresponde ao tamaho do estado.\n");

    priority = _heuristicType;
    this->state = new char[static_cast<size_t>(outOfBoundsPosition) + 2];
    memcpy(this->state, state, static_cast<size_t>(outOfBoundsPosition));

    this->state[outOfBoundsPosition] = outOfBoundsChar;
    this->state[outOfBoundsPosition + 1] = '\0';

    numbersRemaining = 0;
    currentNumber = 0;
    pathRoot = outOfBoundsPosition;
    pathHead = outOfBoundsPosition;

    for (short i = 0; i < 26; ++i)
    {
        numbers[i].upperLetter = static_cast<char>(A + i);
        numbers[i].lowerLetter = static_cast<char>(a + i);
        numbers[i].position2 = outOfBoundsPosition;
        numbers[i].position1 = outOfBoundsPosition;
        numbers[i].manhattanDistance = 0;
        connected[i] = true;
    }

    for (int i = 0; i < outOfBoundsPosition; i++)
        if (isalpha(state[i]))
        {
            if (numbers[state[i] - A].position1 == outOfBoundsPosition)
                numbers[state[i] - A].position1 = i;
            else if (numbers[state[i] - A].position2 == outOfBoundsPosition)
            {   // Temos agora as duas posicoes, podemos calcular a distancia Manhattan
                numbers[state[i] - A].position2 = i;
                if (priority != PriorityValue::cost)
                    numbers[state[i] - A].manhattanDistance = calcManhattanDistance(numbers[state[i] - A].position1,
                        numbers[state[i] - A].position2);
            }
            else
                throw std::invalid_argument("NumberLink::NumberLink() As letras deve ser pares\n");
            connected[state[i] - A] = false;
            ++numbersRemaining;
        }

    if (numbersRemaining % 2 != 0)
        throw std::invalid_argument("NumberLink::NumberLink() As letras deve ser pares\n");
    numbersRemaining /= 2;
    --numbersRemaining;
    setNextNumber();
}

NumberLink::~NumberLink() { delete[] state; }

std::string NumberLink::toString()
{
    unmaskPathRoot();
    std::string returnValue = "\n";
    for (int i = 0; i < qntLines; i++)
    {
        for (int j = 0; j < qntColumns; j++)
        {
            returnValue += state[qntColumns * i + j];
            returnValue += ' ';
        }
        returnValue += '\n';
    }
    maskPathRoot();
    return returnValue;
}

std::string NumberLink::toString(char* state_)
{
    unmaskPathRoot();
    std::string returnValue = "\n";
    for (int i = 0; i < qntLines; i++)
    {
        for (int j = 0; j < qntColumns; j++)
        {
            returnValue += state_[qntColumns * i + j];
            returnValue += ' ';
        }
        returnValue += '\n';
    }
    maskPathRoot();
    return returnValue;
}

bool NumberLink::operator==(Node& node) { return strcmp(state, ((NumberLink&)node).state) == 0; }

bool NumberLink::isSolution() { return isConnected() && numbersRemaining == 0; }

void NumberLink::resetState()
{
    // nao e necessario, uma vez que os estados sao estaticos
}

int NumberLink::calcManhattanDistance(int startPosition, int endPosition)
{
    const int horizontalDistance = abs(startPosition % qntColumns - endPosition % qntColumns);
    const int verticalDistance = abs(startPosition / qntColumns - endPosition / qntColumns);
    // o objetivo � estar conexo com o destino e n�o coincidir com ele, dai o -1
    return horizontalDistance + verticalDistance - 1;
}

int NumberLink::remainingManhattanDistances()
{
    int returnValue = 0;
    for (int i = currentNumber + 1; i < 26; i++)
        returnValue += numbers[i].manhattanDistance;
    return returnValue;
}

void NumberLink::updateSuccessorStats(NumberLink* successor)
{
    if (priority == PriorityValue::cost) return;
    successor->heuristic = calcManhattanDistance(successor->pathHead, numbers[currentNumber].position1);
    successor->heuristic += remainingManhattanDistances();
}


// Gera os sucessores
void NumberLink::genSuccessors(DLList<Node*>& successors)
{
    if (state[pathRoot] == outOfBoundsChar)
        return;
    // Se a letra atual esta conexa, avanca para a proxima.
    // Caso nao exista proxima letra termina a fun�ao.
    if (isConnected())
    {
        --numbersRemaining;
        connected[currentNumber] = true;
        setNextNumber();
        if (state[pathRoot] == outOfBoundsChar)
            return;
    }

    for (int i = 0; i < 4; i++)
    {
        if (state[aroundPathHead[i]] != '.')
            continue;
        NumberLink* successor = (NumberLink*)getClone();
        successor->moveTo(aroundPathHead[i]);
        if (successor->isSelfConnectingPath() || successor->is360V2() || successor->isDeadState())
            delete successor;
        else
        {
            updateSuccessorStats(successor);
            successors.addToTail((Node*)successor);
        }
    }
}

int NumberLink::goBack(int position, int places, char* stateCpy)
{
    if (isOutOfBounds(position))
        return outOfBoundsPosition;
    int upPos, downPos, leftPos, rightPos;
    for (int i = 0; i < places; i++)
    {
        stateCpy[position] = '#';
        //std::cout << toString(stateCpy);
        if (*look(Direction::left, position, stateCpy, leftPos) == state[pathHead])
            position = leftPos;
        else if (*look(Direction::right, position, stateCpy, rightPos) == state[pathHead])
            position = rightPos;
        else if (*look(Direction::up, position, stateCpy, upPos) == state[pathHead])
            position = upPos;
        else if (*look(Direction::down, position, stateCpy, downPos) == state[pathHead])
            position = downPos;
        else return outOfBoundsPosition;
    }

    return position;
}

bool NumberLink::is360V2()
{
    char* stateCpy = new char[static_cast<size_t>(outOfBoundsPosition) + 2];
    memcpy(stateCpy, state, static_cast<size_t>(outOfBoundsPosition) + 2);
    stateCpy[pathRoot] = state[pathHead];
    const int back4Pos = goBack(pathHead, 4, stateCpy);

    if (!isOutOfBounds(back4Pos) && calcManhattanDistance(pathHead, back4Pos) == 1 && state[(pathHead + back4Pos) / 2]
        == '.')
    {
        //std::cout << toString(stateCpy);
        delete[] stateCpy;
        return true;
    }
    const int back5Pos = goBack(back4Pos, 1, stateCpy);
    if (!isOutOfBounds(back5Pos) && calcManhattanDistance(pathHead, back5Pos) == 2)
    {
        const int minPos = pathHead < back5Pos ? pathHead : back5Pos;
        const int maxPos = minPos == pathHead ? back5Pos : pathHead;
        if (pathHead / qntColumns == back5Pos / qntColumns)
        {   // Mesma linha, verifica se os espa�os entre a curva estao vazios.
            if (stateCpy[minPos + 1] == '.' && stateCpy[minPos + 2] == '.' && minPos + 3 == maxPos)
            {
                //std::cout << toString(stateCpy);
                delete[] stateCpy;
                return true;
            }
        }
        else
        {   // mesma coluna, verifica se os espa�os entre a curva estao vazios.
            if (stateCpy[minPos + qntColumns] == '.' && stateCpy[minPos + qntColumns * 2] == '.' && minPos + qntColumns
                * 3 == maxPos)
            {
                //std::cout << toString(stateCpy);
                delete[] stateCpy;
                return true;
            }
        }
    }

    delete[] stateCpy;
    return false;
}

// Verifica se o caminho esta em contacto com ele proprio
bool NumberLink::isSelfConnectingPath()
{
    const char pathLetter = numbers[currentNumber].lowerLetter;
    state[pathRoot] = pathLetter;
    int count = 0;
    for (int i = 0; i < 4; ++i)
        if (state[aroundPathHead[i]] == pathLetter)
            count++;
    maskPathRoot();
    return count > 1;
}

// Verifica se o caminho faz curva de 360, com 1 espaco de intervalo (tipo U)
bool NumberLink::is360()
{
    const char pathLetter = numbers[currentNumber].lowerLetter;
    state[pathRoot] = pathLetter;
    bool flag = false;
    for (int i = 0; i < 4 && !flag; i++)
    {
        if (state[aroundPathHead[i]] != '.')
            continue;
        int nextPosition;
        int discard;
        // verifica verticalmente
        if (*look(Direction::left, aroundPathHead[i], state, discard) == pathLetter &&
            *look(Direction::right, aroundPathHead[i], state, discard) == pathLetter)
        {
            if ((*look(Direction::down, aroundPathHead[i], state, nextPosition) == pathLetter &&
                 *look(Direction::left, nextPosition, state, discard) == pathLetter &&
                 *look(Direction::right, nextPosition, state, discard) == pathLetter) ||
                (*look(Direction::up, aroundPathHead[i], state, nextPosition) == pathLetter &&
                 *look(Direction::left, nextPosition, state, discard) == pathLetter &&
                 *look(Direction::right, nextPosition, state, discard) == pathLetter))
                flag = true;
        }
            // verifica horizontalmente
        else if (*look(Direction::up, aroundPathHead[i], state, discard) == pathLetter &&
                 *look(Direction::down, aroundPathHead[i], state, discard) == pathLetter)
        {
            if ((*look(Direction::left, aroundPathHead[i], state, nextPosition) == pathLetter &&
                 *look(Direction::up, nextPosition, state, discard) == pathLetter &&
                 *look(Direction::down, nextPosition, state, discard) == pathLetter) ||
                (*look(Direction::right, aroundPathHead[i], state, nextPosition) == pathLetter &&
                 *look(Direction::up, nextPosition, state, discard) == pathLetter &&
                 *look(Direction::down, nextPosition, state, discard) == pathLetter))
                flag = true;
        }
    }
    maskPathRoot();
    return flag;
}

// Funcao recursiva que tenta alcancar um caracter apartir de uma posicao.
// IMPORTANTE: passar uma copia do estado, pois este e alterado.
bool NumberLink::canConnect(char* stateCopy, int startPosition, int endPosition)
{
    stateCopy[startPosition] = '#';
    //std::cout << toString(stateCopy);
    int upPos, downPos, leftPos, rightPos;
    const char* leftChar = look(Direction::left, startPosition, stateCopy, leftPos);
    const char* rightChar = look(Direction::right, startPosition, stateCopy, rightPos);
    const char* upChar = look(Direction::up, startPosition, stateCopy, upPos);
    const char* downChar = look(Direction::down, startPosition, stateCopy, downPos);

    const bool positionFound = (upPos == endPosition) || (leftPos == endPosition) ||
                               (rightPos == endPosition) || (downPos == endPosition);

    if (positionFound) return true;
    if (*upChar == '.' && canConnect(stateCopy, upPos, endPosition)) return true;
    if (*leftChar == '.' && canConnect(stateCopy, leftPos, endPosition)) return true;
    if (*rightChar == '.' && canConnect(stateCopy, rightPos, endPosition)) return true;
    if (*downChar == '.' && canConnect(stateCopy, downPos, endPosition)) return true;

    return false;
}

// Verifica se e possivel conectar as restantes letras
bool NumberLink::isDeadState()
{
    bool isDead = false;
    char* stateCpy = new char[static_cast<size_t>(outOfBoundsPosition) + 2];

    // Testa todas as letras restantes
    const int nextLetterIndex = currentNumber + 1;
    for (int i = nextLetterIndex; i < totalNumbers && !isDead; i++)
    {
        if (connected[i])
            continue;
        memcpy(stateCpy, state, static_cast<size_t>(outOfBoundsPosition) + 2);
        isDead = !canConnect(stateCpy, numbers[i].position1, numbers[i].position2);
    }
    delete[] stateCpy;
    return isDead;
}

bool NumberLink::isOutOfBounds(int position)
{
    return position < 0 ? true : (position >= outOfBoundsPosition ? true : false);
}

bool NumberLink::operator>(Node& node)
{
    int thisPriority = 0, otherPriority = 0;

    if (priority == PriorityValue::costPlusHeuristic)
    {
        thisPriority = cost + heuristic;
        otherPriority = node.cost + node.heuristic;
    }
    else if (priority == PriorityValue::heuristic)
    {
        thisPriority = heuristic;
        otherPriority = node.heuristic;
    }
    else if (priority == PriorityValue::cost)
    {
        thisPriority = cost;
        otherPriority = node.cost;
    }    
    if (thisPriority > otherPriority)
        return true;
    return false;
}

bool NumberLink::operator<(Node& node)
{
    return !(*this > node);
}

