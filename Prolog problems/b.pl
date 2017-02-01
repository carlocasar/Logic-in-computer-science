
writeP([]).
writeP([X,Y|S]) :- write(Y), write(' jumps over '), write(X), nl, writeP(S).


sumJump([X1,X2],[Y1,Y2],[R1,R2]) :- R1 is X1+Y1, R2 is X2+Y2.
halfJump([X,Y],[RX,RY]) :- RX is X>>1, RY is Y>>1.

betweenchk(X, L, R) :- L =< X, X =< R.
limit([X,Y]) :- X >= Y, betweenchk(X, 1, 5), betweenchk(Y, 1, 5).

puzzle(Tablero, T, T) :- length(Tablero,1). 
puzzle(Tablero, T, Camino) :-
    select(Ball1, Tablero, TableroP), 
    member(Jump, [[0,2],[0,-2],[2,0],[-2,0],[2,2],[-2,-2]]), 

    sumJump(Ball1, Jump, JumpPos), 
    limit(JumpPos), 
    \+member(JumpPos, TableroP),
    
    halfJump(Jump, JumpHalf),
    sumJump(Ball1, JumpHalf, Ball2), 
    select(Ball2, TableroP, TableroPP),
    puzzle([JumpPos|TableroPP], [Ball1,Ball2|T], Camino).

solve(Tablero) :-
    puzzle(Tablero, [], Camino),
    reverse(Camino, F),
    writeP(F).