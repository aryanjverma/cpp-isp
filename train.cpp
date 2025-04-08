#include <iostream>
#include <tuple>
#include <vector>
#include <fstream>
#include "network.hpp"
#include "image_reader.hpp"
#include "print.hpp"

using namespace std;
tuple<vector<vector<float>>,vector<vector<float>>> generate_inputs_and_outputs(int amount_per_class);
tuple<vector<vector<float>>,vector<vector<float>>> test_generate_inputs_and_outputs(int amount_per_class);
vector<float> generate_output(int value);
void save_model(NeuralNetwork nn, string filename);
void load_model(NeuralNetwork& nn, string filename);
int main(){
    
    vector<int> layer_sizes = {10};
    NeuralNetwork nn(layer_sizes, 784);
    
    cout << "\n";
    //load_model(nn, "model.txt");
    
    int amount_per_class = 100;
    
    vector<vector<float>> inputs;
    vector<vector<float>> outputs;
    tie(inputs, outputs) = generate_inputs_and_outputs(amount_per_class);
    
    nn.train(inputs, outputs, 30, 0.001);
    
    save_model(nn, "model.txt");
    
    return 0;
    
}
tuple<vector<vector<float>>,vector<vector<float>>> generate_inputs_and_outputs(int amount_per_class){
    vector<vector<float>> inputs;
    vector<vector<float>> outputs;
    for(int j = 0; j < amount_per_class; j++){
        for(int i = 0; i < 10; i++){
            vector<float> input = read_image("dataset/" + to_string(i) + "/" + to_string(i) + "/" + to_string(j) + ".png");
            inputs.push_back(input);
            outputs.push_back(generate_output(i));
        }
    }
    return make_tuple(inputs, outputs);
}
tuple<vector<vector<float>>,vector<vector<float>>> test_generate_inputs_and_outputs(int amount_per_class){
    vector<vector<float>> inputs;
    vector<vector<float>> outputs;
    for(int j = 0; j < amount_per_class; j++){
        for(int i = 0; i < 10; i++){
            vector<float> input = read_image("dataset/" + to_string(i) + "/" + to_string(i) + "/" + to_string(j) + ".png");
            inputs.push_back(input);
            if(i == 0){
                outputs.push_back({1});
            }
            else{
                outputs.push_back({0});
            }
        }
    }
    return make_tuple(inputs, outputs);
}


vector<float> generate_output(int value){
    vector<float> output(10, 0);
    output[value] = 1;
    return output;
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