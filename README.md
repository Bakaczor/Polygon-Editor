# Polygon Editor
Polygon Editor to prosta aplikacja pozwalająca na tworzenie i edytowanie wielokątów. Program został zbudowany na Qt 6.5.0, w standardzie C++17.

# Klawiszologia
W tej sekcji znajdują się informacje na temat korzystania z aplikacji.

## Wielokąt
### Tworzenie
By zacząć budować wielokąt, należy podwójnie nacisnąć **lewy przycisk myszy**. W ten sposób można dodawać kolejne wierzchołki. By zakończyć budowanie, należy kliknąć podwójnie na pierwszy dodany wierzchołek. Jeżeli chcemy zrezygnować z budowania wielokąta wystarczy podwójnie kliknąć **prawy przycisk myszy**.

### Przeciąganie
By przeciągnąć wielokąt, należy nacisnąć w jego wnętrzu **prawym przyciskiem myszy** i nie odpuszczając przemieścić go w inne miejsce.

### Usuwanie 
By usunąć wielokąt należy go zaznaczyć kilkając w jego wnętrzu (zaświeci się na czerwono), a następnie użyć klawisza `Delete`.

## Krawędź
### Przeciąganie
By przeciągnąć krawędź, należy nacisnąć na nią **prawym przyciskiem myszy** i nie odpuszczając przemieścić ją w inne miejsce.

### Zmiana orientacji
By zmienić orientację krawędzi należy ją zaznaczyć poprzez kliknięcie (zaświeci się na czerwono), a następnie wybrać jedną z opcji dostępnych z lewej strony interfejsu:
- `Horizontal` - krawędź stale pozioma
- `Vertical` - krawędź stale pionowa
- `None` - krawędź swobodna

## Wierzchołek
### Dodawanie
By dodać nowy wierzchołek na jednej z krawędzi należy kliknąć na nią w wybranym miejscu **środkowym przyciskiem myszy** (kółkiem).

### Przeciąganie
By przeciągnąć wierzchołek należy na niego nacisnąć **prawym przyciskiem myszy** i nie odpuszczając przemieścić go w inne miejsce.

### Usuwanie
By usunąć wierzchołek należy go zaznaczyć klikając na niego (zaświeci się na czerwono), a następnie użyć klawisza `Delete`.

## Wielokąt odsunięty
By aktywować wielokąt odsunięty, należy zaznaczyć wielokąt poprzez kliknięcie w jego wnętrzu (zaświeci się na czerwono), a następnie wybrać wartość odsunięcia poprzez przeciągnięcie na suwaku po lewej stronie.

# Opis algorytmów
## Orientacja krawędzi
### Algorytm
1. W momencie, gdy użytkownik zmieni orientację krawędzi, sprawdzane jest czy krawędź jest zaznaczona i czy sąsiadujące krawędzie nie mają już ustawionej takiej samej orientacji.
2. Jeżeli walidacja zakończona jest pozytywnie, orientacja krawędzi jest zmieniana, a ze współrzędnych jej wierzchołków (x-owych dla `Vertical`, y-owych dla `Horizontal`) wyliczana jest różnica, której połowa dodawana jest do pierwszego i odejmowana od drugiego wierzchołka (w efekcie oba znajdują się w punkcie będącym średnią arytmetyczną ze względu na daną współrzędną).

### Przeciąganie wierzchołków
Jeżeli krawędź incydentna z przeciąganym wierzchołkiem ma ustawioną orientację, jej nieincydentny wierzchołek również jest przeciągany, ale tylko wzdłuż jednej z osi (OX dla `Vertical`, OY dla `Horizontal`).

### Przeciąganie krawędzi
Jeżeli krawędź incydentna z przeciąganą krawędzią ma ustawioną orientację, jej nieincydentny wierzchołek również jest przeciągany, ale tylko wzdłuż jednej z osi (OX dla `Vertical`, OY dla `Horizontal`).

**Wyjątkiem** od tego jest przeciwprostokątna trójkąta prostokątnego - w takim wypadku przemieszczana jest cała figura.

## Wielokąt odsunięty
### Algorytm
1. Na samym początku sprawdzane jest skierowanie wielokąta (liczony jest iloczyn wektorowy dla wierzchołka o najmniejszych współrzędnych oraz jego sąsiadów). Jeżeli wielokąt zbudowany jest zgodnie z ruchem wskazówek zegara zmieniany jest znak offsetu na ujemny (przyjęta konwencja).
2. Następnie wyliczane są odciniki odsunięte - dla każdej pary wierzchołków wyliczany jest wektor prostopadły, który następnie jest normowany do odpowiedniej długości. Wierzchołki odsunięte powstają poprzez przesunięcie wierzchołków wielokąta o ten wektor.
3. Kolejnym krokiem jest wyliczenie przecięć prostych, które tworzone są przez sąsiadujące odcinki - rozwiązywany jest układ dwóch równań liniowych przy użyciu wzorów Cramera.
4. W dalszej kolejności wyszukiwane są samoprzecięcia - każda krawędź jest sprawdzana ze wszystkimi nieincydentymi krawędziami w następujący sposób:
   
    ```
    1. Niech p1 i p2 będą wierzchołkami pierwszego odcinka, a p3 i p4 drugiego.
    2. Przyjmijmy oznaczenia: p = p1, q = p3, r - wektor p1p2, s - wektor p3p4.
    3. Niech r_s = r × s, jeżeli jest on równy zero, to wektory są równoległe.
    4. Jeżeli nie: niech t = pq × s, zaś u = pq × r - oba podzielone przez r_s.
    5. t i u są stosunkami wektorów wyznaczających punkt przecięcia do długości odcinków, zatem jeżeli oba znajdują się w przedziale [0, 1], to przecięcie istnieje i jest ono równe t * r (lub równoważnie u * s).
    ```

    Tak znalezione samoprzecięcia są sortowane według odległości od punktu p1 i wstawiane do ciągu wierzchołków.
5. W kolejnym kroku wykonywane są równocześnie dwie operacje:
   - Ilekroć natrafimy na samoprzecięcie zmieniamy to, czy dodajemy bazowe wierzchołki do rozwiązania, czy nie.
   - Samoprzecięcia wrzucane są na stos wraz z indeksami. Jeżeli szczyt stosu pokrywa się z obecnym wierzchołkiem, sprawdzamy ile "dobrych" wierzchołków znajduje się pomiędzy - jeżeli co najmniej dwa, natrafiliśmy na pętlę.
    
    Zobrazujmy to na przykładzie.

    ```
    Niech (0, 1, P, 2, 3, N, 4, 5, 6, 7, 8, N, P, 9, 10, 11) oznaczają ciąg kolejnych wierzchołków, a N i P będą samoprzecięciami. 

    Dodane: 0, 1
    Niedodane:
    Zmiana stanu: P

    Dodane: 0, 1, P
    Niedodane: 2, 3
    Zmiana stanu: N

    Dodane: 0, 1, P, N, 4, 5, 6, 7, 8
    Niedodane: 2, 3
    Zmiana stanu: N - pętla! (N oznaczane jest do usunięcia, w tym momencie sprawdzamy dodatkowo czy N, P i 9 nie są współliniowe, jeżeli tak, P oznaczane jest do usunięcia)

    Dodane: 0, 1, P
    Niedodane: 2, 3, N, 4, 5, 6, 7, 8, N
    Zmiana stanu: P - pętla! (jak wyżej, jednak ponieważ na stosie jest tylko jeden wierzchołek, traktujemy P jako część głównej pętli i nie usuwamy jego pierwszego wystąpienia z rozwiązania)

    Dodane: 0, 1, P, 9, 10, 11
    Niedodane: 2, 3, N, 4, 5, 6, 7, 8, N, P

    Finalnie znaleźliśmy jędną pętlę (N, 4, 5, 6, 7, 8), a głównym rozwiązaniem jest (0, 1, P, 9, 10, 11).
    ``` 
6. Pod koniec sprawdzana jest każda para sąsiadujących punktów głównego rozwiązania pod kątem zdublowanych wartości - jeżeli takowa zostanie napotkana (choć w teorii nie powinna) usuwamy jeden z wierzchołków.
7. Finalnie, spradzamy każdą trójkę punktów p1, p2, p3 pod kątem współliniowości, poprzez obliczenie p2p1 × p2p3. Jeżeli takowa zachodzi, usuwamy punkt p2 z finalnego rozwiązania.

## Kontakt
W razie pytań proszę się ze mną kontatkować pod mailem bartosz.kaczorowski2.stud@pw.edu.pl.