#include <iostream>
#include <vector>

#ifndef PARSER_H
#define PARSER_H

template <typename Tree>
class Parser
{
public:
    Parser() {}
    ~Parser() {}

    std::string parse(std::string &request, Tree *tr)
    { 
        std::string answer;
        if (request.length() < 4)
        {
            answer = "bad request: 0";
            return answer;
        } 
        else if (request.find("add") == 0)
        {
            int gr, ph;
            std::string name;
            std::vector<std::string> tokens = subbs(request);
            if (tokens.size() != 4)
            {
                tokens.clear();
                answer = "bad request: 1";
                return answer;
            }
            else
            {
                try
                {
                    name = tokens[1];
                    gr = stoi(tokens[2]);
                    ph = stoi(tokens[3]);
                }
                catch (std::invalid_argument)
                {
                    tokens.clear();
                    answer = "bad request: 2";
                    return answer;
                }
                catch (std::out_of_range)
                {
                    tokens.clear();
                    answer = "bad request: 3";
                    return answer;
                }
                if ((gr > 1000 || gr < 100) || (ph >= 100000 || ph < 10000) || (tokens[2].size() != 3 || tokens[3].size() != 5))
                {
                    tokens.clear();
                    answer = "bad request: 4";
                    return answer;
                }
                else
                {
                    record tmp(name, gr, ph);
                    auto key = tr->find(&tmp);
                    if (key == nullptr)
                        tr->insert(&tmp);
                    answer = "succesful add request";
                    return answer;
                }
            }
        }
        else if (request.find("select") == 0)
        {
            std::vector<std::string> tokens = subbs(request);

            if (tokens.size() < 3)
            {
                answer = "bad request: 5";
                return answer;
            }
            else if (tokens[1] != "where")
            {
                answer = "bad request: 6";
                return answer;
            }
            std::vector<record> obj = make_all(tokens);
            if (obj.empty())
            {
                tokens.clear();
                answer = "bad request: 7";
                return answer;
            }
            tr->find_select(tr->getRoot(), obj);
            tokens.clear();
            obj.clear();
            answer = "succesful select request";
            return answer;
        }
        else if (request.find("delete") == 0)
        {
            int gr, ph;
            std::string name;
            std::vector<std::string> tokens = subbs(request);
            if (tokens.size() != 4)
            {
                tokens.clear();
                answer = "bad request: 8";
                return answer;
            }
            else
            {
                try
                {
                    name = tokens[1];
                    gr = stoi(tokens[2]);
                    ph = stoi(tokens[3]);
                }
                catch (std::invalid_argument)
                {
                    tokens.clear();
                    answer = "bad request: 9";
                    return answer;
                }
                catch (std::out_of_range)
                {
                    tokens.clear();
                    answer = "bad request: 10";
                    return answer;
                }
                if ((gr > 1000 || gr < 100) || (ph >= 100000 || ph < 10000) || (tokens[2].size() != 3 || tokens[3].size() != 5))
                {
                    tokens.clear();
                    answer = "bad request: 11";
                    return answer;
                }
                else
                {
                    record tmp(name, gr, ph);
                    tr->remove(&tmp);
                    tr->print(0);
                    answer = "succesful delete request";
                    return answer;
                }
            }
        }

        else if (request.find("info") == 0)
        {
            if (request.size() == 4)
            {
                std::cout << "welcome! you have 6 commands to run:\n"
                             "add - 3 fields required\n"
                             "select - 3 fields + 'and' 'or' 'not' without brackets <=5 logical connectives\n"
                             "delete - 3 fields required\n"
                             "exit - to complete session\n"
                             "info - to repeat this message\n"
                             "print tree - to see the current tree\n"
                             "the group number consists of 3 digits, phone number - 5\n";
                answer = "succesful info request";
                return answer;
            }
            else
            {
                answer = "bad request: 12";
                return answer;
            }
        }

        else if (request.find("stop") == 0)
        {
            if (request.size() == 4)
            {
                answer = "STOP, CLOSING CONNECTION";
                return answer;
            }
            else
            {
                answer = "bad request: 13";
                return answer;
            }
        }

        else if (request.find("print tree") == 0)
        {
            if (request.size() == 10)
            {
                tr->print(0);
                answer = "succesful print tree request";
                return answer;
            }
            else
            {
                answer = "bad request: 14";
                return answer;
            }
        }

        else
        {
            answer = "bad request: 15";
            return answer;
        }
    }

private:
    std::vector<std::string> subbs(const std::string &inData)
    {
        size_t pos_start = 0, pos_end;
        std::string token;
        std::vector<std::string> tokens;

        while ((pos_end = inData.find(" ", pos_start)) != std::string::npos)
        {
            token = inData.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + 1;
            tokens.push_back(token);
        }
        tokens.push_back(inData.substr(pos_start));
        return tokens;
    }

    std::vector<record> make_all(const std::vector<std::string> &tokens)
    {
        int kol = 0, mis = 0;
        int last = -1;
        std::string nam("");
        int gr = -1, ph = -1;
        std::vector<record> obj;

        for (size_t i = 2; i < tokens.size(); i++)
        {
            if (tokens[i] == "or")
            {
                if (last > 2)
                {
                    mis = 1;
                    break;
                }
                if (kol > 0)
                {
                    obj.push_back(record(nam, gr, ph));
                    gr = -1;
                    ph = -1;
                    nam = "";
                    last = 3;
                    kol = 0;
                }
                else
                {
                    mis = 1;
                    break;
                }
            }
            else if (tokens[i] == "and")
            {
                if (last > 2)
                {
                    mis = 1;
                    break;
                }
                last = 4;
            }
            else
            { 
                if (tokens[i].find("name=") == 0)
                { 
                    if (last < 3 && last != -1)
                    {
                        mis = 1;
                        break;
                    }
                    if (tokens[i].size() == 5)
                    {
                        mis = 1;
                        break;
                    }
                    if (nam.size() > 0)
                    {
                        mis = 1;
                        break;
                    } 
                    nam = tokens[i].substr(5, tokens[i].size() - 5);
                    last = 0;
                    kol++;
                }
                else if (tokens[i].find("group=") == 0)
                {
                    if (last < 3 && last != -1)
                    {
                        mis = 1;
                        break;
                    }
                    if (tokens[i].length() == 6)
                    {
                        mis = 1;
                        break;
                    } 
                    if (gr != -1)
                    {
                        mis = 1;
                        break;
                    }
                    try
                    {
                        gr = stoi((tokens[i].substr(6, tokens[i].length() - 6)));
                    }
                    catch (std::invalid_argument)
                    {
                        mis = 1;
                        break;
                    } 
                    if (gr < 100 || gr > 999 || tokens[i].length() != 9)
                    {
                        mis = 1;
                        break;
                    }
                    last = 1;
                    kol++;
                }
                else if (tokens[i].find("phone=") == 0)
                {
                    if (last < 3 && last != -1)
                    {
                        mis = 1;
                        break;
                    }
                    if (tokens[i].length() == 6)
                    {
                        mis = 1;
                        break;
                    } //номера нет
                    if (ph != -1)
                    {
                        mis = 1;
                        break;
                    }
                    try
                    {
                        ph = stoi((tokens[i].substr(6, tokens[i].length() - 6)));
                    }
                    catch (std::invalid_argument)
                    {
                        mis = 1;
                        break;
                    } 
                    if (ph < 10000 || ph >= 100000 || tokens[i].length() != 11)
                    {
                        mis = 1;
                        break;
                    }
                    last = 2;
                    kol++;
                }
                else
                {
                    mis = 1; 
                    break;
                }
            }
        }
        if (last > 2 || last == -1)
        {
            mis = 1;
        }
        if (kol > 0)
        {
            obj.push_back(record(nam, gr, ph));
            gr = -1;
            ph = -1;
            nam = "";
        }
        else
        {
            mis = 1;
        }

        if (mis == 1)
        {
            std::vector<record> vect;
            return vect;
        }
        else
            return obj;
    }
};

#endif /*PARSER_H*/