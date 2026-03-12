#include "../include/Conexao.h"
#include <iostream>

using namespace std;

PGconn* Conexao::conn = nullptr;

bool Conexao::conectar(const string& dbname, const string& user, 
                       const string& password, const string& host, const string& port) {
    string connStr = "dbname=" + dbname + " user=" + user + 
                     " password=" + password + " host=" + host + " port=" + port;
    
    conn = PQconnectdb(connStr.c_str());
    
    if (PQstatus(conn) == CONNECTION_OK) {
        cout << "Conectado ao banco: " << dbname << "\n";
        return true;
    }
    
    cerr << "Falha na conexao: " << PQerrorMessage(conn) << "\n";
    PQfinish(conn);
    conn = nullptr;
    return false;
}

void Conexao::desconectar() {
    if (conn) {
        PQfinish(conn);
        conn = nullptr;
        cout << "Desconectado do banco.\n";
    }
}

PGconn* Conexao::get() { 
    return conn; 
}

bool Conexao::ok() { 
    return conn && PQstatus(conn) == CONNECTION_OK; 
}

// Helper RAII para escape de strings
Conexao::EscapedString::EscapedString(const string& s) {
    str = conn ? PQescapeLiteral(conn, s.c_str(), s.length()) : nullptr;
}

Conexao::EscapedString::~EscapedString() {
    if (str) PQfreemem(str);
}