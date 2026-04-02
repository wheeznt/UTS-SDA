#include <iostream>
#include <cstring>
#include <limits>
using namespace std;

// ============ KONSTANTA UKURAN BUFFER ============
const int MAX_JUDUL = 100;
const int MAX_AUTHOR = 100;
const int MAX_NAMA = 50;
const int MAX_NIM = 20;
const int MAX_PRODI = 50;
const int MAX_NOHP = 15;
const int MAX_PASSWORD = 30;
const int MAX_USERNAME = 30;
const int MAX_BUFFER = 100;

// ============ STRUCT BUKU (Double Linked List) ============
struct Buku {
    char judul[MAX_JUDUL];
    char author[MAX_AUTHOR];
    int tahunTerbit;
    int stok;
    Buku *prev;
    Buku *next;
};

// ============ STRUCT PEMINJAMAN (Single Linked List) ============
struct Peminjaman {
    char judulBuku[MAX_JUDUL];
    char authorBuku[MAX_AUTHOR];
    int tahunTerbit;
    Peminjaman *next;
};

// ============ STRUCT MAHASISWA (Single Linked List) ============
struct Mahasiswa {
    char nama[MAX_NAMA];
    char nim[MAX_NIM];
    char password[MAX_PASSWORD];
    char prodi[MAX_PRODI];
    int angkatan;
    char noHp[MAX_NOHP];
    Peminjaman *daftarPinjaman;
    Mahasiswa *next;
};

// ============ VARIABEL GLOBAL ============
Buku *headBuku = NULL;
Buku *tailBuku = NULL;
Mahasiswa *headMhs = NULL;

// ============ HELPER: Reset stream cin ============
void resetCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============ HELPER: Input string dengan validasi panjang ============
void inputString(const char *prompt, char *buffer, int maxLen) {
    while (true) {
        cout << prompt;
        cin.getline(buffer, maxLen);
        if (strlen(buffer) == 0) {
            cout << "  Input tidak boleh kosong! Coba lagi." << endl;
        } else if (strlen(buffer) >= maxLen - 1) {
            cout << "  Input terlalu panjang! Maksimal " << (maxLen - 1) << " karakter. Coba lagi." << endl;
        } else {
            break;
        }
    }
}

// ============ HELPER: Input integer dengan validasi range ============
int inputInt(const char *prompt, int minVal, int maxVal, int skipValue) {
    int value;
    while (true) {
        cout << prompt;
        if (!(cin >> value)) {
            cout << "  Input harus berupa angka! Coba lagi." << endl;
            resetCin();
            continue;
        }
        resetCin();

        // Skip: gunakan nilai di luar range (misal skipValue di luar min-max)
        if (minVal <= skipValue && skipValue <= maxVal) {
            if (value == skipValue) return value;
        }

        if (value < minVal || value > maxVal) {
            cout << "  Input harus antara " << minVal << " - " << maxVal << "! Coba lagi." << endl;
        } else {
            return value;
        }
    }
}

// ============ HELPER: Input integer (tanpa batas atas) ============
int inputIntUnbound(const char *prompt, int minVal) {
    int value;
    while (true) {
        cout << prompt;
        if (!(cin >> value)) {
            cout << "  Input harus berupa angka! Coba lagi." << endl;
            resetCin();
            continue;
        }
        resetCin();
        if (value < minVal) {
            cout << "  Input tidak boleh kurang dari " << minVal << "! Coba lagi." << endl;
        } else {
            return value;
        }
    }
}

// ============ HELPER: Input char ya/tidak ============
char inputYaTidak(const char *prompt) {
    char jawab;
    while (true) {
        cout << prompt;
        if (!(cin >> jawab)) {
            cout << "  Input tidak valid! Coba lagi." << endl;
            resetCin();
            continue;
        }
        resetCin();
        if (jawab == 'y' || jawab == 'Y' || jawab == 'n' || jawab == 'N') {
            return jawab;
        }
        cout << "  Masukkan 'y' atau 'n'! Coba lagi." << endl;
    }
}

// ============ FUNGSI BUKU (CRUD - Double Linked List) ============

void tambahBuku() {
    Buku *baru = new Buku;
    cout << "\n=== Tambah Buku Baru ===" << endl;

    inputString("Judul       : ", baru->judul, MAX_JUDUL);
    inputString("Pengarang   : ", baru->author, MAX_AUTHOR);
    baru->tahunTerbit = inputIntUnbound("Tahun Terbit: ", 1);
    baru->stok = inputIntUnbound("Stok        : ", 0);

    baru->prev = NULL;
    baru->next = NULL;

    if (headBuku == NULL) {
        headBuku = baru;
        tailBuku = baru;
    } else {
        tailBuku->next = baru;
        baru->prev = tailBuku;
        tailBuku = baru;
    }
    cout << "Buku berhasil ditambahkan!" << endl;
}

void tampilkanBuku() {
    if (headBuku == NULL) {
        cout << "\nBelum ada data buku." << endl;
        return;
    }
    cout << "\n=======================================================" << endl;
    cout << "                    DAFTAR BUKU                        " << endl;
    cout << "=======================================================" << endl;

    Buku *curr = headBuku;
    int no = 1;
    while (curr != NULL) {
        cout << no << ". " << curr->judul << endl;
        cout << "   Pengarang    : " << curr->author << endl;
        cout << "   Tahun Terbit : " << curr->tahunTerbit << endl;
        cout << "   Stok         : " << curr->stok << endl;
        cout << "-------------------------------------------------------" << endl;
        curr = curr->next;
        no++;
    }
}

// cari buku berdasarkan nomor urut TAMPILAN (1-based)
// khusus untuk konteks peminjaman agar nomor sinkron
Buku* cariBukuByNomorTampilan(int nomor) {
    Buku *curr = headBuku;
    int no = 1;
    while (curr != NULL) {
        if (no == nomor) return curr;
        curr = curr->next;
        no++;
    }
    return NULL;
}

// cari buku berdasarkan nomor urut (1-based, semua buku)
Buku* cariBukuByNomor(int nomor) {
    return cariBukuByNomorTampilan(nomor);
}

// ============ FUNGSI MENCEGAH HAPUS BUKU YANG SEDANG DIPINJAM ============
bool cekBukuSedangDipinjam(const char *judul) {
    Mahasiswa *mhs = headMhs;
    while (mhs != NULL) {
        Peminjaman *p = mhs->daftarPinjaman;
        while (p != NULL) {
            if (strcmp(p->judulBuku, judul) == 0) {
                return true;
            }
            p = p->next;
        }
        mhs = mhs->next;
    }
    return false;
}

void ubahBuku() {
    if (headBuku == NULL) {
        cout << "\nBelum ada data buku." << endl;
        return;
    }

    tampilkanBuku();

    int pilih = inputIntUnbound("Pilih nomor buku yang ingin diubah: ", 1);

    Buku *target = cariBukuByNomor(pilih);
    if (target == NULL) {
        cout << "Nomor buku tidak ditemukan!" << endl;
        return;
    }

    cout << "\n=== Ubah Data Buku ===" << endl;
    cout << "(Kosongkan input untuk skip)" << endl;

    char buffer[MAX_BUFFER];

    // ❌ JANGAN ADA cin.ignore DI SINI

    // ===== Judul =====
    cout << "Judul baru [" << target->judul << "]: ";
    cin.getline(buffer, MAX_BUFFER);
    if (strlen(buffer) > 0) {
        strcpy(target->judul, buffer);
    }

    // ===== Pengarang =====
    cout << "Pengarang baru [" << target->author << "]: ";
    cin.getline(buffer, MAX_BUFFER);
    if (strlen(buffer) > 0) {
        strcpy(target->author, buffer);
    }

    // ===== Tahun =====
    cout << "Tahun terbit baru [" << target->tahunTerbit << "]: ";
    cin.getline(buffer, MAX_BUFFER);
    if (strlen(buffer) > 0) {
        int thn = atoi(buffer);
        if (thn > 0) target->tahunTerbit = thn;
    }

    // ===== Stok =====
    cout << "Stok baru [" << target->stok << "]: ";
    cin.getline(buffer, MAX_BUFFER);
    if (strlen(buffer) > 0) {
        int s = atoi(buffer);
        if (s >= 0) target->stok = s;
    }

    cout << "Data buku berhasil diubah!" << endl;
}

void hapusBuku() {
    if (headBuku == NULL) {
        cout << "\nBelum ada data buku." << endl;
        return;
    }
    tampilkanBuku();

    int pilih = inputIntUnbound("Pilih nomor buku yang ingin dihapus: ", 1);

    Buku *target = cariBukuByNomor(pilih);
    if (target == NULL) {
        cout << "Nomor buku tidak ditemukan!" << endl;
        return;
    }

    // Fix M3: Cek apakah buku sedang dipinjam mahasiswa
    if (cekBukuSedangDipinjam(target->judul)) {
        cout << "\nBuku \"" << target->judul << "\" tidak bisa dihapus!" << endl;
        cout << "Buku tersebut sedang dipinjam oleh mahasiswa." << endl;
        cout << "Harap minta mahasiswa mengembalikan buku terlebih dahulu." << endl;
        return;
    }

    // hapus node dari double linked list
    if (target == headBuku && target == tailBuku) {
        headBuku = NULL;
        tailBuku = NULL;
    } else if (target == headBuku) {
        headBuku = headBuku->next;
        headBuku->prev = NULL;
    } else if (target == tailBuku) {
        tailBuku = tailBuku->prev;
        tailBuku->next = NULL;
    } else {
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }

    cout << "Buku \"" << target->judul << "\" berhasil dihapus!" << endl;
    delete target;
}

// ============ FUNGSI MAHASISWA ============

Mahasiswa* cariMahasiswa(const char *nim) {
    Mahasiswa *curr = headMhs;
    while (curr != NULL) {
        if (strcmp(curr->nim, nim) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

Mahasiswa* registrasiMahasiswa(const char *nimInput) {
    // Fix M5: Cek NIM duplikat
    if (cariMahasiswa(nimInput) != NULL) {
        cout << "\nNIM " << nimInput << " sudah terdaftar!" << endl;
        return NULL;
    }

    Mahasiswa *baru = new Mahasiswa;
    cout << "\n=== Registrasi Mahasiswa Baru ===" << endl;
    cout << "NIM           : " << nimInput << " (otomatis)" << endl;
    strcpy(baru->nim, nimInput);

    inputString("Nama          : ", baru->nama, MAX_NAMA);
    inputString("Buat Password : ", baru->password, MAX_PASSWORD);
    inputString("Program Studi : ", baru->prodi, MAX_PRODI);
    baru->angkatan = inputIntUnbound("Tahun Angkatan: ", 2000);
    inputString("Nomor HP      : ", baru->noHp, MAX_NOHP);

    baru->daftarPinjaman = NULL;
    baru->next = NULL;

    // tambah ke depan linked list mahasiswa
    if (headMhs == NULL) {
        headMhs = baru;
    } else {
        baru->next = headMhs;
        headMhs = baru;
    }

    cout << "Registrasi berhasil! Selamat datang, " << baru->nama << endl;
    return baru;
}

Mahasiswa* loginMahasiswa() {
    char nim[MAX_NIM];
    cout << "\n=== Login Mahasiswa ===" << endl;
    cout << "Masukkan NIM: ";
    cin.getline(nim, MAX_NIM);

    // Validasi NIM tidak kosong
    while (strlen(nim) == 0) {
        cout << "NIM tidak boleh kosong! Masukkan NIM: ";
        cin.getline(nim, MAX_NIM);
    }

    Mahasiswa *mhs = cariMahasiswa(nim);
    if (mhs != NULL) {
        char pw[MAX_PASSWORD];
        cout << "Password    : ";
        cin.getline(pw, MAX_PASSWORD);

        if (strcmp(mhs->password, pw) == 0) {
            cout << "Login berhasil! Selamat datang, " << mhs->nama << "!" << endl;
            return mhs;
        } else {
            cout << "Password salah!" << endl;
            return NULL;
        }
    }

    cout << "NIM belum terdaftar." << endl;
    char jawab = inputYaTidak("Ingin registrasi sekarang? (y/n): ");
    if (jawab == 'y' || jawab == 'Y') {
        return registrasiMahasiswa(nim);
    }
    return NULL;
}

void tampilProfilMahasiswa(Mahasiswa *mhs) {
    cout << "\n=== Profil Mahasiswa ===" << endl;
    cout << "Nama          : " << mhs->nama << endl;
    cout << "NIM           : " << mhs->nim << endl;
    cout << "Program Studi : " << mhs->prodi << endl;
    cout << "Angkatan      : " << mhs->angkatan << endl;
    cout << "No. HP        : " << mhs->noHp << endl;
}

// ============ FUNGSI PEMINJAMAN ============

void pinjamBuku(Mahasiswa *mhs) {
    if (headBuku == NULL) {
        cout << "\nBelum ada buku yang tersedia." << endl;
        return;
    }

    // Tampilkan HANYA buku berstok > 0, nomor urut sinkron dengan linked list
    cout << "\n=== Daftar Buku Tersedia ===" << endl;
    Buku *curr = headBuku;
    int no = 1;
    int jumlahTersedia = 0;
    while (curr != NULL) {
        if (curr->stok > 0) {
            cout << no << ". " << curr->judul << " (" << curr->author << ", "
                 << curr->tahunTerbit << ") - Stok: " << curr->stok << endl;
            jumlahTersedia++;
        }
        curr = curr->next;
        no++;
    }

    if (jumlahTersedia == 0) {
        cout << "Semua buku sedang habis stoknya." << endl;
        return;
    }

    // Fix M2: Nomor sinkron — cari berdasarkan linked list posisi,
    // tapi hanya return jika stok > 0
    int pilih = inputIntUnbound("\nPilih nomor buku yang ingin dipinjam: ", 1);

    Buku *target = NULL;
    curr = headBuku;
    no = 1;
    while (curr != NULL) {
        if (curr->stok > 0 && no == pilih) {
            target = curr;
            break;
        }
        if (curr->stok > 0) {
            no++;
        }
        curr = curr->next;
    }

    if (target == NULL) {
        cout << "Nomor buku tidak valid!" << endl;
        return;
    }

    // Fix: cek duplikat dengan judul + author, bukan hanya judul
    Peminjaman *cek = mhs->daftarPinjaman;
    while (cek != NULL) {
        if (strcmp(cek->judulBuku, target->judul) == 0
            && strcmp(cek->authorBuku, target->author) == 0) {
            cout << "Kamu sudah meminjam buku ini!" << endl;
            return;
        }
        cek = cek->next;
    }

    target->stok--;

    Peminjaman *baru = new Peminjaman;
    strncpy(baru->judulBuku, target->judul, MAX_JUDUL - 1);
    baru->judulBuku[MAX_JUDUL - 1] = '\0';
    strncpy(baru->authorBuku, target->author, MAX_AUTHOR - 1);
    baru->authorBuku[MAX_AUTHOR - 1] = '\0';
    baru->tahunTerbit = target->tahunTerbit;
    baru->next = NULL;

    if (mhs->daftarPinjaman == NULL) {
        mhs->daftarPinjaman = baru;
    } else {
        baru->next = mhs->daftarPinjaman;
        mhs->daftarPinjaman = baru;
    }

    cout << "Buku \"" << target->judul << "\" berhasil dipinjam!" << endl;
}

void lihatPinjaman(Mahasiswa *mhs) {
    cout << "\n=== Riwayat Peminjaman ===" << endl;
    cout << "Mahasiswa: " << mhs->nama << " (" << mhs->nim << ")" << endl;
    cout << "-------------------------------------------------------" << endl;

    if (mhs->daftarPinjaman == NULL) {
        cout << "Belum ada buku yang dipinjam." << endl;
        return;
    }

    Peminjaman *curr = mhs->daftarPinjaman;
    int no = 1;
    while (curr != NULL) {
        cout << no << ". " << curr->judulBuku << endl;
        cout << "   Pengarang    : " << curr->authorBuku << endl;
        cout << "   Tahun Terbit : " << curr->tahunTerbit << endl;
        curr = curr->next;
        no++;
    }
}

void kembalikanBuku(Mahasiswa *mhs) {
    if (mhs->daftarPinjaman == NULL) {
        cout << "\nKamu tidak sedang meminjam buku apapun." << endl;
        return;
    }

    lihatPinjaman(mhs);
    int pilih = inputIntUnbound("Pilih nomor buku yang ingin dikembalikan: ", 1);

    Peminjaman *curr = mhs->daftarPinjaman;
    Peminjaman *prev = NULL;
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

    // Fix M3: Jika buku sudah tidak ada di katalog, tetap hapus record tapi beri warning
    bool bukuDitemukan = false;
    Buku *bk = headBuku;
    while (bk != NULL) {
        if (strcmp(bk->judul, curr->judulBuku) == 0) {
            bk->stok++;
            bukuDitemukan = true;
            break;
        }
        bk = bk->next;
    }

    if (!bukuDitemukan) {
        cout << "Warning: Buku \"" << curr->judulBuku << "\" tidak ditemukan di katalog." << endl;
        cout << "Record peminjaman tetap dihapus tanpa menambah stok." << endl;
    }

    if (prev == NULL) {
        mhs->daftarPinjaman = curr->next;
    } else {
        prev->next = curr->next;
    }

    cout << "Buku \"" << curr->judulBuku << "\" berhasil dikembalikan!" << endl;
    delete curr;
}

// ============ MENU ============

bool loginAdmin() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int percobaan = 3;

    cout << "\n=== Login Admin ===" << endl;
    while (percobaan > 0) {
        cout << "Username: ";
        cin.getline(username, MAX_USERNAME);
        cout << "Password: ";
        cin.getline(password, MAX_PASSWORD);

        if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
            cout << "Login berhasil! Selamat datang, Admin." << endl;
            return true;
        }

        percobaan--;
        if (percobaan > 0) {
            cout << "Username atau password salah! Sisa percobaan: " << percobaan << endl;
        } else {
            cout << "Gagal login! Percobaan habis." << endl;
        }
    }
    return false;
}

void menuAdmin() {
    int pilih;
    do {
        cout << "\n==============================" << endl;
        cout << "       MENU ADMIN" << endl;
        cout << "==============================" << endl;
        cout << "1. Tampilkan Daftar Buku" << endl;
        cout << "2. Tambah Buku" << endl;
        cout << "3. Ubah Data Buku" << endl;
        cout << "4. Hapus Buku" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        cout << "Pilihan: ";

        if (!(cin >> pilih)) {
            cout << "Pilihan tidak valid! Masukkan angka." << endl;
            resetCin();
            continue;
        }
        resetCin();

        switch (pilih) {
            case 1: tampilkanBuku(); break;
            case 2: tambahBuku(); break;
            case 3: ubahBuku(); break;
            case 4: hapusBuku(); break;
            case 0: cout << "Kembali ke menu utama..." << endl; break;
            default: cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilih != 0);
}

void menuMahasiswa(Mahasiswa *mhs) {
    int pilih;
    do {
        cout << "\n==============================" << endl;
        cout << "     MENU MAHASISWA" << endl;
        cout << "==============================" << endl;
        cout << "Halo, " << mhs->nama << "!" << endl;
        cout << "------------------------------" << endl;
        cout << "1. Lihat Daftar Buku" << endl;
        cout << "2. Pinjam Buku" << endl;
        cout << "3. Lihat Buku yang Dipinjam" << endl;
        cout << "4. Kembalikan Buku" << endl;
        cout << "5. Lihat Profil" << endl;
        cout << "0. Logout" << endl;
        cout << "Pilihan: ";

        if (!(cin >> pilih)) {
            cout << "Pilihan tidak valid! Masukkan angka." << endl;
            resetCin();
            continue;
        }
        resetCin();

        switch (pilih) {
            case 1: tampilkanBuku(); break;
            case 2: pinjamBuku(mhs); break;
            case 3: lihatPinjaman(mhs); break;
            case 4: kembalikanBuku(mhs); break;
            case 5: tampilProfilMahasiswa(mhs); break;
            case 0: cout << "Logout berhasil." << endl; break;
            default: cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilih != 0);
}

// ============ FUNGSI TAMBAHAN: bersihkan memori ============
void bersihkanMemori() {
    Buku *bk = headBuku;
    while (bk != NULL) {
        Buku *temp = bk;
        bk = bk->next;
        delete temp;
    }

    Mahasiswa *mhs = headMhs;
    while (mhs != NULL) {
        Peminjaman *p = mhs->daftarPinjaman;
        while (p != NULL) {
            Peminjaman *tmp = p;
            p = p->next;
            delete tmp;
        }
        Mahasiswa *tmpMhs = mhs;
        mhs = mhs->next;
        delete tmpMhs;
    }
}

// ============ MAIN ============
int main() {
    int pilih;

    cout << "=======================================================" << endl;
    cout << "    SISTEM MANAJEMEN PERPUSTAKAAN SEDERHANA" << endl;
    cout << "=======================================================" << endl;

    do {
        cout << "\n==============================" << endl;
        cout << "        MENU UTAMA" << endl;
        cout << "==============================" << endl;
        cout << "1. Masuk sebagai Admin" << endl;
        cout << "2. Masuk sebagai Mahasiswa" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan: ";

        if (!(cin >> pilih)) {
            cout << "Pilihan tidak valid! Masukkan angka." << endl;
            resetCin();
            continue;
        }
        resetCin();

        switch (pilih) {
            case 1:
                if (loginAdmin()) {
                    menuAdmin();
                }
                break;
            case 2: {
                Mahasiswa *mhs = loginMahasiswa();
                if (mhs != NULL) {
                    menuMahasiswa(mhs);
                }
                break;
            }
            case 0:
                cout << "\nTerima kasih telah menggunakan sistem perpustakaan!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilih != 0);

    bersihkanMemori();
    return 0;
}
