#pragma once
#include <string>

class Agendamento {
private:
    int id;
    std::string nomeCliente;
    std::string telefone;
    std::string servico;
    std::string dia;
    std::string horario;
    std::string barbeiro;
    double preco;

public:
    Agendamento();
    Agendamento(int id, const std::string& nomeCliente, const std::string& telefone,
                const std::string& servico, const std::string& dia, const std::string& horario,
                const std::string& barbeiro, double preco);

    // Getters
    int getId() const { return id; }
    std::string getNomeCliente() const { return nomeCliente; }
    std::string getTelefone() const { return telefone; }
    std::string getServico() const { return servico; }
    std::string getDia() const { return dia; }
    std::string getHorario() const { return horario; }
    std::string getBarbeiro() const { return barbeiro; }
    double getPreco() const { return preco; }

    // Setters
    void setNomeCliente(const std::string& nome) { nomeCliente = nome; }
    void setTelefone(const std::string& tel) { telefone = tel; }
    void setServico(const std::string& serv) { servico = serv; }
    void setDia(const std::string& d) { dia = d; }
    void setHorario(const std::string& h) { horario = h; }
    void setBarbeiro(const std::string& b) { barbeiro = b; }
    void setPreco(double p) { preco = p; }

    // Exibição
    void exibirResumo() const;
    void exibirCompleto() const;
};
