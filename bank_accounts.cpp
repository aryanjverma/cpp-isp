#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int balance;
int balanceLine;

void createAccount();
void accessAccount();
bool authenticate(string user, string pass);
void withdraw();
void deposit();
void updateAccount();

int main(){
    string response;
    while(response != "3"){
        cout << "Welcome to the bank\n";
        cout << "What do you wanna do?\n";
        cout << "1. Create account.\n";
        cout << "2. Access account.\n";
        cout << "3. Leave the bank.\n";
        cout << "Choose a number: ";
        getline(cin, response);
        if(response == "1"){
            createAccount();
        }
        else if(response == "2"){
            accessAccount();
        }
        else if(response == "3"){
            cout << "bye";
            return 0;
        }
        else{
            cout << "please enter a number 1-3\n";
        }
    }
    return 0;
}

void createAccount(){
    fstream myFile;
    myFile.open("user_info.txt",ios::app);
    if(myFile.is_open()){
        string username;
        string password;
        cout << "Print username: ";
        getline(cin, username);
        myFile << username << '\n';

        cout << "Print password: ";
        getline(cin, password);
        myFile << password << '\n';
        
        myFile << 0 << '\n';
        
        myFile.close();
    }
}

void accessAccount(){
    cout << "Sign into account\n";
    string response;
    while(true){
        string username;
        string password;
        cout << "Enter username: ";
        getline(cin,username);
        cout << "Enter password: ";
        getline(cin,password);
        bool success = authenticate(username,password);
        if(success){
            break;
        }
        else{
            while(true){
                cout << "Account credentials not found, choose an option.\n";
                cout << "1. Try again\n";
                cout << "2. Leave\n";    
                getline(cin,response);
                if(response == "1"){
                    break;
                }
                else if(response == "2"){
                    return;
                }
                else{
                    cout << "enter valid response\n";
                }
            }   
        }
    }
    while(true){
        cout << "Here is your balance: $" << balance << "\n";
        cout << "What do you wanna do?\n";
        cout << "1. Withdraw money.\n";
        cout << "2. Deposit money.\n";
        cout << "3. Exit this account\n";
        cout << "Choose a number: ";
        getline(cin, response);
        if(response == "1"){
            withdraw();
        }
        else if (response == "2"){
            deposit();
        }
        else if (response == "3"){
            cout << "Leaving account\n";
            updateAccount();
            return;
        }
        else{
            cout << "enter valid response\n";
        }
    }

}

bool authenticate(string user, string pass){
    fstream myFile;
    myFile.open("user_info.txt",ios::in);
    string line;
    if(!myFile.is_open()){
        cout << "error: unable to open file";
        return 0;
    }
    int num = 0;
    int check = 0;
    while(getline(myFile,line)){
        if (check == 2){
            balance = stoi(line);
            balanceLine = num;
            myFile.close();
            return 1;
        }
        if (check == 1 && line == pass){
            check = 2;
        }
        else{
            check = 0;
        }
        if(num % 3 ==0 && line == user){
            check = 1;
        }
        num ++;
        
    }
    myFile.close();
    return 0;

}

void withdraw(){
    string amount; 
    cout << "How much money would you like to withdraw: ";
    getline(cin, amount);
    if(stoi(amount) > balance){
        cout << "Can't withdraw that amount.\n";
        return;
    }
    else{
        balance -= stoi(amount);
        cout << "Withdrawal succesful";
        return;
    }
}

void deposit(){
    string amount; 
    cout << "How much money would you like to deposit: ";
    getline(cin, amount);
    balance += stoi(amount);
    cout << "Deposit succesful";
    return;
}

void updateAccount(){
    fstream myFile;
    myFile.open("user_info.txt",ios::in);
    string line;
    if(!myFile.is_open()){
        cout << "error: unable to open file";
        return;
    }
    vector<string> lines;
    int num = 0;
    int check = 0;
    while(getline(myFile,line)){
        lines.push_back(line);
    }
    myFile.close();
    lines[balanceLine] = to_string(balance);
    myFile.open("user_info.txt",ios::out);
    if(!myFile.is_open()){
        cout << "error: unable to open file";
        return;
    }
    for(string s : lines){
        myFile << s << '\n';
    }
    myFile.close();
    return;
}