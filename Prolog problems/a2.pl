
limiteTablero(X, L, R) :- L =< X, X =< R.

unPaso([Es1, Es2], [Sig1, Sig2]) :-
    member([PasoX, PasoY], [[1,2],[1,-2],[-1,2],[-1,-2],[2,1],[2,-1],[-2,1],[-2,-1]]),
    Sig1 is Es1 + PasoX,
    limiteTablero(Sig1, 1, 9), %% 9 es el tamaño del tablero
    Sig2 is Es2 + PasoY,
    limiteTablero(Sig2, 1, 9).


camino(E,E,C,C):- length(C,15).
camino(EstadoActual, EstadoFinal, CaminoHastaAhora, CaminoTotal ):-  
  unPaso( EstadoActual, EstSiguiente),
  \+member(EstSiguiente,CaminoHastaAhora),
  length(CaminoHastaAhora,N),
  N < 15, %% pasos del caballo
  camino( EstSiguiente, EstadoFinal, [EstSiguiente|CaminoHastaAhora], CaminoTotal).

% #\+ no pertany
solucionOptima:-
  camino([0,0],[7,1] ,[[0,0]] , C),
  reverse(C, R),
  write(R).
  
