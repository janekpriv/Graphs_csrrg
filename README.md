# Louvain-clustering

Projekt **louvain-clustering* to implementacja narzędzi i algorytmów grafowych napisana w języku **C**. Repozytorium zawiera kod źródłowy, pliki wejściowe oraz skrypt kompilacji oparty o Makefile.

## Struktura projektu

```
Graphs_csrrg/
├── input/            # Pliki wejściowe (np. opisy grafów)
├── lib/              # Biblioteki pomocnicze
├── output/           # Wyniki działania programu
├── src/              # Kod źródłowy
│   ├── main.c        # Główny plik programu
│   └── ...           # Pozostałe moduły
├── gdiv              # (opcjonalny komponent — opisz w razie potrzeby)
├── makefile          # Skrypt budowania projektu
└── README.md         # Dokumentacja
```

##  Kompilacja i uruchomienie

Aby skompilować projekt, wykonaj:

```bash
git clone https://github.com/janekpriv/Graphs_csrrg.git
cd Graphs_csrrg
make
```

Powstały plik wykonywalny (np. `graphs`) uruchomisz poleceniem:

```bash
./graphs input/example.txt
```

##  Opis działania programu

Program realizuje operacje na grafach, m.in.:

* tworzenie grafu z pliku wejściowego,
* odczyt i zapis danych o grafach,
* przetwarzanie struktury grafu,
* (tu możesz dopisać konkretne algorytmy, jeśli są zaimplementowane).

Możesz doprecyzować tę sekcję po dodaniu szczegółów implementacyjnych.

## 📥 Format plików wejściowych

Przykład pliku wejściowego:

```
5 7
1 2
1 3
2 3
2 4
3 5
4 5
...
```

Pierwsza linia oznacza: `liczba_wierzchołków liczba_krawędzi`.
Każda kolejna linia reprezentuje jedną krawędź.

##  Przykład wyjścia

```
BFS start: 1
1 2 3 4 5
```

##  Wymagania

* Kompilator zgodny ze standardem **C99** lub nowszym,
* **make**,
* (opcjonalnie) dodatkowe biblioteki znajdujące się w katalogu `lib`.


## 📄 Licencja

Projekt udostępniony na licencji **MIT** (zmień, jeśli korzystasz z innej).
