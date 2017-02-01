unPaso([_,Ea2], [0,Ea2]). % #buida,  
unPaso([_,Ea2], [5,Ea2]). % #emplena	

unPaso([Ea1,_], [Ea1,0]).
unPaso([Ea1,_], [Ea1,8]).

unPaso([Ea1,Ea2], [Es1,Es2]):- Es1 is max(Ea1 - (8-Ea2), 0), Es2 is min(Ea1 + Ea2, 8). % #vaciar primero en segundo
unPaso([Ea1,Ea2], [Es1,Es2]):- Es1 is min(Ea1 + Ea2, 5), Es2 is max(Ea2 - (5-Ea1), 0).	% #vaciar segundo en primero

camino(E,E,C,C).
camino(EstadoActual, EstadoFinal, CaminoHastaAhora, CaminoTotal ):-  
  unPaso( EstadoActual, EstSiguiente),
  \+member(EstSiguiente,CaminoHastaAhora),
  camino( EstSiguiente, EstadoFinal, [EstSiguiente|CaminoHastaAhora], CaminoTotal).

% #\+ no pertany
solucionOptima:-
  nat(N),
  camino([0,0] , [0,4], [[0,0]] , C),
  length(C,N),
  reverse(C,F),
  write(F).
  
nat(0).
nat(N):- nat(N1), N is N1 + 1.
