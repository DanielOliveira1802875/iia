#pragma once
#include "Node.h"
using namespace std;


class NumberLink : public Node
{
    static int qntLines;
    static int qntColumns;
    static int stateSize;
    static const char outOfBoundsChar = '\0';
    static const char maskChar = '%';
    int lettersPosition[27];
    int maskPosition;
    char charToConnect;
    int livePosition;
    int numOfLettersRemaining;
    char* state;
    NumberLink();
    //int findChar(char character);
    void move(int position);
    bool isConnected();
    const char* upCharacter(int startPosition, const char* someState, int& endPosition);
    const char* downCharacter(int startPosition, const char* someState, int& endPosition);
    const char* leftCharacter(int startPosition, const char* someState, int& endPosition);
    const char* rightCharacter(int startPosition, const char* someState, int& endPosition);
    bool isSelfConnectingPath();
    bool canConnect(char character, char* stateCopy, int startPosition);
    bool isDeadState();
public:    
    Node* getClone() override;
    char getNextChar();
    NumberLink(char* state, int qntLines, int qntColumns);
    ~NumberLink() override;
    string toString() override;
    bool operator==(Node& node)override;
    bool isSolution() override;
    void resetState() override;
    void genSuccessors(DLList<Node*>& successors) override;    
};
