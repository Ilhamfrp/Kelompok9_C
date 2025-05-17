#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Kontak {
    string nama;
    string noHP;
    string email;
    Kontak* next;
};

// Baca dari file
void bacaFile(Kontak*& head) {
    ifstream file("kontak.txt");
    if (!file) {
        cout << "File kontak.txt belum ada.\n";
        return;
    }

    string nama, noHP, email;
    while (getline(file, nama) && getline(file, noHP) && getline(file, email)) {
        Kontak* baru = new Kontak;
        baru->nama = nama;
        baru->noHP = noHP;
        baru->email = email;
        baru->next = nullptr;

        if (head == nullptr) {
            head = baru;
        } else {
            Kontak* temp = head;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = baru;
        }
    }

    file.close();
}

// Simpan ke file
void simpanFile(Kontak* head) {
    ofstream file("kontak.txt");
    Kontak* temp = head;
    while (temp != nullptr) {
        file << temp->nama << "\n";
        file << temp->noHP << "\n";
        file << temp->email << "\n";
        temp = temp->next;
    }
    file.close();
}

// Tambah kontak baru
void tambahKontak(Kontak*& head) {
    Kontak* baru = new Kontak;
    cout << "Masukkan Nama  : ";
    getline(cin, baru->nama);
    cout << "Masukkan No HP : ";
    getline(cin, baru->noHP);
    cout << "Masukkan Email : ";
    getline(cin, baru->email);
    baru->next = nullptr;

    if (head == nullptr) {
        head = baru;
    } else {
        Kontak* temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = baru;
    }

    cout << "Kontak berhasil ditambahkan!\n";
}

// Tampilkan semua kontak
void tampilkanKontak(Kontak* head) {
    if (head == nullptr) {
        cout << "Daftar kontak kosong.\n";
        return;
    }

    Kontak* temp = head;
    int nomor = 1;
    while (temp != nullptr) {
        cout << nomor++ << ". Nama : " << temp->nama << "\n";
        cout << "   No HP: " << temp->noHP << "\n";
        cout << "   Email: " << temp->email << "\n\n";
        temp = temp->next;
    }
}

// Cari kontak berdasarkan nama
void cariKontak(Kontak* head, const string& keyword) {
    bool ditemukan = false;
    Kontak* temp = head;
    while (temp != nullptr) {
        if (temp->nama.find(keyword) != string::npos) {
            cout << "Nama  : " << temp->nama << "\n";
            cout << "No HP : " << temp->noHP << "\n";
            cout << "Email : " << temp->email << "\n\n";
            ditemukan = true;
        }
        temp = temp->next;
    }
    if (!ditemukan)
        cout << "Kontak dengan nama \"" << keyword << "\" tidak ditemukan.\n";
}

void urutkanKontak(Kontak* head) {
    if (head == nullptr) return;

    bool swapped;
    do {
        swapped = false;
        Kontak* temp = head;
        while (temp->next != nullptr) {
            if (temp->nama > temp->next->nama) {
                swap(temp->nama, temp->next->nama);
                swap(temp->noHP, temp->next->noHP);
                swap(temp->email, temp->next->email);
                swapped = true;
            }
            temp = temp->next;
        }
    } while (swapped);

    cout << "Kontak berhasil diurutkan berdasarkan nama.\n";
}

void hapusSemua(Kontak*& head) {
    while (head != nullptr) {
        Kontak* hapus = head;
        head = head->next;
        delete hapus;
    }
}

int main() {
    Kontak* head = nullptr;
    bacaDariFile(head);
    int pilihan;
    string dummy, cari;

    do {
        cout << "\n=== MENU DAFTAR KONTAK ===\n";
        cout << "1. Tambah Kontak\n";
        cout << "2. Tampilkan Semua Kontak\n";
        cout << "3. Cari Kontak\n";
        cout << "4. Urutkan Kontak\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tambahKontak(head);
                simpanKeFile(head);
                break;
            case 2:
                tampilkanKontak(head);
                break;
            case 3:
                cout << "Masukkan nama yang dicari: ";
                getline(cin, cari);
                cariKontak(head, cari);
                break;
            case 4:
                urutkanKontak(head);
                simpanKeFile(head);
                break;
            case 5:
                cout << "Keluar dari program " << endl;
                break;
            default:
                cout << "Pilihan tidak valid " << endl;
        }
    } while (pilihan != 0);

    hapusSemua(head);
    return 0;
}

