#include "../include/Consultas.h"
#include "../include/Conexao.h"
#include <sstream>
#include <iostream>

using namespace std;

// Executa query e retorna resultado 
PGresult* Consultas::exec(const string& sql) {
    if (!Conexao::ok()) return nullptr;
    
    PGresult* res = PQexec(Conexao::get(), sql.c_str());
    ExecStatusType status = PQresultStatus(res);
    
    if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
        cerr << "Erro SQL: " << PQerrorMessage(Conexao::get()) << "\n";
        PQclear(res);
        return nullptr;
    }
    return res;
}

// Converte uma tupla do resultado da consulta em um objeto Agendamento
Agendamento Consultas::rowToAgendamento(PGresult* res, int row) {
    return Agendamento(
        stoi(PQgetvalue(res, row, 0)),
        PQgetvalue(res, row, 1),
        PQgetvalue(res, row, 2),
        PQgetvalue(res, row, 3),
        PQgetvalue(res, row, 4),
        PQgetvalue(res, row, 5),
        PQgetvalue(res, row, 6),
        stod(PQgetvalue(res, row, 7))
    );
}

// insert into
int Consultas::inserir(const Agendamento& ag) {
    ostringstream sql;
    sql << "INSERT INTO agendamentos (nome_cliente, telefone, servico, dia, horario, barbeiro, preco) VALUES ("
        << ESC(ag.getNomeCliente()) << ", " << ESC(ag.getTelefone()) << ", "
        << ESC(ag.getServico()) << ", " << ESC(ag.getDia()) << ", "
        << ESC(ag.getHorario()) << ", " << ESC(ag.getBarbeiro()) << ", "
        << ag.getPreco() << ") RETURNING id;";
    
    PGresult* res = exec(sql.str());
    if (!res) return -1;
    
    int id = PQntuples(res) > 0 ? stoi(PQgetvalue(res, 0, 0)) : -1;
    PQclear(res);
    return id;
}

// update where
bool Consultas::alterar(const Agendamento& ag) {
    ostringstream sql;
    sql << "UPDATE agendamentos SET "
        << "nome_cliente=" << ESC(ag.getNomeCliente()) << ", "
        << "telefone=" << ESC(ag.getTelefone()) << ", "
        << "servico=" << ESC(ag.getServico()) << ", "
        << "dia=" << ESC(ag.getDia()) << ", "
        << "horario=" << ESC(ag.getHorario()) << ", "
        << "barbeiro=" << ESC(ag.getBarbeiro()) << ", "
        << "preco=" << ag.getPreco()
        << " WHERE id=" << ag.getId() << ";";
    
    PGresult* res = exec(sql.str());
    if (!res) return false;
    
    bool ok = string(PQcmdTuples(res)) != "0";
    PQclear(res);
    return ok;
}

// delete from
bool Consultas::remover(int id) {
    PGresult* res = exec("DELETE FROM agendamentos WHERE id=" + to_string(id) + ";");
    if (!res) return false;
    
    bool ok = string(PQcmdTuples(res)) != "0";
    PQclear(res);
    return ok;
}

unique_ptr<Agendamento> Consultas::buscarPorId(int id) {
    PGresult* res = exec("SELECT id, nome_cliente, telefone, servico, dia, horario, barbeiro, preco "
                         "FROM agendamentos WHERE id=" + to_string(id) + ";");
    
    if (!res || PQntuples(res) == 0) {
        if (res) PQclear(res);
        return nullptr;
    }
    
    auto ag = make_unique<Agendamento>(rowToAgendamento(res, 0));
    PQclear(res);
    return ag;
}

vector<Agendamento> Consultas::buscarPorNome(const string& nome) {
    vector<Agendamento> lista;
    
    string pattern = "%" + nome + "%";
    ostringstream sql;
    sql << "SELECT id, nome_cliente, telefone, servico, dia, horario, barbeiro, preco "
        << "FROM agendamentos WHERE LOWER(nome_cliente) LIKE LOWER(" << ESC(pattern) << ") ORDER BY id;";
    
    PGresult* res = exec(sql.str());
    if (!res) return lista;
    
    for (int i = 0; i < PQntuples(res); i++)
        lista.push_back(rowToAgendamento(res, i));
    
    PQclear(res);
    return lista;
}

vector<Agendamento> Consultas::listarTodos() {
    vector<Agendamento> lista;
    
    PGresult* res = exec("SELECT id, nome_cliente, telefone, servico, dia, horario, barbeiro, preco "
                         "FROM agendamentos ORDER BY id;");
    if (!res) return lista;
    
    for (int i = 0; i < PQntuples(res); i++)
        lista.push_back(rowToAgendamento(res, i));
    
    PQclear(res);
    return lista;
}

bool Consultas::horarioOcupado(const string& dia, const string& horario, 
                               const string& barbeiro, int ignorarId) {
    ostringstream sql;
    sql << "SELECT COUNT(*) FROM agendamentos WHERE "
        << "dia=" << ESC(dia) << " AND horario=" << ESC(horario) 
        << " AND barbeiro=" << ESC(barbeiro);
    
    if (ignorarId > 0) sql << " AND id!=" << ignorarId;
    sql << ";";
    
    PGresult* res = exec(sql.str());
    if (!res) return false;
    
    int count = stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return count > 0;
}

double Consultas::getValorTotal() {
    PGresult* res = exec("SELECT COALESCE(SUM(preco), 0) FROM agendamentos;");
    if (!res) return 0.0;
    
    double total = stod(PQgetvalue(res, 0, 0));
    PQclear(res);
    return total;
}

// contagem por campo
map<string, int> Consultas::contagemPor(const string& campo) {
    map<string, int> result;
    
    PGresult* res = exec("SELECT " + campo + ", COUNT(*) FROM agendamentos GROUP BY " + campo + ";");
    if (!res) return result;
    
    for (int i = 0; i < PQntuples(res); i++)
        result[PQgetvalue(res, i, 0)] = stoi(PQgetvalue(res, i, 1));
    
    PQclear(res);
    return result;
}

// soma de valor por campo
map<string, double> Consultas::valorPor(const string& campo) {
    map<string, double> result;
    
    PGresult* res = exec("SELECT " + campo + ", SUM(preco) FROM agendamentos GROUP BY " + campo + ";");
    if (!res) return result;
    
    for (int i = 0; i < PQntuples(res); i++)
        result[PQgetvalue(res, i, 0)] = stod(PQgetvalue(res, i, 1));
    
    PQclear(res);
    return result;
}