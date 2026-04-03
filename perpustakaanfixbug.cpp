#include <iostream>
#include <cstring>
using namespace std;

// ============================================================
//   SISTEM PERPUSTAKAAN SEDERHANA
//   Mata Kuliah : Struktur Data dan Algoritma (RL216)
//   Materi      : Array, Struct, Pointer,
//                 Single Linked List, Double Linked List
// ============================================================


// ==================== KONSTANTA ====================

const int MAX_MAHASISWA = 5;  // <-- Array: kapasitas data mahasiswa


// ==================== STRUCT ====================

// Node Peminjaman -> Single Linked List
struct Peminjaman {
    char judul[100];
    Peminjaman* next;   // pointer ke node berikutnya
};

// Data Mahasiswa -> disimpan dalam Array of Struct
struct Mahasiswa {
    char nim[20];
    char nama[50];
    char password[30];
    Peminjaman* daftarPinjam;   // pointer ke kepala single linked list
};

// Node Buku -> Double Linked List
struct Buku {
    char judul[100];
    char pengarang[100];
    int  tahun;
    int  stok;
    Buku* prev;   // pointer ke node sebelumnya
    Buku* next;   // pointer ke node berikutnya
};


// ==================== VARIABEL GLOBAL ====================

// ARRAY of Struct: data akun mahasiswa yang bisa login
Mahasiswa dataMhs[MAX_MAHASISWA] = {
    {"2301001", "Januar Haykal",  "pass123", NULL},
    {"2301002", "Siti Aisyah",    "siti456", NULL},
    {"2301003", "Budi Santoso",   "budi789", NULL},
};
int jumlahMhs = 3;

// Head & tail untuk Double Linked List buku
Buku* headBuku = NULL;
Buku* tailBuku = NULL;


// ==================== FUNGSI BUKU (Double Linked List) ====================

// Tambah buku baru di akhir double linked list
void tambahBuku(const char* judul, const char* pengarang, int tahun, int stok) {
    Buku* baru   = new Buku;
    strcpy(baru->judul,     judul);
    strcpy(baru->pengarang, pengarang);
    baru->tahun  = tahun;
    baru->stok   = stok;
    baru->prev   = NULL;
    baru->next   = NULL;

    if (headBuku == NULL) {
        headBuku = tailBuku = baru;
    } else {
        tailBuku->next = baru;
        baru->prev     = tailBuku;
        tailBuku       = baru;
    }
}

// Isi data buku awal saat program pertama kali dijalankan
void inisialisasiDataBuku() {
    tambahBuku("Pemrograman C++",            "Bjarne Stroustrup", 2020, 3);
    tambahBuku("Struktur Data",              "Mark Allen Weiss",  2019, 2);
    tambahBuku("Algoritma dan Pemrograman",  "Rinaldi Munir",     2021, 4);
    tambahBuku("Basis Data",                 "Ramez Elmasri",     2018, 2);
    tambahBuku("Rekayasa Perangkat Lunak",   "Ian Sommerville",   2022, 1);
}

// Tampilkan semua buku (traverse double linked list dari head ke tail)
void tampilkanBuku() {
    if (headBuku == NULL) {
        cout << "Belum ada data buku." << endl;
        return;
    }
    cout << "\n=== Daftar Buku Perpustakaan ===" << endl;
    Buku* curr = headBuku;
    int   no   = 1;
    while (curr != NULL) {
        cout << no << ". " << curr->judul << endl;
        cout << "   Pengarang : " << curr->pengarang << endl;
        cout << "   Tahun     : " << curr->tahun     << endl;
        cout << "   Stok      : " << curr->stok      << endl;
        cout << "---------------------------------" << endl;
        curr = curr->next;
        no++;
    }
}

// Cari node buku berdasarkan nomor urut tampilan (1-based)
Buku* cariBukuByNomor(int nomor) {
    Buku* curr = headBuku;
    int   no   = 1;
    while (curr != NULL) {
        if (no == nomor) return curr;
        curr = curr->next;
        no++;
    }
    return NULL;
}


// ==================== FUNGSI PEMINJAMAN (Single Linked List) ====================

// Tampilkan daftar buku yang sedang dipinjam mahasiswa
void lihatPinjaman(Mahasiswa* mhs) {
    cout << "\n=== Buku yang Kamu Pinjam ===" << endl;
    if (mhs->daftarPinjam == NULL) {
        cout << "Kamu belum meminjam buku apapun." << endl;
        return;
    }
    Peminjaman* curr = mhs->daftarPinjam;
    int no = 1;
    while (curr != NULL) {
        cout << no << ". " << curr->judul << endl;
        curr = curr->next;
        no++;
    }
}

// Pinjam buku: kurangi stok, tambah ke single linked list peminjaman
void pinjamBuku(Mahasiswa* mhs) {
    tampilkanBuku();

    cout << "Masukkan nomor buku yang ingin dipinjam (0 untuk batal): ";
    int pilih;
    cin >> pilih;

    if (pilih == 0) {
        cout << "Peminjaman dibatalkan." << endl;
        return;
    }

    Buku* target = cariBukuByNomor(pilih);
    if (target == NULL) {
        cout << "Nomor buku tidak ditemukan!" << endl;
        return;
    }
    if (target->stok == 0) {
        cout << "Maaf, stok buku ini sedang habis." << endl;
        return;
    }

    // Cek apakah buku sudah pernah dipinjam
    Peminjaman* cek = mhs->daftarPinjam;
    while (cek != NULL) {
        if (strcmp(cek->judul, target->judul) == 0) {
            cout << "Kamu sudah meminjam buku ini sebelumnya!" << endl;
            return;
        }
        cek = cek->next;
    }

    // Kurangi stok buku
    target->stok--;

    // Tambah ke depan single linked list peminjaman
    Peminjaman* baru = new Peminjaman;
    strcpy(baru->judul, target->judul);
    baru->next           = mhs->daftarPinjam;
    mhs->daftarPinjam    = baru;

    cout << "Berhasil meminjam buku \"" << target->judul << "\"!" << endl;
}

// Kembalikan buku: hapus dari single linked list, tambah stok buku kembali
void kembalikanBuku(Mahasiswa* mhs) {
    if (mhs->daftarPinjam == NULL) {
        cout << "Kamu tidak sedang meminjam buku apapun." << endl;
        return;
    }

    lihatPinjaman(mhs);
    cout << "Masukkan nomor buku yang ingin dikembalikan: ";
    int pilih;
    cin >> pilih;

    Peminjaman* curr = mhs->daftarPinjam;
    Peminjaman* prev = NULL;
    int idx = 1;
    while (curr != NULL && idx < pilih) {
        prev = curr;
        curr = curr->next;
        idx++;
    }

    if (curr == NULL) {
        cout << "Nomor tidak valid!" << endl;
        return;
    }

    // Kembalikan stok buku di double linked list
    Buku* bk = headBuku;
    while (bk != NULL) {
        if (strcmp(bk->judul, curr->judul) == 0) {
            bk->stok++;
            break;
        }
        bk = bk->next;
    }

    // Hapus node dari single linked list peminjaman
    if (prev == NULL) mhs->daftarPinjam = curr->next;
    else              prev->next        = curr->next;

    cout << "Buku \"" << curr->judul << "\" berhasil dikembalikan. Terima kasih!" << endl;
    delete curr;
}

// Donasi buku: jika sudah ada tambah stok, jika baru tambah ke double linked list
void donasiBuku() {
    cout << "\n=== Donasi Buku ===" << endl;

    char judul[100], pengarang[100];
    int  tahun, jumlah;

    cout << "Judul buku   : ";
    cin.getline(judul, 100);
    cout << "Pengarang    : ";
    cin.getline(pengarang, 100);
    cout << "Tahun terbit : ";
    cin >> tahun;
    cout << "Jumlah buku  : ";
    cin >> jumlah;

    // Cek apakah buku sudah ada di katalog
    Buku* curr = headBuku;
    while (curr != NULL) {
        if (strcmp(curr->judul, judul) == 0) {
            curr->stok += jumlah;
            cout << "Buku sudah ada di katalog. Stok bertambah menjadi "
                 << curr->stok << ". Terima kasih atas donasinya!" << endl;
            return;
        }
        curr = curr->next;
    }

    // Buku baru -> tambah ke double linked list
    tambahBuku(judul, pengarang, tahun, jumlah);
    cout << "Terima kasih! Buku \"" << judul << "\" berhasil didonasikan ke perpustakaan." << endl;
}


// ==================== LOGIN ====================

// Login sederhana: cocokkan NIM dan password dari array dataMhs
Mahasiswa* login() {
    char nim[20], pw[30];

    cout << "\n=== Login Mahasiswa ===" << endl;
    cout << "NIM      : ";
    cin.getline(nim, 20);
    cout << "Password : ";
    cin.getline(pw, 30);

    for (int i = 0; i < jumlahMhs; i++) {
        if (strcmp(dataMhs[i].nim, nim) == 0 &&
            strcmp(dataMhs[i].password, pw) == 0) {
            cout << "Login berhasil! Selamat datang, "
                 << dataMhs[i].nama << "!" << endl;
            return &dataMhs[i];
        }
    }

    cout << "NIM atau password salah. Coba lagi." << endl;
    return NULL;
}


// ==================== MENU MAHASISWA ====================

void menuMahasiswa(Mahasiswa* mhs) {
    int pilih;
    do {
        cout << "\n==============================" << endl;
        cout << "     MENU MAHASISWA"            << endl;
        cout << "==============================" << endl;
        cout << "Halo, " << mhs->nama << "!"     << endl;
        cout << "------------------------------" << endl;
        cout << "1. Lihat Daftar Buku"           << endl;
        cout << "2. Pinjam Buku"                 << endl;
        cout << "3. Lihat Buku yang Dipinjam"    << endl;
        cout << "4. Kembalikan Buku"             << endl;
        cout << "5. Donasi Buku"                 << endl;
        cout << "0. Logout"                      << endl;
        cout << "Pilihan: ";
        cin  >> pilih;
        cin.ignore();   // buang newline sisa setelah angka

        switch (pilih) {
            case 1: tampilkanBuku();       break;
            case 2: pinjamBuku(mhs);       break;
            case 3: lihatPinjaman(mhs);    break;
            case 4: kembalikanBuku(mhs);   break;
            case 5: donasiBuku();          break;
            case 0: cout << "Logout berhasil. Sampai jumpa, "
                         << mhs->nama << "!" << endl; break;
            default: cout << "Pilihan tidak valid. Coba lagi." << endl;
        }
    } while (pilih != 0);
}


// ==================== MAIN ====================

int main() {
    // Isi data buku awal saat program dijalankan
    inisialisasiDataBuku();

    cout << "========================================" << endl;
    cout << "    SISTEM PERPUSTAKAAN MAHASISWA"       << endl;
    cout << "    UPI Kampus Cibiru"                   << endl;
    cout << "========================================" << endl;

    // Loop login sampai berhasil
    Mahasiswa* mhs = NULL;
    while (mhs == NULL) {
        mhs = login();
    }

    // Setelah login langsung ke menu mahasiswa
    menuMahasiswa(mhs);

    return 0;
}