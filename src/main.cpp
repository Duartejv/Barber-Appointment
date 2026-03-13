#include <iostream>
#include <fstream>
#include <string>
#include "../include/Conexao.h"
#include "../include/MenuAgendamentos.h"

using namespace std;

int main() {
    string dbname, user, password, host, port;

    // colocar aqui o mesmo nome do arquivo com as configuraçoes do sql
    ifstream arq("./config_exemplo.txt");
    if (!arq.is_open()) {
        cout << "Erro ao abrir config_exemplo.txt" << endl;
        return 1;
    }

    getline(arq, dbname);
    getline(arq, user);
    getline(arq, password);
    getline(arq, host);
    getline(arq, port);
    arq.close();

    cout << "Bem-vindo ao Sistema de Agendamentos da Barbearia JP!" << endl;
    cout << "\nConectando ao banco de dados PostgreSQL..." << endl;

    if (!Conexao::conectar(dbname, user, password, host, port)) {
        cout << "\nFalha na conexao. Verifique:\n"
             << "  1. PostgreSQL rodando\n"
             << "  2. Banco '" << dbname << "' existe\n"
             << "  3. Credenciais corretas\n\n"
             << "Continuar sem banco? (s/n): ";
        
        char op;
        cin >> op;
        if (op != 's' && op != 'S') {
            cout << "Encerrando.\n";
            return 1;
        }
        cout << "\nExecutando sem persistencia!\n";
    }
    
    MenuAgendamentos menu;
    menu.executar();
    
    if (Conexao::ok()) Conexao::desconectar();
    
    return 0;
}