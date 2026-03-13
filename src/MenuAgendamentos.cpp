#include "../include/MenuAgendamentos.h"
#include <iostream>

using namespace std;

void MenuAgendamentos::executar() {
    int opcao;

    do {
        cout << "\n==============================\n"
             << "   BARBEARIA JP - AGENDAMENTOS\n"
             << "==============================\n"
             << "  1. Inserir agendamento\n"
             << "  2. Alterar agendamento\n"
             << "  3. Pesquisar por nome\n"
             << "  4. Remover agendamento\n"
             << "  5. Listar todos\n"
             << "  6. Exibir um agendamento\n"
             << "  7. Gerar relatorio\n"
             << "  0. Sair\n"
             << "==============================\n"
             << "  Opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1: controle.inserir(); break;
            case 2: controle.alterar(); break;
            case 3: controle.pesquisarPorNome(); break;
            case 4: controle.remover(); break;
            case 5: controle.listarTodos(); break;
            case 6: controle.exibirUm(); break;
            case 7: relatorio.gerar(); break;
            case 0: cout << "\nAte mais!\n"; break;
            default: cout << "\nOpcao invalida.\n";
        }
    } while (opcao != 0);
}
