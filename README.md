# Horspool Algorithm Documentation

## Konsep Bad Character Heuristic
Bad Character Heuristic merupakan salah satu kunci efisiensi dari algoritma pencarian string (seperti Boyer–Moore dan Horspool). Intinya, saat terjadi mismatch antara pola dan teks, kita dapat menggunakan informasi tentang karakter yang menyebabkan ketidakcocokan (disebut juga bad character) untuk menentukan seberapa jauh pola bisa digeser ke kanan sehingga perbandingan berikutnya tidak perlu dilakukan pada posisi yang pasti tidak akan cocok.

### Ide Utama
#### Preprocessing Tabel Bad Character:

Sebelum pencarian dimulai, kita membuat sebuah tabel (array) yang menyimpan posisi terakhir (indeks terakhir) kemunculan setiap karakter dalam pola.
Jika suatu karakter tidak muncul sama sekali dalam pola, kita set nilainya menjadi -1.
Contoh: Untuk pola "ABC", tabel akan menjadi:
- `BadChar['A'] = 0`
- `BadChar['B'] = 1`
- `BadChar['C'] = 2`
- Karakter lain → -1

#### Saat Terjadi Mismatch:

Misalkan kita sedang menyelaraskan pola ke teks dan melakukan perbandingan dari kanan ke kiri.
Jika pada posisi j di pola (dengan indeks dihitung dari kanan) terjadi mismatch, artinya karakter pada teks di posisi shift + j tidak sama dengan karakter P[j] di pola.
Kita kemudian lihat nilai pada tabel bad character untuk karakter yang menyebabkan mismatch (misalnya, T[shift + j]).

### Dua Kasus Utama:
#### Karakter Bad Tidak Ada di Pola:
Jika karakter tersebut tidak ada di dalam pola (nilai tabel = -1), maka pergeseran yang optimal adalah menggeser pola sejauh j + 1 posisi. Ini karena tidak ada kemungkinan terjadi kecocokan pada alignment yang melibatkan karakter tersebut.

#### Karakter Bad Ada di Pola:
Jika karakter tersebut ada di dalam pola, misalnya pada indeks k (dengan k = nilai dari tabel), kita bisa menggeser pola sehingga kemunculan terakhir karakter tersebut (pada posisi k) sejajar dengan karakter di teks yang menyebabkan mismatch. Pergeseran yang dihitung adalah:
```
shift = max(1, j - k)
```
Fungsi max memastikan bahwa pergeseran minimal adalah 1 agar pola selalu bergeser ke kanan.

### Langkah demi Langkah Saat Mismatch
#### Perbandingan dari Kanan ke Kiri:
Saat pola diselaraskan dengan teks, perbandingan dilakukan dari indeks m-1 ke 0 (dari kanan ke kiri).

#### Terjadi Mismatch pada Posisi j:
Katakanlah, saat perbandingan, karakter pada posisi j pada pola tidak cocok dengan karakter pada T[shift + j].

#### Ambil Karakter Penyebab Mismatch:
Karakter penyebab mismatch adalah T[shift + j]. Kita lihat pada tabel bad character untuk karakter tersebut.

#### Hitung Nilai Pergeseran:
- Jika nilai pada tabel untuk karakter tersebut adalah -1, artinya karakter itu tidak ada di dalam pola, sehingga kita geser pola sebanyak j + 1.
- Jika karakter tersebut muncul di dalam pola, katakanlah pada indeks k, maka pergeseran yang optimal adalah j - k.
- Selalu gunakan nilai minimal 1, sehingga secara keseluruhan:
```bash
shift = max(1, j - BadChar[ T[shift + j] ])
```

### Manfaat Utama
1. **Mengurangi Perbandingan**: Dengan melompati bagian-bagian teks yang tidak mungkin menghasilkan kecocokan, algoritma mengurangi jumlah perbandingan karakter secara drastis pada kasus rata-rata.

2. **Kesederhanaan Implementasi**: Tabel bad character dapat dibangun dengan satu iterasi melalui pola, sehingga preprocessing ini efisien.

3. **Optimal pada Teks Acak**: Jika teks dan pola bersifat acak, kemungkinan terjadi mismatch pada karakter paling kanan cukup tinggi, sehingga pergeseran biasanya besar dan jumlah perbandingan menjadi rendah.

### Ilustrasi Sederhana
Misalkan:
- Pola: "ABC"
- Teks (alignment tertentu): Bagian teks yang sedang dibandingkan adalah "ABX"

#### Proses:
1. Mulai perbandingan dari kanan:
   - Bandingkan P[2] = 'C' dengan T[2] = 'X'.
   - Mismatch terjadi karena 'C' ≠ 'X'.
2. Periksa tabel bad character:
   - Jika `BadChar['X'] = -1` (karakter 'X' tidak ada di pola), maka pergeseran = `max(1, 2 - (-1)) = max(1, 3) = 3`.
   - Artinya, kita dapat menggeser pola sejauh 3 posisi ke kanan.

#### Contoh Lain:
Misalnya, pada alignment lain, perbandingan menghasilkan mismatch:
- P[2] = 'C' dengan T[2] = 'B' (dan `BadChar['B'] = 1` karena dalam pola 'B' muncul di indeks 1).
- Maka pergeseran = `max(1, 2 - 1) = 1`.

## 2. Pencarian Pola di Teks
Setelah melakukan preprocessing (pembuatan tabel bad character), langkah selanjutnya adalah mencari pola di dalam teks. Proses pencarian ini melibatkan beberapa tahap, yaitu alignment pola ke dalam teks, perbandingan karakter secara terbalik (dari kanan ke kiri), dan perhitungan pergeseran (shift) berdasarkan hasil perbandingan.

### A. Inisialisasi dan Kondisi Awal
#### Variabel shift:
- Inisialisasi variabel shift dengan nilai 0. 
- Variabel ini menentukan posisi (indeks) pada teks di mana pola saat ini akan diselaraskan (aligned).
- Contoh: Jika shift = 0, pola akan disamakan dengan teks mulai dari indeks 0 sampai indeks m-1, di mana m adalah panjang pola.

#### Kondisi Loop Utama:
Loop utama dijalankan selama masih ada ruang yang cukup di teks untuk menempatkan pola secara utuh. Kondisi ini diwakili oleh:
```bash
while (shift <= panjangTeks - panjangPola)
```
Hal ini memastikan bahwa kita hanya mencoba alignment di mana sisa teks memiliki panjang minimal sama dengan pola. Jika tidak, tidak ada kemungkinan untuk terjadi kecocokan yang valid.

### B. Alignment Pola ke dalam Teks
Pada setiap iterasi dari loop utama, pola diselaraskan ke teks pada posisi shift.
Misalnya, jika teks adalah "ABAAABCDABAACD" dan pola adalah "ABC":
- Jika shift = 0, pola "ABC" diselaraskan dengan teks[0..2].
- Jika shift = 4, pola "ABC" diselaraskan dengan teks[4..6].

Pada titik ini, kita sudah tahu bahwa pada posisi shift, pola akan dibandingkan dengan bagian teks yang dimulai dari indeks tersebut.

### C. Perbandingan Karakter dari Kanan ke Kiri
Setelah pola diselaraskan, langkah berikutnya adalah melakukan perbandingan karakter antara pola dan bagian teks yang diselaraskan:

#### Inisialisasi j:
- Set variabel j dengan nilai panjangPola - 1 (yaitu, indeks terakhir pada pola).
- Alasan: Algoritma Horspool membandingkan karakter dari kanan ke kiri. Mulai dari karakter paling kanan membantu segera mendeteksi mismatch.

#### Inner While Loop untuk Perbandingan:
Pada setiap iterasi, lakukan perbandingan:
```bash
while (j >= 0 && pola[j] == teks[shift + j])
    j--;
```

#### Penjelasan:
- Kondisi j >= 0: Memastikan bahwa perbandingan masih dilakukan dalam batas pola.
- Kondisi pola[j] == teks[shift + j]: Mengecek apakah karakter pada posisi j di pola sama dengan karakter pada posisi shift + j di teks.

#### Proses:
- Jika kondisi terpenuhi (karakter cocok), maka j dikurangi (decrement) sehingga perbandingan berlanjut ke karakter berikutnya ke kiri.

#### Hasil:
- Jika seluruh karakter cocok, maka j akan menjadi -1.
- Jika terjadi mismatch, loop berhenti, dan j akan menunjukkan indeks pada pola di mana terjadi perbedaan.

### D. Pengambilan Keputusan Berdasarkan Hasil Perbandingan
Setelah inner loop selesai, ada dua skenario:

#### Seluruh Pola Cocok (j < 0):
- **Kondisi**: Jika j menjadi -1, berarti setiap karakter pada pola telah cocok dengan bagian teks pada posisi shift.
- **Tindakan**:
  - Simpan nilai shift ke dalam daftar kecocokan (occurrences).
  - Untuk mencari kecocokan selanjutnya, pola harus digeser ke kanan.
- **Pergeseran**: Biasanya dihitung dengan melihat karakter yang langsung berada setelah pola yang cocok di teks. Misalnya:
  ```bash
  shift = shift + (panjangPola - BadChar[teks[shift + panjangPola]])
  ```
- **Jika tidak ada karakter setelah kecocokan**: Geser minimal 1 posisi.
- **Tujuan**: Memastikan bahwa setelah menemukan kecocokan, algoritma tidak melakukan perbandingan ulang pada bagian teks yang sama.

#### Terjadi Mismatch (j ≥ 0):
- **Kondisi**: Jika j masih bernilai non-negatif, berarti terjadi mismatch pada posisi j pada pola.
- **Identifikasi Karakter Buruk**: Karakter pada teks yang menyebabkan mismatch adalah teks[shift + j].
- **Perhitungan Pergeseran (Shift)**: Menggunakan tabel bad character, pergeseran dihitung dengan rumus:
  ```bash
  shift = shift + max(1, j - BadChar[teks[shift + j]])
  ```

#### Penjelasan Rumus:
- j: Posisi pada pola di mana terjadi mismatch.
- BadChar[teks[shift + j]]: Indeks terakhir kemunculan karakter yang menyebabkan mismatch di dalam pola.
- Selisih (j - nilai tabel): Menentukan seberapa jauh kita bisa menggeser pola.
- Fungsi max(1, ...): Menjamin bahwa pergeseran minimal adalah 1.

#### Tujuan:
Pergeseran ini menghindari pemeriksaan ulang pada alignment yang tidak mungkin menghasilkan kecocokan.

### E. Pembaruan dan Iterasi Selanjutnya
- **Update shift**: Setelah menghitung nilai pergeseran, nilai shift diperbarui sehingga pola digeser ke posisi baru pada teks.
- **Pengulangan Proses**: Dilakukan kembali selama masih ada cukup ruang di teks untuk menempatkan pola secara lengkap.
- **Terminasi**: Loop utama berhenti ketika shift melebihi panjangTeks - panjangPola.

### Contoh Ilustrasi
- **Teks**: "ABAAABCDABAACD" (panjang = 14)
- **Pola**: "ABC" (panjang = 3)

#### Contoh Iterasi (shift = 8):
- **Alignment**:
  - Teks[8] = A
  - Teks[9] = B
  - Teks[10] = A
  - Bagian yang dibandingkan: "ABA"

- **Perbandingan**:
  - Mulai dengan j = 2 (karakter terakhir pola, yaitu 'C')
  - Bandingkan: P[2] ('C') vs T[8+2] (T[10] = 'A')
  - Hasil: mismatch karena 'C' tidak sama dengan 'A'

- **Penghitungan Shift**:
  - Karakter penyebab mismatch: T[10] = 'A'
  - Dari tabel bad character, BadChar['A'] = 0
  - Shift increment = max(1, 2 - 0) = 2
  - Shift baru = 8 + 2 = 10

### Ringkasan Pencarian Pola di Teks
1. Inisialisasi: Tentukan nilai awal shift = 0.
2. Alignment: Tempatkan pola pada teks berdasarkan nilai shift.
3. Perbandingan Karakter (dari kanan ke kiri): Mulai dari indeks terakhir pola.
4. Evaluasi Hasil Perbandingan:
   - Jika pola cocok, catat posisi dan hitung shift berikutnya.
   - Jika terjadi mismatch, hitung pergeseran dengan rumus bad character.
5. Update Shift: Geser pola ke posisi baru.
6. Terminasi: Proses selesai ketika seluruh teks telah diperiksa.
