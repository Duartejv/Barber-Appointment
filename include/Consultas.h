#pragma once
#include "Agendamento.h"
#include <libpq-fe.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

class Consultas {
private:
    static PGresult* exec(const std::string& sql);
    static Agendamento rowToAgendamento(PGresult* res, int row);

public:
    // CRUD
    static int inserir(const Agendamento& ag);
    static bool alterar(const Agendamento& ag);
    static bool remover(int id);
    static std::unique_ptr<Agendamento> buscarPorId(int id);
    static std::vector<Agendamento> buscarPorNome(const std::string& nome);
    static std::vector<Agendamento> listarTodos();

    //ignorarId = -1 para evitar conflito ao editar um registro cadastrado
    static bool horarioOcupado(const std::string& dia, const std::string& horario, 
                               const std::string& barbeiro, int ignorarId = -1); 

    
    // Relatorios
    static double getValorTotal();
    // conta agendamentos por campo
    static std::map<std::string, int> contagemPor(const std::string& campo);

    // retorna o total do agrupamento do campo
    static std::map<std::string, double> valorPor(const std::string& campo);
};