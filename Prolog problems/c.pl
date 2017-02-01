
numNutrients(5).

product(milk,[2,4,5]).
product(meat,[1,3]).
product(sugar,[1,2,3,4,5]).

pasos(_,Camino,Camino,Num,Num,_).
pasos(K,Camino,L,Num,_,MyListNutrients):-
  product(Nom,LlistaNut),  
  \+member(Nom, Camino),
  union(LlistaNut,MyListNutrients,L3), 
  length(L3, Longitut), 
  K1 is K - 1,
  K1 >= 0,
  pasos(K1,[Nom|Camino],L,Num,Longitut,L3).
  
   

shoping(K,L):-
  numNutrients(Num),
  pasos(K,[], L, Num, 0,[]),
  write(L).
	
	
pert(X,[X|_]).
pert(X,[_|Y]) :- pert(X,Y).

union([], L, L) :- !.
union([H|T], L, R) :-
  memberchk(H, L), !,
  union(T, L, R).
union([H|T], L, [H|R]) :-
  union(T, L, R).