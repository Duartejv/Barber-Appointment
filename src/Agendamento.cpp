#include "../include/Agendamento.h"
#include <iostream>
#include <iomanip>

using namespace std;

Agendamento::Agendamento() 
    : id(0), preco(0.0) {}

Agendamento::Agendamento(int id, const string& nomeCliente, const string& telefone,
                         const string& servico, const string& dia, const string& horario,
                         const string& barbeiro, double preco)
    : id(id), nomeCliente(nomeCliente), telefone(telefone), servico(servico), 
      dia(dia), horario(horario), barbeiro(barbeiro), preco(preco) {}

void Agendamento::exibirResumo() const {
    cout << "  [" << id << "] " << nomeCliente << " (" << telefone << ") | " 
         << servico << " | " << dia << " " << horario << " | " << barbeiro 
         << " | R$ " << fixed << setprecision(2) << preco << "\n";
}

void Agendamento::exibirCompleto() const {
    cout << "========================================\n"
         << "  ID:       " << id << "\n"
         << "  Cliente:  " << nomeCliente << "\n"
         << "  Telefone: " << telefone << "\n"
         << "  Servico:  " << servico << "\n"
         << "  Dia:      " << dia << "\n"
         << "  Horario:  " << horario << "\n"
         << "  Barbeiro: " << barbeiro << "\n"
         << "  Preco:    R$ " << fixed << setprecision(2) << preco << "\n"
         << "========================================\n";
}
