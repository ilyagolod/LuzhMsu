#include <iostream>
#include "server.h"
#include "RBTree.h"

#ifndef CLIENT_H
#define CLIENT_H

template <typename Tree>
class Client{
public:
    Client() {
        CLIENT_PORT_ = rand()%1000+3000;
        CLIENT_NAME = std::to_string(rand()%256) + "." + std::to_string(rand()%256)+ ".0.0";  
        //мы ничем не ограничены и нам не нужно много клиентов, шанс совпадения минимален
    }  

    ~Client(){
        CLIENT_NAME.~basic_string(); //на всякий случай вызовем руками, хотя так и нельзя
        CLIENT_PORT_=0;
    }

    //возвращает 0-ок -1 - ошибка
    int sendToServer(Server<Tree>& server, std::string& data){
        return server.send_to_server(data, CLIENT_PORT_);
    }

    //возвращает код первого из запросов, код которого не был запрошен
    std::string readFromServer(Server<Tree>& server){ 
        return server.get_answer_pack(CLIENT_PORT_);
    }

    //обертка над send и read, шлет запрос и отдает ответ сервера на запрос
    std::string make_request(Server<Tree>& server, std::string& data){
        int er = server.send_to_server(data, CLIENT_PORT_);
        return server.get_answer_pack(CLIENT_PORT_);
    }

    int connect(Server<Tree>& server){
        //следующий вывод для 3го теста
        std::cout<< "trying to connect:" << CLIENT_PORT_<< std::endl;
        return server.connect(CLIENT_PORT_);
    }

    int disconnect(Server<Tree>& server){
        return server.disconnect(CLIENT_PORT_);
    }


private:
    int CLIENT_PORT_;
    std::string CLIENT_NAME;
};

#endif