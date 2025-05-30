#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

struct Kontak {
    string nama;
    string noHP;
    string email;
    Kontak* next;
};

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

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

bool nomorsudahada(Kontak* head, const string& noHP) {
    while (head != nullptr) {
        if (head->noHP == noHP) {
            return true;
        }
        head = head->next;
    }
    return false;
}

bool nomordiblokir(Kontak* head, const string& noHP){
    ifstream file ("blokir.txt");
    if (!file) {
        return false;
    }
    string nama, nomor, email;
    while (getline(file, nama) && getline(file, nomor) && getline(file, email)) {
        if (nomor == noHP) {
            file.close();
            return true;
        }
    }
}
 
void tambahKontak(Kontak*& head) {
    Kontak* baru = new Kontak;
    cout << "Masukkan Nama  : ";
    getline(cin, baru->nama);
    cout << "Masukkan No HP : ";
    getline(cin, baru->noHP);
    cout << "Masukkan Email : ";
    getline(cin, baru->email);
    if (baru->nama.empty() || baru->noHP.empty() || baru->email.empty()) {
        cout << "Semua harus diisi.\n";
        delete baru;
        return;
    }
    if (nomorsudahada(head, baru->noHP)) {
        cout << "Nomor HP sudah ada dalam daftar kontak.\n";
        delete baru;
        return;
    }
    if (nomordiblokir(head, baru->noHP)) {
        cout << "Nomor HP ini sudah diblokir.\n";
        delete baru;
        return;
    }
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

void cariKontak(Kontak* head, const string& keyword) {
    string lowerKeyword = toLower(keyword);
        bool ditemukan = false;
        Kontak* temp = head;
        while (temp != nullptr) {
        if (toLower(temp->nama).find(lowerKeyword) != string::npos) {
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
void editnamakontak(Kontak*& head, const string& keyword) {
    string lowerKeyword = toLower(keyword);
    Kontak* temp = head;
    while (temp != nullptr)
    {
        if (toLower(temp->nama) == lowerKeyword) {
            cout << "masukkan nama baru : ";
            getline(cin, temp->nama);

            cout << "kontak \"" << keyword << "\" berhasil diubah menjadi \"" << temp->nama << "\".\n";
         return;     }
         temp = temp->next;
    }
    cout << "Kontak dengan nama \"" << keyword << "\" tidak ditemukan.\n";  
}

void hapusKontak(Kontak*& head, const string& keyword, bool blokir = false) {
    string lowerKeyword = toLower(keyword);
    if (head == nullptr) {
        if(!blokir)
        cout << "Daftar kontak kosong.\n";
        return;
    }

    Kontak* temp = head;
    Kontak* prev = nullptr;
    bool ditemukan = false;

    while (temp != nullptr) {
        if (toLower(temp->nama) == lowerKeyword) {
            ditemukan = true;
            if (prev == nullptr) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            delete temp;
            if(!blokir)
            cout << "Kontak \"" << keyword << "\" berhasil dihapus.\n";
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    if (!ditemukan && !blokir) {
        cout << "Kontak dengan nama \"" << keyword << "\" tidak ditemukan.\n";
    }
}

void urutkanKontak(Kontak* head) {
    if (head == nullptr) {
        cout << "daftar kontak kosong, tidak ada yang bisa diurutkan.\n";
        return;
    }
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

void blokirNomor(Kontak*& head) {
    string nama;
    cout << "Masukkan nama kontak yang ingin diblokir: ";
    getline(cin, nama);

    Kontak* temp = head;
    bool ditemukan = false;

    string simpannama, simpannomor, simpanhp;

    while (temp != nullptr) {
        
        if (toLower(temp->nama) == toLower(nama)) {
            simpannama = temp->nama;
            simpannomor = temp->noHP;
            simpanhp = temp->email;
            ditemukan = true;
            break;
        }
        temp = temp->next;
    }

    if (ditemukan) {
        string data;
        ifstream bacaLama("blokir.txt");
        string baris;
        while (getline(bacaLama, baris)) {
            data += baris + "\n";
        }
        bacaLama.close();

       
        data += simpannama + "\n" + simpannomor + "\n" + simpanhp + "\n";

        ofstream blokir("blokir.txt");
        blokir << data;
        blokir.close();

  
        hapusKontak(head, nama, true);
        simpanFile(head);

        cout << "Kontak \"" << nama << "\" berhasil diblokir dan dihapus dari daftar utama.\n";
    } else {
        cout << "Kontak \"" << nama << "\" tidak ditemukan di daftar.\n";
    }
}



void tampilkanBlokir() {
    ifstream file("blokir.txt");
    if (!file) {
        cout << "Belum ada nomor yang diblokir.\n";
        return;
    }

    string nama, noHP, email;
    int nomor = 1;
    while (getline(file, nama) && getline(file, noHP) && getline(file, email)) {
        cout << nomor++ << ". Nama : " << nama << endl;
        cout << "   No HP: " << noHP << endl;
        cout << "   Email: " << email << endl << endl;
    }

    file.close();
}


int main() {
    Kontak* head = nullptr;
    bacaFile(head);
    int pilihan;
    string dummy, cari;



    do {
        cout << setfill('=') << setw(35) << " "                 << endl;
        cout << "Selamat Datang di Kontak!" << endl;
        cout << setfill('=') << setw(35) << " "                 << endl;
        cout << "Silakan pilih layanan :"                           << endl;
        cout << "1. Tambah Kontak" << endl;
        cout << "2. Tampilkan Semua Kontak" << endl;
        cout << "3. Cari Kontak" << endl;
        cout << "4. Urutkan Kontak" << endl;
        cout << "5. edit Kontak" << endl;
        cout << "6. Hapus Kontak" << endl;
        cout << "7. Blokir Nomor" << endl;
        cout << "8. Lihat Kontak Terblokir" << endl;
        cout << "9. Keluar" << endl;
        cout << setfill('-') << setw(35) << " "                 << endl;
        cout << "Pilih: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                tambahKontak(head);
                simpanFile(head);
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
                simpanFile(head);
                break;
            case 5:
                cout << "Masukkan nama kontak yang ingin diubah: ";
                getline(cin, cari);

                editnamakontak(head, cari);
                simpanFile(head);

                break;
              case 6:
               cout << "Masukkan nama kontak yang ingin dihapus: ";
                getline(cin, cari);
                hapusKontak(head, cari);
                simpanFile(head);
                break;
            case 7:
                blokirNomor(head);  
                break;
            case 8:
                 tampilkanBlokir();
                break;
            case 9:
            cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 9);

    hapusSemua(head);
    return 0;
}

