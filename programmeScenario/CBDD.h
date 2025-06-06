#ifndef BDD_H // Changed include guard to be more generic or could be CBDD_H
#define BDD_H

#include <MySQL_Connection.h>
#include <IPAddress.h>

// Class renamed from CSockets to CBDD
class CBDD {
private:
    MySQL_Connection& _mysqlCon;
    IPAddress& _serverIP;
    int _port;
    const char* _user;
    const char* _password;
    const char* _dbName;
    const char* _tableName;

    bool _isConnected;
    unsigned long _lastErrorTime;
    const unsigned long _reconnectInterval;

public:
    
    CBDD(MySQL_Connection& con, IPAddress& serverIP, int port, const char* user, const char* pass, const char* db, const char* table);
    ~CBDD();

    bool connect();
    bool sendData(int SAT, int Capnie, int FC, int pressionArterielle, int FR);
    bool isConnected();
    void setConnected(bool status);
};

#endif