#pragma once
#include "AgendamentoControle.h"
#include "RelatorioAgendamentos.h"

class MenuAgendamentos {
private:
    AgendamentoControle controle;
    RelatorioAgendamentos relatorio;

public:
    void executar();
};
