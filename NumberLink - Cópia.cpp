#include "NumberLink.h"

#include <cstring>
using namespace std;

int NumberLink::stateSize = 0;
int NumberLink::qntColumns = 0;
int NumberLink::qntLines = 0;

NumberLink::NumberLink()
{
    state = nullptr;
}

//int NumberLink::findChar(char character)
//{
//    for (size_t i = 0; i < stateSize; i++)
//        if (state[i] == character) return (int)i;
//    return -1;
//}


void NumberLink::move(int position)
{
    if (position < 0 || position >= stateSize || state[position] != '.')
        throw invalid_argument("NumberLink::move() Posicao invalida!");
    state[position] = tolower(charToConnect);
    livePosition = position;
}

bool NumberLink::isConnected()
{
    int endPositionDiscard;
    const char* rightChar = rightCharacter(livePosition, state, endPositionDiscard);
    const char* leftChar = leftCharacter(livePosition, state, endPositionDiscard);
    const char* upChar = upCharacter(livePosition, state, endPositionDiscard);
    const char* downChar = downCharacter(livePosition, state, endPositionDiscard);

    if (*upChar == charToConnect) return true;
    if (*rightChar == charToConnect) return true;
    if (*leftChar == charToConnect) return true;
    if (*downChar == charToConnect) return true;

    return false;
}

const char* NumberLink::upCharacter(int startPosition, const char* someState, int& endPosition)
{
    endPosition = startPosition - qntColumns;
    const bool isPositionValid = endPosition > -1 && endPosition < stateSize;
    if (isPositionValid)
        return &(someState[endPosition]);
    endPosition = -1;
    return &outOfBoundsChar;
}

const char* NumberLink::downCharacter(int startPosition, const char* someState, int& endPosition)
{
    endPosition = startPosition + qntColumns;
    const bool isPositionValid = endPosition > -1 && endPosition < stateSize;
    if (isPositionValid)
        return &(someState[endPosition]);
    endPosition = -1;
    return &outOfBoundsChar;
}

const char* NumberLink::leftCharacter(int startPosition, const char* someState, int& endPosition)
{
    endPosition = startPosition - 1;
    const bool isPositionValid = (endPosition / qntColumns) == (startPosition / qntColumns) &&
                                 endPosition > -1 && endPosition < stateSize;
    if (isPositionValid)
        return &(someState[endPosition]);
    endPosition = -1;
    return &outOfBoundsChar;
}

const char* NumberLink::rightCharacter(int startPosition, const char* someState, int& endPosition)
{
    endPosition = startPosition + 1;
    const bool isPositionValid = (endPosition / qntColumns) == (startPosition / qntColumns) &&
                                 endPosition > -1 && endPosition < stateSize;
    if (isPositionValid)
        return &(someState[endPosition]);
    endPosition = -1;
    return &outOfBoundsChar;
}


Node* NumberLink::getClone()
{
    NumberLink* newState = new NumberLink();
    newState->state = new char[stateSize + 2];
    memcpy(newState->state, state, stateSize + 2);
    memcpy(newState->lettersPosition, lettersPosition, sizeof(int) * 27);
    newState->livePosition = livePosition;
    newState->charToConnect = charToConnect;
    newState->maskPosition = maskPosition;
    newState->numOfLettersRemaining = numOfLettersRemaining;
    return static_cast<Node*>(newState);
}

char NumberLink::getNextChar()
{
    for (short i = 0; i < 26; i++)
        if (lettersPosition[i] != stateSize)
            return static_cast<char>('A' + i);
    return static_cast<char>('Z' + 1);
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

    this->state = new char[stateSize +2];    
    memcpy(this->state, state, stateSize+1);
    this->state[stateSize + 2] = '\0';
    numOfLettersRemaining = 0;
    for (int i = 0; i < 27 ; i++)    
        lettersPosition[i] = stateSize;
    for (int i = 0 ; i < stateSize - 1; i++)
        if(isalpha(state[i]) && lettersPosition[state[i] - 65] == stateSize)
        {
            lettersPosition[state[i] - 65] = i;
            ++numOfLettersRemaining;
        }
    --numOfLettersRemaining;
    charToConnect = getNextChar();
    livePosition = lettersPosition[charToConnect - 'A'];
    this->state[livePosition] = maskChar; // Para que não se conete a ele proprio
    maskPosition = livePosition;
}

NumberLink::~NumberLink() { delete[] state; }

string NumberLink::toString()
{
    string returnValue = "\n";
    for (int i = 0; i < qntLines; i++)
    {
        for (int j = 0; j < qntColumns; j++)
        {
            if (state[qntColumns * i + j] == maskChar)
                returnValue += charToConnect;
            else
                returnValue += state[qntColumns * i + j];
            returnValue += ' ';
        }
        returnValue += '\n';
    }
    return returnValue;
}

bool NumberLink::operator==(Node& node) { return strcmp(state, ((NumberLink&)node).state) == 0; }

bool NumberLink::isSolution() { return numOfLettersRemaining == 0; }

void NumberLink::resetState()
{
    // nao e necessario uma vez que os estados sao estaticos
}


// Gera os sucessores
void NumberLink::genSuccessors(DLList<Node*>& successors)
{
    // Se a letra atual esta conexa, avanca para a proxima.
    // Caso nao exista proxima letra termina a funçao.
    if (isConnected())
    {
        --numOfLettersRemaining;
        state[maskPosition] = charToConnect;
        lettersPosition[charToConnect - 'A'] = stateSize;
        charToConnect = getNextChar();        
        livePosition = lettersPosition[charToConnect - 'A'];
        if (livePosition == stateSize)
            return;
        state[livePosition] = maskChar;
        maskPosition = livePosition;
    }

    NumberLink* successor;
    int upPos, downPos, leftPos, rightPos;
    const char* upChar = upCharacter(livePosition, state, upPos);
    const char* downChar = downCharacter(livePosition, state, downPos);
    const char* leftChar = leftCharacter(livePosition, state, leftPos);
    const char* rightChar = rightCharacter(livePosition, state, rightPos);


    if (*rightChar == '.')
    {
        successor = (NumberLink*)getClone();
        successor->move(rightPos);
        if (successor->isSelfConnectingPath() || successor->isDeadState())
            delete successor;
        else
            successors.addToHead((Node*)successor);
    }
    if (*leftChar == '.')
    {
        successor = (NumberLink*)getClone();
        successor->move(leftPos);
        if (successor->isSelfConnectingPath() || successor->isDeadState())
            delete successor;
        else
            successors.addToHead((Node*)successor);
    }
    if (*downChar == '.')
    {
        successor = (NumberLink*)getClone();
        successor->move(downPos);
        if (successor->isSelfConnectingPath() || successor->isDeadState())
            delete successor;
        else
            successors.addToHead((Node*)successor);
    }
    if (*upChar == '.')
    {
        successor = (NumberLink*)getClone();
        successor->move(upPos);
        if (successor->isSelfConnectingPath() || successor->isDeadState())
            delete successor;
        else
            successors.addToHead((Node*)successor);
    }
}

// Verifica se o caminho esta em contacto com ele proprio
// Caso esteja, o percurso nao é o mais eficiente
bool NumberLink::isSelfConnectingPath()
{    
    state[maskPosition] = (char)tolower(charToConnect);
    int count = 0;
    int endPositionDiscard;

    if (state[livePosition] == *(upCharacter(livePosition, state, endPositionDiscard))) ++count;
    if (state[livePosition] == *(leftCharacter(livePosition, state, endPositionDiscard))) ++count;
    if (state[livePosition] == *(rightCharacter(livePosition, state, endPositionDiscard))) ++count;
    if (state[livePosition] == *(downCharacter(livePosition, state, endPositionDiscard))) ++count;

    state[maskPosition] = maskChar;
    return count > 1;
}


// Funcao recursiva que tenta alcancar um caracter apartir de uma posicao.
// IMPORTANTE: NAO passar o estado original, esta funcao e destrutiva.
bool NumberLink::canConnect(char character, char* stateCopy, int startPosition)
{
    stateCopy[startPosition] = '#';
    int upPos, downPos, leftPos, rightPos;
    const char* upChar = upCharacter(startPosition, stateCopy, upPos);
    const char* leftChar = leftCharacter(startPosition, stateCopy, leftPos);
    const char* rightChar = rightCharacter(startPosition, stateCopy, rightPos);
    const char* downChar = downCharacter(startPosition, stateCopy, downPos);

    const bool characterFound = (*upChar == character) || (*leftChar == character) ||
                                (*rightChar == character) || (*downChar == character);

    if (characterFound) return true;
    if (*rightChar == '.' && canConnect(character, stateCopy, rightPos)) return true;
    if (*leftChar == '.' && canConnect(character, stateCopy, leftPos)) return true;
    if (*downChar == '.' && canConnect(character, stateCopy, downPos)) return true;
    if (*upChar == '.' && canConnect(character, stateCopy, upPos)) return true;
    return false;
}

// Faz uma copia do estado atual e verifica se é possivel conectar as restantes letras
bool NumberLink::isDeadState()
{
    bool isDead = false;
    char* stateCpy = new char[stateSize + 1];
    // Testa todas as letras posteriores a atual (letras nao conexas)
    for (int i = 1; !isDead; i++)
    {
        char nextChar = (char)(charToConnect + i);
        const int startPosition = lettersPosition[nextChar - 'A'];
        if (startPosition == stateSize) 
            break;
        memcpy(stateCpy, state, stateSize + 1);
        isDead = !canConnect(nextChar, stateCpy, startPosition);        
    }
    delete[] stateCpy;
    return isDead;
}


