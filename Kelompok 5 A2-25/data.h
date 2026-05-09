#ifndef DATA_H
#define DATA_H

#include <string>
using namespace std;


struct Pengguna {
    string username;
    string password;
    string nama_lengkap;
    string no_telepon;
    string alamat;
    string role;
    bool   aktif;
};

struct Barang {
    int    id;
    string nama;
    string satuan;
    float  stok;
    float  harga;
    bool   aktif;
};


#define MAKS_USER   100
#define MAKS_BARANG  50

extern Pengguna daftar_pengguna[MAKS_USER];
extern int      jumlah_pengguna;

extern Barang   daftar_barang[MAKS_BARANG];
extern int      jumlah_barang;

extern string username_aktif;
extern string role_aktif;
extern string nama_aktif;

#endif