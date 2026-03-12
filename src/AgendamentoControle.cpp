#include "../include/AgendamentoControle.h"
#include "../include/Conexao.h"
#include "../include/Consultas.h"
#include "../include/DadosBarbearia.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

// Helper para verificar conexão
static bool verificarConexao() {
    if (!Conexao::ok()) {
        cout << "Erro: Banco de dados nao conectado.\n";
        return false;
    }
    return true;
}

// Helper para ler inteiro com validação
static int lerInteiro(const string& prompt, int min, int max) {
    int valor;
    cout << prompt;
    cin >> valor;
    return (valor >= min && valor <= max) ? valor : -1;
}

// Helper para limpar buffer
static void limparBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void AgendamentoControle::inserir() {
    if (!verificarConexao()) return;
    
    string nome, telefone;
    cout << "\n--- Novo Agendamento ---\n";
    
    limparBuffer();
    cout << "Nome do cliente: ";
    getline(cin, nome);
    cout << "Telefone: ";
    getline(cin, telefone);
    
    exibirServicos();
    int codServico = lerInteiro("  Escolha o servico (1-" + to_string(TOTAL_SERVICOS) + "): ", 1, TOTAL_SERVICOS);
    const Servico* servico = buscarServico(codServico);
    if (!servico) { cout << "Servico invalido.\n"; return; }
    
    exibirBarbeiros();
    int codBarbeiro = lerInteiro("  Escolha o barbeiro (1-" + to_string(TOTAL_BARBEIROS) + "): ", 1, TOTAL_BARBEIROS);
    const Barbeiro* barbeiro = buscarBarbeiro(codBarbeiro);
    if (!barbeiro) { cout << "Barbeiro invalido.\n"; return; }
    
    exibirDias();
    int codDia = lerInteiro("  Escolha o dia (1-" + to_string(TOTAL_DIAS) + "): ", 1, TOTAL_DIAS);
    if (codDia < 0) { cout << "Dia invalido.\n"; return; }
    string dia = TABELA_DIAS[codDia - 1];
    
    exibirHorarios();
    int codHorario = lerInteiro("  Escolha o horario (1-" + to_string(TOTAL_HORARIOS) + "): ", 1, TOTAL_HORARIOS);
    if (codHorario < 0) { cout << "Horario invalido.\n"; return; }
    string horario = TABELA_HORARIOS[codHorario - 1];
    
    if (Consultas::horarioOcupado(dia, horario, barbeiro->nome)) {
        cout << "\nHorario indisponivel! " << barbeiro->nome << " ja tem agendamento.\n";
        return;
    }
    
    Agendamento novo(0, nome, telefone, servico->nome, dia, horario, barbeiro->nome, servico->preco);
    int id = Consultas::inserir(novo);
    
    if (id > 0)
        cout << "\nAgendamento #" << id << " cadastrado! " << servico->nome 
             << " | R$ " << fixed << setprecision(2) << servico->preco << " | " 
             << barbeiro->nome << " | " << dia << " " << horario << "\n";
    else
        cout << "Erro ao inserir.\n";
}

void AgendamentoControle::alterar() {
    if (!verificarConexao()) return;
    
    int id;
    cout << "\n--- Alterar Agendamento ---\nInforme o ID: ";
    cin >> id;
    
    auto agPtr = Consultas::buscarPorId(id);
    if (!agPtr) { cout << "Agendamento nao encontrado.\n"; return; }
    
    Agendamento ag = *agPtr;
    cout << "\nAgendamento encontrado:\n";
    ag.exibirCompleto();
    
    string nome, telefone;
    limparBuffer();
    
    cout << "Novo nome [Enter = manter]: ";
    getline(cin, nome);
    if (!nome.empty()) ag.setNomeCliente(nome);
    
    cout << "Novo telefone [Enter = manter]: ";
    getline(cin, telefone);
    if (!telefone.empty()) ag.setTelefone(telefone);
    
    exibirServicos();
    int codServico = lerInteiro("  Novo servico [0 = manter]: ", 0, TOTAL_SERVICOS);
    if (codServico > 0) {
        if (const Servico* s = buscarServico(codServico)) {
            ag.setServico(s->nome);
            ag.setPreco(s->preco);
        }
    }
    
    string barbeiroAntigo = ag.getBarbeiro(), diaAntigo = ag.getDia(), horarioAntigo = ag.getHorario();
    
    exibirBarbeiros();
    int codBarbeiro = lerInteiro("  Novo barbeiro [0 = manter]: ", 0, TOTAL_BARBEIROS);
    if (codBarbeiro > 0)
        if (const Barbeiro* b = buscarBarbeiro(codBarbeiro)) ag.setBarbeiro(b->nome);
    
    exibirDias();
    int codDia = lerInteiro("  Novo dia [0 = manter]: ", 0, TOTAL_DIAS);
    if (codDia > 0) ag.setDia(TABELA_DIAS[codDia - 1]);
    
    exibirHorarios();
    int codHorario = lerInteiro("  Novo horario [0 = manter]: ", 0, TOTAL_HORARIOS);
    if (codHorario > 0) ag.setHorario(TABELA_HORARIOS[codHorario - 1]);
    
    // Verificar conflito se mudou horário/dia/barbeiro
    if ((ag.getDia() != diaAntigo || ag.getHorario() != horarioAntigo || ag.getBarbeiro() != barbeiroAntigo) &&
        Consultas::horarioOcupado(ag.getDia(), ag.getHorario(), ag.getBarbeiro(), ag.getId())) {
        cout << "Conflito! Horario ocupado.\n";
        return;
    }
    
    cout << (Consultas::alterar(ag) ? "Alterado com sucesso!\n" : "Nenhuma alteracao.\n");
}

void AgendamentoControle::pesquisarPorNome() const {
    if (!verificarConexao()) return;
    
    string termo;
    cout << "\n--- Pesquisar por Nome ---\n";
    limparBuffer();
    cout << "Nome (ou parte): ";
    getline(cin, termo);
    
    auto resultados = Consultas::buscarPorNome(termo);
    
    if (resultados.empty()) {
        cout << "Nenhum encontrado.\n";
        return;
    }
    
    cout << "\nResultados (" << resultados.size() << "):\n";
    for (const auto& ag : resultados) ag.exibirResumo();
}

void AgendamentoControle::remover() {
    if (!verificarConexao()) return;
    
    int id;
    cout << "\n--- Remover Agendamento ---\nInforme o ID: ";
    cin >> id;
    
    auto agPtr = Consultas::buscarPorId(id);
    if (!agPtr) { cout << "Nao encontrado.\n"; return; }
    
    cout << "\nRemover este:\n";
    agPtr->exibirCompleto();
    
    char conf;
    cout << "Confirma? (s/n): ";
    cin >> conf;
    
    if (conf == 's' || conf == 'S')
        cout << (Consultas::remover(id) ? "Removido!\n" : "Falha ao remover.\n");
    else
        cout << "Cancelado.\n";
}

void AgendamentoControle::listarTodos() const {
    if (!verificarConexao()) return;
    
    auto lista = Consultas::listarTodos();
    
    if (lista.empty()) {
        cout << "\nNenhum agendamento.\n";
        return;
    }
    
    cout << "\n--- Agendamentos (" << lista.size() << ") ---\n";
    for (const auto& ag : lista) ag.exibirResumo();
}

void AgendamentoControle::exibirUm() const {
    if (!verificarConexao()) return;
    
    int id;
    cout << "\n--- Exibir Agendamento ---\nInforme o ID: ";
    cin >> id;
    
    auto agPtr = Consultas::buscarPorId(id);
    if (agPtr) 
        agPtr->exibirCompleto();
    else 
        cout << "Nao encontrado.\n";
}
