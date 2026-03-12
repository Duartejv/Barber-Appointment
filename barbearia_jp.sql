-- Script para criar o banco de dados e tabelas do sistema de agendamentos

-- Criar banco de dados
CREATE DATABASE barbearia_jp;

-- Conectar ao banco
\c barbearia_jp;

-- Criar sequencia para IDs
CREATE SEQUENCE IF NOT EXISTS agendamento_id_seq START 1;

-- Criar tabela de agendamentos
CREATE TABLE IF NOT EXISTS agendamentos (
    id INTEGER PRIMARY KEY DEFAULT nextval('agendamento_id_seq'),
    nome_cliente VARCHAR(100) NOT NULL,
    telefone VARCHAR(20) NOT NULL,
    servico VARCHAR(50) NOT NULL,
    dia VARCHAR(20) NOT NULL,
    horario VARCHAR(5) NOT NULL,
    barbeiro VARCHAR(50) NOT NULL,
    preco DECIMAL(10,2) NOT NULL,
    data_criacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    -- Garantir que nao haja conflito de horario para o mesmo barbeiro
    CONSTRAINT unique_agendamento UNIQUE (dia, horario, barbeiro) -- unico agendamento
);

-- Criar índices para melhor performance
CREATE INDEX IF NOT EXISTS idx_agendamentos_dia ON agendamentos(dia);
CREATE INDEX IF NOT EXISTS idx_agendamentos_barbeiro ON agendamentos(barbeiro);
CREATE INDEX IF NOT EXISTS idx_agendamentos_nome_cliente ON agendamentos(nome_cliente);

-- Comentários nas tabelas
COMMENT ON TABLE agendamentos IS 'Agendamentos da barbearia';
COMMENT ON COLUMN agendamentos.id IS 'Identificador único do agendamento';
COMMENT ON COLUMN agendamentos.nome_cliente IS 'Nome do cliente';
COMMENT ON COLUMN agendamentos.telefone IS 'Telefone de contato';
COMMENT ON COLUMN agendamentos.servico IS 'Serviço agendado';
COMMENT ON COLUMN agendamentos.dia IS 'Dia da semana';
COMMENT ON COLUMN agendamentos.horario IS 'Horário do agendamento (HH:MM)';
COMMENT ON COLUMN agendamentos.barbeiro IS 'Barbeiro responsável';
COMMENT ON COLUMN agendamentos.preco IS 'Preço do serviço';
COMMENT ON COLUMN agendamentos.data_criacao IS 'Data e hora do cadastro';
