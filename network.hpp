#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>
#include <random>
#include <valarray>
#include <algorithm>
#include "print.hpp"


using namespace std;
float generate_random_float(float min, float max) {
    // Use a static random number generator for efficiency
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

vector<float> generate_random_array(int size, float min = 0.0f, float max = 1.0f) {
    // Generate a random array of floats between min and max
    vector<float> array;
    for(int i=0; i < size; i++){
        array.push_back(generate_random_float(min,max));
    }
    return array;
}

class NeuralNetwork{
public:
    vector<int> layer_sizes;
    int input_size;
    vector<vector<vector<float>>> weights;
    vector<vector<float>> biases;
    NeuralNetwork(vector<int> Layer_Sizes, int Input_Size){
        layer_sizes = Layer_Sizes;
        input_size = Input_Size;
        init();
    }
    vector<float> feed_forward(vector<float> input){
        // Runs the input through the network and returns the output
        for(int i = 0 ;i < layer_sizes.size(); i++){
            input = sigmoid(add(dot(input,weights[i]),biases[i]));
        }
        return input;
    }
    void train(vector<vector<float>> inputs, vector<vector<float>> outputs, int epochs, float learning_rate, float weight_decay = 0.0f){
        float MSE;
        
        for(int i = 0; i < epochs; i++){
            MSE = 0.0f;
            
            cout << "Epoch number: " << i + 1 << "\n";
            for(int j = 0; j < inputs.size(); j++){
                cout << "Image number: " << j << "\n";
                vector<float> input = inputs[j];
                vector<float> output = outputs[j];
                vector<vector<float>> activations;  // Not used in original code
                vector<vector<float>> layer_inputs;
                vector<vector<float>> layer_outputs;
                
                // Forward pass - store all layer inputs and outputs for backprop
                activations.push_back(input); // Store input as first activation
                
                for(int k = 0; k < layer_sizes.size(); k++){
                    if (k == 0){
                        if(layer_sizes.size() == 1){
                            layer_inputs.push_back(dot(input, weights[0]));
                            layer_outputs.push_back(sigmoid(add(layer_inputs[k], biases[k])));
                        }
                        else{
                            
                            layer_inputs.push_back(dot(input, weights[0]));
                            
                            layer_outputs.push_back(relu(add(layer_inputs[k], biases[k])));
                        }
                    }
                    else{
                        if(k == layer_sizes.size()-1){
                            
                            layer_inputs.push_back(dot(layer_outputs[k-1], weights[k]));
                            layer_outputs.push_back(sigmoid(add(layer_inputs[k], biases[k])));
                        }
                        else{
                            layer_inputs.push_back(dot(layer_outputs[k-1], weights[k]));
                            layer_outputs.push_back(relu(add(layer_inputs[k], biases[k])));
                        }
                    }
                    
                    activations.push_back(layer_outputs[k]); // Store output as next activation
                }
                
                cout << "Pass through complete\n";
                print(layer_outputs[0]);
                
                
                // Calculate error and MSE
                
                vector<float> error = add(negate(layer_outputs.back()), output);
                print(error);
                MSE += inner_product(error.begin(), error.end(), error.begin(), 0.0f);
                
                // Backpropagation
                vector<vector<float>> deltas(layer_sizes.size());
                
                // Calculate output layer delta
                deltas[layer_sizes.size()-1].resize(layer_sizes.back());
                
                for(int l = 0; l < layer_sizes.back(); l++){
                    // delta = error * sigmoid_derivative(output)
                    deltas[layer_sizes.size()-1][l] = error[l] * layer_outputs.back()[l] * (1 - layer_outputs.back()[l]);
                    
                }
                // Calculate hidden layer deltas
                for(int k = layer_sizes.size()-2; k >= 0; k--){
                    deltas[k].resize(layer_sizes[k]);
                    for(int l = 0; l < layer_sizes[k]; l++){
                        float sum = 0.0f;
                        // Sum weighted deltas from next layer
                        for(int m = 0; m < layer_sizes[k+1]; m++){
                            sum += deltas[k+1][m] * weights[k+1][m][l];
                        }
                        // I made it leaky ReLU
                        // delta = sum * relu_derivative(output)
                        if(layer_outputs[k][l] > 0){
                            deltas[k][l] = sum;
                        } else {
                            deltas[k][l] = 0.01 * sum; // Leaky ReLU derivative
                        }
                    }
                }
                
                // Update weights and biases
                for(int k = layer_sizes.size()-1; k >= 0; k--){
                    // Update biases: bias += learning_rate * delta
                    for(int l = 0; l < layer_sizes[k]; l++){
                        biases[k][l] += learning_rate * deltas[k][l];
                    }
                    
                    // Update weights: weight += learning_rate * delta * input
                    vector<float> layer_input;
                    if(k == 0){
                        layer_input = input;
                    } else {
                        layer_input = layer_outputs[k-1];
                    }
                    
                    for(int l = 0; l < layer_sizes[k]; l++){
                        for(int m = 0; m < (k == 0 ? input_size : layer_sizes[k-1]); m++){
                            weights[k][l][m] += learning_rate * deltas[k][l] * layer_input[m] - weight_decay * weights[k][l][m];
                        }
                    }
                }
                
                cout << "Backpropagation complete\n";
                cout << "MSE: " << MSE / inputs.size() << endl;
                

            }
        }
    }
    
    
private:
    // Initialize weights and biases
    // The weights and biases are initialized to small random values
    void init(){
        for (int i = 0; i < layer_sizes.size(); i ++){
            if(i == 0){
                if(layer_sizes.size() == 1){
                    biases.push_back(generate_random_array(layer_sizes[i],-0.1,0.1));
                }
                else{
                    cout << i << " Hi\n";
                    biases.push_back(generate_random_array(layer_sizes[i],0.1,0.3));
                }
            }
            else{
                if(i == layer_sizes.size()-1){
                    biases.push_back(generate_random_array(layer_sizes[i],-0.1,0.1));
                }
                else{
                    biases.push_back(generate_random_array(layer_sizes[i],0.1,0.3));
                }                        
            }
            vector<vector<float>> layer;
            int iterations = layer_sizes[i];
            
            for(int j =0; j < iterations; j++){
                if (i == 0){
                    if(layer_sizes.size() == 1){
                        layer.push_back(generate_random_array(input_size,-0.1,0.1));
                    }
                    else{
                        cout << i << " Hi\n";
                        layer.push_back(generate_random_array(input_size,0.01,0.1));
                    }
                }
                else{
                    if(i == layer_sizes.size()-1){
                        layer.push_back(generate_random_array(layer_sizes[i-1],-0.1,0.1));
                    }
                    else{
                        exit(3);
                    layer.push_back(generate_random_array(layer_sizes[i-1],0.01,0.1));
                    }                        
                }
            }
            weights.push_back(layer);
        }

        return;
    }
    //derivative of sigmoid
    float d_sigmoid(float y){
        return sigmoid(y) * (1 - sigmoid(y));
    }
    //for vectors
    vector<float> d_sigmoid(vector<float> y){
        for(int i = 0; i < y.size(); i ++){
            y[i] = d_sigmoid(y[i]);
        }
        return y;
    }
    //technically it's leaky relu, but yeah
    float relu(float y){
        if(y < 0){
            return y * 0.01;
        }
        else{
            return y;
        }
    }
    //for vectors
    vector<float> relu(vector<float> y){
        for(int i = 0; i < y.size(); i ++){
            y[i] = relu(y[i]);
        }
        return y;
    }
    //sigmoid function
    float sigmoid(float y){
        return 1 / (1 + exp(-1 * y));
    }
    //sigmoid for vectors
    vector<float> sigmoid(vector<float> y){
        for(int i = 0; i < y.size(); i ++){
            y[i] = sigmoid(y[i]);
        }
        return y;
    }
    //dot product for 2D arrays by vectors
    vector<float> dot(vector<float> input, vector<vector<float>> weights){
        vector<float> product(weights.size(), 0.0f);
        for(int i = 0; i < weights.size(); i++){
            product[i] = inner_product(input.begin(), input.end(), weights[i].begin(), 0.0f);
        }
        return product;
    }
    //dot product for 1D arrays by vectors
    float dot(vector<float> input, vector<float> weights){
        float product = 0.0f;
        for(int i = 0; i < input.size(); i++){
            product += input[i] * weights[i];
        }
        return product;
    }
    //adding two vectors
    vector<float> add(vector<float> one, vector<float> two){
        vector<float> sum(one.size());
        for(int i = 0; i < one.size(); i++){
            if(one.size() != two.size()){
                cout << "Error: Arrays are not of same size\n";
                exit(1);
                return sum;
            }
            sum[i] = one[i] + two[i];
        }
        return sum;
    }
    //negating a vector
    vector<float> negate(vector<float> one){
        vector<float> negated(one.size());
        for(int i = 0; i < one.size(); i++){
            negated[i] = -1 * one[i];
        }
        return negated;
    }

};
#endif