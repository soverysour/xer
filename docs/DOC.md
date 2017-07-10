# Despre #

Xer este un joc de tipul [roguelike](https://en.wikipedia.org/wiki/Roguelike) simplu, in care scopul este de a trece prin 
toate nivelele, prin nivele generate intamplator si monstri pozitionati intamplator, cu comportamente diferite. Jocul 
se joaca intr-un terminal sau emulator de terminal.

# Instalare #

Dependente:
   \- Un sistem cu access la libraria Ncurses ( Linux, *BSD, macOS, **NU** Windows )
   \- CMake ( build tool-ul folosit )
   \- GCC ( compiler-ul folosit, Clang nu a fost testat suficient ) 
   \- git ( optional, pentru clonarea repositoriului )
   \- valgrind ( optional, pentru verificarea scurgerilor de memorie, etc ale programului )
   \- astyle ( optional, pentru stilizarea surselor )
   \- doxygen ( pentru a genera documentatie bazata pe codul sursa )

\* Valgrind, astyle si doxygen au fost folosite deja.

# Pentru clonare si compilare #

```
git clone https://bitbucket.org/RedAsATortoise/xer.git
cd xer
mkdir bin && cd bin
cmake -D CMAKE_C_COMPILER=gcc ..
make
./Xer # Pentru rularea jocului
```

# Instructiuni #

Obiectivul jocului este de a trece prin toate nivelele. Pentru a te misca, poti folosi tastele:

Tastele vi:

```
Y K U 
H . L 
B J N
```

Sau tastele numpad-ului:

```
7 8 9
4 5 6 
1 2 3
```

Pentru a trece de la un nivel la altul, trebuie sa gasesti scarile. Ele sunt reprezentate prin `>`. Atunci cand te afli 
pe scari, poti apasa `>` pentru a trece la urmatorul nivel.

In drumul spre ultimul nivel va trebui sa treci prin nivele generate intamplator si sa eviti monstri, putand insa sa 
retaliezi, decizie strategica ce va trebui sa o asumi.

Pentru a inchide jocul, apasa `Q` ( nu `q` ).
