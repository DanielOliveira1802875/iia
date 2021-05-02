
#include "Search.h"
#include "NumberLink.h"

int main()
{
    //     INSTANCIA EXEMPLO
    /*char state[] = "...D....C..BE....CA.....E............A....B...D..";
    NumberLink* test = new NumberLink(state, 7, 7);*/

   /*std::cout << "\n#############################\n        INSTANCIA 1\n#############################";
    char inst1[] = "AB.....AB";
    NumberLink* state1 = new NumberLink(inst1, 3, 3);
    Search search1(state1);
    delete state1;*/


    //std::cout << "\n#############################\n        INSTANCIA 2\n#############################";
    //char inst2[] = "A.B...B.A";
    //NumberLink* state2 = new NumberLink(inst2, 3, 3);
    //Search search2(state2);
    //delete state2;

    //std::cout << "\n#############################\n        INSTANCIA 3\n#############################";
    //char inst3[] = "AB.CD........AB.CD";
    //NumberLink* state3 = new NumberLink(inst3, 3, 6);
    //Search search3(state3);
    //delete state3;


    /*std::cout << "\n#############################\n        INSTANCIA 4\n#############################";
    char inst4[] = "AB..CD...........................AB...CD";
    NumberLink* state4 = new NumberLink(inst4, 4, 10, Heuristic::bestFs);
    Search search4(state4);
    delete state4;*/


    /*std::cout << "\n#############################\n        INSTANCIA 5\n#############################";
    char inst5[] = "............A....B....B.CD.A..C........D";
    NumberLink* state5 = new NumberLink(inst5, 4, 10, Heuristic::bestFs);
    Search search5(state5);
    delete state5;*/


    //std::cout << "\n#############################\n        INSTANCIA 6\n#############################";
    //char inst6[] = "D........C..A....B....B....A..C........D";
    //NumberLink* state6 = new NumberLink(inst6, 4, 10);
    //Search search6(state6);
    //delete state6;


    /*std::cout << "\n#############################\n        INSTANCIA 7\n#############################";
    char inst7[] = "C..............D.BA............DA........E...B..E.......C...";
    NumberLink* state7 = new NumberLink(inst7, 6, 10,Heuristic::bestFs);
    Search search7(state7);
    delete state7;*/


    //std::cout << "\n#############################\n        INSTANCIA 8\n#############################";
    //char inst8[] = "............A....B....B.CD.A..C........D............E....F....F.GH.E..G........H";
    //NumberLink* state8 = new NumberLink(inst8, 8, 10);
    //Search search8(state8);
    //delete state8;



    /*std::cout << "\n#############################\n        INSTANCIA 9\n#############################";
    char inst9[] = "...D....C..BE....CA.....E............A....B...D..";
    NumberLink* state9 = new NumberLink(inst9, 7, 7, Heuristic::bestFs);
    Search search9(state9);
    delete state9;*/



    /*std::cout << "\n#############################\n        INSTANCIA 10\n#############################";    
    char inst10[] = "............BC..D......D.E..............F.E.G........................H..ABCAHF..G";
    NumberLink* state10 = new NumberLink(inst10, 9, 9, Heuristic::aStar);
    Search search10(state10);
    delete state10;*/



    /*std::cout << "\n#############################\n        INSTANCIA 11\n#############################";
    char inst11[] = "..........A..BA.HI..........I....G..F.E...F..H..JCBE........C.....G.J.D..................D..........";
    NumberLink* state11 = new NumberLink(inst11, 10, 10, Heuristic::aStar);
    Search search11(state11);
    delete state11;*/


    std::cout << "\n#############################\n        INSTANCIA 12\n#############################";    
    char inst12[] = "AD............C..H...E......B.B.H..............................G........GF.....................A....D...F...E....C.......";
    NumberLink* state12 = new NumberLink(inst12, 11, 11,Heuristic::aStar);
    Search search12(state12);
    delete state12;

    /*std::cout << "\n#############################\n        INSTANCIA 12\n#############################";
    char inst13[] = "..@......@......@..A.....@......@......@..@.....@......@....@....@.@....@.....@...@......@....@.........@.@....@..@.@..@..@.........@.......@......@.@....@..@.....@.@..@...@......@.@..@..@..@.@..@.......@....@.....@...@...@...@....@....@.......@....@....@...@...@....@....@....@...@....@....@....@...@....@....@....@......@....@....@....@...@....@....@....@...@.......@....@....@.A.@....@....@....@..";
    NumberLink* state13 = new NumberLink(inst13, 20, 20, Heuristic::aStar);
    Search search13(state13);
    delete state13;*/
    
    return 0;
}


