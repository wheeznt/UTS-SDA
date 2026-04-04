#include <iostream>
#include <cstring>
using namespace std;

const int MAX_MAHASISWA = 5;

struct Peminjaman {
    char judul[100];
    Peminjaman* next;
};

struct Mahasiswa {
    char nim[20], nama[50], password[30];
    Peminjaman* daftarPinjam;
};

struct Buku {
    char judul[100], pengarang[100];
    int  tahun, stok;
    Buku *prev, *next;
};

Mahasiswa dataMhs[MAX_MAHASISWA] = {
    {"2508627", "Januar Haykal", "pass123", NULL},
    {"2507821", "Azka Athallah",   "aska123", NULL},
    {"2506208", "Firdaus Akmal",  "daus123", NULL},
};
int jumlahMhs = 3;

Buku* headBuku = NULL;
Buku* tailBuku = NULL;


// ini helper
int inputInt(const char* prompt, int minVal, int maxVal) {
    int nilai;
    while (true) {
        cout << prompt;
        if (cin >> nilai && nilai >= minVal && nilai <= maxVal) {
            cin.ignore();
            return nilai;
        }
        cout << "  Input tidak valid! Masukkan angka " << minVal << "-" << maxVal << ".\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void inputString(const char* prompt, char* buffer, int maxLen) {
    while (true) {
        cout << prompt;
        cin.getline(buffer, maxLen);
        if (strlen(buffer) > 0) break;
        cout << "   Input tidak boleh kosong! Coba lagi.\n";
    }
}


void tambahBuku(const char* judul, const char* pengarang, int tahun, int stok) {
    Buku* b = new Buku;
    strcpy(b->judul, judul); strcpy(b->pengarang, pengarang);
    b->tahun = tahun; b->stok = stok; b->prev = b->next = NULL;
    if (!headBuku) { headBuku = tailBuku = b; }
    else { tailBuku->next = b; b->prev = tailBuku; tailBuku = b; }
}

void inisialisasiDataBuku() {
    tambahBuku("Pemrograman C++",                                   "Yulia Retnowati, S.Pd., M.T.",         2020, 3);
    tambahBuku("Struktur Data",                                     "Yulia Retnowati, S.Pd., M.T.",         2019, 2);
    tambahBuku("Landasan Antropologis dan Sosiologis Pendidikan",   "Yusuf Tri Herlambang, S.Pd., M.Pd.",   2021, 4);
    tambahBuku("Pendidikan Kewarganegaraan",                        "Yayang Furi Furnamasari, M.Pd.",       2018, 2);
    tambahBuku("Membuat Bagan dengan Mudah",                        "Dr. Agus Mulyana, M.Pd.",              2022, 1);
}

void tampilkanBuku() {
    if (!headBuku) { cout << "Belum ada data buku.\n"; return; }
    cout << "\n=== Daftar Buku Perpustakaan ===\n";
    Buku* curr = headBuku;
    for (int no = 1; curr; curr = curr->next, no++) {
        cout << no << ". " << curr->judul << "\n"
             << "   Pengarang : " << curr->pengarang << "\n"
             << "   Tahun     : " << curr->tahun << "\n"
             << "   Stok      : " << curr->stok << "\n"
             << "---------------------------------\n";
    }
}

int hitungTotalBuku() {
    int n = 0;
    for (Buku* b = headBuku; b; b = b->next) n++;
    return n;
}

Buku* cariBukuByNomor(int nomor) {
    Buku* curr = headBuku;
    for (int i = 1; curr && i < nomor; i++, curr = curr->next);
    return curr;
}


void lihatPinjaman(Mahasiswa* mhs) {
    cout << "\n=== Buku yang Kamu Pinjam ===\n";
    if (!mhs->daftarPinjam) { cout << "Kamu belum meminjam buku apapun.\n"; return; }
    Peminjaman* curr = mhs->daftarPinjam;
    for (int no = 1; curr; curr = curr->next, no++)
        cout << no << ". " << curr->judul << "\n";
}

void pinjamBuku(Mahasiswa* mhs) {
    if (!headBuku) { cout << "Belum ada buku di perpustakaan.\n"; return; }

    tampilkanBuku();
    int pilih = inputInt("Nomor buku yang ingin dipinjam (0=batal): ", 0, hitungTotalBuku());
    if (pilih == 0) { cout << "Peminjaman dibatalkan.\n"; return; }

    Buku* target = cariBukuByNomor(pilih);
    if (target->stok == 0) { cout << "Stok buku ini sedang habis.\n"; return; }

    for (Peminjaman* cek = mhs->daftarPinjam; cek; cek = cek->next)
        if (strcmp(cek->judul, target->judul) == 0) {
            cout << "Kamu sudah meminjam buku ini!\n"; return;
        }

    target->stok--;
    Peminjaman* baru = new Peminjaman;
    strcpy(baru->judul, target->judul);
    baru->next = mhs->daftarPinjam;
    mhs->daftarPinjam = baru;
    cout << "Berhasil meminjam \"" << target->judul << "\"!\n";
}

void kembalikanBuku(Mahasiswa* mhs) {
    if (!mhs->daftarPinjam) { cout << "Kamu tidak sedang meminjam buku apapun.\n"; return; }
    lihatPinjaman(mhs);

    int total = 0;
    for (Peminjaman* p = mhs->daftarPinjam; p; p = p->next) total++;
    int pilih = inputInt("Nomor buku yang ingin dikembalikan: ", 1, total);

    Peminjaman *curr = mhs->daftarPinjam, *prev = NULL;
    for (int i = 1; i < pilih; i++) { prev = curr; curr = curr->next; }

    for (Buku* bk = headBuku; bk; bk = bk->next)
        if (strcmp(bk->judul, curr->judul) == 0) { bk->stok++; break; }

    if (prev) prev->next = curr->next;
    else      mhs->daftarPinjam = curr->next;

    cout << "Buku \"" << curr->judul << "\" berhasil dikembalikan. Terima kasih!\n";
    delete curr;
}

void donasiBuku() {
    cout << "\n=== Donasi Buku ===\n";
    char judul[100], pengarang[100];

    inputString("Judul buku   : ", judul, 100);
    inputString("Pengarang    : ", pengarang, 100);
    int tahun  = inputInt("Tahun terbit : ", 1000, 9999);
    int jumlah = inputInt("Jumlah buku  : ", 1, 999);

    for (Buku* b = headBuku; b; b = b->next)
        if (strcmp(b->judul, judul) == 0) {
            b->stok += jumlah;
            cout << "Stok diperbarui menjadi " << b->stok << ". Terima kasih!\n";
            return;
        }

    tambahBuku(judul, pengarang, tahun, jumlah);
    cout << "Buku \"" << judul << "\" berhasil didonasikan!\n";
}

Mahasiswa* login() {
    char nim[20], pw[30];
    cout << "\n=== Login Mahasiswa ===\n";
    cout << "NIM      : "; cin.getline(nim, 20);
    cout << "Password : "; cin.getline(pw, 30);

    for (int i = 0; i < jumlahMhs; i++)
        if (strcmp(dataMhs[i].nim, nim) == 0 && strcmp(dataMhs[i].password, pw) == 0) {
            cout << "Selamat datang, " << dataMhs[i].nama << "!\n";
            return &dataMhs[i];
        }

    cout << "NIM atau password salah. Coba lagi.\n";
    return NULL;
}


void menuMahasiswa(Mahasiswa* mhs) {
    int pilih;
    do {
        cout << "\n==============================\n"
             << "     MENU MAHASISWA\n"
             << "==============================\n"
             << "Halo, " << mhs->nama << "!\n"
             << "------------------------------\n"
             << "1. Lihat Daftar Buku\n"
             << "2. Pinjam Buku\n"
             << "3. Lihat Buku yang Dipinjam\n"
             << "4. Kembalikan Buku\n"
             << "5. Donasi Buku\n"
             << "0. Logout\n";
        pilih = inputInt("Pilihan: ", 0, 5);

        switch (pilih) {
            case 1: tampilkanBuku();     break;
            case 2: pinjamBuku(mhs);     break;
            case 3: lihatPinjaman(mhs);  break;
            case 4: kembalikanBuku(mhs); break;
            case 5: donasiBuku();        break;
            case 0: cout << "Logout berhasil. Sampai jumpa, " << mhs->nama << "!\n"; break;
        }
    } while (pilih != 0);
}

int main() {
    inisialisasiDataBuku();
    cout << "========================================\n"
         << "    SISTEM PERPUSTAKAAN MAHASISWA\n"
         << "    UPI Kampus Cibiru\n"
         << "========================================\n";

    Mahasiswa* mhs = NULL;
    while (!mhs) mhs = login();
    menuMahasiswa(mhs);

    return 0;
}