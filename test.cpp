#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cassert>

#include "server.h"
#include "client.h"
#include "RBTree.h"
#include "record.h" 

int main() {
    
    std::string filename= "out.txt";

    //тест1 проверяет особенности соединения
    {
        std::cout << "---------------\n"
                     "-----TEST1------\n"
                     "---------------\n";
        Server<RBTree<record>> serv1(filename);
        Client<RBTree<record>> client1;
        Client<RBTree<record>> client2;
        client1.connect(serv1);
        //клиент1 подсоединился к серверу и теперь может стать реквесты, а клиент2 - не может
        std::string request = "select where group=210";
        auto answer1 = client1.make_request(serv1, request);
        auto answer2 = client2.make_request(serv1, request);
        std::cout<< "with     connection: <" << answer1<< ">" << std::endl;
        std::cout<< "without  connection: <" << answer2<< ">" << std::endl;
        //далее отсоединим клиента 1 от сервера и попробуем обратиться
        client1.disconnect(serv1);
        auto answer3 = client1.make_request(serv1, request);
        std::cout<< "after disconnection: <" << answer3<< ">" << std::endl;
    }
    
    //тест2 проверяет распознавание запросов
    {
        std::cout << "---------------\n"
                     "-----TEST2------\n"
                     "---------------\n";
        Server<RBTree<record>> serv1(filename);
        Client<RBTree<record>> client1;
        client1.connect(serv1);
        std::cout << "now check the quality of parsing\n"
                     "to stop testing enter 'exit'\n";
        std::string request;
        while(true){
            std::cout<< "enter your request:\n";
            getline(std::cin, request);
            if(request=="exit") break;
            auto answer_code = client1.make_request(serv1, request);
            std::cout<< "answer code:\n" << answer_code<< std::endl;
        }

        client1.disconnect(serv1);
    }

    /*/ 
        тест3  - это как бы стресс-тест, 15 клиентов сделают 15 запросов серверу
        это однопоточные запросы, так что все все равно будет выполняться линейно, но:
        только 10 клиентов имеют доступ к серверу и только 100 последних запросов, 
        которые не были запрошены, залогированы
        поэтому: 1) сервер не пустит последние 5 клиентов
        пусть для удобства запрос все будут делать один и тот же select с разными группами
    /*/
    {
        std::cout << "---------------\n"
                     "-----TEST3------\n"
                     "---------------\n";
        Server<RBTree<record> > serv1(filename);
        
        std::vector<Client<RBTree<record> > > clients;
        for(size_t i=0;i<15;i++){
            clients.emplace_back();
        }
        for(size_t i=0;i<15;i++){
            std::cout<< "\nSTEP: " << i <<std::endl;
            std::string c_request = "select where group="+ std::to_string(i+200);
            
            int is_con = clients[i].connect(serv1);
            std::cout << "mistake in connection? "<< is_con<< std::endl;
            std::cout<< "request made: "<< c_request<< std::endl;
            auto answer_code = clients[i].make_request(serv1, c_request);
            std::cout <<"answer_code: "<< answer_code<< std::endl;
        }

        std::cout << "\n";
        clients[4].disconnect(serv1);
        clients[12].connect(serv1);
        std::string request = "select where group=210";
        auto answer_code = clients[12].make_request(serv1, request);
        std::cout <<"answer_code: "<< answer_code<< std::endl;


    }

    return 0;
}


