#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
using namespace std;

struct MenuItem {
    int id;
    string nama;
    double harga;
};

struct OrderNode {
    string namaPemesan;
    string nomorKamar;
    string namaItem;
    double harga;
    OrderNode* next;
    OrderNode* prev;

    OrderNode(string cname, string rnum, string iname, double p)
        : namaPemesan(cname), nomorKamar(rnum), namaItem(iname), harga(p),
          next(nullptr), prev(nullptr) {}
};

class OrderList {
public:
    OrderNode* head;
    OrderNode* tail;

    OrderList() : head(nullptr), tail(nullptr) {}

    void tambahDiAkhir(string namaPemesan, string nomorKamar, string namaItem, double harga, bool dariFile = false) {
        OrderNode* simpulBaru = new OrderNode(namaPemesan, nomorKamar, namaItem, harga);
        if (!head) {
            head = tail = simpulBaru;
        } else {
            tail->next = simpulBaru;
            simpulBaru->prev = tail;
            tail = simpulBaru;
        }

        if (!dariFile) {
            ofstream fout("orders.txt", ios::app);
            fout << namaPemesan << "," << nomorKamar << "," << namaItem << "," << harga << "\n";
            fout.close();
        }
    }

    void simpanKeFile(const string& namaFile) {
        ofstream fout(namaFile);
        OrderNode* temp = head;
        while (temp) {
            fout << temp->namaPemesan << "," << temp->nomorKamar << ","
                 << temp->namaItem << "," << temp->harga << "\n";
            temp = temp->next;
        }
        fout.close();
    }

    void hapusPesanan(string namaPemesan) {
        OrderNode* temp = head;
        bool ditemukan = false;
        double total = 0;

        while (temp) {
            if (temp->namaPemesan == namaPemesan) {
                if (!ditemukan) {
                    cout << "\nData pesanan atas nama " << namaPemesan << ":\n";
                    ditemukan = true;
                }
                cout << "  - Kamar: " << temp->nomorKamar
                     << ", Item: " << temp->namaItem
                     << ", Harga: Rp " << temp->harga << endl;
                total += temp->harga;
            }
            temp = temp->next;
        }

        if (!ditemukan) {
            cout << "Pesanan tidak ditemukan.\n";
            return;
        }

        cout << ">> Total Pesanan: Rp " << total << endl;

        char konfirmasi;
        cout << "Yakin ingin menghapus semua pesanan atas nama ini? (y/n): ";
        cin >> konfirmasi;
        cin.ignore();

        if (konfirmasi != 'y' && konfirmasi != 'Y') {
            cout << "Penghapusan dibatalkan.\n";
            return;
        }

        temp = head;
        while (temp) {
            if (temp->namaPemesan == namaPemesan) {
                OrderNode* untukHapus = temp;
                if (temp->prev) temp->prev->next = temp->next;
                else head = temp->next;
                if (temp->next) temp->next->prev = temp->prev;
                else tail = temp->prev;
                temp = temp->next;
                delete untukHapus;
            } else {
                temp = temp->next;
            }
        }

        simpanKeFile("orders.txt");
        cout << "Pesanan atas nama " << namaPemesan << " berhasil dihapus.\n";
    }

    void tampilkan() {
        if (!head) {
            cout << "\nBelum ada pesanan.\n";
            return;
        }

        cout << "\n=== Daftar Pesanan ===\n";

        vector<pair<string, string>> daftarPemesan;
        daftarPemesanUnik(daftarPemesan);

        for (const auto& pemesan : daftarPemesan) {
            string nama = pemesan.first;
            string kamar = pemesan.second;
            cout << "\nPemesan: " << nama << " (Kamar: " << kamar << ")\n";

            OrderNode* temp = head;
            double total = 0;
            while (temp) {
                if (temp->namaPemesan == nama && temp->nomorKamar == kamar) {
                    cout << "  - Item: " << temp->namaItem << ", Harga: Rp " << temp->harga << endl;
                    total += temp->harga;
                }
                temp = temp->next;
            }
            cout << "  >> Total Harga: Rp " << total << endl;
        }
    }

    void cariPesanan(string namaPemesan) {
        OrderNode* temp = head;
        bool ditemukan = false;
        double total = 0;
        while (temp) {
            if (temp->namaPemesan == namaPemesan) {
                if (!ditemukan) {
                    cout << "\nPesanan ditemukan:\n";
                    ditemukan = true;
                }
                cout << "Pemesan: " << temp->namaPemesan
                     << ", Kamar: " << temp->nomorKamar
                     << ", Item: " << temp->namaItem
                     << ", Harga: Rp " << temp->harga << endl;
                total += temp->harga;
            }
            temp = temp->next;
        }
        if (!ditemukan) {
            cout << "Pesanan atas nama " << namaPemesan << " tidak ditemukan.\n";
        } else {
            cout << "Total Harga: Rp " << total << endl;
        }
    }

    void daftarPemesanUnik(vector<pair<string, string>>& daftar) {
        set<string> sudahAda;
        OrderNode* temp = head;
        while (temp) {
            string gabung = temp->namaPemesan + "|" + temp->nomorKamar;
            if (sudahAda.find(gabung) == sudahAda.end()) {
                daftar.push_back({temp->namaPemesan, temp->nomorKamar});
                sudahAda.insert(gabung);
            }
            temp = temp->next;
        }
    }

    void muatDariFile(const string& namaFile) {
        ifstream fin(namaFile);
        string baris;
        while (getline(fin, baris)) {
            size_t pos1 = baris.find(',');
            size_t pos2 = baris.find(',', pos1 + 1);
            size_t pos3 = baris.find(',', pos2 + 1);
            if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) continue;
            string namaPemesan = baris.substr(0, pos1);
            string nomorKamar = baris.substr(pos1 + 1, pos2 - pos1 - 1);
            string namaItem = baris.substr(pos2 + 1, pos3 - pos2 - 1);
            double harga = stod(baris.substr(pos3 + 1));
            tambahDiAkhir(namaPemesan, nomorKamar, namaItem, harga, true);
        }
        fin.close();
    }
};

void tampilkanMenu(const vector<MenuItem>& menu, const string& judul) {
    cout << "\n-- " << judul << " --\n";
    for (const auto& item : menu) {
        cout << item.id << ". " << item.nama << " - Rp " << item.harga << endl;
    }
}

int main() {
    OrderList daftarPesanan;
    const string namaFile = "orders.txt";

    vector<MenuItem> menuMakanan = {
        {1, "Nasi Goreng", 15000},
        {2, "Mie Ayam", 12000},
        {3, "Sate Ayam", 20000},
        {4, "Bakso", 18000}
    };
    vector<MenuItem> menuMinuman = {
        {1, "Es Teh", 5000},
        {2, "Jus Jeruk", 10000},
        {3, "Air Mineral", 3000},
        {4, "Kopi", 7000}
    };

    cout << "=== Selamat Datang di Aplikasi Pemesanan Hotel Semtoro ===\n";

    tampilkanMenu(menuMakanan, "Menu Makanan");
    tampilkanMenu(menuMinuman, "Menu Minuman");

    daftarPesanan.muatDariFile(namaFile);

    int pilihan;
    do {
        cout << "\n=== Menu Utama ===\n";
        cout << "1. Tambah Pemesanan\n";
        cout << "2. Hapus Pemesanan\n";
        cout << "3. Tampilkan Pemesanan\n";
        cout << "4. Cari Pesanan\n";
        cout << "5. Keluar\n";
        cout << "Pilih opsi: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1) {
            vector<pair<string, string>> daftarPemesan;
            daftarPesanan.daftarPemesanUnik(daftarPemesan);

            cout << "\n=== Daftar Nama Pemesan yang Sudah Ada ===\n";
            for (int i = 0; i < daftarPemesan.size(); ++i) {
                cout << i + 1 << ". " << daftarPemesan[i].first
                     << " (Kamar: " << daftarPemesan[i].second << ")\n";
            }
            cout << daftarPemesan.size() + 1 << ". Tambah nama pemesan baru\n";

            int pilih;
            cout << "Tambah nama pemesan baru/ pilih nama pemesan yang sudah ada(nomor): ";
            cin >> pilih;
            cin.ignore();

            string namaPemesan, nomorKamar;
            if (pilih >= 1 && pilih <= daftarPemesan.size()) {
                namaPemesan = daftarPemesan[pilih - 1].first;
                nomorKamar = daftarPemesan[pilih - 1].second;
                cout << "Melanjutkan pesanan untuk " << namaPemesan << " (Kamar: " << nomorKamar << ")\n";
            } else {
                cout << "Masukkan Nama Pemesan Baru: ";
                getline(cin, namaPemesan);
                cout << "Masukkan Nomor Kamar: ";
                getline(cin, nomorKamar);
            }

            char ulang;
            do {
                int pilihKategori, pilihItem;
                cout << "Pilih kategori: 1. Makanan  2. Minuman: ";
                cin >> pilihKategori;

                if (pilihKategori == 1) {
                    tampilkanMenu(menuMakanan, "Menu Makanan");
                    cout << "Pilih makanan (1-4): ";
                    cin >> pilihItem;
                    if (pilihItem >= 1 && pilihItem <= 4) {
                        MenuItem dipilih = menuMakanan[pilihItem - 1];
                        daftarPesanan.tambahDiAkhir(namaPemesan, nomorKamar, dipilih.nama, dipilih.harga);
                        cout << "Pesanan berhasil ditambahkan.\n";
                    }
                } else if (pilihKategori == 2) {
                    tampilkanMenu(menuMinuman, "Menu Minuman");
                    cout << "Pilih minuman (1-4): ";
                    cin >> pilihItem;
                    if (pilihItem >= 1 && pilihItem <= 4) {
                        MenuItem dipilih = menuMinuman[pilihItem - 1];
                        daftarPesanan.tambahDiAkhir(namaPemesan, nomorKamar, dipilih.nama, dipilih.harga);
                        cout << "Pesanan berhasil ditambahkan.\n";
                    }
                } else {
                    cout << "Kategori tidak valid.\n";
                }

                cout << "Pesan lagi? (y/t): ";
                cin >> ulang;
                cin.ignore();
            } while (ulang == 'y' || ulang == 'Y');

        } else if (pilihan == 2) {
            cout << "\n=== Daftar Pemesan dan Pesanannya ===\n";
            vector<pair<string, string>> daftarPemesan;
            daftarPesanan.daftarPemesanUnik(daftarPemesan);

            for (const auto& pemesan : daftarPemesan) {
                string nama = pemesan.first;
                string kamar = pemesan.second;
                double total = 0;
                cout << "\nPemesan: " << nama << " (Kamar: " << kamar << ")\n";

                OrderNode* temp = daftarPesanan.head;
                while (temp) {
                    if (temp->namaPemesan == nama && temp->nomorKamar == kamar) {
                        cout << "  - " << temp->namaItem << " | Rp " << temp->harga << endl;
                        total += temp->harga;
                    }
                    temp = temp->next;
                }
                cout << "  >> Total: Rp " << total << endl;
            }

            cout << "\n=== Pilih Jenis Penghapusan ===\n";
            cout << "1. Hapus semua pesanan\n";
            cout << "2. Hapus salah satu pemesan\n";
            cout << "3. Hapus beberapa pemesan\n";
            cout << "Pilihan Anda: ";
            int pilihHapus;
            cin >> pilihHapus;
            cin.ignore();

            if (pilihHapus == 1) {
                char konfirmasi;
                cout << "Yakin ingin menghapus SEMUA pesanan? (y/n): ";
                cin >> konfirmasi;
                cin.ignore();
                if (konfirmasi == 'y' || konfirmasi == 'Y') {
                    OrderNode* temp = daftarPesanan.head;
                    while (temp) {
                        OrderNode* untukHapus = temp;
                        temp = temp->next;
                        delete untukHapus;
                    }
                    daftarPesanan.head = nullptr;
                    daftarPesanan.tail = nullptr;
                    daftarPesanan.simpanKeFile("orders.txt");
                    cout << "Semua pesanan berhasil dihapus.\n";
                } else {
                    cout << "Penghapusan dibatalkan.\n";
                }
            } else if (pilihHapus == 2) {
                cout << "Masukkan nama pemesan yang ingin dihapus: ";
                string nama;
                getline(cin, nama);
                daftarPesanan.hapusPesanan(nama);
            } else if (pilihHapus == 3) {
                cout << "Masukkan beberapa nama pemesan yang ingin dihapus (pisahkan dengan koma): ";
                string input;
                getline(cin, input);
                vector<string> namaList;
                size_t start = 0, end;
                while ((end = input.find(',', start)) != string::npos) {
                    string nama = input.substr(start, end - start);
                    namaList.push_back(nama);
                    start = end + 1;
                }
                namaList.push_back(input.substr(start));

                for (string& nama : namaList) {
                    // Hilangkan spasi di awal/akhir
                    while (!nama.empty() && nama.front() == ' ') nama.erase(0, 1);
                    while (!nama.empty() && nama.back() == ' ') nama.pop_back();
                    if (!nama.empty()) {
                        daftarPesanan.hapusPesanan(nama);
                    }
                }
            } else {
                cout << "Pilihan tidak valid.\n";
            }

        } else if (pilihan == 3) {
            daftarPesanan.tampilkan();

        } else if (pilihan == 4) {
            string cariNama;
            cout << "Masukkan nama pemesan yang dicari: ";
            getline(cin, cariNama);
            daftarPesanan.cariPesanan(cariNama);

        } else if (pilihan == 5) {
            cout << "Terima kasih telah menggunakan aplikasi.\n";
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }

    } while (pilihan != 5);

    return 0;
}
