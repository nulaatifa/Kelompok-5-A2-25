#include "user.h"
#include <iomanip>
#include <iostream>
#include <string>
#include "../database_handler.h"

// 1. UPDATE PARAMETER: Tambahkan pointer ke Pengguna (users)
void orderItem(std::vector<Barang>* items, std::vector<Rute>* routes, std::vector<Pesanan>* trxs, std::vector<Pengguna>* users) {
    if (items->empty()) {
        std::cout << "\n\033[1;31mBelum ada barang untuk dipesan. Menunggu Admin.\033[0m\n";
        return;
    }

    displayItems(items);
    std::cout << "\nMasukkan Kode Barang yang ingin dibeli: ";
    std::string kode = getValidStringUser();

    Barang* selectedItem = nullptr;
    for (auto& it : *items) {
        if (it.id_barang == kode) {
            selectedItem = &it;
            break;
        }
    }

    if (selectedItem == nullptr) {
        std::cout << "\033[1;31mBarang tidak ditemukan.\033[0m\n";
        return;
    }

    std::cout << "Masukkan jumlah beli: ";
    long long qty = getValidNumberUser();

    if (qty > selectedItem->stok) {
        std::cout << "\033[1;31mStok tidak mencukupi. Stok saat ini: " << selectedItem->stok << "\033[0m\n";
        return;
    }

    long long subtotal = selectedItem->harga * qty;
    long long ongkir = 0;
    std::string tipeBeli = "Ditempat";

    std::vector<std::string> tipeOptions = {"Beli Ditempat", "Diantar (Kirim ke Alamat)"};
    int tipePilihan = inquirerMenuUser("PILIH TIPE PEMBELIAN", tipeOptions);

    clearScreenUser();
    std::cout << "\033[1;36mMelanjutkan pesanan: " << selectedItem->nama_barang << " (x" << qty << ")\033[0m\n\n";

    if (tipePilihan == 1) { // Jika pilih Diantar
        if (routes->empty()) {
            std::cout << "\033[1;31mBelum ada rute pengiriman dari Admin. Transaksi dibatalkan.\033[0m\n";
            return;
        }

        std::cout << "\033[1;32m=== DAFTAR RUTE PENGIRIMAN (DARI SAMARINDA) ===\033[0m\n";
        std::cout << "---------------------------------------------------\n";
        std::cout << std::left << std::setw(15) << "Kode Rute" << std::setw(25) << "Tujuan Kota" << "Jarak\n";
        std::cout << "---------------------------------------------------\n";

        for (const auto& r : *routes) {
            std::cout << std::left << std::setw(15) << r.id_rute << std::setw(25) << r.tujuan << r.jarak << " km\n";
        }
        std::cout << "---------------------------------------------------\n";
        
        std::cout << "\nMasukkan Kode Rute Pengiriman: ";
        std::string ruteKode = getValidStringUser();

        Rute* selectedRoute = nullptr;
        for (auto& r : *routes) {
            if (r.id_rute == ruteKode) {
                selectedRoute = &r;
                break;
            }
        }

        if (selectedRoute == nullptr) {
            std::cout << "\033[1;31mRute tidak valid. Transaksi dibatalkan.\033[0m\n";
            return;
        }

        // --- SISTEM PILIH LAYANAN (REGULER / STANDAR / PREMIUM) ---
        std::vector<std::string> serviceOptions;
        std::vector<long long> serviceCosts;
        std::vector<std::string> serviceNames;

        if (selectedRoute->biaya_reguler > 0) {
            serviceOptions.push_back("Reguler (Rp" + std::to_string((long long)selectedRoute->biaya_reguler) + ") - " + selectedRoute->estimasi_reguler);
            serviceCosts.push_back((long long)selectedRoute->biaya_reguler);
            serviceNames.push_back("Reguler");
        }
        if (selectedRoute->biaya_standar > 0) {
            serviceOptions.push_back("Standar (Rp" + std::to_string((long long)selectedRoute->biaya_standar) + ") - " + selectedRoute->estimasi_standar);
            serviceCosts.push_back((long long)selectedRoute->biaya_standar);
            serviceNames.push_back("Standar");
        }
        if (selectedRoute->biaya_premium > 0) {
            serviceOptions.push_back("Premium (Rp" + std::to_string((long long)selectedRoute->biaya_premium) + ") - " + selectedRoute->estimasi_premium);
            serviceCosts.push_back((long long)selectedRoute->biaya_premium);
            serviceNames.push_back("Premium");
        }

        if (serviceOptions.empty()) {
            std::cout << "\033[1;31mMaaf, rute ini sedang tidak melayani pengiriman (belum ada harga yang diatur Admin).\033[0m\n";
            return;
        }

        int servicePick = inquirerMenuUser("PILIH LAYANAN PENGIRIMAN (" + selectedRoute->kota_asal + " -> " + selectedRoute->tujuan + ")", serviceOptions);
        
        ongkir = serviceCosts[servicePick];
        tipeBeli = "Diantar (" + serviceNames[servicePick] + ")"; // Contoh hasil: "Diantar (Premium)"
    }

    long long grandTotal = subtotal + ongkir;
    
    // Stok barang otomatis berkurang
    selectedItem->stok -= qty;

    std::string newTrxId = "TRX00" + std::to_string(trxs->size() + 1);
    
    // Catatan: Tipe beli sekarang akan merekam jenis pengiriman, misal "Diantar (Premium)"
    Pesanan newTrx = {newTrxId, selectedItem->nama_barang, static_cast<int>(qty), static_cast<double>(grandTotal), tipeBeli, "Menunggu Konfirmasi"};
    
    // Pesanan baru ditambahkan ke vector
    trxs->push_back(newTrx);

    // 2. AUTO-UPDATE JSON KETIKA PESANAN SUKSES
    // Urutan: Barang (stok update), Pesanan (nambah baru), Rute, User
    simpanData(*items, *trxs, *routes, *users);

    std::cout << "\n\033[1;33mTotal Pesanan (Barang + Ongkir): Rp" << grandTotal << "\033[0m\n";
    std::cout << "\033[1;32mPesanan berhasil dibuat dengan ID: " << newTrxId << "\033[0m\n";
    std::cout << "[+] Data Stok Barang dan Pesanan JSON otomatis diperbarui!\n";
    std::cout << "\033[1;36mSilakan tunggu konfirmasi Admin sebelum melakukan pembayaran di menu Bayar Pesanan.\033[0m\n";
}