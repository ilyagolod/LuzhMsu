#include <iostream>
#include "RBTree.h"
#include "parser.h"
#include <fstream>
#include <cassert>
#include <vector>
#include <queue>
//#include <pair>

#ifndef SERVER_H
#define SERVER_H

#define MAX_CLIENTS_NUMBER 10
#define MAX_LOG_NUMBER 100

template <typename Tree>
class Server{
public:
    Server(std::string& input_filename)
    {   
        PORT_ = rand()%1000+2000;
        std::cout<< "CREATING NEW SERVER: PORT=" << PORT_<< std::endl;;
        
        database = Tree();
        std::ifstream infile(input_filename);
        assert(infile);
        database.read(infile);   
        infile.close();

        std::cout<< "SERVER CREATED: PORT=" << PORT_<< std::endl;;
        //parser = Parser<Tree>();
         //какой-то номер порта
        // std::vector<int> clients_list; //вектор из подключенных клиентов
        // std::queue<std::pair<std::string, int>> answer_packs; //хранит 100 ответов
    }

    ~Server(){
       
        answer_packs.clear();
        clients_list.clear();
        PORT_ = 0;
    }

    //возвращает -1 если ничего не вышло, 0 если все хорошо
    int send_to_server(std::string& request, int Port){
        if (std::find(clients_list.begin(), clients_list.end(), Port)==clients_list.end()){
            return -1;
        }
        std::string answer_pack = parser.parse(request, &database);
        if(answer_pack=="STOP, CLOSING CONNECTION"){
            disconnect(Port);
            for(size_t i=0;i<answer_packs.size();i++){
                if(answer_packs[i].first==Port){
                    auto iterator = std::find(answer_packs.begin(), answer_packs.end(), answer_packs[i]);
                    answer_packs.erase(iterator);//как это сделать лучше?
                }
            }
        }
        else{
            if(answer_packs.size()==MAX_CLIENTS_NUMBER)
                answer_packs.erase(answer_packs.begin());
            std::pair<int, std::string> tmp(Port, answer_pack);
            answer_packs.push_back(tmp);
        }
        return 0;
    }

    std::string get_answer_pack(int Port){
        std::string answer="bad request";
        if(std::find(clients_list.begin(), clients_list.end(), Port)==clients_list.end()){
            return answer; //этого клиента нет в списке серверов и он не может просить ответ
        }
        else{
            for(size_t i=0; i<answer_packs.size();i++){
                if(answer_packs[i].first==Port){
                    std::string pp = answer_packs[i].second;
                    auto iterator = std::find(answer_packs.begin(), answer_packs.end(), answer_packs[i]);
                    answer_packs.erase(iterator);//как это сделать лучше?
                    return pp;
                }
            }
            return answer;
        }
    }

    int connect(int Port){
        if(clients_list.size()==MAX_CLIENTS_NUMBER ){
            return -1;
        }

        bool key = (std::find(clients_list.begin(), clients_list.end(), Port)==clients_list.end());
        if (key){
            clients_list.push_back(Port);
        }
        return 0;
    }

    int disconnect(int Port){
        auto pos = std::find(clients_list.begin(), clients_list.end(), Port);
        if (pos!=clients_list.end()){
            clients_list.erase(pos);
            return 0;
        }
        
        return -1;
        
    }

private:
    int PORT_ ;
    std::vector<int> clients_list; //вектор из подключенных клиентов, максимум 10
    std::vector<std::pair<int, std::string> > answer_packs; //хранит информацию о 100 последних запросах
    Tree database; //база данных 
    Parser<Tree> parser; //функциональность обработки запросов и сервера разделена
};

#endif