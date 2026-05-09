#include <iostream>
#include <string>
#include <cctype>
#include "validasi.h"
#include "data.h"
using namespace std;

static void clearScreenLogin() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void tampilHeader();
void tampilGaris();
void validasiNamaLengkap(const string &nama, const string &field);
bool cekUsernameTerdaftar(const string &username);
Pengguna* cariPenggunaLogin(const string &username, const string &password);
void menuRegister();
bool menuLogin();


void tampilHeader() {
    cout << "\n====================================================" << endl;
    cout << "     MOLOTOV - TOKO MATERIAL BANGUNAN ONLINE" << endl;
    cout << "====================================================" << endl;
}

void tampilGaris() {
    cout << "----------------------------------------------------" << endl;
}



void validasiNamaLengkap(const string &nama, const string &field) {
    validasiTidakKosong(nama, field);
    validasiMinPanjang(nama, 3, field);
    validasiMaksPanjang(nama, 50, field);

    bool adaHuruf = false;
    for (int i = 0; i < (int)nama.length(); i++) {
        char c = nama[i];
        if (isalpha(c)) {
            adaHuruf = true;
        } else if (c == ' ') {
            continue;
        } else if (isdigit(c)) {
            throw runtime_error(field + " tidak boleh mengandung angka.");
        } else {
            throw runtime_error(field + " tidak boleh mengandung simbol '" + c + "'.");
        }
    }

    if (!adaHuruf)
        throw runtime_error(field + " harus mengandung huruf.");
}


bool cekUsernameTerdaftar(const string &username) {
    for (int i = 0; i < jumlah_pengguna; i++) {
        if (daftar_pengguna[i].username == username)
            return true;
    }
    return false;
}


Pengguna* cariPenggunaLogin(const string &username, const string &password) {
    for (int i = 0; i < jumlah_pengguna; i++) {
        if (daftar_pengguna[i].username == username &&
            daftar_pengguna[i].password == password &&
            daftar_pengguna[i].aktif    == true)
            return &daftar_pengguna[i];
    }
    return nullptr;
}


void menuRegister() {
    bool selesai = false;

    while (!selesai) {
        clearScreenLogin();
        tampilHeader();
        cout << "           REGISTRASI AKUN BARU" << endl;
        tampilGaris();

        try {
            if (jumlah_pengguna >= MAKS_USER)
                throw runtime_error("Kapasitas user penuh. Hubungi admin.");

            string nama, username, password, konfirmasi, telepon, alamat;

            cout << "  Nama Lengkap : ";
            getline(cin, nama);
            validasiNamaLengkap(nama, "Nama Lengkap");

            cout << "  Username     : ";
            getline(cin, username);
            username = trim(username);
            validasiUsername(username, "Username");
            if (cekUsernameTerdaftar(username))
                throw runtime_error("Username \"" + username + "\" sudah dipakai. Silakan pilih username lain.");

            cout << "  No Telepon   : ";
            getline(cin, telepon);
            telepon = trim(telepon);
            validasiTidakKosong(telepon, "No Telepon");
            for (int i = 0; i < (int)telepon.length(); i++) {
                if (!isdigit(telepon[i]))
                    throw runtime_error("No Telepon hanya boleh berisi angka.");
            }
            if ((int)telepon.length() < 9 || (int)telepon.length() > 13)
                throw runtime_error("No Telepon harus antara 9-13 digit.");

            cout << "  Alamat       : ";
            getline(cin, alamat);
            validasiTidakKosong(alamat, "Alamat");
            validasiMaksPanjang(alamat, 100, "Alamat");

            cout << "  Password     : ";
            getline(cin, password);
            validasiMinPanjang(password, 8, "Password");
            validasiMaksPanjang(password, 30, "Password");

            cout << "  Konfirmasi   : ";
            getline(cin, konfirmasi);
            if (password != konfirmasi)
                throw runtime_error("Konfirmasi password tidak cocok.");

            daftar_pengguna[jumlah_pengguna] = {
                username, password, nama, telepon, alamat, "Customer", true
            };
            jumlah_pengguna++;

            tampilGaris();
            cout << "  Registrasi berhasil! Silahkan login." << endl;
            tidur(2);
            selesai = true;

        } catch (exception &e) {
            tampilGaris();
            cout << "  [!] " << e.what() << endl;
            tampilGaris();

            cout << "  Coba lagi? (y/n) : ";
            if (inputYaTidak()) {
                cout << "  Memuat ulang form";
                for (int i = 0; i < 4; i++) {
                    cout << "." << flush;
                    tidur(1);
                }
            } else {
                cout << "  Registrasi dibatalkan." << endl;
                tidur(2);
                selesai = true;
            }
        }
    }
}


bool menuLogin() {
    bool selesai = false;

    while (!selesai) {
        clearScreenLogin();
        tampilHeader();
        cout << "                   LOGIN" << endl;
        tampilGaris();

        int percobaan = 3;

        while (percobaan > 0) {
            try {
                string username, password;

                cout << "  Username : ";
                getline(cin, username);
                username = trim(username);

                cout << "  Password : ";
                getline(cin, password);

                validasiTidakKosong(username, "Username");
                validasiTidakKosong(password, "Password");

                Pengguna* p = cariPenggunaLogin(username, password);
                if (p == nullptr)
                    throw runtime_error("Username atau password salah.");

                username_aktif = p->username;
                role_aktif     = p->role;
                nama_aktif     = p->nama_lengkap;

                tampilGaris();
                cout << "  Login berhasil!" << endl;
                cout << "  Selamat datang, " << nama_aktif << "!" << endl;
                tampilGaris();
                tidur(2);
                return true;

            } catch (exception &e) {
                percobaan--;
                cout << "  [!] " << e.what();

                if (percobaan > 0) {
                    cout << " | Sisa percobaan: " << percobaan << endl;
                    tampilGaris();
                } else {
                    cout << endl;
                    tampilGaris();
                    cout << "  Akses ditolak. Terlalu banyak percobaan gagal." << endl;
                    tampilGaris();

                    cout << "  Coba lagi? (y/n) : ";
                    if (inputYaTidak()) {
                        cout << "  Memuat ulang halaman login";
                        for (int i = 0; i < 6; i++) {
                            cout << "." << flush;
                            tidur(1);
                        }
                        cout << endl;
                    } else {
                        cout << "  Kembali ke menu utama." << endl;
                        tidur(2);
                        selesai = true;
                    }
                }
            }
        }
    }

    return false;
}