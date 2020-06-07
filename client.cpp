#include <iostream>
#include "server.h"
#include "client.h"

template <typename Tree>
Client<Tree>::Client()
{
    CLIENT_PORT_ = rand()%1000+3000;
    CLIENT_NAME = std::to_string(rand() % 256) + "." + std::to_string(rand() % 256) + ".0.0";
}

template <typename Tree>
Client<Tree>::~Client(){
    CLIENT_NAME.~basic_string(); 
    CLIENT_PORT_=0;
}

template <typename Tree>
int Client<Tree>::sendToServer(Server<Tree>& server, std::string& data){
    return server.send_to_server(data, CLIENT_PORT_);
}

template <typename Tree>
std::string Client<Tree>::readFromServer(Server<Tree>& server){ 
    return server.get_answer_pack(CLIENT_PORT_);
}

template <typename Tree>
std::string Client<Tree>::make_request(Server<Tree>& server, std::string& data){
    int er = server.send_to_server(data, CLIENT_PORT_);
    return server.get_answer_pack(CLIENT_PORT_);
}

template <typename Tree>
int Client<Tree>::connect(Server<Tree>& server){
    std::cout<< "trying to connect:" << CLIENT_PORT_<< std::endl;
    return server.connect(CLIENT_PORT_);
}

template <typename Tree>
int Client<Tree>::disconnect(Server<Tree>& server){
    return server.disconnect(CLIENT_PORT_);
}