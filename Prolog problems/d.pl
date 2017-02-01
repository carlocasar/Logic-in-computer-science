cities([1,2,3,4]).
road(1,2,10).
road(1,4,20).
road(2,3,25).
road(3,4,12).

%% La idea es ir iterando sobre las posibles carreteras y meterlas mientras cumplan la distancia menor que K y mientras el grafo tenga N-1 aristas donde N es el numero de ciudades. Tambien como condicion de terminar, es un
%% set que tenemos que tienes la union de las ciudades de las carreteras que hemos ido poniendo, hacemos el length y cuando sea == numnero ciudades termina, o simplemente comparando ciudades nuetras con cities izi.

pasos(_,_,Cities,Cities,MyRoads,MyRoads).
pasos(K, MyK, MyListCiudades, Cities, MyRoads, Roads):-
  road(Cit1,Cit2,Long),
  MyKNew is MyK + Long,
  K >= MyKNew,
  append(MyRoads,[[Cit1,Cit2,Long]], MRoad),
  length(MRoad, Arestes),
  union(MyListCiudades,[Cit1,Cit2], MCit),
  length(MCit, Vertex),
  VertexAux is Vertex - 1,
  Arestes = VertexAux, 			%%% Si no, no es un arbol
  pasos(K, MyKNew, MCit, Cities, MRoad, Roads).


mainroads(K,M):-
  cities(Cities),
  pasos(K, 0, [], Cities, [],M),
  write(M).
  

union([],L,L).
union([X|L1], L2, L3):-
	pert(X,L2),!,
	union(L1,L2,L3).
union([X|L1],L2,[X|L3]):-
	union(L1,L2,L3).
	
	
pert(X,[X|_]).
pert(X,[_|Y]) :- pert(X,Y). 