#include <iostream>
#include <cstring>
using namespace std;

// ============ STRUCT BUKU (Double Linked List) ============
struct Buku {
    char judul[100];
    char author[100];
    int tahunTerbit;
    int stok;
    Buku *prev;
    Buku *next;
};

// ============ STRUCT PEMINJAMAN (Single Linked List) ============
struct Peminjaman {
    char judulBuku[100];
    char authorBuku[100];
    int tahunTerbit;
    Peminjaman *next;
};

// ============ STRUCT MAHASISWA (Single Linked List) ============
struct Mahasiswa {
    char nama[50];
    char nim[20];
    char password[30];
    char prodi[50];
    int angkatan;
    char noHp[15];
    Peminjaman *daftarPinjaman; // linked list peminjaman tiap mahasiswa
    Mahasiswa *next;
};

// ============ VARIABEL GLOBAL ============
Buku *headBuku = NULL;
Buku *tailBuku = NULL;
Mahasiswa *headMhs = NULL;

// ============ FUNGSI BUKU (CRUD - Double Linked List) ============

void tambahBuku() {
    Buku *baru = new Buku;
    cout << "\n=== Tambah Buku Baru ===" << endl;
    cin.ignore();
    cout << "Judul       : ";
    cin.getline(baru->judul, 100);
    cout << "Pengarang   : ";
    cin.getline(baru->author, 100);
    cout << "Tahun Terbit: ";
    cin >> baru->tahunTerbit;
    cout << "Stok        : ";
    cin >> baru->stok;

    baru->prev = NULL;
    baru->next = NULL;

    // kalau list masih kosong
    if (headBuku == NULL) {
        headBuku = baru;
        tailBuku = baru;
    } else {
        // tambah di belakang
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

// cari buku berdasarkan nomor urut, return pointer ke node
Buku* cariBukuByNomor(int nomor) {
    Buku *curr = headBuku;
    int idx = 1;
    while (curr != NULL) {
        if (idx == nomor) return curr;
        curr = curr->next;
        idx++;
    }
    return NULL;
}

void ubahBuku() {
    if (headBuku == NULL) {
        cout << "\nBelum ada data buku." << endl;
        return;
    }
    tampilkanBuku();
    int pilih;
    cout << "Pilih nomor buku yang ingin diubah: ";
    cin >> pilih;

    Buku *target = cariBukuByNomor(pilih);
    if (target == NULL) {
        cout << "Nomor buku tidak ditemukan!" << endl;
        return;
    }

    cout << "\n=== Ubah Data Buku ===" << endl;
    cout << "(Kosongkan jika tidak ingin mengubah, ketik '-' lalu enter)" << endl;
    cin.ignore();

    char buffer[100];
    cout << "Judul baru [" << target->judul << "]: ";
    cin.getline(buffer, 100);
    if (strcmp(buffer, "-") != 0 && strlen(buffer) > 0)
        strcpy(target->judul, buffer);

    cout << "Pengarang baru [" << target->author << "]: ";
    cin.getline(buffer, 100);
    if (strcmp(buffer, "-") != 0 && strlen(buffer) > 0)
        strcpy(target->author, buffer);

    cout << "Tahun terbit baru [" << target->tahunTerbit << "] (0 = skip): ";
    int thn;
    cin >> thn;
    if (thn != 0) target->tahunTerbit = thn;

    cout << "Stok baru [" << target->stok << "] (-1 = skip): ";
    int s;
    cin >> s;
    if (s != -1) target->stok = s;

    cout << "Data buku berhasil diubah!" << endl;
}

void hapusBuku() {
    if (headBuku == NULL) {
        cout << "\nBelum ada data buku." << endl;
        return;
    }
    tampilkanBuku();
    int pilih;
    cout << "Pilih nomor buku yang ingin dihapus: ";
    cin >> pilih;

    Buku *target = cariBukuByNomor(pilih);
    if (target == NULL) {
        cout << "Nomor buku tidak ditemukan!" << endl;
        return;
    }

    // hapus node dari double linked list
    if (target == headBuku && target == tailBuku) {
        // cuma 1 node
        headBuku = NULL;
        tailBuku = NULL;
    } else if (target == headBuku) {
        headBuku = headBuku->next;
        headBuku->prev = NULL;
    } else if (target == tailBuku) {
        tailBuku = tailBuku->prev;
        tailBuku->next = NULL;
    } else {
        // node di tengah
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }

    cout << "Buku \"" << target->judul << "\" berhasil dihapus!" << endl;
    delete target;
}

// ============ FUNGSI MAHASISWA ============

// cari mahasiswa berdasarkan NIM, return pointer
Mahasiswa* cariMahasiswa(const char *nim) {
    Mahasiswa *curr = headMhs;
    while (curr != NULL) {
        if (strcmp(curr->nim, nim) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

// registrasi mahasiswa baru (NIM sudah dari login, tidak perlu input lagi)
Mahasiswa* registrasiMahasiswa(const char *nimInput) {
    Mahasiswa *baru = new Mahasiswa;
    cout << "\n=== Registrasi Mahasiswa Baru ===" << endl;
    cout << "NIM           : " << nimInput << " (otomatis)" << endl;
    strcpy(baru->nim, nimInput);

    cin.ignore();
    cout << "Nama          : ";
    cin.getline(baru->nama, 50);
    cout << "Buat Password : ";
    cin.getline(baru->password, 30);
    cout << "Program Studi : ";
    cin.getline(baru->prodi, 50);
    cout << "Tahun Angkatan: ";
    cin >> baru->angkatan;
    cin.ignore();
    cout << "Nomor HP      : ";
    cin.getline(baru->noHp, 15);

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

// login mahasiswa
Mahasiswa* loginMahasiswa() {
    char nim[20];
    cout << "\n=== Login Mahasiswa ===" << endl;
    cin.ignore();
    cout << "Masukkan NIM: ";
    cin.getline(nim, 20);

    Mahasiswa *mhs = cariMahasiswa(nim);
    if (mhs != NULL) {
        // NIM ditemukan, minta password
        char pw[30];
        cout << "Password    : ";
        cin.getline(pw, 30);

        if (strcmp(mhs->password, pw) == 0) {
            cout << "Login berhasil! Selamat datang, " << mhs->nama << "!" << endl;
            return mhs;
        } else {
            cout << "Password salah!" << endl;
            return NULL;
        }
    }

    // belum terdaftar
    cout << "NIM belum terdaftar." << endl;
    char jawab;
    cout << "Ingin registrasi sekarang? (y/n): ";
    cin >> jawab;
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

    // tampilkan buku yang stoknya > 0
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

    int pilih;
    cout << "\nPilih nomor buku yang ingin dipinjam: ";
    cin >> pilih;

    Buku *target = cariBukuByNomor(pilih);
    if (target == NULL) {
        cout << "Nomor buku tidak valid!" << endl;
        return;
    }
    if (target->stok <= 0) {
        cout << "Stok buku ini sedang habis!" << endl;
        return;
    }

    // cek apakah mahasiswa sudah pinjam buku ini
    Peminjaman *cek = mhs->daftarPinjaman;
    while (cek != NULL) {
        if (strcmp(cek->judulBuku, target->judul) == 0) {
            cout << "Kamu sudah meminjam buku ini!" << endl;
            return;
        }
        cek = cek->next;
    }

    // kurangi stok
    target->stok--;

    // simpan ke linked list peminjaman mahasiswa
    Peminjaman *baru = new Peminjaman;
    strcpy(baru->judulBuku, target->judul);
    strcpy(baru->authorBuku, target->author);
    baru->tahunTerbit = target->tahunTerbit;
    baru->next = NULL;

    // tambah di depan
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
    int pilih;
    cout << "Pilih nomor buku yang ingin dikembalikan: ";
    cin >> pilih;

    // cari node peminjaman ke-pilih
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

    // kembalikan stok buku
    Buku *bk = headBuku;
    while (bk != NULL) {
        if (strcmp(bk->judul, curr->judulBuku) == 0) {
            bk->stok++;
            break;
        }
        bk = bk->next;
    }

    // hapus node peminjaman
    if (prev == NULL) {
        // hapus node pertama
        mhs->daftarPinjaman = curr->next;
    } else {
        prev->next = curr->next;
    }

    cout << "Buku \"" << curr->judulBuku << "\" berhasil dikembalikan!" << endl;
    delete curr;
}

// ============ MENU ============

// login admin sederhana
bool loginAdmin() {
    char username[30];
    char password[30];
    int percobaan = 3;

    cout << "\n=== Login Admin ===" << endl;
    while (percobaan > 0) {
        cin.ignore();
        cout << "Username: ";
        cin.getline(username, 30);
        cout << "Password: ";
        cin.getline(password, 30);

        // cek username dan password
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
        cin >> pilih;

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
        cin >> pilih;

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
    // hapus semua buku
    Buku *bk = headBuku;
    while (bk != NULL) {
        Buku *temp = bk;
        bk = bk->next;
        delete temp;
    }

    // hapus semua mahasiswa beserta peminjamannya
    Mahasiswa *mhs = headMhs;
    while (mhs != NULL) {
        // hapus daftar peminjaman mahasiswa ini
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
        cin >> pilih;

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

    // bersihkan memori sebelum keluar
    bersihkanMemori();

    return 0;
}
