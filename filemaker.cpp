#include <cstdlib>
#include <cstdio>
#include <ctime>
//we don't need to make a new file every time we run test
//c-style work
int main() {
    FILE * out;
    char * name = new char[10];
    int group, number;
    size_t numRecords=10;
    out = fopen("out1.txt", "w");
    //if !fopen... if !name... if !inData...

    srand( time(0) );

    name[7]='\0';
    for(size_t i=0;i<numRecords;i++){ //looks like a mess
        group = rand()%600+101;
        number = (rand()%10000+rand()%100+111)%10000+10000; //5 digits only
        for(size_t j=0;j<7;j++)
            name[j]=rand()%93+33;
        fprintf(out, "%s %d %d\n", name, group, number);
    }
    fclose(out);
    delete[] name;
    return 0;
}