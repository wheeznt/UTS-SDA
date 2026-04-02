# AUDIT TOTAL — perpustakaan.cpp

---

## 1. ANALISIS USE CASE & EDGE CASE

| No | Use Case | Dampak | Status |
|----|----------|--------|--------|
| 1 | Input `tahunTerbit` berupa string (misal: "dua") saat `cin >> tahunTerbit` | Stream gagal, program masuk state error, semua subsequent `cin` gagal. Crash/loop tak terbatas. | **Not Handled** |
| 2 | Input `stok` berupa string (misal: "lima") | Stream gagal, state error. | **Not Handled** |
| 3 | Input `stok` bernilai negatif (misal: -3) | Stok buku jadi negatif, peminjaman tidak bisa dikembalikan, inkonsistensi data. | **Not Handled** |
| 4 | Input `tahunTerbit` bernilai 0 | Tahun 0 tidak valid, tapi accepted karena validasi tidak ada. | **Not Handled** |
| 5 | Input `tahunTerbit` > tahun saat ini (misal: 9999) atau < 1000 | Tidak ada validasi range tahun. | **Not Handled** |
| 6 | Judul buku > 100 karakter | Buffer overflow pada `char judul[100]`, data menimpa memory lain. | **Not Handled** |
| 7 | Pengarang > 100 karakter | Buffer overflow pada `char author[100]`. | **Not Handled** |
| 8 | Nama mahasiswa > 50 karakter | Buffer overflow pada `char nama[50]`. | **Not Handled** |
| 9 | NIM > 20 karakter | Buffer overflow pada `char nim[20]`. | **Not Handled** |
| 10 | Program Studi > 50 karakter | Buffer overflow pada `char prodi[50]`. | **Not Handled** |
| 11 | Nomor HP > 15 karakter | Buffer overflow pada `char noHp[15]`. | **Not Handled** |
| 12 | Input `angkatan` non-integer | Stream gagal. | **Not Handled** |
| 13 | NIM yang diinput saat login mengandung newline/spasi | `cin.getline(nim, 20)` menyimpan whitespace, pencarian gagal. | **Not Handled** |
| 14 | Registrasi dengan NIM yang SUDAH TERDAFTAR | NIM duplikat masuk tanpa cek, data mahasiswa terpisah. Mahasiswa dengan NIM sama bisa login tapi mengakses data berbeda. | **Not Handled** |
| 15 | Input `pilih` menu utama non-integer | Stream gagal, loop infinite. | **Not Handled** |
| 16 | Input `pilih` nomor buku non-integer | Stream gagal. | **Not Handled** |
| 17 | Input `pilih` nomor pengembalian non-integer | Stream gagal. | **Not Handled** |
| 18 | Input pilihan menu admin/mahasiswa non-integer | Stream gagal. | **Not Handled** |
| 19 | Username/password admin input kosong (langsung ENTER) | `cin.getline` membaca string kosong, login gagal. Tapi tidak crash. | **Handled** (graceful fail) |
| 20 | Nama saat registrasi kosong (langsung ENTER) | Field nama tersimpan kosong, tidak ada warning. | **Not Handled** |
| 21 | NIM saat registrasi kosong | NIM kosong, pencarian selalu gagal. | **Not Handled** |
| 22 | Program Studi kosong | Tersimpan kosong, tidak ada validasi. | **Not Handled** |
| 23 | Angkatan kosong (langsung ENTER setelah cin >>) | Stream menangkap newline, gagal baca integer. | **Not Handled** |
| 24 | Nomor HP kosong | Tersimpan kosong. | **Not Handled** |
| 25 | Input `pilih` = 0 saat pilih nomor buku | `cariBukuByNomor(0)` idx=0 tidak pernah match, return NULL. Pesan "Nomor buku tidak valid!" muncul. | **Handled** (tapi pesan misleading — 0 bukan "tidak valid", tapi "out of range") |
| 26 | Input `pilih` negatif saat pilih nomor buku | `idx` tidak pernah match, return NULL. | **Handled** |
| 27 | Admin menghapus buku yang SEDANG DIPINJAM oleh mahasiswa | Stok buku di-DLL dihapus, tapi record `Peminjaman` di `Mahasiswa->daftarPinjaman` tetap ada. Saat mahasiswa mengembalikan, `kembalikanBuku` mencari buku dengan `strcmp(judul)` di linked list yang sudah dihapus — **tidak ditemukan**. Stok tidak ditambah, tapi node peminjaman tetap dihapus. Orphan record, data tidak konsisten. | **Not Handled** |
| 28 | Ubah stok: input -1 tidak di-skip (kontradiksi dengan instruksi "0 = skip") | Di `ubahBuku`, tahun menggunakan 0 sebagai skip, stok menggunakan -1. User menginput -1 ingin skip tapi stok malah diset ke -1. | **Not Handled** |
| 29 | Ubah tahun terbit: input tahun 0 | Kode `if (thn != 0)` tidak mengupdate, tapi user mengira 0 itu tahun. | **Not Handled** (ambigu) |
| 30 | Ubah stok: input stok negatif | `if (s != -1)` mengupdate stok jadi negatif. Tidak ada guard `s >= 0`. | **Not Handled** |
| 31 | Kembalikan buku: buku sudah dihapus admin sebelum dikembalikan | Loop `while(bk != NULL)` tidak menemukan buku, `bk` jadi NULL. Stok tidak ditambah. Node peminjaman tetap dihapus. **Data inconsistency + data loss**. | **Not Handled** |
| 32 | Pinjam buku: nomor yang dipilih merujuk ke buku dengan stok 0 (tidak ditampilkan di list) | `cariBukuByNomor` mencari berdasarkan nomor urut VISIBLE. Jika buku ke-5 punya stok 0 dan tidak ditampilkan, list menampilkan 4 item. User pilih 5 → `cariBukuByNomor(5)` return NULL atau return buku ke-5 yang tidak ada di list. **CRASH POTENTIAL** karena pointer NULL tidak dicek setelah cariBukuByNomor di pinjamBuku. | **Partially Handled** (baris 298-301 cek NULL tapi tidak menangani kasus buku ditemukan tapi stok 0 karena alasan urutan) |
| 33 | Ada 2+ buku dengan judul SAMA tapi author BERBEDA | `pinjamBuku` cek duplikat dengan `strcmp(judul)`, tidak cek author. User bisa pinjam dua buku berbeda (karena author beda). | **Not Handled** |
| 34 | Tampilan daftar buku `pinjamBuku`: inkonsistensi nomor | Nomor urut `no++` di baris 285 increment SELALU, tapi `cout` hanya untuk `stok > 0`. Jadi jika ada 5 buku (3 berstok, 2 tidak), nomor yang ditampilkan: 1,2,3 tapi di linked list nomor urutnya 1,2,3,4,5. User pilih "2" → `cariBukuByNomor(2)` return buku urut-2 (bisa stok 0). | **Not Handled** |
| 35 | Buffer overflow: `strcpy(baru->judul, target->judul)` tanpa cek panjang | Jika `target->judul` > 100 (bisa terjadi dari input di `tambahBuku`), strcpy overflow. | **Not Handled** |
| 36 | Input username/password admin > 30 karakter | Buffer overflow pada `char username[30]` dan `char password[30]`. | **Not Handled** |

---

## 2. DETEKSI AMBIGUITY & DESIGN FLAW

### Ambigu

- **Baris 135**: `"Tahun terbit baru [" << target->tahunTerbit << "] (0 = skip)"` — Tapi tahun 0 adalah tahun Masehi yang valid secara integer. User tidak bisa menggunakan tahun 0 meskipun logikanya untuk skip.

- **Baris 140**: `"Stok baru [" << target->stok << "] (-1 = skip)"` — Kontradiksi dengan line 135. Mengapa tahun pakai 0, stok pakai -1? Tidak konsisten dan membingungkan.

- **Baris 297 vs 279-286**: `cariBukuByNomor` return berdasarkan nomor urut dalam linked list (1-based). Tapi `pinjamBuku` menampilkan hanya buku berstok > 0. **Nomor yang dilihat user ≠ nomor yang dicari sistem.** Ambigu parah.

### Design Flaw

- **Tidak ada unique identifier untuk buku.** Buku diidentifikasi oleh judul (`strcmp`). Dua buku berbeda dengan judul sama akan dianggap sama di `pinjamBuku`, `kembalikanBuku`, `ubahBuku`, `hapusBuku`.

- **Login admin hardcoded.** Username "admin" + password "admin123" ada di source code. Semua orang yang baca source code tahu kredensial.

- **Tidak ada batas pinjam.** Mahasiswa bisa pinjam semua buku di perpustakaan tanpa batasan.

- **`cin.ignore()` di `loginAdmin` baris 417** — Dilakukan di AWAL loop SEBELUM `cin.getline`. Jika user konsistent memasukkan input < 30 karakter, tidak ada masalah. Tapi jika input sebelumnya di main menu ada sisa newline, `cin.ignore()` mengabaikan 1 karakter pertama dari username. Contoh: di main menu `cin >> pilih` (menu utama), newline `\n` tersisa. `cin.ignore()` di `loginAdmin` mengabaikan newline. Tapi jika ada sisa karakter lebih dari 1, bisa mengabaikan karakter pertama input.

- **`cin.ignore()` di baris 249**: Setelah `cin >> jawab` (char), newline `\n` tersisa di buffer. `registrasiMahasiswa` dipanggil, `cin.ignore()` di baris 202 akan mengabaikan karakter pertama NIM. **Edge case: jika user ketik 'y' + ENTER, newline dibuang, OK. Tapi jika input berbekas dari stream sebelumnya, bisa corruption.**

- **Tidak ada persistence.** Semua data hilang saat program ditutup. Tidak ada save/load.

- **Tidak ada mekanisme logout yang membersihkan state.** Menu mahasiswa hanya set pointer `mhs`, loop keluar, kembali ke main. Pointer `mhs` masih valid (data ada di heap). Tidak ada memory leak struktural tapi ada potensi confusion.

- **Double linked list untuk buku tidak pernah memanfaatkan `prev` pointer.** Tidak ada operasi backward traversal. `prev` selalu NULL saat insert. Ini bukan bug tapi waste of design.

---

## 3. IDENTIFIKASI BUG (MAJOR & MINOR)

### MAJOR BUG

| # | Bug | Lokasi | Dampak |
|---|-----|--------|--------|
| M1 | **Buffer overflow pada semua input string** | `tambahBuku` (baris 46,48), `registrasiMahasiswa` (baris 204,206,208,213), `ubahBuku` (baris 126,131), `loginAdmin` (baris 419,421) | Memory corruption, potential code execution, crash tidak terduga. Semua field char[] tidak divalidasi panjangnya. |
| M2 | **Nomor urut buku tidak sinkron antara tampilan dan pencarian** | `pinjamBuku` baris 276-286 vs `cariBukuByNomor` | User melihat nomor 1-3, memilih "2", tapi sistem mencari node urut-2 yang bisa jadi buku berbeda (stok 0 atau tidak muncul). Bisa menyebabkan pinjam buku berstok 0 yang tidak dicegah karena overlap dengan check stok. |
| M3 | **Hapus buku yang sedang dipinjam = orphan record** | `hapusBuku` + `kembalikanBuku` | Mahasiswa tidak bisa mengembalikan buku (loop tidak menemukan judul), stok tidak di-increment, tapi record peminjaman tetap dihapus. Data loss permanen. |
| M4 | **Stream failure (cin >>) tidak di-reset** | Semua `cin >>` (baris 50,52,112,210,295,367,155,451,479) | Jika user memasukkan input non-integer saat diminta integer, `cin.fail()` triggered. Semua subsequent `cin >>` langsung return tanpa meminta input. Program masuk state rusak tanpa pesan error yang jelas. |
| M5 | **NIM duplikat tidak dicek saat registrasi** | `registrasiMahasiswa` + `cariMahasiswa` | Dua mahasiswa berbeda bisa registrasi dengan NIM sama. Login mencari NIM → return node pertama. Node kedua menjadi orphan, tidak bisa diakses. Data inconsistency. |

### MINOR BUG

| # | Bug | Lokasi | Dampak |
|---|-----|--------|--------|
| m1 | **Input tahun 0 dianggap skip (ambigu dengan tahun Masehi)** | `ubahBuku` baris 135-138 | Jika user benar-benar ingin set tahun ke 0, tidak bisa. |
| m2 | **Inkonsistensi skip value: tahun=0, stok=-1** | `ubahBuku` baris 135-143 | User harus hafal dua aturan berbeda. Kontradiksi desain. |
| m3 | **cin.ignore() di loginAdmin mengabaikan 1 karakter per loop** | `loginAdmin` baris 417 | Jika input user <30 karakter dan ada situasi buffer tertentu, bisa discard karakter pertama input username. |
| m4 | **Judul sama tapi author beda dianggap duplikat saat pinjam** | `pinjamBuku` baris 310 | Cek duplikat hanya bandingkan judul, bukan kombinasi judul+author. |
| m5 | **Logout tidak ada feedback visual yang jelas** | `menuMahasiswa` baris 487 | Hanya print "Logout berhasil.", langsung kembali ke main menu tanpa jeda. User bisa tidak sadar sudah logout. |
| m6 | **Tidak ada clear screen** | Semua menu | Layar menumpuk output, UX buruk. |
| m7 | **`prev` pointer di struct Buku tidak pernah digunakan untuk traversal backward** | Struct Buku, semua fungsi | Dead code, membingungkan maintainer. |
| m8 | **Konstanta magic numbers**: 100, 50, 20, 15, 30 tidak didefinisikan sebagai const/define | Seluruh file | Jika perlu ubah batas, harus ubah manual di banyak tempat. |
| m9 | **Login gagal tidak membersihkan buffer sebelum kembali** | `loginAdmin` baris 432-434, `loginMahasiswa` baris 245-252 | Jika login gagal, kembali ke menu utama, stream mungkin masih dalam fail state. |
| m10 | **`ubahBuku`: input tahun 0 tidak membedakan "skip" vs "setel ke 0"** | `ubahBuku` baris 138 | Tahun valid bisa saja 0 dalam konteks tertentu (tidak ada dalam sistem ini), tapi ambigu. |

---

## 4. VALIDATION CHECKLIST

```
- apakah semua 30+ use case sudah tertangani?
  TIDAK. Dari 36 use case yang diidentifikasi:
  - 29 Not Handled
  - 5 Handled (termasuk 1 Partially Handled)
  - 2 ambigu

- apakah masih ada kemungkinan crash?
  YA, BEBERAPA:
  1. Buffer overflow (M1) — jelas bisa crash
  2. Stream failure tanpa reset (M4) — program masuk state error
  3. Nomor urut tidak sinkron (M2) — memilih buku tidak sesuai ekspektasi
  4. Orphan record saat hapus buku (M3) — inkonsistensi data fatal

- apakah semua input sudah tervalidasi?
  TIDAK. Hampir semua input TIDAK divalidasi:
  - Semua integer: tidak ada range check
  - Semua string: tidak ada length check, tidak ada empty check
  - NIM duplikat: tidak ada
  - Stok non-negatif: tidak ada
  - Tahun valid: tidak ada
  - No HP format: tidak ada
```

---

## 5. KESIMPULAN

Kode ini memiliki **5 MAJOR bug** dan **10 MINOR bug**. Dari 36 edge case yang diuji, **80% tidak ditangani**. Kemungkinan crash tinggi terutama dari buffer overflow dan stream failure state. Desain tidak memiliki validasi input sama sekali, dan ada ambiguitas logika yang bisa menyebabkan inkonsistensi data kritis (orphan loan record saat buku dihapus).
