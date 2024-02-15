#include <iostream>
#include <utility>
#include <set>
#include <fstream>
#include "brain.hpp"
using namespace std;
 
void Brain::CreatCity()
{
    // std::ifstream Taxi_in ("Taxi.txt");
    // std::ifstream Bus_in ("Bus.txt");
    // int index;
    // string node;
    // pair<int, string>name_index;
    // set<pair<int,string>>set1;
    // if(Taxi_in.is_open())
    // {
    //     while(Taxi_in >> node >> index )
    //     {
    //         name_index.first = index;
    //         name_index.second = node;
    //         set1.insert(name_index);
    //     }
    // }

    // else 
    // cout << "Error occured while opening file"<<endl;

    // if(Bus_in.is_open())
    // {
    //     while(Bus_in >> node >> index )
    //     {
    //         name_index.first = index;
    //         name_index.second = node;
    //         set1.insert(name_index);
    //     }
    // }

    // else 
    // cout << "Error occured while opening file"<<endl;
    Path p;
    p.read_file();
    cout<< p.index_to_string_convertor(54) << " " << p.string_to_index_convertor("Meydan-eShohada"); 

}

int main()
{
    Brain test;
}


