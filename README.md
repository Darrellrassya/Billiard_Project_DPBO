# Demo Minggu 02 — Sistem Window & Meja

Slice kode utama yang **berdiri sendiri** untuk demo Minggu 2. Folder ini tidak
butuh folder `repo/`; cukup C++17 dan SFML 2.6.

Tujuannya: saat presentasi, yang dijalankan hanya fitur yang sudah dibahas
minggu ini — bukan seluruh game final.

## Build dan jalankan

```bash
cd demo/week-02-window-table
make
./app
```

Atau sekali jalan:

```bash
./run.sh
```

## Kontrol

| Aksi | Tombol |
|---|---|
| Mulai dari menu | Klik **MAIN SEKARANG** atau `Enter` |
| Luncurkan bola ke arah tertentu | Panah atau `WASD` (sekali tekan = satu dorongan) |
| Hentikan bola | `Space` |
| Reset bola ke titik awal | `R` |
| Simulasi state Game Over | `G` (saat Playing) |
| Main lagi dari Game Over | `R` |
| Keluar | `Esc` atau tombol close |

Bola demo digerakkan manual dan memantul memakai `Table::handleWallCollision`.
Friction, tumbukan antar-bola, dan cue **tidak** dipakai di sini karena itu
materi Minggu 3.

## Yang termasuk (sesuai deck Minggu 2)

- Window 1080 × 660, title bar + close, tanpa resize.
- `setFramerateLimit(60)`.
- Game loop `handleEvents() → update() → render()`.
- State machine `MainMenu / Playing / GameOver`.
- Meja 4 lapis: `outerFrame`, `woodRail`, `cushionBorder`, `feltArea`.
- Bayangan meja, 6 pocket, 18 diamond sight, baulk line, dua spot.
- Wall collision dengan pocket gate (`pocketRadius * 1.28`).
- Deteksi bola masuk lubang (`pocketRadius - 1`).
- Draw order: shadow → frame → rail → cushion → felt → line/spot → diamond →
  pocket.

## Yang belum termasuk (minggu 3+)

- `Ball::update()`: friction dan gerak otomatis.
- `Table::resolveBallCollisions()`: tumbukan antar-bola.
- Cue, power meter, garis bidik.
- ScoreBoard, giliran, aturan 8-ball.
- Menu dan HUD final.
- Sound dan tekstur.

`Table.cpp`, `Ball.cpp`, `Table.hpp`, `Ball.hpp`, dan `main.cpp` disalin apa
adanya dari repo. Hanya `Game.cpp` yang dipangkas agar tidak memanggil sistem
minggu 3+.

## Peta ke slide presentasi

| Slide | Bukti di demo |
|---|---|
| 04 Window | Judul, ukuran 1080 × 660, style, cap 60 FPS |
| 05 Game loop | `Game::run()` dengan tiga langkah |
| 06 Event & state | Klik menu, `G` untuk Game Over, `R` untuk restart |
| 07 Blueprint | Posisi meja (130, 125), bounds 860 × 430, cue ball (345, 340) |
| 08 Anatomi meja | Empat lapisan dan bayangan |
| 09 Pockets | Enam lubang; bola bisa masuk dan dihitung |
| 10 Detail visual | 18 diamond, baulk line, dua spot |
| 11 Wall collision | Bola memantul; mulut pocket tidak memantul |
| 12 Draw order | Urutan gambar di `Table::draw()` |
| 13 Integrasi | `handleWallCollision()` + `isBallPotted()` dipanggil tiap frame |

## Sumber

- Repo: <https://github.com/Darrellrassya/sfml_biliard_dpbo>
- Commit: `c254188` — `feat: initial commit billiard game 2D`
- `src/Table.cpp` baris 4–181 dan `src/Ball.cpp` disalin tanpa perubahan.
- `src/Game.cpp` diadaptasi dari versi penuh; bagian menu, cue, scoreboard,
  dan sub-stepping fisika dihilangkan.
