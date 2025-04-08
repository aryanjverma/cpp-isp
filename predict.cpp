#include <iostream>
#include <tuple>
#include <vector>
#include <fstream>
#include "network.hpp"
#include "image_reader.hpp"
#include "print.hpp"

using namespace std;
vector<float> process_image(string path);
void load_model(NeuralNetwork& nn, string filename);
void save_model(NeuralNetwork nn, string filename);
int main(){    
    vector<int> layer_sizes = {10};
    NeuralNetwork nn(layer_sizes, 784);
    load_model(nn, "model.txt");
    
    string path;
    cout << "Enter image path: ";
    getline(cin, path);
    vector<float> image = process_image(path);
    vector<float> prediction = nn.feed_forward(image);
    cout << "Prediction: \n";
    print(prediction);
    return 0;
}
vector<float> process_image(string path){
    vector<float> image = read_image(path);
    return image;
}
void load_model(NeuralNetwork& nn, string filename){
    cout << "Loading model\n";
    fstream myFile;
    myFile.open(filename,ios::in);
    if(myFile.is_open()){
        int layer_size;
        myFile >> layer_size;
        vector<int> layer_sizes;
        for(int i = 0; i < layer_size; i++){
            int size;
            myFile >> size;
            layer_sizes.push_back(size);
        }
        int input_size;
        myFile >> input_size;
        nn = NeuralNetwork(layer_sizes, input_size);
        for(int i = 0; i < nn.biases.size(); i++){
            for(int j = 0; j < nn.biases[i].size(); j++){
                float bias;
                myFile >> bias;
                nn.biases[i][j] = bias;
            }
        }
        for(int i = 0; i < nn.weights.size(); i++){
            for(int j = 0; j < nn.weights[i].size(); j++){
                for(int k = 0; k < nn.weights[i][j].size(); k++){
                    float weight;
                    myFile >> weight;
                    nn.weights[i][j][k] = weight;
                }
            }
        }
        cout << "Model loaded\n";
    }
    else{
        cout << "error: unable to open file";
    }
    myFile.close();
    return;
}
void save_model(NeuralNetwork nn, string filename){
    cout << "Saving model\n";
    fstream myFile;
    myFile.open(filename,ios::out);
    if(myFile.is_open()){
        myFile << nn.layer_sizes.size() << '\n';
        for(int i = 0; i < nn.layer_sizes.size(); i++){
            myFile << nn.layer_sizes[i] << '\n';
        }
        myFile << nn.input_size << '\n';
        for(int i = 0; i < nn.biases.size(); i++){
            for(int j = 0; j < nn.biases[i].size(); j++){
                myFile << nn.biases[i][j] << '\n';
            }
        }
        for(int i = 0; i < nn.weights.size(); i++){
            for(int j = 0; j < nn.weights[i].size(); j++){
                for(int k = 0; k < nn.weights[i][j].size(); k++){
                    myFile << nn.weights[i][j][k] << '\n';
                }
            }
        }
        cout << "Model saved\n";
    }
    else{
        cout << "error: unable to open file";
    }
    myFile.close();
    return;
}