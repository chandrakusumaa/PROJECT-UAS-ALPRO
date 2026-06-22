#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

struct Pesanan { // menyimpan data pesanan
    string namaPelanggan;
    string kodeMenu;
    string namaMenu;
    int harga;
    Pesanan* next;
};

struct Menu { // menyimpan data menu
    int kodeKantin;
    string kodeMenu;
    string namaMenu;
    int harga;
};

struct Queue { // menyatakan struktur data dalam queue
    Pesanan* front;
    Pesanan* rear;
};

namespace antrian{ //namespace untuk bagian pengelolaan data

void createQueue(Queue &Q) { Q.front = nullptr; Q.rear = nullptr; } //membuat queue baru 

inline bool isEmpty(const Queue &Q) { //validasi data queue
    return Q.front == nullptr;
}

void enqueue(Queue &Q, string nama, string kode, string menu, int harga) { // menambahkan data kedalam antrian
    Pesanan* baru = new Pesanan{nama, kode, menu, harga, nullptr};
    if (isEmpty(Q)) { Q.front = baru; Q.rear = baru; } // jika queue kosong
    else { Q.rear->next = baru; Q.rear = baru; } // jika queue tidak kosong
}

void dequeue(Queue &Q) { //menghapus data dari queue
    if (!isEmpty(Q)) {
        Pesanan* hapus = Q.front;
        cout << "Pesanan " << hapus->namaPelanggan << " (" << hapus->namaMenu << ") selesai.\n";
        Q.front = Q.front->next;
        delete hapus;
        if (Q.front == nullptr) Q.rear = nullptr;
    } else cout << "Queue kosong.\n"; 
}

void tampilNormal(Pesanan* p) { //menampilkan data pesanan
    cout << p->namaPelanggan << " - " << p->namaMenu << " (Rp" << p->harga << ")\n";
}

void printQueue(Queue Q, void (*callback)(Pesanan*)) { //traversal queue dan callback function (tampilNormal)
    if (isEmpty(Q)) {
        cout << "Queue kosong.\n";
        return;
    }

    Pesanan* bantu = Q.front;

    while (bantu) { 
        callback(bantu);
        bantu = bantu->next;
    }
}
}

namespace fiturkantin{

vector<Menu> bacaSemuaMenu(string file) { //membaca dan menyimpan data dalam vector
    vector<Menu> daftarMenu;
    ifstream fin(file);
    string kodeK, kodeU, nama;
    int harga;

    while (getline(fin, kodeK, ',')) {

        getline(fin, kodeU, ',');
        getline(fin, nama, ',');

        fin >> harga;
        fin.ignore();

        Menu m;
        m.kodeKantin = stoi(kodeK);
        m.kodeMenu = kodeU;
        m.namaMenu = nama;
        m.harga = harga;
        daftarMenu.push_back(m);
    }

    fin.close();
    return daftarMenu;
}

// Tambah menu ke file .txt
void tambahMenu(string file, int kodeKantin) {
    ofstream fout(file, ios::app); //file handling
    string kodeUnik, nama;
    int harga;
    cout << "Masukkan kode unik menu: "; cin >> kodeUnik;
    cout << "Masukkan nama menu: "; cin.ignore(); getline(cin, nama);
    cout << "Masukkan harga: ";
    cin >> harga;
    try { //Exception Handling untuk validasi input harga
    if (harga <= 0)
        throw harga;
    }
    catch (int x) {
        cout << "Harga tidak boleh <= 0\n";
        return;
    }
    fout << kodeKantin << "," << kodeUnik << "," << nama << "," << harga << endl;
    fout.close();
    cout << "Menu berhasil ditambahkan.\n";
}

// menampilkan menu berdasarkan kode kantin
void tampilMenu(string file, int kodeKantin) {
    vector<Menu> daftar = bacaSemuaMenu(file);
    for(auto it = daftar.begin(); it != daftar.end(); ++it){
        if(it->kodeKantin == kodeKantin){
            cout<< it->kodeMenu<< " - " << it->namaMenu << " (Rp" << it->harga << ")\n";
        }
    }
}

void tampilMenu(string file){ //menampilkan seluruh menu
    vector<Menu> daftar = bacaSemuaMenu(file);
    cout << "\n=== SEMUA MENU ===\n";
    for(auto it = daftar.begin(); it != daftar.end(); ++it){
        cout << "[" << it->kodeKantin << "] " << it->kodeMenu << " - " << it->namaMenu << " (Rp" << it->harga << ")\n";
    }
}



void urutHarga(string file) { //mengurutkan data menu dari harga terkecil
    vector<Menu> data = bacaSemuaMenu(file);
    sort(data.begin(), data.end(), [](Menu a, Menu b){ //menggunakan STL sort
        return a.harga < b.harga;
    });

    cout << "\n=== MENU TERMURAH ===\n";
    for(auto m : data) {
        cout << m.namaMenu << " - Rp" << m.harga << endl;
    }
}

void hitungMenuKantin(string file, int kodeKantin){ //menghitung jumlah menu
    vector<Menu> data = bacaSemuaMenu(file);
    
    int jumlah = count_if(data.begin(), data.end(), [kodeKantin](Menu m){ //menggunakan STL count
        return m.kodeKantin == kodeKantin;
        }
    );

    cout << "\nJumlah menu : " << jumlah << endl;
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
        if (stoi(kodeK) == kodeKantin && kodeU == kodeCari) { //mengubah string kodeK menjadi int
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
        cout << "1. Tambah menu\n2. Lihat menu kantin\n3. Lihat pesanan\n4. Hapus pesanan\n5. Hitung jumlah menu\n0. Kembali\n";
        cout << "Pilihan: "; cin >> pilihan;

        switch(pilihan) {
            case 1: 
                fiturkantin::tambahMenu(file, kodeKantin); 
                break;
            case 2: 
                fiturkantin::tampilMenu(file, kodeKantin); 
                break;
            case 3: 
                antrian::printQueue(Q, antrian::tampilNormal); //callbackl function
                break;
            case 4: 
                antrian::dequeue(Q); 
                break;
            case 5: 
                fiturkantin::hitungMenuKantin(file, kodeKantin);
                break;
        }
    } while(pilihan != 0);
}

// Menu pelanggan
void menuPelanggan(string file, Queue &kantinMakananBerat, Queue &kantinMinuman, Queue &kantinRingan) {
    int pilihan;
    do {
        cout << "\n--- Menu Pelanggan ---\n";
        cout << "1. Lihat menu kantin\n2. Pesan menu\n3. Semua menu\n4. Urutkan menu termurah\n0. Kembali\n";
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

                Queue* targetQueue; //menentukan antrian pesanan
                if (pilihKantin == 1) targetQueue = &kantinMakananBerat;
                else if (pilihKantin == 2) targetQueue = &kantinMinuman;
                else targetQueue = &kantinRingan;

                if (cariMenu(file, pilihKantin, kode, menu, harga)) { //validasi kode pesanan
                    antrian::enqueue(*targetQueue, nama, kode, menu, harga);
                    cout << "Pesanan berhasil ditambahkan.\n";
                } else cout << "Kode menu tidak ditemukan.\n";
                break;
            }
            case 3: 
                tampilMenu(file);
                break;
            
            case 4:
                urutHarga(file);;
                break;
        }
    } while(pilihan != 0);
}

}

int main() {
    Queue kantinMakananBerat, kantinMinuman, kantinRingan; //deklarasi queue
    antrian::createQueue(kantinMakananBerat);
    antrian::createQueue(kantinMinuman);
    antrian::createQueue(kantinRingan);

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
            do {
                cout << "\n=== PILIH KANTIN ===\n";
                cout << "1. Makanan Berat\n";
                cout << "2. Minuman\n";
                cout << "3. Makanan Ringan\n";
                cout << "0. Kembali\n";
                cout << "Pilihan: ";
                cin >> kantin;

                Queue* targetQueue = nullptr;
                int kodeKantin;

                if (kantin == 1) {
                    targetQueue = &kantinMakananBerat;
                    kodeKantin = 1;
                }
                else if (kantin == 2) {
                    targetQueue = &kantinMinuman;
                    kodeKantin = 2;
                }
                else if (kantin == 3) {
                    targetQueue = &kantinRingan;
                    kodeKantin = 3;
                }

                if (kantin >= 1 && kantin <= 3) { //validasi input hanya 1-3
                    fiturkantin::menuIbuKantin(file, kodeKantin, *targetQueue);
                }
                else if (kantin != 0) {
                    cout << "Pilihan tidak valid!\n";
                }

            } while (kantin != 0);
        
        }else if (role == 2) {
            fiturkantin::menuPelanggan(file, kantinMakananBerat, kantinMinuman, kantinRingan);
        }
    } while(role != 0);

    cout << "Program selesai.\n";
    return 0;
}
