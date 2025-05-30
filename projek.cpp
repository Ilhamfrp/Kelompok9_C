#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

struct Kontak {
    string nama;
    string noHP;
    string email;
    Kontak* next;
};

string toLower(const string& str) {
    string res = str;
    transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

bool nomorSudahAda(Kontak* head, const string& noHP) {
    while (head) {
        if (head->noHP == noHP) return true;
        head = head->next;
    }
    return false;
}

bool nomorDiblokir(const string& noHP) {
    ifstream file("blokir.txt");
    string nama, nomor, email;
    while (getline(file, nama) && getline(file, nomor) && getline(file, email)) {
        if (nomor == noHP) return true;
    }
    return false;
}

void tambahKontak(Kontak*& head) {
    Kontak* baru = new Kontak;
    cout << setfill('-') << setw(46) << " " << endl;
    cout << "Masukkan Nama  : ";
    getline(cin, baru->nama);
    cout << "Masukkan No HP : ";
    getline(cin, baru->noHP);
    cout << "Masukkan Email : ";
    getline(cin, baru->email);
    cout << setfill('-') << setw(46) << " " << endl;

    if (baru->nama.empty() || baru->noHP.empty() || baru->email.empty()) {
        cout << "Semua data harus diisi" << endl;
        delete baru;
        return;
    }
    if (nomorSudahAda(head, baru->noHP)) {
        cout << "Nomor HP sudah ada dalam daftar" << endl;
        delete baru;
        return;
    }
    if (nomorDiblokir(baru->noHP)) {
        cout << "Nomor ini diblokir" << endl;
        delete baru;
        return;
    }

    baru->next = nullptr;
    if (!head) {
        head = baru;
    } else {
        Kontak* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = baru;
    }
    cout << "Kontak berhasil ditambahkan" << endl;
}

void tampilkanKontak(Kontak* head) {
    if (!head) {
        cout << "Daftar kontak kosong" << endl;
        return;
    }
    int no = 1;
    while (head) {
        cout << setfill('-') << setw(46) << " " << endl;
        cout << no++ << ". Nama  : " << head->nama << endl;
        cout << "   No HP : " << head->noHP << endl;
        cout << "   Email : " << head->email << endl;
        head = head->next;
    
    }
    cout << setfill('-') << setw(46) << " " << endl;
}

void cariKontak(Kontak* head, const string& keyword) {
    string key = toLower(keyword);
    bool ditemukan = false;
    while (head) {
        if (toLower(head->nama).find(key) != string::npos) {
            cout << setfill('-') << setw(46) << " " << endl;
            cout << "Nama  : " << head->nama << endl;
            cout << "No HP : " << head->noHP << endl;
            cout << "Email : " << head->email << endl;
            ditemukan = true;
            cout << setfill('-') << setw(46) << " " << endl;
        }
        head = head->next;
    }
    if (!ditemukan) {
        cout << "Kontak tidak ditemukan" << endl;
    }
}

void editNamaKontak(Kontak* head, const string& keyword) {
    string key = toLower(keyword);
    bool ditemukan = false;
    while (head) {
        if (toLower(head->nama).find(key) != string::npos) {
            cout << setfill('-') << setw(46) << " " << endl;
            cout << "Kontak ditemukan" << endl;
            cout << setfill('.') << setw(46) << " " << endl;
            cout << "Nama  : " << head->nama << endl;
            cout << "No HP : " << head->noHP << endl;
            cout << "Email : " << head->email << endl << endl;
            cout << setfill('.') << setw(46) << " " << endl;
            
            cout << "Masukkan nama baru: ";
            getline(cin, head->nama);
            cout << setfill('-') << setw(46) << " " << endl;
            cout << "Nama berhasil diperbarui." << endl;
            cout << setfill('-') << setw(46) << " " << endl;
            ditemukan = true;
            break;
        }
        head = head->next;
    }
    if (!ditemukan) {
        cout << "Kontak tidak ditemukan" << endl;
    }
}


void hapusKontak(Kontak*& head, const string& keyword) {
    Kontak *temp = head, *prev = nullptr;
    while (temp) {
        if (toLower(temp->nama).find(toLower(keyword)) != string::npos) {
            if (!prev) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            delete temp;
            cout << setfill('-') << setw(46) << " " << endl;
            cout << "Kontak berhasil dihapus" << endl;
            cout << setfill('-') << setw(46) << " " << endl;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << "Kontak tidak ditemukan" << endl;
}

void blokirKontak(Kontak*& head, const string& keyword) {
    Kontak *temp = head, *prev = nullptr;
    while (temp) {
        if (toLower(temp->nama).find(toLower(keyword)) != string::npos) {
            cout << setfill('-') << setw(46) << " " << endl;
            cout << "Kontak yang akan diblokir" << endl;
            cout << setfill('.') << setw(46) << " " << endl;
            cout << "Nama  : " << temp->nama << endl;
            cout << "No HP : " << temp->noHP << endl;
            cout << "Email : " << temp->email << endl;
            cout << setfill('.') << setw(46) << " " << endl;

            ofstream file("blokir.txt", ios::app);
            file << temp->nama << endl << temp->noHP << endl << temp->email << endl;
            file.close();

            if (!prev) head = temp->next;
            else prev->next = temp->next;
            delete temp;

            cout << "Kontak berhasil diblokir" << endl;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << "Kontak tidak ditemukan" << endl;
}

void unblockKontak(Kontak*& head, const string& noHP) {
    ifstream file("blokir.txt");
    ofstream tempFile("temp.txt");
    string nama, nomor, email;
    bool ditemukan = false;

    while (getline(file, nama) && getline(file, nomor) && getline(file, email)) {
        if (nomor == noHP) {
            Kontak* baru = new Kontak{nama, nomor, email, nullptr};
            if (!head) head = baru;
            else {
                Kontak* p = head;
                while (p->next) p = p->next;
                p->next = baru;
            }
            cout << setfill('-') << setw(46) << " " << endl;
            cout << "Kontak berhasil di-unblock" << endl;
            cout << setfill('.') << setw(46) << " " << endl;
            cout << "Nama  : " << nama << endl;
            cout << "No HP : " << nomor << endl;
            cout << "Email : " << email << endl;
            cout << setfill('.') << setw(46) << " " << endl;
            ditemukan = true;
        } else {
            tempFile << nama << endl << nomor << endl << email << endl;
        }
    }
    file.close();
    tempFile.close();
    remove("blokir.txt");
    rename("temp.txt", "blokir.txt");

    if (!ditemukan)
        cout << "Nomor tidak ditemukan dalam daftar blokir" << endl;
}


void sortirKontak(Kontak*& head) {
    if (!head || !head->next) return;
    vector<Kontak*> v;
    Kontak* temp = head;
    while (temp) {
        v.push_back(temp);
        temp = temp->next;
    }
    sort(v.begin(), v.end(), [](Kontak* a, Kontak* b) {
        return toLower(a->nama) < toLower(b->nama);
    });
    for (size_t i = 0; i < v.size() - 1; ++i)
        v[i]->next = v[i + 1];
    v.back()->next = nullptr;
    head = v[0];
    cout << "Kontak berhasil diurutkan berdasarkan nama" << endl;
}

void simpanFile(Kontak* head) {
    ofstream file("kontak.txt");
    while (head) {
        file << head->nama << endl
             << head->noHP << endl
             << head->email << endl;
        head = head->next;
    }
    file.close();
    cout << "Data berhasil disimpan ke file" << endl;
}

void bacaFile(Kontak*& head) {
    ifstream file("kontak.txt");
    string nama, noHP, email;
    while (getline(file, nama) && getline(file, noHP) && getline(file, email)) {
        Kontak* baru = new Kontak{nama, noHP, email, nullptr};
        if (!head) head = baru;
        else {
            Kontak* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }
    file.close();
}

void tampilkanBlokir() {
    ifstream file("blokir.txt");
    if (!file) {
        cout << "Belum ada nomor yang diblokir" << endl;
        return;
    }

    string nama, noHP, email;
    int nomor = 1;
    while (getline(file, nama) && getline(file, noHP) && getline(file, email)) {
        cout << setfill('-') << setw(46) << " " << endl;
        cout << nomor++ << ". Nama  : " << nama << endl;
        cout << "   No HP : " << noHP << endl;
        cout << "   Email : " << email << endl;
    }
    cout << setfill('-') << setw(46) << " " << endl;

    file.close();
}


int main() {
    Kontak* head = nullptr;
    bacaFile(head);

    int pilih;
    string input;

    do {
        cout << "\n===== MENU KONTAK =====" << endl;
        cout << "1. Tambah Kontak" << endl;
        cout << "2. Tampilkan Kontak" << endl;
        cout << "3. Cari Kontak" << endl;
        cout << "4. Edit Nama Kontak" << endl;
        cout << "5. Sortir Kontak" << endl;
        cout << "6. Hapus Kontak" << endl;
        cout << "7. Blokir Kontak" << endl;
        cout << "8. Tampilkan Daftar Blokir" << endl;
        cout << "9. Unblock Kontak" << endl;
        cout << "10. Simpan & Keluar" << endl;
        cout << "Pilih: ";
        cin >> pilih;
        cin.ignore();

        switch (pilih) {
            case 1: tambahKontak(head); break;

            case 2: tampilkanKontak(head); break;

            case 3:
            cout << setfill('-') << setw(46) << " " << endl;
                cout << "Masukkan nama yang dicari: ";
                getline(cin, input);
                cariKontak(head, input);
                cout << setfill('-') << setw(46) << " " << endl;
                break;
            case 4:
            cout << setfill('-') << setw(46) << " " << endl;
                cout << "Masukkan nama yang ingin diedit: ";
                getline(cin, input);
                editNamaKontak(head, input);
                cout << setfill('-') << setw(46) << " " << endl;
                break;
            case 5:
                sortirKontak(head);
                break;
            case 6:
            cout << setfill('-') << setw(46) << " " << endl;
                cout << "Masukkan nama yang ingin dihapus: ";
                getline(cin, input);
                hapusKontak(head, input);
                cout << setfill('-') << setw(46) << " " << endl;
                break;
            case 7:
            cout << setfill('-') << setw(46) << " " << endl;
                cout << "Masukkan nama yang ingin diblokir: ";
                getline(cin, input);
                blokirKontak(head, input);
                cout << setfill('-') << setw(46) << " " << endl;
                break;
            case 8:
                tampilkanBlokir();
                cin.get();
                break;               
            case 9:
            cout << setfill('-') << setw(46) << " " << endl;
                cout << "Masukkan No HP yang ingin di-unblock: ";
                getline(cin, input);
                unblockKontak(head, input);
                break;
            case 10:
                simpanFile(head);
                cout << "Terima kasih!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
        }

    } while (pilih != 10);

    return 0;
}
