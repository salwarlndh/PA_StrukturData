#include <iostream>
#include <bits/stdc++.h>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

string admUsn = "admin";
string admPw = "admin";

struct menu {
    int id;
    string nama, jenis;
    long double harga;

    menu *next = NULL;
};

struct order {
    int id;
    string nama, username, menu;
    int jumlah;
    long double total;

    order *next = NULL;
    order *prev = NULL;
};

struct user {
    int id;
    string nama, username, password;

    user *next = NULL;
};

struct cart {
    string username, nameUser, nameMenu;
    int jumlah;
    long double total;

    cart *next = NULL;
};

struct checkout {
    string username, nameUser, nameMenu, status;
    int jumlah;
    long double total;

    checkout *next = NULL;
};

// single linked list
menu *headmenu = NULL;
user *headuser = NULL;
cart *headcart = NULL;

// queue
checkout *headco = NULL;

// double linked list
order *headorder = NULL;
order *tailorder = NULL;

void messageAlert(string str) {
    cout << "\n[ " << str << " ]" << endl;
    cout << "[press any key to continue]";
    getch();
}

// cek apakah numerik
bool isNum(string str) {
    bool numeric;
    if (str == "-0") {
        return false;
    } else {
        for (int i = 0; i < str.length(); i++) {
            if (i == 0) {
                if (str[0] == '-') {
                    if (str.length() == 1) {
                        numeric = false;
                        continue;
                    }
                    numeric = true;
                    continue;
                } else if (isdigit(str[i])) {
                    numeric = true;
                    continue;
                } else {
                    numeric = false;
                    break;
                }
            } else {
                if (isdigit(str[i])) {
                    numeric = true;
                    continue;
                }
            }
            numeric = false;
            break;
        }
        return numeric;
    }
}

// cek apakah data menu kosong
bool isMenuEmpty(menu *head) {
    if (head == NULL) {
        return true;
    }
    return false;
}

// cek apakah data keranjang kosong
bool isCartEmpty(cart *head) {
    if (head == NULL) {
        return true;
    }
    return false;
}

int mainMenu() {
    string pilmenu;
    int pilmenuInt;
    while(true) {
        system("CLS");
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|              MAIN MENU             |" << endl
             << "|    ----------------------------    |" << endl
             << "|                                    |" << endl
             << "|    [1] Admin           [2] User    |" << endl
             << "|    [3] Register        [0] Exit    |" << endl
             << "|------------------------------------|" << endl
             << "| ~> Enter your choice : "; getline(cin, pilmenu);
        cout << "|------------------------------------|" << endl;
        if (!isNum(pilmenu)) {
            messageAlert("Please enter number");
        } else {
            pilmenuInt = stoi(pilmenu);
            break;
        }
    }
    return pilmenuInt;
}

// menghitung panjang/banyak data user
int lengthUser(user *head) {
    int jml = 0;
    user *temp = head;
    while (temp != NULL)
    {
        temp = temp->next;
        jml++;
    }
    return jml;
}

// ============== READ FILE ==============
void read_menu(menu **head) {
    string isi, id, nama, jenis, harga;
    int id_int;
    long double harga_ldb;
    ifstream file;
    // membuka file untuk membaca file ke program
    file.open("menus.csv", ios::in);
    if(file.is_open()) {
        // proses membaca file
        while (getline(file, isi)) {
            menu *cur = *head;
            menu *newMenu = new menu;
            stringstream ss(isi);

            // mengubah isi data pada file menjadi string semua
            getline(ss, id, ',');
            getline(ss, nama, ',');
            getline(ss, jenis, ',');
            getline(ss, harga, '\n');

            stringstream id_string(id);
            stringstream harga_string(harga);

            // mengembalikkan nilai yang sebelumnya string menjadi integer kembali
            id_string >> id_int;
            harga_string >> harga_ldb;

            // memasukkan data ke dalam linked list
            newMenu->id = id_int;
            newMenu->nama = nama;
            newMenu->jenis = jenis;
            newMenu->harga = harga_ldb;

            if(*head == NULL) {
                *head = newMenu;
            } else {
                while(cur->next != NULL) {
                    cur = cur->next;
                }
                cur->next = newMenu;
            }
        }
    }
    file.close();
}

void read_user(user **head) {
    ifstream file;
    string isi,id, nama, username, password;
    int id_int;
    // membuka file untuk membaca file ke program 
    file.open("users.csv", ios::in);
    if (file.is_open()) {
        // proses membaca file
        while (getline(file, isi)) {
            user *cur = *head;
            user *newUser = new user;
            stringstream ss(isi);

            // mengubah isi data pada file menjadi string semua
            getline(ss, id, ',');
            getline(ss, nama, ',');
            getline(ss, username, ',');
            getline(ss, password, '\n');

            stringstream id_string(id);

            // mengembalikan nilai yang sebelumnya string menjadi int kembali
            id_string >> id_int;

            // memasukkan data ke dalam linked list
            newUser->id = id_int;
            newUser->nama = nama;
            newUser->username = username;
            newUser->password = password;

            if(*head == NULL) {
                *head = newUser;
            }
            else {
                while(cur->next != NULL) {
                    cur = cur->next;
                }
                cur->next = newUser;
            }
        }
    }
    file.close();
}

void read_cart(cart **head) {
    ifstream file;
    string isi, username, nameUser, nameMenu, jumlah, total;
    int jumlah_int;
    long double total_ldb;
    // membuka file untuk membaca file ke program
    file.open("cart.csv", ios::in);
    if (file.is_open()) {
        while (getline(file, isi)) {
            cart *cur = *head;
            cart *newCart = new cart;
            stringstream ss(isi);

            // mengubah isi data pada file menjadi string semua
            getline(ss, username, ',');
            getline(ss, nameUser, ',');
            getline(ss, nameMenu, ',');
            getline(ss, jumlah, ',');
            getline(ss, total, '\n');

            stringstream jumlah_string(jumlah);
            stringstream total_string(total);

            // mengembalikan nilai yang sebelumnya string menjadi integer kembali
            jumlah_string >> jumlah_int;
            total_string >> total_ldb;

            // memasukkan data ke dalam linked list
            newCart->username = username;
            newCart->nameUser = nameUser;
            newCart->nameMenu = nameMenu;
            newCart->jumlah = jumlah_int;
            newCart->total = total_ldb;
            if (*head = NULL) {
                *head = newCart;
            }
            else {
                while (cur->next != NULL) {
                    cur = cur->next;
                }
                cur->next = newCart;
            }
        }
    }
    // menutup file
    file.close();
}

void read_checkout(checkout **head) {
    ifstream file;
    string isi, username, nameUser, nameMenu, jumlah, total;
    int jumlah_int;
    long double total_ldb;
    // membuka file untuk membaca file ke program
    file.open("checkout.csv", ios::in);
    if (file.is_open()) {
        while (getline(file, isi)) {
            checkout *cur = *head;
            checkout *newCheckout = new checkout;
            stringstream ss(isi);

            // mengubah isi data pada file menjadi string semua
            getline(ss, username, ',');
            getline(ss, nameUser, ',');
            getline(ss, nameMenu, ',');
            getline(ss, jumlah, ',');
            getline(ss, total, '\n');

            stringstream jumlah_string(jumlah);
            stringstream total_string(total);

            // mengembalikan nilai yang sebelumnya string menjadi integer kembali
            jumlah_string >> jumlah_int;
            total_string >> total_ldb;

            // memasukkan data ke dalam linked list
            newCheckout->username = username;
            newCheckout->nameUser = nameUser;
            newCheckout->nameMenu = nameMenu;
            newCheckout->jumlah = jumlah_int;
            newCheckout->total = total_ldb;
            if (*head = NULL) {
                *head = newCheckout;
            }
            else {
                while (cur->next != NULL) {
                    cur = cur->next;
                }
                cur->next = newCheckout;
            }
        }
    }
    // menutup file
    file.close();
}

void read_order(order **head, order **tail) {
    ifstream file;
    string isi, id, nama, jumlah, total;
    int id_int, jumlah_int;
    long double total_ldb;
    // membuka file untuk membaca file ke program 
    file.open("order.csv", ios::in); 
        // proses membaca file
        if (file.is_open()) {
            while (getline(file, isi)) {
                // order *cur = *head;
                order *newOrder = new order;
                stringstream ss(isi);

                // mengubah isi data pada file menjadi string semua
                getline(ss, id, ',');
                getline(ss, nama, ',');
                getline(ss, jumlah, ',');
                getline(ss, total, '\n');

                stringstream id_string(id);
                stringstream jumlah_string(jumlah);
                stringstream total_string(total);

                // mengembalikan nilai yang sebelumnya string menjadi int kembali
                id_string >> id_int;
                jumlah_string >> jumlah_int;
                total_string >> total_ldb;

                // memasukkan data ke dalam linked list
                newOrder->id = id_int;
                newOrder->nama = nama;
                newOrder->jumlah = jumlah_int;
                newOrder->total = total_ldb;
                if (*head == NULL && *tail == NULL){
                    newOrder->prev = *tail;
                    newOrder->next = NULL;
                    *head = newOrder;
                    *tail = newOrder;
                } else {
                    newOrder->prev = *tail;
                    newOrder->next = NULL;
                    // mengubah next pada tail menjadi node baru dan node baru menjadi tail
                    (*tail)->next = newOrder;
                    *tail = newOrder;
                }
            }
        }
    // menutup file
    file.close();
}

// ============== WRITE FILE ==============
void create_menu(menu *head) {
    fstream file;
    menu *cur_menu = head;
    // membuka file untuk menulis data ke file
    file.open("menus.csv", ios::trunc | ios::out | ios::in);
    if (file.is_open()) {
        // traversal
        while (cur_menu != NULL) {
            file << cur_menu->id << ",";
            file << cur_menu->nama << ",";
            file << cur_menu->jenis << ",";
            file << cur_menu->harga << endl;
            cur_menu = cur_menu->next;
        }
    }
    // menutup file
    file.close();
}

void create_user(user *head) {
    fstream file;
    user *cur_akun = head;
    // membuka file untuk menulis data ke file
    file.open("users.csv", ios::trunc | ios::out | ios::in);
    if (file.is_open()) {
        // traversal
        while (cur_akun != NULL) {
            file << cur_akun->id << ",";
            file << cur_akun->nama << ",";
            file << cur_akun->username << ",";
            file << cur_akun->password << endl;
            cur_akun = cur_akun->next;
        }
    }
    // menutup file
    file.close();
}

void create_cart(cart *head) {
    fstream file;
    cart *cur_cart = head;

    file.open("cart.csv", ios::trunc | ios::out | ios::in);
    if (file.is_open()) {
        // traversal
        while (cur_cart != NULL) {
            file << cur_cart->username << ",";
            file << cur_cart->nameUser << ",";
            file << cur_cart->nameMenu << ",";
            file << cur_cart->jumlah << ",";
            file << cur_cart->total << endl;
            cur_cart = cur_cart->next;
        }
    }
    // menutup file
    file.close();
}

void create_checkout(checkout *head) {
    fstream file;
    checkout *cur_checkout = head;

    file.open("checkout.csv", ios::trunc | ios::out | ios::in);
    if (file.is_open()) {
        // traversal
        while (cur_checkout != NULL) {
            file << cur_checkout->username << ",";
            file << cur_checkout->nameUser << ",";
            file << cur_checkout->nameMenu << ",";
            file << cur_checkout->jumlah << ",";
            file << cur_checkout->total << endl;
            cur_checkout = cur_checkout->next;
        }
    }
    // menutup file
    file.close();
}

void create_order(order *head) {
    fstream file;
    order *cur_order = head;
    // membuka file untuk menulis data ke file
    file.open("orders.csv", ios::trunc | ios::out | ios::in);
    if(file.is_open()) {
        // travesal
        while (cur_order != NULL) {
            file << cur_order->id << ",";
            file << cur_order->nama << ",";
            file << cur_order->jumlah << ",";
            file << cur_order->total << endl;
            cur_order = cur_order->next;
        }
    }
    // menutup file
    file.close();
}

// mengubah text menjadi lowercase
string toLowerCase(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
}

// registrasi user
void regis(user **head) {
    string usn;
    bool loop = true;
    user *cur = *head;
    user *newUser = new user();

    system("CLS");
    cout << "\n|------------------------------------|" << endl
         << "|    ||                        ||    |" << endl
         << "|              REGISTER              |" << endl
         << "|    ----------------------------    |" << endl
         << "|     Name         : "; getline(cin, newUser->nama);
    while (loop) {
        cout << "|     Username     : "; getline(cin, usn);

        if (toLowerCase(usn) == "admin") { // jika user input username admin maka tidak bisa digunakan
            messageAlert("Username already in use");
        } else {
            while (cur != NULL) {
                if (cur->username == usn) { // cek jika username telah tersedia
                    messageAlert("Username already in use");
                    break;
                } else {
                    cur = cur->next;
                }
            }
            if (cur == NULL) { // berhasil jika username tidak ditemukan
                newUser->username = usn;
                loop = false;
                break;
            }
        }

    }

    cout << "|     Password     : "; getline(cin, newUser->password);
    user *temp = *head;
    // user *temp = *head;
    if (*head == NULL) {
        newUser->id = 1; // id = 1 jika data user kosong
        newUser->next = *head;
        *head = newUser;
        messageAlert("Data successfully added");
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        newUser->id = temp->id + 1; // id += 1 ketika data user tersedia
        temp->next = newUser;
        messageAlert("Data successfully added");
    }
}

void mainMenuAdm();
void loginAdm() {
    int loginAttempt = 1;
    string inputUsn, inputPw;

    while (loginAttempt <= 3) {
        system("CLS");
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|             ADMIN PAGE             |" << endl
             << "|    ----------------------------    |" << endl
             << "|     Username : "; getline(cin, inputUsn);
        cout << "|     Password : "; getline(cin, inputPw);
        cout << "|------------------------------------|" << endl;

        if (inputUsn == admUsn && inputPw == admPw) {
            mainMenuAdm();
            break;
        } else {
            if (loginAttempt < 3) {
                messageAlert("Incorrect username or password. Please try again");
                getch();
            }
        }

        if (loginAttempt == 3) { // ketika gagal 3 kali
            messageAlert("Failed three times, automatically return to menu");
        }
        loginAttempt++;
    }
}

void menuUser(user *pengguna);
void loginUser(user *head) {
    int loginAttempt = 1;
    string inputUsn, inputPw;
    bool ada = false;

    while (loginAttempt <= 3) {
        user *cur = head;
        system("CLS");
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|              USER PAGE             |" << endl
             << "|    ----------------------------    |" << endl
             << "|     Username : "; getline(cin, inputUsn);
        cout << "|     Password : "; getline(cin, inputPw);
        cout << "|------------------------------------|" << endl;

        while (cur != NULL) {
            ada = true;
            if (cur->username == inputUsn && cur->password == inputPw) {
                menuUser(cur);
                loginAttempt = 4;
                break;
            } else {
                cur = cur->next;
            }

            if (cur == NULL && loginAttempt < 3) {
                messageAlert("Incorrect username or password. Please try again");
                break;
            }
        }

        if (loginAttempt == 3) {
             messageAlert("Failed three times, automatically return to menu");
        }
        if (!ada && loginAttempt < 3) {
            messageAlert("Incorrect username or password. Please try again");
        }
        loginAttempt++;
    }
}

void jenisOrderMenu(string username, string nameUser, string jenis) {
    string pilorder;
    while (true) {
        system("CLS");
        cout << "\nOrder Menu\n";
        cout << "[Tekan 0 untuk kembali]\n\n";
        int ind = 0;
        menu *tempmenu = headmenu;
        while (tempmenu != NULL) {
            if (tempmenu->jenis == jenis) { // menampilkan menu sesuai jenisnya
                ind++;
                cout << "\n|--------------------------------------|" << endl
                     << "| ~                Menu              ~ |" << endl
                     << "  [ " << ind << " ]" << endl
                     << "  " << tempmenu->nama << endl
                     << "  Rp " << tempmenu->harga << endl
                     << "|--------------------------------------|" << endl;
            }
            tempmenu = tempmenu->next;
        }
        if (ind == 0) {
            messageAlert("Data not found");
            break;
        } else {
            cout << "~> Select menu : "; getline(cin, pilorder);
            if (!isNum(pilorder)) {
                messageAlert("Invalid input");
            } else {
                int pilorderint = stoi(pilorder);
                if (pilorderint < 0 || pilorderint > ind) {
                    messageAlert("Data not found");
                } else if (pilorderint == 0) {
                    break;
                } else {
                    string jumlah;
                    int index = 0;
                    menu *tempmenu = headmenu;
                    while (index != pilorderint) {
                        if (tempmenu->jenis == jenis) {
                            index += 1;
                            if (index != pilorderint) {
                                tempmenu = tempmenu->next;
                            }
                        } else {
                            tempmenu = tempmenu->next;
                        }
                    }
                    cout << "~> Enter amount : "; getline(cin, jumlah);
                    if (!isNum(jumlah)) {
                        messageAlert("Invalid input");
                    } else {
                        cart *newCart = new cart;
                        cart *tempcart = headcart;
                        newCart->username = username;
                        newCart->nameUser = nameUser;
                        newCart->nameMenu = tempmenu->nama;
                        newCart->jumlah = stoi(jumlah);
                        newCart->total = (tempmenu->harga * stoi(jumlah));
                        if (headcart == NULL) {
                            newCart->next = headcart;
                            headcart = newCart;
                            messageAlert("Succesfully added order");
                            break;
                        } else {
                            while (tempcart->next != NULL) {
                                tempcart = tempcart->next;
                            }
                            tempcart->next = newCart;
                            messageAlert("Succesfully added order");
                            break;
                        }
                    }
                }
            }
        }
    }
}

void userOrderMenu(cart **headcart, menu *headmenu, order **headorder, order **tailorder, string username, string nameUser) {
    string pilmenuorder;
    while (true) {
        system("CLS");
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|            Category Menu           |" << endl
             << "|    ----------------------------    |" << endl
             << "|                                    |" << endl
             << "|    [1] Main Dish                   |" << endl
             << "|    [2] Side Dish                   |" << endl
             << "|    [3] Drinks                      |" << endl
             << "|    [0] Back                        |" << endl
             << "|------------------------------------|" << endl
             << "| ~> Enter your choice : "; getline(cin, pilmenuorder);
        cout << "|------------------------------------|" << endl;
        if (!isNum(pilmenuorder)) {
            messageAlert("Invalid input");
        } else {
            int pilmenuorderint = stoi(pilmenuorder);
            if (pilmenuorderint == 1) {
                jenisOrderMenu(username, nameUser, "Main Dish");
            } else if (pilmenuorderint == 2) {
                jenisOrderMenu(username, nameUser, "Side Dish");
            } else if (pilmenuorderint == 3) {
                jenisOrderMenu(username, nameUser, "Drinks");
            } else if (pilmenuorderint == 0) {
                break;
            } else {
                messageAlert("Option doesn't exist");
            }
        }
    }
}

void displayCartUser(cart *head, string username) {
    if (head == NULL) {
        messageAlert("Data not found");
    } else {
        cart *temp = head;
        int i = 0;
        cout << "\n|-------------------------------------------------------------------------------------------------------------|" << endl
             << "|    ||                                                                                                 ||    |" << endl
             << "|                                                  Cart Order                                                 |" << endl
             << "|    -----------------------------------------------------------------------------------------------------    |" << endl
             << "|                                                                                                             |" << endl
             << "| No. |     Name     |      Username       |           Menu           |   Amount   |          Total           |" << endl
             << "|-------------------------------------------------------------------------------------------------------------|" << endl;
        while (temp != NULL) {
            if (temp->username == username) {
                i++;
                cout << "| " << setiosflags(ios::left) << setw(4) << i << "|" 
                     << setiosflags(ios::left) << setw(14) << temp->nameUser << "|" 
                     << setw(21) << temp->username << "|" 
                     << setw(26) << temp->nameMenu << "|" 
                     << setw(12) << temp->jumlah<< "|" 
                     << setw(23) << "Rp " << temp->total<< "|" << endl
                     << "|------------------------------------------------------------------------------------------|" << endl;
            }
            temp = temp->next;
        }
        getch();
    }
}

void checkoutOrderUser(cart **headCart, checkout **headCo, string username, string nameUser) {
    bool co;
    while (true) {
        system("CLS");
        cout << "Checkout Order" << endl;
        cart *temp = *headCart;
        int iterasi = 0;
        string nomorco;
        cout << "\n|-----------------------------------------------------------------------|" << endl
             << "|    ||                                                           ||    |" << endl
             << "|                              Checkout Order                           |" << endl
             << "|    ---------------------------------------------------------------    |" << endl
             << "|                                                                       |" << endl
             << "| No. |           Menu           |     Amount     |        Total        |" << endl
             << "|-----------------------------------------------------------------------|" << endl;
        while (temp != NULL) {
            if (temp->username == username) {
                iterasi++;
                cout << "| " << setiosflags(ios::left) << setw(4) << iterasi << "|"
                     << setiosflags(ios::left) << setw(26) << temp->nameMenu << "|"
                     << setw(16) << temp->jumlah << "|"
                     << setw(18) << "Rp " << temp->total << "|" << endl
                     << "|-----------------------------------------------------------------------|" << endl;
            }
            temp = temp->next;
        }
        if (iterasi == 0) {
            messageAlert("Data not found");
            break;
        } else {
            cout << "[ Tekan 0 untuk kembali ]\nCheckout Nomor Berapa? "; getline(cin, nomorco);
            if (!isNum(nomorco)) {
                messageAlert("Invalid input");
            } else {
                int nomorcoint = stoi(nomorco);
                cart *tempCart = *headCart;
                if (nomorcoint < 0 || nomorcoint > iterasi) {
                    messageAlert("Data not found");
                } else if (nomorcoint == 0) {
                    break;
                } else {
                    int tanda = 0;
                    while (tanda != nomorcoint) {
                        if (tempCart->username == username) {
                            tanda += 1;
                            if (tanda != nomorcoint) {
                                tempCart = tempCart->next;
                            }
                        } else {
                            tempCart = tempCart->next;
                        }
                    }
                    while (true) {
                        checkout *newCo = new checkout;
                        newCo->nameMenu = tempCart->nameMenu;
                        newCo->nameUser = tempCart->nameUser;
                        newCo->total = tempCart->total;
                        newCo->jumlah = tempCart->jumlah;
                        newCo->status = "Waiting";
                        newCo->username = tempCart->username;
                        if (*headCo == NULL) {
                            newCo->next = *headCo;
                            *headCo = newCo;
                            co = true;
                        } else {
                            checkout *tempCo = *headCo;
                            while (tempCo->next != NULL) {
                                tempCo = tempCo->next;
                            }
                            tempCo->next = newCo;
                            co = true;
                        }

                        // delete cart
                        if (tempCart == *headCart) {
                            *headCart = (*headCart)->next;
                        } else {
                            tanda = 0;
                            cart *temp = *headCart;
                            while (tanda != nomorcoint) {
                                if (temp->username == username) {
                                    tanda += 1;
                                } else {
                                    temp = temp->next;
                                }
                            }
                            temp->next = tempCart->next;
                        }
                        delete tempCart;
                        break;
                    }
                    if (co) {
                        messageAlert("Checkout successful");
                        break;
                    }
                }
            }
        }
    }
}

// cek apakah checkout kosong berdasarkan username
bool isCOEmptyUsn(checkout *head, string username) {
    checkout *temp = head;
    while (temp != NULL) {
        if (temp->username == username) {
            return false;
        } else {
            temp = temp->next;
        }
    }
    return true;
}

// menampilkan data checkout sesuai username
void displayCheckoutUser(checkout *head, string username) {
    if (head == NULL) {
        messageAlert("Data not found");
    } else {
        checkout *temp = head;
        int i = 0;
        cout << "\n|------------------------------------------------------------------------------------------|" << endl
             << "|    ||                                                                              ||    |" << endl
             << "|                                        Order Status                                      |" << endl
             << "|    ----------------------------------------------------------------------------------    |" << endl
             << "|                                                                                          |" << endl
             << "| No. |           Menu           |     Amount     |        Total        |      Status      |" << endl
             << "|------------------------------------------------------------------------------------------|" << endl;
        while (temp != NULL) {
            if (temp->username == username) {
                i++;
                cout << "| " << setiosflags(ios::left) << setw(4) << i << "|" 
                     << setiosflags(ios::left) << setw(26) << temp->nameMenu << "|" 
                     << setw(22) << temp->jumlah << "|" 
                     << setw(18) << "Rp " << temp->total << "|" 
                     << setw(18) << temp->status<< "|" << endl
                     << "|------------------------------------------------------------------------------------------|" << endl;
            }
            temp = temp->next;
        }
        getch();
        if (i == 0) {
            messageAlert("Data not found");
        }
    }
}
// cek apakah order kosong berdasarkan username
bool isOrderEmptyUsn(order *head, string username) {
    order *temp = head;
    while (temp != NULL) {
        if (temp->username == username) {
            return false;
        } else {
            temp = temp->next;
        }
    }
    return true;
}
// menampilkan riwayat order berdasarkan username
void historyOrderUser(order *head, string username) {
    if (head == NULL) {
        messageAlert("Data not found");
    } else {
        order *temp = head;
        int i = 0;
        cout << "\n|-------------------------------------------------------------------------------------------------------------|" << endl
             << "|    ||                                                                                                 ||    |" << endl
             << "|                                                  History Order                                              |" << endl
             << "|    -----------------------------------------------------------------------------------------------------    |" << endl
             << "|                                                                                                             |" << endl
             << "| No. |     Name     |      Username       |           Menu           |   Amount   |          Total           |" << endl
             << "|-------------------------------------------------------------------------------------------------------------|" << endl;
        while (temp != NULL) {
            if (temp->username == username) {
                i++;
                cout << "| " << setiosflags(ios::left) << setw(4) << i << "|" 
                     << setiosflags(ios::left) << setw(14) << temp->nama << "|" 
                     << setw(21) << temp->username << "|" 
                     << setw(26) << temp->menu << "|"  
                     << setw(12) << temp->jumlah<< "|"  
                     << setw(23) << "Rp " << temp->total<< "|" << endl
                     << "|------------------------------------------------------------------------------------------|" << endl;
                temp = temp->next;
            }
        }
        if (i == 0) {
            messageAlert("Data not found");
        } else {
            getch();
        }
    }
}
// cek apakah keranjang kosong berdasarkan username
bool isCartEmptyUsn(cart *head, string username) {
    cart *temp = head;
    while (temp != NULL) {
        if (temp->username == username) {
            return false;
        } else {
            temp = temp->next;
        }
    }
    return true;
}

void menuUser(user *pengguna) {
    string pilMenuUser;
    while (true) {
        system("CLS");
        cout << "\nHello, " << pengguna->nama << endl;
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|        Welcome to Margamedon       |" << endl
             << "|    ----------------------------    |" << endl
             << "|                                    |" << endl
             << "|    [1] Order Menu                  |" << endl
             << "|    [2] Show Cart                   |" << endl
             << "|    [3] Checkout Order              |" << endl
             << "|    [4] View Order Status           |" << endl
             << "|    [5] View Order History          |" << endl
             << "|    [0] Log Out                     |" << endl
             << "|------------------------------------|" << endl
             << "| ~> Enter your choice : "; getline(cin, pilMenuUser);
        cout << "|------------------------------------|" << endl;
        if (!isNum(pilMenuUser)) {
            messageAlert("Invalid input");
        } else {
            int pilMenuUserInt = stoi(pilMenuUser);
            if (pilMenuUserInt == 1) {
                userOrderMenu(&headcart, headmenu, &headorder, &tailorder, pengguna->username, pengguna->nama);
            } else if (pilMenuUserInt == 2) {
                system("CLS");
                if (isCartEmptyUsn(headcart, pengguna->username)) {
                    messageAlert("Data not found");
                } else {
                    displayCartUser(headcart, pengguna->username);
                }
            } else if (pilMenuUserInt == 3) {
                checkoutOrderUser(&headcart, &headco, pengguna->username, pengguna->nama);
            } else if (pilMenuUserInt == 4) {
                system("CLS");
                if (isCOEmptyUsn(headco, pengguna->username)) {
                    messageAlert("Data not found");
                } else {
                    displayCheckoutUser(headco, pengguna->username);
                }
            } else if (pilMenuUserInt == 5) {
                system("CLS");
                if (isOrderEmptyUsn(headorder, pengguna->username)) {
                    messageAlert("Data not found");
                } else {
                    historyOrderUser(headorder, pengguna->username);
                }
            } else if (pilMenuUserInt == 0) {
                break;
            } else {
                messageAlert("Option doesn't exist");
            }
        }
    }
}

void menuAdm();
void orderAdm();
void userAdm(user *head);
void mainMenuAdm() {
    string pil;
    while (true) {
        system("CLS");
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|             ADMIN MENU             |" << endl
             << "|    ----------------------------    |" << endl
             << "|                                    |" << endl
             << "|    [1] Menu                        |" << endl
             << "|    [2] Order                       |" << endl
             << "|    [3] Display Users               |" << endl
             << "|    [0] Log Out                     |" << endl
             << "|------------------------------------|" << endl
             << "| ~> Enter your choice : "; getline(cin, pil);
        cout << "|------------------------------------|" << endl;
        if (!isNum(pil)) {
            messageAlert("Invalid Input");
        } else {
            int pilInt = stoi(pil);
            if (pilInt == 1) {
                menuAdm();
            } else if (pilInt == 2) {
                orderAdm();
            } else if (pilInt == 3) {
                userAdm(headuser);
            } else if (pilInt == 0) {
                break;
            } else {
                messageAlert("Option doesn't exist");
            }
        }
    }
}

menu *newNode() {
    string jenis, harga;
    menu *newMenu = new menu;
    system("CLS");
    cout << "\n|------------------------------------|" << endl
         << "|    ||                        ||    |" << endl
         << "|              Menu Name             |" << endl
         << "|    ----------------------------    |" << endl
         << "|                                    |" << endl
         << "| ~> Enter name here : "; getline(cin, newMenu->nama);
    cout << "|------------------------------------|" << endl;
    while(true) {
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|            Category Menu           |" << endl
             << "|    ----------------------------    |" << endl
             << "|                                    |" << endl
             << "|    [1] Main Dish                   |" << endl
             << "|    [2] Side Dish                   |" << endl
             << "|    [3] Drinks                      |" << endl
             << "|------------------------------------|" << endl
             << "| ~> Enter your choice : "; getline(cin, jenis);
        cout << "|------------------------------------|" << endl;
        if (!isNum(jenis)) {
            messageAlert("Invalid input");
        } else {
            int jenisInt = stoi(jenis);
            if (jenisInt == 1) {
                newMenu->jenis = "Main Dish";
                break;
            } else if (jenisInt == 2) {
                newMenu->jenis = "Side Dish";
                break;
            } else if (jenisInt == 3) {
                newMenu->jenis = "Drinks";
                break;
            } else {
                messageAlert("Option doesn't exist");
            }
        }
    }
    while(true) {
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|             Menu Price             |" << endl
             << "|    ----------------------------    |" << endl
             << "|                                    |" << endl
             << "| ~> Enter price here : "; getline(cin, harga);
        cout << "|------------------------------------|" << endl;
        if (!isNum(harga)) {
            messageAlert("Invalid input");
        } else {
            newMenu->harga = stold(harga);
            break;
        }
    }
    return newMenu;
}

void addMenu(menu **head) {
    menu *newMenu = newNode();
    menu *temp = *head;
    if (*head == NULL) {
        newMenu->id = 1; // id menu = 1 ketika data menu kosong
        newMenu->next = *head;
        *head = newMenu;
        messageAlert("Data successfully added");
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        newMenu->id = temp->id + 1; // id menu += 1 ketika data tersedia
        temp->next = newMenu;
        messageAlert("Data successfully added");
    }
}

// SORTING DISPLAY MENU
menu *sortMenu(menu *a, menu *b);
void FrontBackSplit(menu *source, menu **frontRef, menu **backRef);
void MergeSort(menu **headRef) {
    menu *head = *headRef;
    menu *a;
    menu *b;
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }
    FrontBackSplit(head, &a, &b);
    MergeSort(&a);
    MergeSort(&b);
    *headRef = sortMenu(a, b);
}

menu *sortMenu(menu *a, menu *b) {
    menu *result = NULL;
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);
    if (a->jenis <= b->jenis) {
        result = a;
        result->next = sortMenu(a->next, b);
    }
    else {
        result = b;
        result->next = sortMenu(a, b->next);
    }
    return (result);
}

void FrontBackSplit(menu *source, menu **frontRef, menu **backRef) {
    menu *fast;
    menu *slow;
    slow = source;
    fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}
// menampilkan seluruh menu
void displayMenu(menu *head) {
    if (head == NULL) {
        messageAlert("Data not found");
    } else {
        menu *temp = head;
        int i = 1;
        cout << "\n|------------------------------------------------------------------------------|" << endl
             << "|    ||                                                                  ||    |" << endl
             << "|                                 Display Menu                                 |" << endl
             << "|    ----------------------------------------------------------------------    |" << endl
             << "|                                                                              |" << endl
             << "| No. |           Menu           |        Category       |        Price        |" << endl
             << "|------------------------------------------------------------------------------|" << endl;
        while (temp != NULL) {
            cout << "| " << setiosflags(ios::left) << setw(4) << i << "|" 
                        << setiosflags(ios::left) << setw(26) << temp->nama << "|" 
                        << setw(22) << temp->jenis << "|" 
                        << setw(18) << "Rp " << temp->harga << "|" << endl
                 << "|------------------------------------------------------------------------------|" << endl;
            i++;
            temp = temp->next;
        }
    }
}
// cek apakah data menu kosong
bool isEmptyMenu(menu *head) {
    if (head == NULL) {
        return true;
    }
    return false;
}
// menghitung panjang/banyak data menu
int lengthMenu(menu *head) {
    int jml = 0;
    menu *temp = head;
    while (temp != NULL) {
        temp = temp->next;
        jml++;
    }
    return jml;
}
// update menu berdasarkan iterasi
void updateMenu(menu **head, int idx) {
    if (idx > lengthMenu(*head) || idx == 0) {
        messageAlert("Data not found");
    } else {
        while (true) {
            system("CLS");
            menu *temp = *head;
            string pilUpdateMenu;
            for (int i = 1; i < idx; i++) {
                temp = temp->next;
            }
            cout << "\n|--------------------------------------|" << endl
                 << "| ~                Menu              ~ |" << endl
                 << "  " << idx << endl
                 << "  Menu     : " << temp->nama << endl
                 << "  Category : " << temp->jenis << endl
                 << "  Price    : Rp " << temp->harga << endl
                 << "|--------------------------------------|" << endl;

            cout << "\n|------------------------------------|" << endl
                 << "|    ||                        ||    |" << endl
                 << "|             Update Menu            |" << endl
                 << "|    ----------------------------    |" << endl
                 << "|                                    |" << endl
                 << "|    [1] Change Menu Name            |" << endl
                 << "|    [2] Change Category Name        |" << endl
                 << "|    [3] Change Price Menu           |" << endl
                 << "|    [0] Back                        |" << endl
                 << "|------------------------------------|" << endl
                 << "| ~> Enter your choice : "; getline(cin, pilUpdateMenu);
            cout << "|------------------------------------|" << endl;

            if (!isNum(pilUpdateMenu)) {
                messageAlert("Please enter number");
            } else {
                int pilUpdateMenuInt = stoi(pilUpdateMenu);
                if (pilUpdateMenuInt == 1) {
                    cout << "\n|----------------------------------------|" << endl
                         << "|    ||                            ||    |" << endl
                         << "|             Change Name Menu           |" << endl
                         << "|    --------------------------------    |" << endl
                         << "|                                        |" << endl
                         << "| ~> Name Menu   : " << temp->nama << endl
                         << "| ~> Change Into : "; getline(cin, temp->nama);
                    cout << "|----------------------------------------|" << endl;
                    messageAlert("Data successfully changed");
                    break;
                } else if (pilUpdateMenuInt == 2) {
                    string jenisMenu;
                    cout << "\n|----------------------------------------|" << endl
                         << "|    ||                            ||    |" << endl
                         << "|           Change Category Menu         |" << endl
                         << "|    --------------------------------    |" << endl
                         << "|                                        |" << endl
                         << "| ~> Category Menu : " << temp->jenis << endl;
                    cout << "|----------------------------------------|" << endl;
                    system("CLS");
                    while (true) {
                        cout << "\n|------------------------------------|" << endl
                             << "| >  Change Into :                 < |" << endl
                             << "|                                    |" << endl
                             << "|    [1] Main Dish                   |" << endl
                             << "|    [2] Side Dish                   |" << endl
                             << "|    [3] Drinks                      |" << endl
                             << "|------------------------------------|" << endl
                             << "| ~> Enter your choice : "; getline(cin, jenisMenu);
                        cout << "|------------------------------------|" << endl;
                        if (!isNum(jenisMenu)) {
                            messageAlert("Please enter number");
                            cout << "\n\n";
                        } else {
                            int jenisMenuInt = stoi(jenisMenu);
                            if (jenisMenuInt == 1) {
                                temp->jenis = "Main Dish";
                                break;
                            } else if (jenisMenuInt == 2) {
                                temp->jenis = "Side Dish";
                                break;
                            } else if (jenisMenuInt == 3) {
                                temp->jenis = "Drinks";
                                break;
                            } else {
                                messageAlert("No option");
                                cout << "\n\n";
                            }
                        }
                    }
                    messageAlert("Data successfully changed");
                    break;
                } else if (pilUpdateMenuInt == 3) {
                    cout << "\n|----------------------------------------|" << endl
                         << "|    ||                            ||    |" << endl
                         << "|            Change Price Menu           |" << endl
                         << "|    --------------------------------    |" << endl
                         << "|                                        |" << endl
                         << "| ~> Price Menu  : " << temp->harga << endl;
                    while (true) {
                        string hargaMenu;
                        cout << "| ~> Change Into : "; getline(cin, hargaMenu);
                        if (!isNum(hargaMenu)) {
                            messageAlert("Please enter number");
                        } else {
                            temp->harga = stold(hargaMenu);
                            break;
                        }
                    }
                    messageAlert("Data successfully changed");
                    break;
                } else if (pilUpdateMenuInt == 0) {
                    break;
                } else {
                    messageAlert("Option doesn't exist");
                }
            }
        }
    }
}
// menghapus menu berdasarkan iterasi
void delMenu(menu **head, int idx) {
    if (idx == 0 || idx > lengthMenu(*head)) {
        messageAlert("Data not found");
    } else {
        system("CLS");
        if (idx == 1) {
            cout << "\n|------------------------------------|" << endl
                 << "| ~           Delete Menu          ~ |" << endl
                 << "  " << idx << endl
                 << "  Menu     : " << (*head)->nama << endl
                 << "  Category : " << (*head)->jenis << endl
                 << "  Price    : Rp " << (*head)->harga << endl
                 << "|------------------------------------|" << endl;
            while (true) {
                string confirm;
                cout << "\n Are you sure to delete this data? (y/n): "; getline(cin, confirm);
                if (confirm == "Y" || confirm == "y") {
                    menu *del = *head;
                    *head = (*head)->next;
                    delete del;
                    messageAlert("Data successfully deleted");
                    break;
                } else if (confirm == "N" || confirm == "n") {
                    messageAlert("Data failed to be deleted, return to menu");
                    break;
                } else {
                    messageAlert("Please enter Y or N");
                    cout << "\n\n";
                }
            }
        } else {
            menu *temp = *head;
            for (int i = 1; i < idx-1; i++) {
                temp = temp->next;
            }
            cout << "\n|------------------------------------|" << endl
                 << "| ~           Delete Menu          ~ |" << endl
                 << "  " << idx << endl
                 << "  Menu     : " << (*head)->nama << endl
                 << "  Category : " << (*head)->jenis << endl
                 << "  Price    : Rp " << (*head)->harga << endl
                 << "|------------------------------------|" << endl;
            while (true) {
                string confirm;
                cout << "\n Are you sure to delete this data? (y/n): "; getline(cin, confirm);
                if (confirm == "Y" || confirm == "y") {
                    menu *del = temp->next;
                    temp->next = temp->next->next;
                    delete del;
                    messageAlert("Data successfully deleted");
                    break;
                } else if (confirm == "N" || confirm == "n") {
                    messageAlert("Data failed to be deleted, return to menu");
                    break;
                } else {
                    messageAlert("Please enter Y or N");
                    cout << "\n\n";
                }
            }
        }
    }
}

// SEARCHING MENU
const int NO_OF_CHARS = 256;
void badCharHeuristic(string str, int size, int badChar[NO_OF_CHARS])
{
    for (int i = 0; i < NO_OF_CHARS; i++)
    {
        badChar[i] = -1;
    }

    for (int i = 0; i < size; i++)
    {
        badChar[(int)str[i]] = i;
    }
}
void BoyerMoore(menu *head, string pat)
{
    int m = pat.size();
    menu *temp = head;
    bool found = false;
    while (temp != NULL)
    {
        int n = temp->nama.size();
        int badChar[NO_OF_CHARS];
        badCharHeuristic(pat, m, badChar);
        int s = 0;

        while (s <= (n - m))
        {
            int j = m - 1;

            while (j >= 0 && pat[j] == temp->nama[s + j])
            {
                j--;
            }
            if (j < 0)
            {
                cout << "\n|---------------------------------------|" << endl
                     << "|    ||                            ||   |" << endl
                     << "|                  Result               |" << endl
                     << "|    --------------------------------   |" << endl
                     << "|                                       |" << endl
                     << "     [1] Menu     : " << temp->nama  << endl
                     << "     [2] Category : " << temp->jenis << endl
                     << "     [3] Price    : " << temp->harga << endl
                     << "|---------------------------------------|" << endl;
                cout << "\n";
                found = true;
                break;
            }
            else
            {
                s += max(1, j - badChar[temp->nama[s + j]]);
            }
        }
        temp = temp->next;
    }
    if (!found) {
        cout << "\n Data not found" << endl;
    }
}

void menuAdm() {
    string pilMenuAdm;
    while (true) {
        system("CLS");
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|             Menu Admin             |" << endl
             << "|    ----------------------------    |" << endl
             << "|                                    |" << endl
             << "|    [1] Add Menu                    |" << endl
             << "|    [2] Display Menu                |" << endl
             << "|    [3] Update Menu                 |" << endl
             << "|    [4] Delete Menu                 |" << endl
             << "|    [5] Search Menu                 |" << endl
             << "|    [0] Back                        |" << endl
             << "|------------------------------------|" << endl
             << "| ~> Enter your choice : "; getline(cin, pilMenuAdm);
        cout << "|------------------------------------|" << endl;

        if (!isNum(pilMenuAdm)) {
            messageAlert("Invalid Input");
        } else {
            int pilMenuAdmInt = stoi(pilMenuAdm);
            if (pilMenuAdmInt == 1) {
                system("CLS");
                addMenu(&headmenu);
            } else if (pilMenuAdmInt == 2) {
                system("CLS");
                if (isEmptyMenu(headmenu)) {
                    messageAlert("Data not found");
                } else {
                    MergeSort(&headmenu);
                    displayMenu(headmenu);
                    getch();
                }
            } else if (pilMenuAdmInt == 3) {
                while (true) {
                    string idx;
                    system("CLS");
                    if (!isEmptyMenu(headmenu)) {
                        system("CLS");
                        displayMenu(headmenu);
                        cout << "\n Ubah data di nomor ke berapa? "; getline(cin, idx);
                        if (!isNum(idx)) {
                            messageAlert("Please enter number");
                        } else {
                            updateMenu(&headmenu, stoi(idx));
                            break;
                        }
                    } else {
                        messageAlert("Data not found");
                        break;
                    }
                }
            } else if (pilMenuAdmInt == 4) {
                while (true) {
                    string idx;
                    system("CLS");
                    if (!isEmptyMenu(headmenu)) {
                        system("CLS");
                        displayMenu(headmenu);
                        cout << "\n Hapus data di nomor ke berapa? "; getline(cin, idx);
                        if (!isNum(idx)) {
                            messageAlert("Please enter number");
                        } else {
                            delMenu(&headmenu, stoi(idx));
                            break;
                        }
                    } else {
                        messageAlert("Data not found");
                        break;
                    }
                }
            } else if (pilMenuAdmInt == 5) {
                system("CLS");
                string pattern;
                cout << "\n|------------------------------------|" << endl
                     << "|    ||                        ||    |" << endl
                     << "|             Search Menu            |" << endl
                     << "|    ----------------------------    |" << endl
                     << "|                                    |" << endl
                     << "|    Enter Keyword : "; getline(cin, pattern);
                BoyerMoore(headmenu, pattern);
                getch();
            } else if (pilMenuAdmInt == 0) {
                break;
            } else {
                messageAlert("Option doesn't exist");
            }
        }
    }
}
// menampilkan seluruh data checkout
void displayCheckout(checkout *head) {
    if (head == NULL) {
        messageAlert("Data not found");
    } else {
        checkout *temp = head;
        int i = 1;
        cout << "\n|-----------------------------------------------------------------------------------------------------------|" << endl
             << "|    ||                                                                                               ||    |" << endl
             << "|                                            Display Checkout                                               |" << endl
             << "|    ---------------------------------------------------------------------------------------------------    |" << endl
             << "| No. |    Username    |      Name      |      Menu      |   Amount   |        Total         |    Status    |" << endl
             << "|-----------------------------------------------------------------------------------------------------------|" << endl;
        while (temp != NULL) {
            cout << "| " << setiosflags(ios::left) << setw(5) << i << "|"
                         << setiosflags(ios::left) << setw(16) << temp->username << "|"
                         << setiosflags(ios::left) << setw(16) << temp->nameUser << "|"
                         << setiosflags(ios::left) << setw(16) << temp->nameMenu << "|"
                         << setiosflags(ios::left) << setw(12) << temp->jumlah << "|"
                         << setiosflags(ios::left) << setw(22) << temp->total << "|"
                         << setiosflags(ios::left) << setw(14) << temp->status << "|" << endl;
            cout << "|-----------------------------------------------------------------------------------------------------------|" << endl;
            i++;
            temp = temp->next;
        }
    }
}
// cek apakah checkout kosong
bool isCOEmpty(checkout *head) {
    if (head == NULL) {
        return true;
    }
    return false;
}
// menghitung panjang/banyak order
int lengthOrder(order *head) {
    int jml = 0;
    order *temp = head;
    while (temp != NULL)
    {
        temp = temp->next;
        jml++;
    }
    return jml;
}
// konfirmasi pesanan
void confirmOrder(checkout **headCo, order **headOrder, order **tailOrder) {
    if (*headCo == NULL) {
        messageAlert("Data not found");
    } else {
        checkout *temp = *headCo;
        string confirm;
        bool back;
        while (temp != NULL) {
            while (true) {
                system("CLS");
                cout << "\n|-----------------------------------------------------------------------------------------------------|" << endl
                     << "|    ||                                                                                         ||    |" << endl
                     << "|                                            Confirm Order                                            |" << endl
                     << "|    ---------------------------------------------------------------------------------------------    |" << endl
                     << "|    Username    |      Name      |      Menu      |   Amount   |        Total         |    Status    |" << endl;
                cout << "| " << setiosflags(ios::left) << setw(16) << temp->username << "|"
                             << setiosflags(ios::left) << setw(16) << temp->nameUser << "|"
                             << setiosflags(ios::left) << setw(16) << temp->nameMenu << "|"
                             << setiosflags(ios::left) << setw(12) << temp->jumlah << "|"
                             << setiosflags(ios::left) << setw(22) << temp->total << "|"
                             << setiosflags(ios::left) << setw(14) << temp->status << "|" << endl;
                cout << "|-----------------------------------------------------------------------------------------------------|" << endl;
                cout << "\n Input 0 to return" << endl;
                cout << "Confirm this order? (y/n) : "; getline(cin, confirm);
                if (confirm == "Y" || confirm == "y") {
                    checkout *del = temp;
                    *headCo = (*headCo)->next;

                    order *newOrder = new order;
                    order *tempOrder = *headOrder;
                    newOrder->nama = temp->nameUser;
                    newOrder->username = temp->username;
                    newOrder->menu = temp->nameMenu;
                    newOrder->jumlah = temp->jumlah;
                    newOrder->total = temp->total;

                    if (*headOrder == NULL) {
                        // cout << "Masuk" << endl;
                        newOrder->id = 1;
                        *headOrder = newOrder;
                        *tailOrder = newOrder;
                        back = true;
                        messageAlert("Order successfully confirm");
                    } else {
                        newOrder->id = (*tailOrder)->id + 1;
                        (*tailOrder)->next = newOrder;
                        newOrder->prev = *tailOrder;
                        *tailOrder = newOrder;
                        messageAlert("Order successfully confirm");
                        back = true;
                    }

                    delete del;
                    break;
                } else if (confirm == "0") {
                    back = true;
                    break;
                } else {
                    messageAlert("Invalid input");
                }
            }
            if (back) {
                break;
            }
        }
    }
}
// menampilkan seluruh riwayat pesanan secara asc maupun desc
void orderHistory(order *head, order *tail) {
    if (head == NULL) {
        messageAlert("Data not found");
    } else {
        string pilmenu;
        while (true) {
            cout << "\n|------------------------------------|" << endl
                 << "|    ||                        ||    |" << endl
                 << "|             Menu Admin             |" << endl
                 << "|    ----------------------------    |" << endl
                 << "|                                    |" << endl
                 << "|    [1] Ascending                   |" << endl
                 << "|    [2] Descending                  |" << endl
                 << "|    [0] Back                        |" << endl
                 << "|------------------------------------|" << endl
                 << "| ~> Enter your choice : "; getline(cin, pilmenu);
            cout << "|------------------------------------|" << endl;
            if (!isNum(pilmenu)) {
                messageAlert("Invalid input");
            } else {
                int pilmenuint = stoi(pilmenu);
                if (pilmenuint == 1) {
                    order *temp = head;
                    int i = 1;
                    cout << "\n|------------------------------------------------------------------------------------------------|" << endl
                         << "|    ||                                                                                    ||    |" << endl
                         << "|                                            Order History                                       |" << endl
                         << "|    ----------------------------------------------------------------------------------------    |" << endl
                         << "| No. |     Name      |      Username      |      Menu      |   Amount   |        Total          |" << endl
                         << "|------------------------------------------------------------------------------------------------|" << endl;
                    while (temp != NULL) {
                        cout << "| " << setiosflags(ios::left) << setw(5) << i << "|"
                             << setiosflags(ios::left) << setw(16) << temp->nama << "|"
                             << setiosflags(ios::left) << setw(20) << temp->username << "|"
                             << setiosflags(ios::left) << setw(16) << temp->menu << "|"
                             << setiosflags(ios::left) << setw(12) << temp->jumlah << "|"
                             << setiosflags(ios::left) << setw(22) << temp->total << "|" << endl;
                        cout << "|------------------------------------------------------------------------------------------------|" << endl;
                        i++;
                        temp = temp->next;
                    }
                    getch();
                } else if (pilmenuint == 2) {
                    order *temp = tail;
                    int i = 1;
                    cout << "\n|------------------------------------------------------------------------------------------------|" << endl
                         << "|    ||                                                                                    ||    |" << endl
                         << "|                                            Order History                                       |" << endl
                         << "|    ----------------------------------------------------------------------------------------    |" << endl
                         << "| No. |     Name      |      Username      |      Menu      |   Amount   |        Total          |" << endl
                         << "|------------------------------------------------------------------------------------------------|" << endl;
                    while (temp != NULL) {
                        cout << "| " << setiosflags(ios::left) << setw(5) << i << "|"
                             << setiosflags(ios::left) << setw(16) << temp->nama << "|"
                             << setiosflags(ios::left) << setw(20) << temp->username << "|"
                             << setiosflags(ios::left) << setw(16) << temp->menu << "|"
                             << setiosflags(ios::left) << setw(12) << temp->jumlah << "|"
                             << setiosflags(ios::left) << setw(22) << temp->total << "|" << endl;
                        cout << "|------------------------------------------------------------------------------------------------|" << endl;
                        i++;
                        temp = temp->prev;
                    }
                    getch();
                } else if (pilmenuint == 0) {
                    break;
                } else {
                    messageAlert("\nInvalid input");
                }
            }
        }
    }
}

void orderAdm() {
    string pilorderadm;
    while (true) {
        system("CLS");
        cout << "\n|------------------------------------|" << endl
             << "|    ||                        ||    |" << endl
             << "|               Order                |" << endl
             << "|    ----------------------------    |" << endl
             << "|                                    |" << endl
             << "|    [1] Waiting List                |" << endl
             << "|    [2] Confirm Order               |" << endl
             << "|    [3] Order History               |" << endl
             << "|    [0] Back                        |" << endl
             << "|------------------------------------|" << endl
             << "| ~> Enter your choice : "; getline(cin, pilorderadm);
        cout << "|------------------------------------|" << endl;
        if (!isNum(pilorderadm)) {
            messageAlert("Invalid input");
        } else {
            int pilorderadmint = stoi(pilorderadm);
            if (pilorderadmint == 1) {
                system("CLS");
                if (isCOEmpty(headco)) {
                    messageAlert("Data not found");
                } else {
                    displayCheckout(headco);
                    getch();
                }
            } else if (pilorderadmint == 2) {
                system("CLS");
                confirmOrder(&headco, &headorder, &tailorder);
            } else if (pilorderadmint == 3) {
                system("CLS");
                orderHistory(headorder, tailorder);
            } else if (pilorderadmint == 0) {
                break;
            } else {
                messageAlert("Invalid input");
            }
        }
    }
}

void userAdm(user *head) {
    while (true) {
        system("CLS");
        if (head == NULL) {
            messageAlert("Data not found");
            break;
        } else {
            user *temp = head;
            int i = 1;
            cout << "\n|--------------------------------------------------|" << endl
                 << "|    ||                              ||    |" << endl
                 << "|                 List User                |" << endl
                 << "|    ----------------------------------    |" << endl
                 << "| No. |     Name      |      Username      |" << endl
                 << "|------------------------------------------|" << endl;
            while (temp != NULL) {
                cout << "| " << setiosflags(ios::left) << setw(5) << i << "|"
                             << setiosflags(ios::left) << setw(16) << temp->nama << "|"
                             << setiosflags(ios::left) << setw(20) << temp->username << "|"
                     << "|--------------------------------------------------|" << endl;
                i++;
                temp = temp->next;
            }
            getch();
            break;
        }
    }
}

int main() {
    fstream file;

    read_menu(&headmenu);
    read_user(&headuser);
    read_cart(&headcart);
    read_checkout(&headco);
    read_order(&headorder, &tailorder);

    while (true) {
        int main = mainMenu();
        if (main == 1) {
            loginAdm();
        } else if (main == 2) {
            loginUser(headuser);
        } else if (main == 3) {
            regis(&headuser);
        } else if (main == 0) {
            create_menu(headmenu);
            create_user(headuser);
            create_cart(headcart);
            create_checkout(headco);
            create_order(headorder);
            break;
        } else {
            messageAlert("Option doesn't exist");
        }
    }
}