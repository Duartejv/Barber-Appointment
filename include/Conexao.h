#include <string>
#include <libpq-fe.h>

#pragma once
class Conexao {
private:
    static PGconn* conn;

public:
    static bool conectar(const std::string& dbname, const std::string& user, 
                         const std::string& password, const std::string& host = "localhost", 
                         const std::string& port = "5432");
    static void desconectar();
    static PGconn* get();
    static bool ok();
    
    // Helper para escapar strings (RAII)
    class EscapedString {
        char* str;
    public:
        EscapedString(const std::string& s);
        ~EscapedString();
        operator const char*() const { return str ? str : "''"; }
    };
};

// Macro conveniente
#define ESC(s) Conexao::EscapedString(s)