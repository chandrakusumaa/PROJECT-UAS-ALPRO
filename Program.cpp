#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Pesanan {
    string namaPelanggan;
    string kodeMenu;
    string namaMenu;
    int harga;
    Pesanan* next;
};

struct Queue {
    Pesanan* front;
    Pesanan* rear;
};

void createQueue(Queue &Q) { Q.front = nullptr; Q.rear = nullptr; }
bool isEmpty(Queue Q) { return (Q.front == nullptr); }

void enqueue(Queue &Q, string nama, string kode, string menu, int harga) {
    Pesanan* baru = new Pesanan{nama, kode, menu, harga, nullptr};
    if (isEmpty(Q)) { Q.front = baru; Q.rear = baru; }
    else { Q.rear->next = baru; Q.rear = baru; }
}

void dequeue(Queue &Q) {
    if (!isEmpty(Q)) {
        Pesanan* hapus = Q.front;
        cout << "Pesanan " << hapus->namaPelanggan 
             << " (" << hapus->namaMenu << ") selesai.\n";
        Q.front = Q.front->next;
        delete hapus;
        if (Q.front == nullptr) Q.rear = nullptr;
    } else cout << "Queue kosong.\n";
}

void printQueue(Queue Q) {
    if (isEmpty(Q)) cout << "Queue kosong.\n";
    else {
        Pesanan* bantu = Q.front;
        cout << "=== Daftar Pesanan ===\n";
        while (bantu) {
            cout << bantu->namaPelanggan << " - " 
                 << bantu->namaMenu << " (Rp" << bantu->harga << ")\n";
            bantu = bantu->next;
        }
    }
}

// Tambah menu ke file
void tambahMenu(string file, int kodeKantin) {
    ofstream fout(file, ios::app);
    string kodeUnik, nama;
    int harga;
    cout << "Masukkan kode unik menu: "; cin >> kodeUnik;
    cout << "Masukkan nama menu: "; cin.ignore(); getline(cin, nama);
    cout << "Masukkan harga: "; cin >> harga;
    fout << kodeKantin << "," << kodeUnik << "," << nama << "," << harga << endl;
    fout.close();
    cout << "Menu berhasil ditambahkan.\n";
}

// Tampilkan menu sesuai kode kantin
void tampilMenu(string file, int kodeKantin) {
    ifstream fin(file);
    string kodeK, kodeU, nama;
    int harga;
    cout << "=== Menu Kantin ===\n";
    while (getline(fin, kodeK, ',')) {
        getline(fin, kodeU, ',');
        getline(fin, nama, ',');
        fin >> harga;
        fin.ignore();
        if (stoi(kodeK) == kodeKantin) {
            cout << kodeU << " - " << nama << " (Rp" << harga << ")\n";
        }
    }
    fin.close();
}

// Cari menu berdasarkan kode unik + kode kantin
bool cariMenu(string file, int kodeKantin, string kodeCari, string &nama, int &harga) {
    ifstream fin(file);
    string kodeK, kodeU;
    while (getline(fin, kodeK, ',')) {
        getline(fin, kodeU, ',');
        getline(fin, nama, ',');
        fin >> harga;
        fin.ignore();
        if (stoi(kodeK) == kodeKantin && kodeU == kodeCari) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

// Menu ibu kantin
void menuIbuKantin(string file, int kodeKantin, Queue &Q) {
    int pilihan;
    do {
        cout << "\n--- Menu Ibu Kantin ---\n";
        cout << "1. Tambah menu\n";
        cout << "2. Lihat menu kantin\n";
        cout << "3. Lihat pesanan\n";
        cout << "4. Hapus pesanan\n";
        cout << "0. Kembali\n";
        cout << "Pilihan: "; cin >> pilihan;

        switch(pilihan) {
            case 1: tambahMenu(file, kodeKantin); break;
            case 2: tampilMenu(file, kodeKantin); break;
            case 3: printQueue(Q); break;
            case 4: dequeue(Q); break;
        }
    } while(pilihan != 0);
}

// Menu pelanggan
void menuPelanggan(string file, Queue &kantinMakananBerat, Queue &kantinMinuman, Queue &kantinRingan) {
    int pilihan;
    do {
        cout << "\n--- Menu Pelanggan ---\n";
        cout << "1. Lihat menu kantin\n";
        cout << "2. Pesan menu\n";
        cout << "0. Kembali\n";
        cout << "Pilihan: "; cin >> pilihan;

        switch(pilihan) {
            case 1: {
                int pilihKantin;
                cout << "Pilih kantin:\n1. Makanan Berat\n2. Minuman\n3. Makanan Ringan\nPilihan: ";
                cin >> pilihKantin;
                tampilMenu(file, pilihKantin);
                break;
            }
            case 2: {
                int pilihKantin;
                cout << "Pilih kantin untuk pesan:\n1. Makanan Berat\n2. Minuman\n3. Makanan Ringan\nPilihan: ";
                cin >> pilihKantin;

                string nama, kode, menu;
                int harga;
                cout << "Masukkan nama pelanggan: ";
                cin.ignore(); getline(cin, nama);
                cout << "Masukkan kode menu: "; cin >> kode;

                Queue* targetQueue;
                if (pilihKantin == 1) targetQueue = &kantinMakananBerat;
                else if (pilihKantin == 2) targetQueue = &kantinMinuman;
                else targetQueue = &kantinRingan;

                if (cariMenu(file, pilihKantin, kode, menu, harga)) {
                    enqueue(*targetQueue, nama, kode, menu, harga);
                    cout << "Pesanan berhasil ditambahkan.\n";
                } else cout << "Kode menu tidak ditemukan.\n";
                break;
            }
        }
    } while(pilihan != 0);
}

int main() {
    Queue kantinMakananBerat, kantinMinuman, kantinRingan;
    createQueue(kantinMakananBerat);
    createQueue(kantinMinuman);
    createQueue(kantinRingan);

    string file = "menu.txt"; // semua menu disimpan di satu file
    int role;
    do {
        cout << "\n=== SISTEM KANTIN ===\n";
        cout << "1. Masuk sebagai Ibu Kantin\n";
        cout << "2. Masuk sebagai Pelanggan\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: "; cin >> role;

        if (role == 1) {
            int kantin;
            cout << "Pilih kantin:\n";
            cout << "1. Makanan Berat\n";
            cout << "2. Minuman\n";
            cout << "3. Makanan Ringan\n";
            cout << "Pilihan: "; cin >> kantin;

            Queue* targetQueue;
            int kodeKantin;
            if (kantin == 1) { targetQueue = &kantinMakananBerat; kodeKantin = 1; }
            else if (kantin == 2) { targetQueue = &kantinMinuman; kodeKantin = 2; }
            else { targetQueue = &kantinRingan; kodeKantin = 3; }

            menuIbuKantin(file, kodeKantin, *targetQueue);
        }
        else if (role == 2) {
            menuPelanggan(file, kantinMakananBerat, kantinMinuman, kantinRingan);
        }
    } while(role != 0);

    cout << "Program selesai.\n";
    return 0;
}
