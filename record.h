#include <cstdio>
#include <string>

#ifndef RECORD_H
#define RECORD_H

class record
{
public:
    record(std::string iname = "", int igroup = -1,
           int iphone = -1) : name(iname),
                              group(igroup), phone(iphone)
    {
    }
    record(const record &orig) : name(orig.name),
                                 group(orig.group),
                                 phone(orig.phone)
    {
    }

    record(const std::string &ngp, const std::string &flag)
    {
        size_t left = ngp.find_first_of(" ", 0);
        size_t right = ngp.rfind(" ");
        name = ngp.substr(0, left);
        group = stoi(ngp.substr(left, right));
        phone = stoi(ngp.substr(right, ngp.size()));
    }

public:
    ~record();

    int init(const std::string &str, const int g, const int p);
    void reinit(const std::string &iname, const int igroup, const int iphone);
    void destroy();

    void set_group(const int igroup);
    void set_phone(const int iphone);
    void set_name(const std::string &iname);

    std::string &get_name();
    int get_phone() const;
    int get_group() const;

    void print(FILE *fp = stdout) const;

    bool operator<(const record &x) const noexcept;
    bool operator>(const record &x) const noexcept;
    record &operator=(const record &irec);
    bool operator!=(const record &rec) const noexcept;
    bool operator==(const record &rec) const noexcept;

private:
    std::string name;
    int phone;
    int group;
};

#endif