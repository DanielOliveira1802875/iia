#include "NumberLink.h"

#include <cstring>


int NumberLink::stateSize = 0;
int NumberLink::qntColumns = 0;
int NumberLink::qntLines = 0;
Letter NumberLink::numbers[26];
const char NumberLink::outOfBoundsChar;


NumberLink::NumberLink()
{
    numbersRemaining = currentNumber = 0;
    pathHead = pathRoot = stateSize;
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
    if (position < 0 || position >= stateSize || state[position] != '.')
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
        isPositionValid = endPosition > -1 && endPosition < stateSize;
        break;
    case Direction::down:
        endPosition += qntColumns;
        isPositionValid = endPosition > -1 && endPosition < stateSize;
        break;
    case Direction::left:
        endPosition -= 1;
        isPositionValid = endPosition / qntColumns == line && endPosition > -1 && endPosition < stateSize;
        break;
    case Direction::right:
        endPosition += 1;
        isPositionValid = endPosition / qntColumns == line && endPosition > -1 && endPosition < stateSize;
        break;
    }
    if (isPositionValid)
        return &(someState[endPosition]);
    endPosition = stateSize;
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
    newState->state = new char[static_cast<size_t>(stateSize) + 2];
    memcpy(newState->state, state, static_cast<size_t>(stateSize) + 2);
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
            pathRoot = pathHead = numbers[currentNumber].position;
            maskPathRoot();
            setPosAroundPathHead();
            return;
        }
    pathRoot = pathHead = stateSize;
}


// Inicia os atributos e prepara-se para conetar a letra A
NumberLink::NumberLink(char* state, int qntLines_, int qntColumns_)
{
    if (stateSize != static_cast<int>(strlen(state)))
    {
        qntColumns = qntColumns_;
        qntLines = qntLines_;
        stateSize = qntColumns * qntLines;
    }
    if (stateSize != static_cast<int>(strlen(state)))
        throw std::invalid_argument(
            "NumberLink::NumberLink() O tamanho da string nao corresponde ao tamaho do estado.\n");

    this->state = new char[static_cast<size_t>(stateSize) + 2];
    memcpy(this->state, state, static_cast<size_t>(stateSize));  
    
    this->state[stateSize] = outOfBoundsChar;
    this->state[stateSize + 1] = '\0';

    numbersRemaining = 0;
    currentNumber = 0;
    pathRoot = stateSize;
    pathHead = stateSize;

    for (short i = 0; i < 26; ++i)
    {
        numbers[i].upperLetter = static_cast<char>(A + i);
        numbers[i].lowerLetter = static_cast<char>(a + i);
        numbers[i].position = 0;
        connected[i] = true;
    }

    for (int i = 0; i < stateSize; i++)
        if (isalpha(state[i]))
        {
            numbers[state[i] - A].position = i;
            connected[state[i] - A] = false;
            ++numbersRemaining;
        }
    if (numbersRemaining % 2 != 0)
        throw std::invalid_argument("NumberLink::NumberLink() O numero de letras deve ser par\n");
    numbersRemaining /=2;
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

bool NumberLink::operator==(Node& node) { return strcmp(state, ((NumberLink&)node).state) == 0; }

bool NumberLink::isSolution() { return isConnected() && numbersRemaining == 0; }

void NumberLink::resetState()
{
    // nao e necessario, uma vez que os estados sao estaticos
}


// Gera os sucessores
void NumberLink::genSuccessors(DLList<Node*>& successors)
{
    if (state[pathRoot] == outOfBoundsChar)
        return;
    // Se a letra atual esta conexa, avanca para a proxima.
    // Caso nao exista proxima letra termina a funçao.
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
        if (successor->isSelfConnectingPath() || successor->is360() || successor->isDeadState())
            delete successor;
        else
            successors.addToTail((Node*)successor);
    }
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
bool NumberLink::canConnect(char character, char* stateCopy, int startPosition)
{
    stateCopy[startPosition] = '#';
    int upPos, downPos, leftPos, rightPos;
    const char* upChar = look(Direction::up, startPosition, stateCopy, upPos);
    const char* leftChar = look(Direction::left, startPosition, stateCopy, leftPos);
    const char* rightChar = look(Direction::right, startPosition, stateCopy, rightPos);
    const char* downChar = look(Direction::down, startPosition, stateCopy, downPos);

    const bool characterFound = (*upChar == character) || (*leftChar == character) ||
                                (*rightChar == character) || (*downChar == character);

    if (characterFound) return true;
    if (*downChar == '.' && canConnect(character, stateCopy, downPos)) return true;
    if (*rightChar == '.' && canConnect(character, stateCopy, rightPos)) return true;
    if (*leftChar == '.' && canConnect(character, stateCopy, leftPos)) return true;
    if (*upChar == '.' && canConnect(character, stateCopy, upPos)) return true;

    return false;
}

// Verifica se e possivel conectar as restantes letras
bool NumberLink::isDeadState()
{
    bool isDead = false;
    char* stateCpy = new char[static_cast<size_t>(stateSize) + 1];

    // Testa todas as letras posteriores a atual (letras nao conexas)
    const int nextLetterIndex = currentNumber + 1;
    for (int i = nextLetterIndex; i < totalNumbers && !isDead; i++)
    {
        if (connected[i])
            continue;
        const char nextLetter = numbers[i].upperLetter;
        const int position = numbers[i].position;
        memcpy(stateCpy, state, static_cast<size_t>(stateSize) + 1);
        isDead = !canConnect(nextLetter, stateCpy, position);
    }
    delete[] stateCpy;
    return isDead;
}
