#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // untuk fungsi max()

const int TOTAL_KARAKTER = 256;

std::vector<int> buatTabelKarakterBuruk(const std::string &pola) {
    int panjangPola = pola.size();
    std::vector<int> karakterBuruk(TOTAL_KARAKTER, -1); // Inisialisasi dengan -1

    for (int i = 0; i < panjangPola; i++) {
        karakterBuruk[(int)pola[i]] = i;
    }
    return karakterBuruk;
}

std::vector<int> pencarianHorspool(const std::string &teks, const std::string &pola) {
    std::vector<int> indeks_hasil; // Menyimpan indeks kecocokan
    int panjangTeks = teks.size();
    int panjangPola = pola.size();
    
    if (panjangPola == 0 || panjangTeks < panjangPola)
        return indeks_hasil;
    
    std::vector<int> karakterBuruk = buatTabelKarakterBuruk(pola);
    
    int geser = 0; // Variabel geser menunjukkan posisi saat ini dari pola di dalam teks.
    
    while (geser <= panjangTeks - panjangPola) {
        int j = panjangPola - 1; // Mulai perbandingan dari indeks terakhir pola.
        
        while (j >= 0 && pola[j] == teks[geser + j])
            j--; // Jika cocok, geser j ke kiri (decrement).
        
        if (j < 0) {
            indeks_hasil.push_back(geser);
            geser += (geser + panjangPola < panjangTeks) ? panjangPola - karakterBuruk[(int)teks[geser + panjangPola]] : 1;
        } else {
            geser += std::max(1, j - karakterBuruk[(int)teks[geser + j]]);
        }
    }
    return indeks_hasil;
}

int main() {
    std::string teks = "ABAAABCDABAACD";
    std::string pola = "ABC";

    std::vector<int> hasil = pencarianHorspool(teks, pola);

    std::cout << "Pola ditemukan pada indeks: ";
    for (int indeks : hasil) {
        std::cout << indeks << " ";
    }
    std::cout << "\n";

    return 0;
}