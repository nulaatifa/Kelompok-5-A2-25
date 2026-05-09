#ifndef VALIDASI_H
#define VALIDASI_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <cctype>
#include <thread>
#include <chrono>
using namespace std;

inline void tidur(int detik) {
    this_thread::sleep_for(chrono::seconds(detik));
}

inline string trim(const string &s) {
    int awal  = 0;
    int akhir = (int)s.length() - 1;
    while (awal  <= akhir && s[awal]  == ' ') awal++;
    while (akhir >= awal  && s[akhir] == ' ') akhir--;
    if (awal > akhir) return "";
    return s.substr(awal, akhir - awal + 1);
}

inline int inputPilihan() {
    string input;
    getline(cin, input);
    input = trim(input);
    if (input.empty())
        throw runtime_error("Pilihan tidak boleh kosong.");
    for (int i = 0; i < (int)input.length(); i++)
        if (!isdigit(input[i]))
            throw runtime_error("Pilihan harus berupa angka, bukan huruf atau simbol.");
    if (input.length() > 9)
        throw runtime_error("Pilihan tidak valid.");
    return stoi(input);
}

inline void validasiTidakKosong(const string &nilai, const string &field) {
    if (trim(nilai).empty())
        throw runtime_error(field + " tidak boleh kosong.");
}

inline void validasiMinPanjang(const string &nilai, int min, const string &field) {
    if ((int)nilai.length() < min)
        throw runtime_error(field + " minimal " + to_string(min) + " karakter.");
}

inline void validasiMaksPanjang(const string &nilai, int maks, const string &field) {
    if ((int)nilai.length() > maks)
        throw runtime_error(field + " maksimal " + to_string(maks) + " karakter.");
}

inline void validasiMinInt(int nilai, int min, const string &field) {
    if (nilai < min)
        throw runtime_error(field + " tidak boleh kurang dari " + to_string(min) + ".");
}

inline float inputAngkaPositif(const string &field) {
    string input;
    getline(cin, input);
    input = trim(input);
    if (input.empty())
        throw runtime_error(field + " tidak boleh kosong.");
    bool adaTitik = false, adaAngka = false;
    for (int i = 0; i < (int)input.length(); i++) {
        char c = input[i];
        if (c == '-') throw runtime_error(field + " tidak boleh negatif.");
        if (c == '.') {
            if (adaTitik) throw runtime_error(field + " format angka tidak valid (titik ganda).");
            adaTitik = true; continue;
        }
        if (!isdigit(c)) throw runtime_error(field + " hanya boleh berisi angka. Simbol dan huruf tidak diizinkan.");
        adaAngka = true;
    }
    if (!adaAngka)           throw runtime_error(field + " harus mengandung angka.");
    if (input.length() > 15) throw runtime_error(field + " nilai terlalu besar.");
    float nilai = stof(input);
    if (nilai <= 0)          throw runtime_error(field + " harus lebih dari 0.");
    return nilai;
}

inline bool inputAngkaPositifEdit(const string &field, float &hasil) {
    string input;
    getline(cin, input);
    input = trim(input);
    if (input.empty()) return false;
    bool adaTitik = false, adaAngka = false;
    for (int i = 0; i < (int)input.length(); i++) {
        char c = input[i];
        if (c == '-') throw runtime_error(field + " tidak boleh negatif.");
        if (c == '.') {
            if (adaTitik) throw runtime_error(field + " format angka tidak valid (titik ganda).");
            adaTitik = true; continue;
        }
        if (!isdigit(c)) throw runtime_error(field + " hanya boleh berisi angka. Simbol dan huruf tidak diizinkan.");
        adaAngka = true;
    }
    if (!adaAngka)           throw runtime_error(field + " harus mengandung angka.");
    if (input.length() > 15) throw runtime_error(field + " nilai terlalu besar.");
    float nilai = stof(input);
    if (nilai <= 0)          throw runtime_error(field + " harus lebih dari 0.");
    hasil = nilai;
    return true;
}

inline void validasiNamaBarang(const string &nama, const string &field) {
    validasiTidakKosong(nama, field);
    validasiMaksPanjang(nama, 50, field);
    bool adaHuruf = false;
    for (int i = 0; i < (int)nama.length(); i++) {
        char c = nama[i];
        if      (isalpha(c))                                adaHuruf = true;
        else if (isdigit(c) || c==' ' || c=='/' || c=='-') continue;
        else throw runtime_error(field + " mengandung karakter tidak valid: '" + c + "'.");
    }
    if (!adaHuruf) throw runtime_error(field + " harus mengandung huruf.");
}

inline void validasiUsername(const string &username, const string &field) {
    validasiTidakKosong(username, field);
    validasiMinPanjang(username, 3, field);
    validasiMaksPanjang(username, 20, field);
    if (username.find(' ') != string::npos)
        throw runtime_error(field + " tidak boleh mengandung spasi.");
    for (int i = 0; i < (int)username.length(); i++) {
        char c = username[i];
        if (!isalnum(c) && c != '_' && c != '.')
            throw runtime_error(field + " hanya boleh huruf, angka, '_', atau '.'. Simbol lain tidak diizinkan.");
    }
}

inline bool inputYaTidak() {
    while (true) {
        string input;
        getline(cin, input);
        input = trim(input);

        if (input.empty()) {
            cout << "  [!] Jawaban tidak boleh kosong. Ketik y atau n : ";
            continue;
        }
        if (input.length() > 1) {
            cout << "  [!] Cukup ketik y atau n saja : ";
            continue;
        }

        char c = input[0];

        if (isdigit(c)) {
            cout << "  [!] Tidak boleh angka. Ketik y atau n : ";
            continue;
        }
        if (!isalpha(c)) {
            cout << "  [!] Tidak boleh simbol. Ketik y atau n : ";
            continue;
        }
        if (c != 'y' && c != 'Y' && c != 'n' && c != 'N') {
            cout << "  [!] Hanya y atau n yang diterima : ";
            continue;
        }

        return (c == 'y' || c == 'Y');
    }
}

#endif