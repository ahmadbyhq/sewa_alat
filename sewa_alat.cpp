#include <iostream>
#include <string>
#include <conio.h> // Untuk getch()
#include <limits>

using namespace std;

struct EquipmentItem
{
    int id;
    string name;
    float pricePerDay;
    int stock;
    EquipmentItem *next;
};

struct RentalItem
{
    int id;
    string name;
    float pricePerDay;
    int days;         // Jumlah hari penyewaan
    float totalPrice; // Total harga penyewaan
    RentalItem *next;
};

EquipmentItem *equipmentList = NULL;
RentalItem *rentalHistory = NULL;
int equipmentID = 1;
float totalIncome = 0; // Menyimpan total pendapatan
const string ADMIN_KEY = "admin123";

// Fungsi untuk membersihkan layar
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// fungsi untuk validasi input pilihan yang valid
int getValidIntInput()
{
    int input;
    while (true)
    {
        cin >> input;
        if (cin.fail()) // Jika input bukan integer
        {
            cin.clear();                                         // Bersihkan error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Abaikan input yang salah
            cout << "Input tidak valid! Harap masukkan angka yang valid: ";
        }
        else
        {
            break; // Input valid, keluar dari loop
        }
    }
    return input;
}

// fungsi untuk validasi input harga yang valid
int getValidInputPrice()
{
    float inputPrice;
    while (true)
    {
        cin >> inputPrice;
        if (cin.fail()) // Jika input bukan integer
        {
            cin.clear();                                         // Bersihkan error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Abaikan input yang salah
            cout << "Input tidak valid! Harap masukkan harga yang valid: ";
        }
        else
        {
            break; // Input valid, keluar dari loop
        }
    }
    return inputPrice;
}

// fungsi untuk validasi input stock yang valid
int getValidInputStock()
{
    int inputStock;
    while (true)
    {
        cin >> inputStock;
        if (cin.fail()) // Jika input bukan integer
        {
            cin.clear();                                         // Bersihkan error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Abaikan input yang salah
            cout << "Input tidak valid! Harap masukkan stok yang valid: ";
        }
        else
        {
            break; // Input valid, keluar dari loop
        }
    }
    return inputStock;
}

// Fungsi untuk menunggu pengguna menekan enter
void pauseForEnter()
{
    cout << "Tekan Enter untuk melanjutkan...";
    cin.ignore(); // Mengabaikan karakter sisa dari input sebelumnya
    cin.get();    // Menunggu pengguna menekan Enter
}

// Fungsi untuk input password tanpa menampilkan karakter
string inputPassword()
{
    string password;
    char ch;
    cout << "Masukkan password: ";
    while (true)
    {
        ch = getch(); // Membaca karakter tanpa echo di layar
        if (ch == '\r')
        { // Enter ditekan
            cout << endl;
            break;
        }
        else if (ch == '\b')
        { // Backspace ditekan
            if (!password.empty())
            {
                password.erase(password.size() - 1); // Hapus karakter terakhir
                cout << "\b \b";                     // Hapus karakter dari layar
            }
        }
        else
        {
            password += ch;
            cout << '*'; // Tampilkan bintang untuk setiap karakter
        }
    }
    return password;
}

// Fungsi untuk autentikasi admin
bool autentikasiAdmin()
{
    string inputKey = inputPassword();
    if (inputKey == ADMIN_KEY)
    {
        cout << "Akses diterima.\n";
        pauseForEnter();
        return true;
    }
    else
    {
        cout << "Akses ditolak. Key salah!\n";
        pauseForEnter();
        return false;
    }
}

// Fungsi untuk memperbarui ID secara otomatis
void updateEquipmentIDs()
{
    EquipmentItem *current = equipmentList;
    int newID = 1;
    while (current)
    {
        current->id = newID++;
        current = current->next;
    }
    equipmentID = newID; // Perbarui nilai global ID
}

// Fungsi untuk menampilkan daftar alat secara ascending
void viewEquipmentList()
{
    if (!equipmentList)
    {
        cout << "Maaf, daftar alat masih kosong :(\n";
        return;
    }

    EquipmentItem *current = equipmentList;
    EquipmentItem *sortedList = NULL;

    // Salin dan urutkan secara manual
    while (current)
    {
        EquipmentItem *newItem = new EquipmentItem;
        *newItem = *current;
        newItem->next = NULL;

        if (!sortedList || newItem->id < sortedList->id)
        {
            newItem->next = sortedList;
            sortedList = newItem;
        }
        else
        {
            EquipmentItem *temp = sortedList;
            while (temp->next && temp->next->id < newItem->id)
            {
                temp = temp->next;
            }
            newItem->next = temp->next;
            temp->next = newItem;
        }
        current = current->next;
    }

    // Tampilkan daftar yang sudah diurutkan
    cout << "\nDaftar Alat Outdoor:\n";
    current = sortedList;
    while (current)
    {
        cout << current->id << ". " << current->name << " - Rp " << current->pricePerDay
             << " per hari | Stok: " << current->stock << endl;
        EquipmentItem *temp = current;
        current = current->next;
        delete temp;
    }
    cout << endl;
}

// Fungsi untuk menambahkan alat baru
void addEquipment(string name, float pricePerDay, int stock)
{
    EquipmentItem *newItem = new EquipmentItem;
    newItem->id = equipmentID++;
    newItem->name = name;
    newItem->pricePerDay = pricePerDay;
    newItem->stock = stock;
    newItem->next = equipmentList;
    equipmentList = newItem;
}

void deleteEquipment()
{
    if (!equipmentList)
    {
        cout << "Maaf, daftar alat masih kosong :(\n";
        pauseForEnter();
        return;
    }

    viewEquipmentList();

    int id;
    cout << "\nMasukkan ID alat yang ingin dihapus: ";
    id = getValidIntInput();

    EquipmentItem *current = equipmentList;
    EquipmentItem *prev = NULL;

    while (current && current->id != id)
    {
        prev = current;
        current = current->next;
    }

    if (!current)
    {
        cout << "Alat tidak ditemukan!\n";
        pauseForEnter();
        return;
    }

    if (prev)
    {
        prev->next = current->next;
    }
    else
    {
        equipmentList = current->next;
    }

    delete current;

    // Buat ulang daftar alat yang diurutkan berdasarkan ID
    EquipmentItem *newEquipmentList = NULL;
    current = equipmentList;

    while (current)
    {
        EquipmentItem *newItem = new EquipmentItem;
        *newItem = *current;
        newItem->next = NULL;

        // Tambahkan ke daftar baru
        if (!newEquipmentList || newItem->id < newEquipmentList->id)
        {
            newItem->next = newEquipmentList;
            newEquipmentList = newItem;
        }
        else
        {
            EquipmentItem *temp = newEquipmentList;
            while (temp->next && temp->next->id < newItem->id)
            {
                temp = temp->next;
            }
            newItem->next = temp->next;
            temp->next = newItem;
        }

        current = current->next;
    }

    // Ganti daftar lama dengan daftar baru
    while (equipmentList)
    {
        EquipmentItem *temp = equipmentList;
        equipmentList = equipmentList->next;
        delete temp;
    }
    equipmentList = newEquipmentList;

    // Perbarui ID
    updateEquipmentIDs();

    cout << "Alat berhasil dihapus dan daftar diperbarui!\n";
    pauseForEnter();
}

// Fungsi untuk mengubah data alat
void updateEquipment()
{

    if (!equipmentList)
    {
        cout << "Maaf, daftar alat masih kosong :(\n";
        pauseForEnter();
        return;
    }

    viewEquipmentList();

    int id;
    cout << "\nMasukkan ID alat yang ingin diubah: ";
    id = getValidIntInput();

    EquipmentItem *current = equipmentList;
    while (current && current->id != id)
    {
        current = current->next;
    }

    if (!current)
    {
        cout << "Alat tidak ditemukan!\n";
        pauseForEnter();
        return;
    }

    string newName;
    float newPrice;
    int newStock;
    cout << "Masukkan nama baru: ";
    cin.ignore();
    getline(cin, newName);
    cout << "Masukkan harga sewa baru per hari: ";
    newPrice = getValidInputPrice();
    cout << "Masukkan stok baru: ";
    newStock = getValidInputStock();

    current->name = newName;
    current->pricePerDay = newPrice;
    current->stock = newStock;

    cout << "Data alat berhasil diubah!\n";
    pauseForEnter();
}

// Fungsi untuk menyewa alat
// Fungsi untuk menyewa alat
void rentEquipment()
{
    if (!equipmentList)
    {
        cout << "Daftar alat masih kosong. Silakan kembali dan tambahkan alat terlebih dahulu.\n";
        pauseForEnter();
        return;
    }

    viewEquipmentList();

    int id;
    cout << "\nMasukkan ID alat yang ingin disewa: ";
    id = getValidIntInput();

    EquipmentItem *current = equipmentList;
    while (current && current->id != id)
    {
        current = current->next;
    }

    if (!current)
    {
        cout << "Alat tidak ditemukan!\n";
        pauseForEnter();
        return;
    }

    if (current->stock <= 0)
    {
        cout << "Maaf, stok alat habis!\n";
        pauseForEnter();
        return;
    }

    // Input jumlah hari sewa
    int days;
    cout << "Masukkan jumlah hari penyewaan: ";
    days = getValidIntInput();

    // Hitung total harga
    float totalPrice = current->pricePerDay * days;
    cout << "Total harga sewa untuk " << current->name << " selama " << days << " hari adalah Rp " << totalPrice << endl;

    // Konfirmasi sewa
    char konfirmasi;
    cout << "Apakah Anda ingin melanjutkan penyewaan? (y/n): ";
    cin >> konfirmasi;

    if (konfirmasi == 'y' || konfirmasi == 'Y')
    {
        // Kurangi stok
        current->stock--;

        // Tambahkan ke riwayat penyewaan
        RentalItem *newRental = new RentalItem;
        newRental->id = rentalHistory ? rentalHistory->id + 1 : 1;
        newRental->name = current->name;
        newRental->pricePerDay = current->pricePerDay;
        newRental->days = days;
        newRental->totalPrice = totalPrice; // Simpan total harga
        newRental->next = rentalHistory;
        rentalHistory = newRental;

        // Tambahkan total pendapatan
        totalIncome += totalPrice;

        cout << current->name << " berhasil disewa!\n";
        cout << "Total harga yang harus dibayar: Rp " << totalPrice << endl;
        pauseForEnter();
    }
    else
    {
        cout << "Penyewaan dibatalkan.\n";
        pauseForEnter();
    }
}

// Fungsi untuk melihat riwayat penyewaan
void viewRentalHistory()
{
    if (!rentalHistory)
    {
        cout << "Tidak ada riwayat penyewaan.\n";
        return;
    }

    RentalItem *reversedHistory = NULL;
    RentalItem *current = rentalHistory;

    // Membalik daftar riwayat penyewaan
    while (current)
    {
        RentalItem *newItem = new RentalItem;
        *newItem = *current;
        newItem->next = reversedHistory;
        reversedHistory = newItem;
        current = current->next;
    }

    cout << "\nRiwayat Penyewaan Alat:\n";
    current = reversedHistory;
    while (current)
    {
        cout << current->id << ". " << current->name << " - Rp " << current->pricePerDay << " per hari | "
             << "Jumlah hari: " << current->days << " | Total harga: Rp " << current->totalPrice << endl;
        RentalItem *temp = current;
        current = current->next;
        delete temp;
    }
    cout << endl;
}

int main()
{

    addEquipment("Tenda Camping", 50000, 10);
    addEquipment("Kompor Portable", 30000, 5);
    addEquipment("Sleeping Bag", 20000, 20);

    int choice;
    do
    {
        clearScreen();
        cout << "\n===== Penyewaan Alat Outdoor =====\n";
        cout << "1. Lihat Daftar Alat\n";
        cout << "2. Sewa Alat\n";
        cout << "3. Riwayat Penyewaan\n";
        cout << "4. Admin\n";
        cout << "5. Keluar\n";
        cout << "Pilihan Anda: ";
        choice = getValidIntInput();

        switch (choice)
        {
        case 1:
            clearScreen();
            viewEquipmentList();
            pauseForEnter();
            break;
        case 2:
            clearScreen();
            rentEquipment();
            break;
        case 3:
            clearScreen();
            viewRentalHistory();
            pauseForEnter();
            break;
        case 4:
            if (autentikasiAdmin())
            {
                string name;
                float pricePerDay;
                int stock;
                int adminChoice;
                do
                {
                    clearScreen();
                    cout << "\n===== Admin Menu =====\n";
                    cout << "1. Lihat Daftar Alat\n";
                    cout << "2. Tambah Alat\n";
                    cout << "3. Hapus Alat\n";
                    cout << "4. Ubah Data Alat\n";
                    cout << "5. Lihat Total Pendapatan\n"; // Menambah pilihan total pendapatan
                    cout << "6. Logout\n";
                    cout << "Pilihan Anda: ";
                    adminChoice = getValidIntInput();

                    switch (adminChoice)
                    {
                    case 1:
                        clearScreen();
                        viewEquipmentList();
                        pauseForEnter();
                        break;
                    case 2:
                        clearScreen();
                        cout << "Masukkan nama alat: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "\nMasukkan harga sewa per hari: ";
                        pricePerDay = getValidInputPrice();
                        cout << "\nMasukkan jumlah stok: ";
                        stock = getValidInputStock();
                        addEquipment(name, pricePerDay, stock);
                        cout << "\nAlat berhasil ditambahkan!\n";
                        pauseForEnter();
                        break;
                    case 3:
                        clearScreen();
                        deleteEquipment();
                        break;
                    case 4:
                        clearScreen();
                        updateEquipment();
                        break;
                    case 5:
                        clearScreen();
                        cout << "Total pendapatan dari semua penyewaan: Rp " << totalIncome << endl;
                        pauseForEnter();
                        break;
                    case 6:
                        cout << "\nKembali ke menu utama.\n";
                        pauseForEnter();
                        break;
                    default:
                        cout << "\nPilihan tidak valid!\n";
                        pauseForEnter();
                    }
                } while (adminChoice != 6);
            }
            break;
        case 5:
            clearScreen();
            cout << "Terima kasih telah menggunakan jasa kami\n";
            break;
        default:
            cout << "\nPilihan tidak valid!\n";
            pauseForEnter();
        }
    } while (choice != 5);

    return 0;
}