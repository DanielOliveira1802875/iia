#include "NumberLink.h"

#include <cstring>


int NumberLink::stateSize = 0;
int NumberLink::qntColumns = 0;
int NumberLink::qntLines = 0;
Letter NumberLink::letters[26];
const char NumberLink::outOfBoundsChar;


NumberLink::NumberLink()
{
    lettersRemaining = currentLetterIndex = pathPosition = 0;
    state = nullptr;
    currentLetter = nullptr;
    currentLetterLowerCase = outOfBoundsChar;
    currentLetterUpperCase = outOfBoundsChar;
}

void NumberLink::maskCurrentLetter()
{
    if (currentLetter == nullptr)
        return;
    *currentLetter = maskChar;
}

void NumberLink::unMaskCurrentLetter()
{
    if (currentLetter == nullptr)
        return;
    *currentLetter = currentLetterUpperCase;
}

void NumberLink::moveTo(int position)
{
    if (position < 0 || position >= stateSize || state[position] != '.')
        throw std::invalid_argument("NumberLink::moveTo() Posicao invalida!");
    pathPosition = position;
    state[position] = currentLetterLowerCase;
    setPosAroundPath();
}

bool NumberLink::isConnected()
{
    for (int i = 0; i < 4; ++i)
        if (state[posAroundPath[i]] == currentLetterUpperCase)
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


void NumberLink::setPosAroundPath()
{
    int upPos, downPos, leftPos, rightPos;
    look(Direction::up, pathPosition, state, upPos);
    look(Direction::down, pathPosition, state, downPos);
    look(Direction::left, pathPosition, state, leftPos);
    look(Direction::right, pathPosition, state, rightPos);
    posAroundPath[0] = upPos;
    posAroundPath[1] = leftPos;
    posAroundPath[2] = rightPos;
    posAroundPath[3] = downPos;
}


Node* NumberLink::getClone()
{
    NumberLink* newState = new NumberLink();
    newState->state = new char[static_cast<size_t>(stateSize) + 1];
    memcpy(newState->state, state, static_cast<size_t>(stateSize) + 1);
    memcpy(newState->posAroundPath, posAroundPath, sizeof(int) * 4);
    memcpy(newState->connected, connected, sizeof(bool) * 26);
    newState->pathPosition = pathPosition;
    newState->lettersRemaining = lettersRemaining;
    newState->currentLetterIndex = currentLetterIndex;
    newState->currentLetterLowerCase = currentLetterLowerCase;
    newState->currentLetterUpperCase = currentLetterUpperCase;
    newState->currentLetter = &(newState->state[letters[currentLetterIndex].position]);
    return static_cast<Node*>(newState);
}

void NumberLink::setNextLetter()
{
    unMaskCurrentLetter();
    for (; currentLetterIndex < numOfLetters; currentLetterIndex++)
        if (!connected[currentLetterIndex])
        {
            pathPosition = letters[currentLetterIndex].position;
            currentLetterLowerCase = letters[currentLetterIndex].lowerLetter;
            currentLetterUpperCase = letters[currentLetterIndex].upperLetter;
            currentLetter = &state[pathPosition];
            maskCurrentLetter();
            setPosAroundPath();
            return;
        }
    currentLetter = nullptr;
}


// Inicia os parametros e prepara-se para conetar a letra A
NumberLink::NumberLink(char* state, int qntLines_, int qntColumns_)
{
    if (stateSize == 0)
    {
        qntColumns = qntColumns_;
        qntLines = qntLines_;
        stateSize = qntColumns * qntLines;
    }
    if (stateSize != static_cast<int>(strlen(state)))
        throw std::invalid_argument(
            "NumberLink::NumberLink() O tamanho da string nao corresponde ao tamaho do estado.\n");

    this->state = new char[static_cast<size_t>(stateSize) + 1];
    memcpy(this->state, state, static_cast<size_t>(stateSize) + 1);

    lettersRemaining = 0;
    currentLetter = nullptr;
    currentLetterLowerCase = outOfBoundsChar;
    currentLetterUpperCase = outOfBoundsChar;
    currentLetterIndex = 0;

    for (int i = 0; i < 26; ++i)
    {
        letters[i].upperLetter = static_cast<char>(A + i);
        letters[i].lowerLetter = static_cast<char>(a + i);
        letters[i].position = 0;
        connected[i] = true;
    }

    for (int i = 0; i < stateSize - 1; i++)
        if (isalpha(state[i]) && connected[toupper(state[i] - A)] == true)
        {
            letters[state[i] - A].position = i;
            connected[state[i] - A] = false;
            ++lettersRemaining;
        }
    --lettersRemaining;
    setNextLetter();
}

NumberLink::~NumberLink() { delete[] state; }

std::string NumberLink::toString()
{
    unMaskCurrentLetter();
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
    maskCurrentLetter();
    return returnValue;
}

bool NumberLink::operator==(Node& node) { return strcmp(state, ((NumberLink&)node).state) == 0; }

bool NumberLink::isSolution() { return isConnected() && lettersRemaining == 0; }

void NumberLink::resetState()
{
    // nao e necessario uma vez que os estados sao estaticos
}


// Gera os sucessores
void NumberLink::genSuccessors(DLList<Node*>& successors)
{
    /*if (currentLetter == nullptr) 
        return;*/
    // Se a letra atual esta conexa, avanca para a proxima.
    // Caso nao exista proxima letra termina a funçao.
    if (isConnected())
    {
        connected[currentLetterIndex] = true;
        --lettersRemaining;
        setNextLetter();
        if (currentLetter == nullptr)
            return;
    }

    for (int i = 0; i < 4; i++)
    {
        if (state[posAroundPath[i]] != '.')
            continue;
        NumberLink* successor = (NumberLink*)getClone();
        successor->moveTo(posAroundPath[i]);
        if (successor->isSelfConnectingPath() ||  successor->is360() || successor->isDeadState() )
            delete successor;
        else
            successors.addToTail((Node*)successor);
    }
}

// Verifica se o caminho esta em contacto com ele proprio
// Caso esteja, o percurso nao é o mais eficiente
bool NumberLink::isSelfConnectingPath()
{
    //return false;
    *currentLetter = currentLetterLowerCase;
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (state[posAroundPath[i]] == currentLetterLowerCase)
            count++;
    }
    maskCurrentLetter();
    return count > 1;
}


bool NumberLink::is360()
{
    //return false;

    *currentLetter = currentLetterLowerCase;
    bool flag = false;
    for (int i = 0; i < 4 && !flag; i++)
    {
        if (state[posAroundPath[i]] != '.') continue;
        int nextPosition;
        int discard;
        if (*look(Direction::left, posAroundPath[i], state, discard) == currentLetterLowerCase &&
            *look(Direction::right, posAroundPath[i], state, discard) == currentLetterLowerCase)
        {
            if (*look(Direction::down, posAroundPath[i], state, nextPosition) == currentLetterLowerCase &&
                *look(Direction::left, nextPosition, state, discard) == currentLetterLowerCase &&
                *look(Direction::right, nextPosition, state, discard) == currentLetterLowerCase)
                flag = true;
            else if (*look(Direction::up, posAroundPath[i], state, nextPosition) == currentLetterLowerCase &&
                     *look(Direction::left, nextPosition, state, discard) == currentLetterLowerCase &&
                     *look(Direction::right, nextPosition, state, discard) == currentLetterLowerCase)
                flag = true;
        }
        else if (*look(Direction::up, posAroundPath[i], state, discard) == currentLetterLowerCase &&
                 *look(Direction::down, posAroundPath[i], state, discard) == currentLetterLowerCase)
        {
            if (*look(Direction::left, posAroundPath[i], state, nextPosition) == currentLetterLowerCase &&
                *look(Direction::up, nextPosition, state, discard) == currentLetterLowerCase &&
                *look(Direction::down, nextPosition, state, discard) == currentLetterLowerCase)
                flag = true;
            else if (*look(Direction::right, posAroundPath[i], state, nextPosition) == currentLetterLowerCase &&
                     *look(Direction::up, nextPosition, state, discard) == currentLetterLowerCase &&
                     *look(Direction::down, nextPosition, state, discard) == currentLetterLowerCase)
                flag = true;
        }
    }
    maskCurrentLetter();    
    return flag;
}

// Funcao recursiva que tenta alcancar um caracter apartir de uma posicao.
// IMPORTANTE: NAO passar o estado original, esta funcao e destrutiva.
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

// Faz uma copia do estado atual e verifica se é possivel conectar as restantes letras
bool NumberLink::isDeadState()
{
    //return false;
    bool isDead = false;
    char* stateCpy = new char[static_cast<size_t>(stateSize) + 1];

    // Testa todas as letras posteriores a atual (letras nao conexas)
    const int nextLetterIndex = currentLetterIndex + 1;
    for (int i = nextLetterIndex; i < numOfLetters && !isDead; i++)
    {
        if (connected[i])
            continue;
        const char nextLetter = letters[i].upperLetter;
        const int position = letters[i].position;
        memcpy(stateCpy, state, static_cast<size_t>(stateSize) + 1);
        isDead = !canConnect(nextLetter, stateCpy, position);
    }
    delete[] stateCpy;
    return isDead;
}
