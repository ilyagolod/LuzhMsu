#include <iostream>
#include "server.h"
#include "RBTree.h"

#ifndef CLIENT_H
#define CLIENT_H

template <typename Tree>
class Client
{
public:
    Client();

    ~Client();

    int sendToServer(Server<Tree> &server, std::string &data);

    std::string readFromServer(Server<Tree> &server);

    std::string make_request(Server<Tree> &server, std::string &data);

    int connect(Server<Tree> &server);

    int disconnect(Server<Tree> &server);

private:
    int CLIENT_PORT_;
    std::string CLIENT_NAME;
};

#endif