#include "record.h"
#include <iostream>

record::~record(){
    phone = 0;
    group = 0;
}

void record::print(FILE *fp) const {
	fprintf(fp,"<%s %d %d>", name.c_str(), group, phone);
}

void record::destroy () {
	group=0;
    phone=0;
}

int record::init (const std::string& str, const int g, const int p){
	group=g; phone=p; name=str;
	return 0; 
}

void record::reinit(const std::string& iname, const int igroup, const int iphone){
    init(iname, igroup, iphone);
}

int record::get_group() const{
    return group;
}

std::string& record::get_name(){
    return name;
}

int record::get_phone()const {
    return phone;
}

void record::set_name(const std::string& iname){
    name=iname;
}

void record::set_group(const int igroup){
    group = igroup;
}

void record::set_phone(const int iphone){
    phone=iphone;
}

bool record::operator>(const record &x) const noexcept{
    return (x<*this);
}

bool record::operator<(const record &x) const noexcept{
    return (name<x.name && group<x.group && phone<x.phone);
}

record& record::operator=(const record& irec){
    if(this==&irec) return *this;
    name=irec.name; group=irec.group; phone=irec.phone;
    return *this; 
}

bool record::operator!=(const record& rec) const noexcept{
    return !(*this==rec);
}

bool record::operator==(const record& rec) const noexcept{
    return (rec.name==name && rec.group==group && rec.phone==phone);
}