# 🎮 Joc Sudoku în C

> Dezvoltat de **Cerempei Stefan** — AC CTI-ro C2.1

Un joc Sudoku complet implementat în limbajul C, cu interfață în consolă, sistem de timp, leaderboard și mai multe niveluri de dificultate.

---

## 📋 Cuprins

- [Descriere](#descriere)
- [Funcționalități](#funcționalități)
- [Structura proiectului](#structura-proiectului)
- [Instalare și rulare](#instalare-și-rulare)
- [Cum se joacă](#cum-se-joacă)
- [Niveluri de dificultate](#niveluri-de-dificultate)
- [Leaderboard](#leaderboard)
- [Principii tehnice](#principii-tehnice)

---

## 📖 Descriere

Acest proiect implementează jocul clasic Sudoku în C, rulând direct în terminal. Jucătorul completează o grilă respectând regulile standard Sudoku, în timp ce un cronometru invers numără secundele rămase. Jocul include un sistem de greșeli, niveluri de dificultate și un clasament al celor mai buni jucători.

---

## ✨ Funcționalități

### Joc de bază
- Grilă standard **9×9** (sau extinsă **27×27**)
- Introducerea valorilor cu validare completă în timp real
- Verificare automată dacă Sudoku-ul este completat corect
- Mesaj **"Game Won"** la finalizarea cu succes

### ⏱️ Sistem de timp
- **600 de secunde** (10 minute) per sesiune de joc
- Afișare continuă: `Time left: MM:SS`
- La expirarea timpului: jocul se oprește și apare mesajul **"Game Over"**

### ❌ Sistem de greșeli
- Număr limitat de greșeli permise (maxim **3**)
- La depășirea limitei, jocul se termină automat
- Greșelile sunt salvate în leaderboard

### 🏆 Leaderboard
- Stocarea rezultatelor cu:
  - Timp rămas
  - Număr de greșeli
- Citire/scriere din fișier (CSV sau binar)

---

## 📁 Structura proiectului

```
project_sudoku/
│
├── main.c           # Punctul de intrare — meniu principal, bucla jocului
├── game_logic.c     # Implementarea regulilor și validărilor
├── game_logic.h     # Prototipuri funcții, structuri (Grid, Cell)
├── generator.c      # Algoritmi de generare (Backtracking / Algorithm X)
├── generator.h      # Prototipuri pentru niveluri (Easy, Medium, Hard)
├── ui_display.c     # Afișarea în consolă și culori
├── ui_display.h     # Interfața de afișare
├── leaderboard.c    # Citire/scriere în fișier
├── leaderboard.h    # Structura scorului
└── utils.h          # Constante (dimensiune 9 sau 27), macro-uri
```

---

## 🚀 Instalare și rulare

### Cerințe
- Compilator C (ex: `gcc`)
- Terminal / Consolă

### Compilare

```bash
gcc main.c game_logic.c generator.c ui_display.c leaderboard.c -o sudoku
```

### Rulare

```bash
./sudoku
```

---

## 🎯 Cum se joacă

1. Alege nivelul de dificultate din meniu
2. Grila este generată automat cu celule presetate (blocate)
3. Introdu rând, coloană și valoarea dorită (între 1–9)
4. Regulile de validare verifică automat:
   - Linia și coloana să fie între **1–9**
   - Valoarea să fie între **1–9**
   - Celula să nu fie **presetată (blocată)**
   - Valoarea să fie **unică** pe linie, coloană și în pătratul 3×3
5. Completează toată grila corect pentru a câștiga!

---

## 🎚️ Niveluri de dificultate

| Nivel   | Descriere                          |
|---------|------------------------------------|
| 🟢 Easy   | Multe celule precompletate         |
| 🟡 Medium | Dificultate moderată               |
| 🔴 Hard   | Puține indicii, provocare maximă   |

---

## 🏅 Leaderboard

Rezultatele sunt salvate local și includ:
- **Timp** — secundele rămase la finalizare
- **Greșeli** — numărul de greșeli făcute

Cu cât ai timp mai mult rămas și mai puține greșeli, cu atât scorul este mai bun!

---

## 🔧 Principii tehnice

- **Generare board**: algoritm Backtracking / Algorithm X
- **Validare input**: verificări multiple înainte de plasarea oricărei valori
- **Scalabilitate**: suport pentru grilă extinsă **27×27**
- **Interfață**: afișare colorată în consolă prin `ui_display.c`
- **Persistență date**: leaderboard stocat în fișier CSV sau binar

---

## 🔗 Link-uri utile

- 📦 **Repository GitHub**: [StefanCerempei/Sudoku_Game_in_C](https://github.com/StefanCerempei/Sudoku_Game_in_C)

---

*Proiect realizat în cadrul cursului AC CTI-ro, grupa C2.1.*
