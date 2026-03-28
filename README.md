# Knight's Legacy

**Knight's Legacy** to gra 2D stworzona w języku **C++** przy użyciu biblioteki **SFML**. Projekt skupia się na mechanikach RPG, systemie walki oraz eksploracji mapy.

## Screeny z gry
![Rozgrywka](./screenshots/rozgrywka.png)

## Główne Funkcje
* **Fizyka i Kolizje:** Autorski system wykrywania kolizji i interakcji.
* **AI Przeciwników:** Przeciwnicy posiadają stany (IDLE, WALKING, ATTACKING, DEAD) i inteligentnie podążają za graczem, omijając przeszkody.
* **Interaktywne Budynki:** System budowania z czasem progresu (Pasek postępu nad głową robotnika).
* **Dynamiczne Zasoby:** *
    * **Drzewa:** Możliwość wycinki i regeneracji po czasie.
    * **Owce:** Mechanika ucieczki przed graczem po otrzymaniu obrażeń.
    * **Kopalnie Złota:** Wymagają odbudowy, po której cyklicznie generują surowce.
* **System Dialogów:** Maszyna stanów obsługująca misje i wyświetlająca tekst "literka po literce" dla lepszego klimatu RPG.
* **Wybór Bohatera:** Możliwość wyboru koloru rycerza oraz wpisania własnego nicku.
* **System Rankingu:** Zapisywanie najlepszych czasów do plików `.txt` i wyświetlanie Topowych graczy.

## Technologia
* **Język:** C++20 (standard wymagany dla `std::ranges` i `std::filesystem`)
* **Biblioteka graficzna:** SFML 2.6.2
* **Środowisko:** Visual Studio 2022

## Jak uruchomić?
Gra powinna uruchomić się bez problemu i konieczności pobierania dodatkowych plików
1. Pobierz repozytorium: `git clone https://github.com/Wojtek4321/Knights-Legacy.git`
2. Otwórz `.sln` w Visual Studio 2022.

Wykorzystane assety:
https://pixelfrog-assets.itch.io/tiny-swords
