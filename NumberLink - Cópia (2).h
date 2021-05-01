#pragma once
#include "Node.h"


struct Letter
{
    char upperLetter;
    char lowerLetter;
    unsigned short position;
};

enum class Direction { up = 1, right, left , down };

class NumberLink : public Node
{
    static int qntLines;
    static int qntColumns;
    static int stateSize;
    const static int numOfLetters = 26;
    static const char outOfBoundsChar = '\0';
    static const char maskChar = '%';
    static const int A = 65;
    static const int a = 97;
    static Letter* letters;



    bool connected[numOfLetters] = {false};
    int posAroundHead[4];
    Letter* letterPtr;

    int currentLetterIndex;
    char currentLetterUpperCase;
    char currentLetterLowerCase;
    char* currentLetter;

    int pathPosition;

    int lettersRemaining;
    char* state;
    void setNextLetter();
    NumberLink();

    void moveTo(int position);
    bool isConnected();
    const char* look(Direction direction, int startPosition, const char* someState, int& endPosition);
    void setPosAround();
    bool isRedundant();
    bool is360();
    bool canConnect(char character, char* stateCopy, int startPosition);
    bool isDeadState();
public:
    Node* getClone() override;

    NumberLink(char* state, int qntLines, int qntColumns);
    ~NumberLink() override;
    std::string toString() override;
    bool operator==(Node& node) override;
    bool isSolution() override;
    void resetState() override;
    void genSuccessors(DLList<Node*>& successors) override;
};
