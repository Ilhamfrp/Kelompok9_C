#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
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

bool nomordiblokir( const string& noHP){
   ifstream file("blokir.txt");
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
    return false;
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
    if (nomordiblokir(baru->noHP)) {
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
    vector<Kontak*> daftarCocok;
    Kontak* temp = head;
    string lowerKeyword = toLower(keyword);

    cout << "Hasil pencarian untuk \"" << keyword << "\":\n";

    while (temp != nullptr) {
        if (toLower(temp->nama).find(lowerKeyword) != string::npos) {
            daftarCocok.push_back(temp);
            cout << daftarCocok.size() << ". Nama : " << temp->nama << endl;
            cout << "   No HP: " << temp->noHP << ", Email: " << temp->email << endl;
        }
        temp = temp->next;
    }

    if (daftarCocok.empty()) {
        cout << "Tidak ditemukan kontak dengan nama tersebut.\n";
        return;
    }

    int pilih = 1;
    if (daftarCocok.size() > 1) {
        cout << "Masukkan nomor urutan kontak yang ingin diubah (1-" << daftarCocok.size() << "): ";
        cin >> pilih;
        cin.ignore();
        if (pilih < 1 || pilih > daftarCocok.size()) {
            cout << "Pilihan tidak valid.\n";
            return;
        }
    }

    Kontak* target = daftarCocok[pilih - 1];
    cout << "Masukkan nama baru: ";
    getline(cin, target->nama);
    cout << "Kontak berhasil diubah.\n";
}

void hapusKontak(Kontak*& head, const string& keyword, bool blokir = false) {
    vector<Kontak*> daftarCocok;
    Kontak* temp = head;
    Kontak* prev = nullptr;
    string lowerKeyword = toLower(keyword);

    while (temp != nullptr) {
        if (toLower(temp->nama).find(lowerKeyword) != string::npos) {
            daftarCocok.push_back(temp);
        }
        temp = temp->next;
    }

    if (daftarCocok.empty()) {
        if (!blokir)
            cout << "Kontak dengan nama \"" << keyword << "\" tidak ditemukan.\n";
        return;
    }

    int pilih = 1;
    if (daftarCocok.size() > 1) {
        cout << "Ditemukan beberapa kontak:\n";
        for (size_t i = 0; i < daftarCocok.size(); ++i) {
            cout << i + 1 << ". Nama : " << daftarCocok[i]->nama << endl;
            cout << "   No HP: " << daftarCocok[i]->noHP << ", Email: " << daftarCocok[i]->email << endl;
        }

        cout << "Pilih nomor urutan yang ingin dihapus (1-" << daftarCocok.size() << "): ";
        cin >> pilih;
        cin.ignore();

        if (pilih < 1 || pilih > daftarCocok.size()) {
            cout << "Pilihan tidak valid.\n";
            return;
        }
    }

    Kontak* target = daftarCocok[pilih - 1];

    temp = head;
    prev = nullptr;

    while (temp != nullptr) {
        if (temp == target) {
            if (prev == nullptr) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            delete temp;
            if (!blokir)
                cout << "Kontak \"" << target->nama << "\" berhasil dihapus.\n";
            return;
        }
        prev = temp;
        temp = temp->next;
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
    string keyword;
    cout << "Masukkan nama kontak yang ingin diblokir: ";
    getline(cin, keyword);

    vector<Kontak*> daftarCocok;
    Kontak* temp = head;
    string lowerKeyword = toLower(keyword);

    while (temp != nullptr) {
        if (toLower(temp->nama).find(lowerKeyword) != string::npos) {
            daftarCocok.push_back(temp);
        }
        temp = temp->next;
    }

    if (daftarCocok.empty()) {
        cout << "Kontak dengan nama \"" << keyword << "\" tidak ditemukan.\n";
        return;
    }

    int pilih = 1;
    if (daftarCocok.size() > 1) {
        cout << "Hasil pencarian:\n";
        for (size_t i = 0; i < daftarCocok.size(); ++i) {
            cout << i + 1 << ". Nama : " << daftarCocok[i]->nama << endl;
            cout << "   No HP: " << daftarCocok[i]->noHP << ", Email: " << daftarCocok[i]->email << endl;
        }
        cout << "Masukkan nomor urutan kontak yang ingin diblokir (1-" << daftarCocok.size() << "): ";
        cin >> pilih;
        cin.ignore();
        if (pilih < 1 || pilih > daftarCocok.size()) {
            cout << "Pilihan tidak valid.\n";
            return;
        }
    }

    Kontak* target = daftarCocok[pilih - 1];

    ofstream blokir("blokir.txt", ios::app);
    if (blokir) {
        blokir << target->nama << "\n"
               << target->noHP << "\n"
               << target->email << "\n";
        blokir.close();

        hapusKontak(head, target->nama, true);
        simpanFile(head);

        cout << "Kontak \"" << target->nama << "\" berhasil diblokir dan dihapus dari daftar utama.\n";
    } else {
        cout << "Gagal membuka file blokir.txt.\n";
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

void unblokNomorByNama(Kontak*& head) {
    cout << "Masukkan nama kontak yang ingin di-unblokir: ";
    string keyword;
    getline(cin, keyword);
    string lowerKeyword = toLower(keyword);

    ifstream file("blokir.txt");
    if (!file) {
        cout << "Belum ada nomor yang diblokir.\n";
        return;
    }

    vector<Kontak*> blokirList;
    vector<Kontak*> hasilCocok;

    string nama, noHP, email;
    while (getline(file, nama) && getline(file, noHP) && getline(file, email)) {
        Kontak* k = new Kontak{nama, noHP, email, nullptr};
        blokirList.push_back(k);
        if (toLower(nama).find(lowerKeyword) != string::npos) {
            hasilCocok.push_back(k);
        }
    }
    file.close();

    if (hasilCocok.empty()) {
        cout << "Kontak \"" << keyword << "\" tidak ditemukan di daftar blokir.\n";
        return;
    }

    int pilih = 1;
    if (hasilCocok.size() > 1) {
        cout << "Ditemukan beberapa kontak dengan nama \"" << keyword << "\":\n";
        for (size_t i = 0; i < hasilCocok.size(); ++i) {
            cout << i + 1 << ". Nama : " << hasilCocok[i]->nama << "\n";
            cout << "   No HP: " << hasilCocok[i]->noHP << "\n";
            cout << "   Email: " << hasilCocok[i]->email << "\n\n";
        }
        cout << "Pilih nomor yang ingin di-unblokir (1-" << hasilCocok.size() << "): ";
        cin >> pilih;
        cin.ignore();
        if (pilih < 1 || pilih > hasilCocok.size()) {
            cout << "Pilihan tidak valid.\n";
            return;
        }
    } else {
        cout << "Kontak ditemukan dan akan di-unblokir:\n";
        cout << "Nama : " << hasilCocok[0]->nama << "\n";
        cout << "No HP: " << hasilCocok[0]->noHP << "\n";
        cout << "Email: " << hasilCocok[0]->email << "\n";
    }

    Kontak* target = hasilCocok[pilih - 1];

    Kontak* baru = new Kontak{target->nama, target->noHP, target->email, nullptr};
    if (head == nullptr)
        head = baru;
    else {
        Kontak* temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = baru;
    }

    ofstream out("blokir.txt");
    for (Kontak* k : blokirList) {
        if (k != target) {
            out << k->nama << "\n" << k->noHP << "\n" << k->email << "\n";
        }
        delete k;
    }
    out.close();

    simpanFile(head);
    cout << "Kontak \"" << target->nama << "\" berhasil di-unblok dan ditambahkan kembali ke daftar utama.\n";
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
        cout << "9. Unblokir Nomor" << endl;
        cout << "10. Keluar" << endl;
        cout << setfill('-') << setw(35) << " "                 << endl;
        cout << "Pilih: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                tambahKontak(head);
                simpanFile(head);
                cout << "tekan enter untuk melanjutkan...";
                cin.get();
                break;
            case 2:
                tampilkanKontak(head);
                cout << "tekan enter untuk melanjutkan...";
                cin.get();
                break;
            case 3:
                cout << "Masukkan nama yang dicari: ";
                getline(cin, cari);
                cariKontak(head, cari);
                cout << "tekan enter untuk melanjutkan...";
                cin.get();
                break;
            case 4:
                urutkanKontak(head);
                simpanFile(head);
                cout << "tekan enter untuk melanjutkan...";
                cin.get();
                break;
            case 5:
                cout << "Masukkan nama kontak yang ingin diubah: ";
                getline(cin, cari);
                editnamakontak(head, cari);
                simpanFile(head);
                cout << "tekan enter untuk melanjutkan...";
                cin.get();

                break;
              case 6:
               cout << "Masukkan nama kontak yang ingin dihapus: ";
                getline(cin, cari);
                hapusKontak(head, cari);
                simpanFile(head);
                cout << "tekan enter untuk melanjutkan...";
                cin.get();
                break;
            case 7:
                blokirNomor(head);  
                cout << "tekan enter untuk melanjutkan...";
                cin.get();
                break;
            case 8:
                 tampilkanBlokir();
                cout << "tekan enter untuk melanjutkan...";
                cin.get();
                break;
            case 9:
                unblokNomorByNama(head);
                simpanFile(head);
                cout << "tekan enter untuk melanjutkan...";
                cin.get();
                break;
            case 10:
                cout << "keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 10);

    hapusSemua(head);
    return 0;
}

