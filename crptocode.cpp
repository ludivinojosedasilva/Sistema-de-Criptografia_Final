#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>   
#include <cctype>
#include <string>
using namespace std;

const int SHIFT = 5;
const string TABLE_FILE = "encrypted_table.txt";

using Tuple = tuple<char, char, int>; 


int simpleEncrypt(int value) {
    return (value + 5) % 256;
}
int simpleDecrypt(int value) {
    return (value - 5 + 256) % 256;
}


vector<Tuple> generateTable() {
    vector<Tuple> table;

    for (char c = 'A'; c <= 'Z'; ++c) {
        char sub = 'A' + (c - 'A' + SHIFT) % 26;
        table.emplace_back(c, sub, static_cast<int>(sub));
    }
    for (char c = 'a'; c <= 'z'; ++c) {
        char sub = 'a' + (c - 'a' + SHIFT) % 26;
        table.emplace_back(c, sub, static_cast<int>(sub));
    }

    return table;
}


void saveEncryptedTable(const vector<Tuple>& table) {
    ofstream out(TABLE_FILE);
    for (const auto& t : table) {
        char original, encoded;
        int ascii;
        tie(original, encoded, ascii) = t;
        out << original << ' ' << encoded << ' ' << simpleEncrypt(ascii) << '\n';
    }
    out.close();
}


vector<Tuple> loadTable(bool reverse = false) {
    vector<Tuple> table;
    ifstream in(TABLE_FILE);
    char a, b;
    int c;

    while (in >> a >> b >> c) {
        char decoded = reverse ? a : b;
        char encoded = reverse ? b : a;
        int ascii = simpleDecrypt(c);
        table.emplace_back(encoded, decoded, ascii);
    }

    in.close();
    return table;
}

string substituteText(const string& input, const vector<Tuple>& table) {
    string output = input;
    for (size_t i = 0; i < output.length(); ++i) {
        for (const auto& t : table) {
            if (output[i] == get<0>(t)) {
                output[i] = get<1>(t);
                break;
            }
        }
    }
    return output;
}

void showTable() {
    auto table = loadTable(false);
    cout << "\nTabela de Substituição:\n------------------------\n";
    for (const auto& t : table) {
        cout << get<0>(t) << " -> " << get<1>(t) << " | ASCII codificado: " << get<2>(t) << '\n';
    }
}

int main() {
    vector<Tuple> table = generateTable();
    saveEncryptedTable(table);

    int option;
    string input;

    do {
        cout << "\n\t\t=== UFSC - PROGRAMAÇÃO II ===\n";
        cout << "\n=== MENU ===\n";
        cout << "Escolha uma opção:\n";
        cout << "1. Criptografar texto\n";
        cout << "2. Descriptografar texto\n";
        cout << "3. Visualizar tabela\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1:
                cout << "Digite o texto que deseja criptografar:\n> ";
                getline(cin, input);
                cout << "Texto Criptografado:\n" << substituteText(input, loadTable(false)) << '\n';
                break;
            case 2:
                cout << "Digite o texto a ser descriptografado:\n> ";
                getline(cin, input);
                cout << "Texto Descriptografado:\n" << substituteText(input, loadTable(true)) << '\n';
                break;
            case 3:
                showTable();
                break;
            case 0:
                cout << "Fechando...\n";
                break;
            default:
                cout << "Opção inválida!\n";
        }

    } while (option != 0);

    return 0;
}


