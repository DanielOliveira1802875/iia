#include "NumberLink.h"

#include <cstring>


int NumberLink::stateSize = 0;
int NumberLink::qntColumns = 0;
int NumberLink::qntLines = 0;
Letter* NumberLink::letters;
const char NumberLink::outOfBoundsChar;


NumberLink::NumberLink()
{
    lettersRemaining = currentLetterIndex = pathPosition = 0;
    state = nullptr;
    letterPtr = nullptr;
}

void NumberLink::moveTo(int position)
{
    if (position < 0 || position >= stateSize || state[position] != '.')
        throw std::invalid_argument("NumberLink::moveTo() Posicao invalida!");
    state[position] = letterPtr->lowerLetter;
    pathPosition = position;
    setPosAround();
}

bool NumberLink::isConnected()
{
    for (int i = 0; i < 4; ++i)
        if (state[posAroundHead[i]] == letterPtr->upperLetter)
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


void NumberLink::setPosAround()
{
    int upPos, downPos, leftPos, rightPos;
    look(Direction::up, pathPosition, state, upPos);
    look(Direction::down, pathPosition, state, downPos);
    look(Direction::left, pathPosition, state, leftPos);
    look(Direction::right, pathPosition, state, rightPos);
    posAroundHead[0] = upPos;
    posAroundHead[1] = leftPos;
    posAroundHead[2] = rightPos;
    posAroundHead[3] = downPos;
}


Node* NumberLink::getClone()
{
    NumberLink* newState = new NumberLink();
    newState->state = new char[static_cast<size_t>(stateSize) + 1];
    memcpy(newState->state, state, static_cast<size_t>(stateSize) + 1);
    memcpy(newState->posAroundHead, posAroundHead, sizeof(int) * 4);
    memcpy(newState->connected, connected, sizeof(bool) * 26);
    newState->pathPosition = pathPosition;
    newState->lettersRemaining = lettersRemaining;
    newState->currentLetterIndex = currentLetterIndex;
    newState->letterPtr = letterPtr;
    return static_cast<Node*>(newState);
}

void NumberLink::setNextLetter()
{
    if(letterPtr != nullptr)
        state[letterPtr->position] = letterPtr->upperLetter;
    for (; currentLetterIndex < numOfLetters; currentLetterIndex++)
        if (!connected[currentLetterIndex])
        {
            letterPtr = &letters[currentLetterIndex];
            pathPosition = letterPtr->position;
            state[pathPosition] = maskChar;
            setPosAround();
            return;
        }
    letterPtr = nullptr;
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

    letters = new Letter[26];
    this->state = new char[static_cast<size_t>(stateSize) + 1];
    memcpy(this->state, state, static_cast<size_t>(stateSize) + 1);

    for (int i = 0; i < 26; ++i)
    {
        letters[i].upperLetter = static_cast<char>(A + i);
        letters[i].lowerLetter = static_cast<char>(a + i);
        letters[i].position = 0;
        connected[i] = true;
    }
    lettersRemaining = 0;
    currentLetterIndex = 0;
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
    std::string returnValue = "\n";
    for (int i = 0; i < qntLines; i++)
    {
        for (int j = 0; j < qntColumns; j++)
        {
            if (state[qntColumns * i + j] == maskChar)
                returnValue += letterPtr->upperLetter;
            else
                returnValue += state[qntColumns * i + j];
            returnValue += ' ';
        }
        returnValue += '\n';
    }
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
    if (letterPtr == nullptr) return;
    // Se a letra atual esta conexa, avanca para a proxima.
    // Caso nao exista proxima letra termina a funçao.
    if (isConnected())
    {
        connected[currentLetterIndex] = true;
        --lettersRemaining;
        state[letterPtr->position] = letterPtr->upperLetter;
        connected[currentLetterIndex] = true;
        setNextLetter();
        if (letterPtr == nullptr)
            return;
    }

    for (int i = 0; i < 4; i++)
    {
        if (state[posAroundHead[i]] != '.')
            continue;
        NumberLink* successor = (NumberLink*)getClone();
        successor->moveTo(posAroundHead[i]);
        if (successor->isRedundant() || successor->isDeadState() || is360())
            delete successor;
        else
            successors.addToTail((Node*)successor);
    }

    /* NumberLink* successor;
     int upPos, downPos, leftPos, rightPos;
     const char* upChar = look(Direction::up, pathPosition, state, upPos);
     const char* downChar = look(Direction::down, pathPosition, state, downPos);
     const char* leftChar = look(Direction::left, pathPosition, state, leftPos);
     const char* rightChar = look(Direction::right, pathPosition, state, rightPos);
 
     if (*upChar == '.')
     {
         successor = (NumberLink*)getClone();
         successor->moveTo(upPos);
         if (successor->isRedundant() || successor->isDeadState() || is360())
             delete successor;
         else
             successors.addToTail((Node*)successor);
     }
     if (*leftChar == '.')
     {
         successor = (NumberLink*)getClone();
         successor->moveTo(leftPos);
         if (successor->isRedundant() || successor->isDeadState() || is360())
             delete successor;
         else
             successors.addToTail((Node*)successor);
     }
     if (*rightChar == '.')
     {
         successor = (NumberLink*)getClone();
         successor->moveTo(rightPos);
         if (successor->isRedundant() || successor->isDeadState() || is360())
             delete successor;
         else
             successors.addToTail((Node*)successor);
     }
     if (*downChar == '.')
     {
         successor = (NumberLink*)getClone();
         successor->moveTo(downPos);
         if (successor->isRedundant() || successor->isDeadState() || is360())
             delete successor;
         else
             successors.addToTail((Node*)successor);
     }*/
}

// Verifica se o caminho esta em contacto com ele proprio
// Caso esteja, o percurso nao é o mais eficiente
bool NumberLink::isRedundant()
{
    //return false;
    state[letterPtr->position] = letterPtr->lowerLetter;
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (state[posAroundHead[i]] == state[letterPtr->position])
            count++;
    }
    state[letterPtr->position] = maskChar;
    return count > 1;

    //state[letterPtr->position] = (char)tolower(letterPtr->upperLetter);
    //int count = 0;
    //bool flag = false;

    //int upPosition, leftPosition, rightPosition, downPosition;
    //const char* upChar = (look(Direction::up, pathPosition, state, upPosition));
    //const char* leftChar = (look(Direction::left, pathPosition, state, leftPosition));
    //const char* rightChar = (look(Direction::right, pathPosition, state, rightPosition));
    //const char* downChar = (look(Direction::down, pathPosition, state, downPosition));

    //if (state[pathPosition] == *upChar) ++count;
    //if (state[pathPosition] == *leftChar) ++count;
    //if (state[pathPosition] == *rightChar) ++count;
    //if (state[pathPosition] == *downChar) ++count;    

    ///*if (count > 1)
    //    flag = true;
    //else if (*upChar == '.' && is360(upPosition, state[pathPosition]))
    //    flag = true;
    //else if (*leftChar == '.' && is360(leftPosition, state[pathPosition]))
    //    flag = true;
    //else if (*rightChar == '.' && is360(rightPosition, state[pathPosition]))
    //    flag = true;
    //else if (*downChar == '.' && is360(downPosition, state[pathPosition]))
    //    flag = true;*/

    //state[letterPtr->position] = maskChar;
    //return count > 1;
}


bool NumberLink::is360()
{
    //return false;
    const char lookingFor = letterPtr->lowerLetter;
    state[letterPtr->position] = lookingFor;
    bool flag = false;

    for (int i = 0; i < 4 && !flag; i++)
    {
        if (state[posAroundHead[i]] != '.') continue;
        int nextPosition;
        int discard;
        if (*look(Direction::left, posAroundHead[i], state, discard) == lookingFor &&
            *look(Direction::right, posAroundHead[i], state, discard) == lookingFor)
        {
            if (*look(Direction::down, posAroundHead[i], state, nextPosition) == lookingFor &&
                *look(Direction::left, nextPosition, state, discard) == lookingFor &&
                *look(Direction::right, nextPosition, state, discard) == lookingFor)
                flag = true;
            else if (*look(Direction::up, posAroundHead[i], state, nextPosition) == lookingFor &&
                     *look(Direction::left, nextPosition, state, discard) == lookingFor &&
                     *look(Direction::right, nextPosition, state, discard) == lookingFor)
                flag = true;
        }
        else if (*look(Direction::up, posAroundHead[i], state, discard) == lookingFor &&
                 *look(Direction::down, posAroundHead[i], state, discard) == lookingFor)
        {
            if (*look(Direction::left, posAroundHead[i], state, nextPosition) == lookingFor &&
                *look(Direction::up, nextPosition, state, discard) == lookingFor &&
                *look(Direction::down, nextPosition, state, discard) == lookingFor)
                flag = true;
            else if (*look(Direction::right, posAroundHead[i], state, nextPosition) == lookingFor &&
                     *look(Direction::up, nextPosition, state, discard) == lookingFor &&
                     *look(Direction::down, nextPosition, state, discard) == lookingFor)
                flag = true;
        }
    }
    state[letterPtr->position] = maskChar;

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
