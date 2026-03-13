#include "../include/RelatorioAgendamentos.h"
#include "../include/Conexao.h"
#include "../include/Consultas.h"
#include "../include/DadosBarbearia.h"
#include <iostream>
#include <iomanip>

using namespace std;

// caso o banco ainda nao esteja conectado
void RelatorioAgendamentos::gerar() const {
    if (!Conexao::ok()) {
        cout << "Erro: Banco nao conectado.\n";
        return;
    }

    auto agendamentos = Consultas::listarTodos();
    size_t total = agendamentos.size();

    cout << "\n============================================\n"
         << "       RELATORIO - BARBEARIA JP\n"
         << "============================================\n"
         << "Total de agendamentos: " << total << "\n\n";

    if (total == 0) {
        cout << "Nenhum dado para relatorio.\n";
        return;
    }

    // valor total dos agendamentos
    double valorTotal = Consultas::getValorTotal();
    cout << fixed << setprecision(2)
         << "Valor total:  R$ " << valorTotal << "\n"
         << "Valor medio:  R$ " << (valorTotal / total) << "\n";

    // valor por servico prestado
    auto contagemServ = Consultas::contagemPor("servico");
    auto valorServ = Consultas::valorPor("servico");
    
    cout << "\n--- Por Servico ---\n";
    for (const auto& [serv, qtd] : contagemServ)
        cout << "  " << left << setw(18) << serv 
             << " | Qtd: " << setw(3) << qtd 
             << " | R$ " << fixed << setprecision(2) << valorServ[serv] << "\n";

    // valor por barbeiro 
    auto contagemBarb = Consultas::contagemPor("barbeiro");
    auto valorBarb = Consultas::valorPor("barbeiro");
    
    cout << "\n--- Por Barbeiro ---\n";
    for (const auto& [barb, qtd] : contagemBarb)
        cout << "  " << left << setw(18) << barb 
             << " | Atend: " << setw(3) << qtd 
             << " | R$ " << fixed << setprecision(2) << valorBarb[barb] << "\n";

    // valor por dia
    auto contagemDia = Consultas::contagemPor("dia");
    
    cout << "\n--- Por Dia ---\n";
    for (int i = 0; i < TOTAL_DIAS; i++) {
        int qtd = contagemDia.count(TABELA_DIAS[i]) ? contagemDia[TABELA_DIAS[i]] : 0;
        cout << "  " << left << setw(18) << TABELA_DIAS[i] << " | Agend: " << qtd << "\n";
    }

    cout << "============================================\n";
}