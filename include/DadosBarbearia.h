#pragma once
#include <string>
#include <iostream>
#include <iomanip>

// ============== SERVIÇOS ==============
struct Servico {
    int codigo;
    std::string nome;
    double preco;
};

constexpr int TOTAL_SERVICOS = 8;
inline const Servico TABELA_SERVICOS[TOTAL_SERVICOS] = {
    {1, "Corte 0-6 anos", 10.00}, {2, "Corte", 15.00},
    {3, "Barba", 10.00},          {4, "Sobrancelha", 5.00},
    {5, "Platinado", 30.00},      {6, "Luzes", 30.00},
    {7, "Botox", 20.00},          {8, "Selagem", 20.00}
};

inline const Servico* buscarServico(int codigo) {
    for (const auto& s : TABELA_SERVICOS)
        if (s.codigo == codigo) return &s;
    return nullptr;
}

inline void exibirServicos() {
    std::cout << "\n  --- Servicos ---\n";
    for (const auto& s : TABELA_SERVICOS)
        std::cout << "  " << s.codigo << ". " << std::left << std::setw(16) 
                  << s.nome << " R$ " << std::fixed << std::setprecision(2) << s.preco << "\n";
}

// ============== BARBEIROS ==============
struct Barbeiro {
    int codigo;
    std::string nome;
};

constexpr int TOTAL_BARBEIROS = 2;
inline const Barbeiro TABELA_BARBEIROS[TOTAL_BARBEIROS] = {
    {1, "Joao"}, {2, "Pedro"}
};

inline const Barbeiro* buscarBarbeiro(int codigo) {
    for (const auto& b : TABELA_BARBEIROS)
        if (b.codigo == codigo) return &b;
    return nullptr;
}

inline void exibirBarbeiros() {
    std::cout << "\n  --- Barbeiros ---\n";
    for (const auto& b : TABELA_BARBEIROS)
        std::cout << "  " << b.codigo << ". " << b.nome << "\n";
}

// ============== HORÁRIOS ==============
constexpr int TOTAL_DIAS = 5;
inline const std::string TABELA_DIAS[TOTAL_DIAS] = {
    "Terca-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sabado"
};

constexpr int TOTAL_HORARIOS = 18;
inline const std::string TABELA_HORARIOS[TOTAL_HORARIOS] = {
    "08:00", "08:30", "09:00", "09:30", "10:00", "10:30", "11:00", "11:30",
    "14:00", "14:30", "15:00", "15:30", "16:00", "16:30", "17:00", "17:30", "18:00", "18:30"
};

inline void exibirDias() {
    std::cout << "\n  --- Dias Disponiveis ---\n";
    for (int i = 0; i < TOTAL_DIAS; i++)
        std::cout << "  " << (i + 1) << ". " << TABELA_DIAS[i] << "\n";
}

inline void exibirHorarios() {
    std::cout << "\n  --- Horarios ---\n  Manha:\n";
    for (int i = 0; i < 8; i++)
        std::cout << "  " << std::setw(4) << (i + 1) << ". " << TABELA_HORARIOS[i] << "\n";
    std::cout << "  Tarde:\n";
    for (int i = 8; i < TOTAL_HORARIOS; i++)
        std::cout << "  " << std::setw(4) << (i + 1) << ". " << TABELA_HORARIOS[i] << "\n";
}
