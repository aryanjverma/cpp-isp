#ifndef PRINT_HPP
#define PRINT_HPP
#include <iostream>
#include <vector>

void print(std::vector<float> array){
    for(int i = 0; i < array.size(); i++){
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
}
void print(std::vector<int> array){
    for(int i = 0; i < array.size(); i++){
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
}
void print(float number){
    std::cout << number << "\n";
    std::cout << "\n";
}


#endif