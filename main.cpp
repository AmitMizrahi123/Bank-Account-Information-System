#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class BankAccount 
{
private:
    int accountNumber;
    char firstName[20];
    char lastName[20];
    float totalBalance;
public:
    static int numberOfAccounts;
    int getAccountNumber() { return accountNumber; }
    char* getFirstName() { return firstName; }
    char* getLastName() { return lastName; }
    float getTotalBalance() { return totalBalance; }
    void setAccountNumber(int accountNumber) { this->accountNumber = accountNumber; }
    void setFirstName(string firstName) { this->firstName[20] = (char)firstName[20]; }
    void setLastName(string lastName) { this->lastName[20] = (char)lastName[20]; }
    void setTotalBalance(float totalBalance) { this->totalBalance = totalBalance; }
    void updateData(vector<int> &arrAccountNumber, vector<string> &arrFirstName, vector<string> &arrLastName, vector<float> &arrTotalBalnce);
    void insertDataToList(vector<int> &arrAccountNumber, vector<string> &arrFirstName, vector<string> &arrLastName, vector<float> &arrTotalBalnce, int accountNumber, string firstName, string lastName, float balance);
    bool insertData(vector<int> arrAccountNumber);
    void showData();
    void writeRec(vector<int> arrAccountNumber);
    void readRec();
    void searchRec(vector<int> arrAccountNumber, vector<string> arrFirstName, vector<string> arrLastName, vector<float> &arrTotalBalnce);
    void editRec(vector<int> arrAccountNumber, vector<string> arrFirstName, vector<string> arrLastName, vector<float> &arrTotalBalnce);
    void deleteRec(vector<int> arrAccountNumber, vector<string> arrFirstName, vector<string> arrLastName, vector<float> &arrTotalBalnce);
};

int BankAccount::numberOfAccounts = 0;

void BankAccount::insertDataToList(vector<int> &arrAccountNumber, vector<string> &arrFirstName, vector<string> &arrLastName, vector<float> &arrTotalBalnce, int accountNumber, string firstName, string lastName, float balance)
{
    arrAccountNumber.push_back(this->accountNumber);
    arrFirstName.push_back(this->firstName);
    arrLastName.push_back(this->lastName);
    arrTotalBalnce.push_back(this->totalBalance);
}

void BankAccount::updateData(vector<int> &arrAccountNumber, vector<string> &arrFirstName, vector<string> &arrLastName, vector<float> &arrTotalBalnce)
{
    ifstream inFile;
    inFile.open("record.bank", ios::binary);
    if (!inFile) {
        return;
    }
    if (arrAccountNumber.size() != 0) {
        arrAccountNumber.clear();
        arrFirstName.clear();
        arrLastName.clear();
        arrTotalBalnce.clear();
    }
    while (!inFile.eof()) {
        if(inFile.read(reinterpret_cast<char*>(this), sizeof(*this))) {
            insertDataToList(arrAccountNumber, arrFirstName, arrLastName, arrTotalBalnce, this->accountNumber, this->firstName, this->lastName, this->totalBalance);
        }
    }
}

bool BankAccount::insertData(vector<int> arrAccountNumber)
{
    int tempAccountNumber;
    cout << "Enter Account Number: ";
    cin >> tempAccountNumber;
    for (int i = 0; i < arrAccountNumber.size(); i++) {
        if (tempAccountNumber == arrAccountNumber[i]) {
            cout << "The account number is already exists";
            return false;
        }
    }
    this->setAccountNumber(tempAccountNumber);
    cout << "Enter First Name: ";
    cin >> this->firstName;
    this->firstName[0] = toupper(this->firstName[0]);
    cout << "Enter Last Name: ";
    cin >> this->lastName;
    this->lastName[0] = toupper(this->lastName[0]);
    cout << "Enter Balance: ";
    cin >> this->totalBalance;
    return true;
}

void BankAccount::showData()
{
    cout << "-------------------------------" << endl;
    cout << "Account Number: " << this->getAccountNumber() << endl;
    cout << "First Name: " << this->getFirstName() << endl;
    cout << "Last Name: " << this->getLastName() << endl;
    cout << "Your Balance: " << this->getTotalBalance() << endl;
    cout << "-------------------------------" << endl;
}

void BankAccount::writeRec(vector<int> arrAccountNumber)
{
    bool flagWrite;
    ofstream outFile;
    outFile.open("record.bank", ios::binary|ios::app);
    flagWrite = this->insertData(arrAccountNumber);
    if (flagWrite) {
        outFile.write(reinterpret_cast<char *>(this), sizeof(*this)); // Change to pointer this to char* for writing to file
        BankAccount::numberOfAccounts++;
    }
    outFile.close();
}

void BankAccount::readRec()
{
    ifstream inFile;
    int count;
    inFile.open("record.bank", ios::binary);
    if (!inFile) {
        cout << "Error in opening! File not found" << endl;
        return;
    }
    cout << "\n****Data from file****\n\n";
    while (!inFile.eof()) {
        if(inFile.read(reinterpret_cast<char*>(this), sizeof(*this))) {
            this->showData();
        }
    }
    inFile.close();
}

void BankAccount::searchRec(vector<int> arrAccountNumber, vector<string> arrFirstName, vector<string> arrLastName, vector<float> &arrTotalBalnce)
{
    int option;
    ifstream inFile;
    inFile.open("record.bank", ios::binary);
    if (!inFile) {
        cout << "\nError in opening! File not fo ! Cause no account" << endl;
        return;
    }
    if (!BankAccount::numberOfAccounts) {
        cout << "There are no account right now";
        return;
    }
    updateData(arrAccountNumber, arrFirstName, arrLastName, arrTotalBalnce);
    inFile.seekg(0, ios::end);
    cout << "\nThere are " << BankAccount::numberOfAccounts << " record in the file\n";
    for (int i = 0; i < BankAccount::numberOfAccounts; i++) {
        cout << i + 1 << "---> Account number: " << arrAccountNumber[i] << ", Name: " << arrFirstName[i] << " " << arrLastName[i] << endl;
    }
    cout << "\nEnter Record Number to Search: ";
    cin >> option;
    inFile.seekg((option-1)*sizeof(*this));
    inFile.read(reinterpret_cast<char*>(this), sizeof(*this));
    this->showData();
}

void BankAccount::editRec(vector<int> arrAccountNumber, vector<string> arrFirstName, vector<string> arrLastName, vector<float> &arrTotalBalnce)
{
    int option, update, changeAccountNumber, i;
    char changeFirstName[20], changeLastName[20];
    float changeBalance;
    fstream ioFile;
    ioFile.open("record.bank", ios::in|ios::binary);
    if(!ioFile) {
        cout<<"\nError in opening! File Not Found! Cause no account"<<endl;
        return;
    }
    if (!BankAccount::numberOfAccounts) {
        cout << "There are no account right now" << endl;
        return;
    }
    updateData(arrAccountNumber, arrFirstName, arrLastName, arrTotalBalnce);
    ioFile.seekg(0, ios::end);
    cout << "\nThere are " << BankAccount::numberOfAccounts << " record in the file\n";
    for (i = 0; i <= BankAccount::numberOfAccounts; i++) {
        if (i == BankAccount::numberOfAccounts) {
            cout << i + 1 << "---> Cancel update" << endl; 
        } else {
            cout << i + 1 << "---> Account number: " << arrAccountNumber[i] << ", Name: " << arrFirstName[i] << " " << arrLastName[i] << endl;
        }
    }
    cout << "\nEnter Record Number to Update: ";
    cin >> option;
    if (option == i) {
        cout << "Cancel update" << endl;
        return;
    }
    ioFile.seekg((option-1)*sizeof(*this));
    ioFile.read(reinterpret_cast<char*>(this), sizeof(*this));
    cout << "Record " << option << " has following data" << endl;
    this->showData();
    ioFile.close();
    ioFile.open("record.bank", ios::out|ios::in|ios::binary);
    ioFile.seekp((option-1)*sizeof(*this));
    cout << "\nEnter update field" << endl;
    cout << "1---> Account number" << endl;
    cout << "2---> First name" << endl;
    cout << "3---> Last name" << endl;
    cout << "4---> Balance" << endl;
    cout << "5---> Cancel update" << endl;
    cout << "Enter your choice: ";
    cin >> update;
    switch (update)
    {
    case 1:
        cout << "Enter new account number: ";
        cin >> changeAccountNumber;
        this->setAccountNumber(changeAccountNumber);
        break;
    case 2:
        cout << "Enter new first name: ";
        cin >> changeFirstName;
        changeFirstName[0] = toupper(changeFirstName[0]);
        this->setFirstName(changeFirstName);
        break;
    case 3:
        cout << "Enter new last name: ";
        cin >> changeLastName;
        changeLastName[0] = toupper(changeLastName[0]);
        this->setLastName(changeLastName);
        break;
    case 4:
        cout << "Enter new balance: ";
        cin >> changeBalance;
        this->setTotalBalance(changeBalance);
        break;
    case 5:
        cout << "Cancel update";
        return;
    default:
        cout << "Error update field";
        return;
    }
    ioFile.write(reinterpret_cast<char*>(this), sizeof(*this));
    this->showData();
}

void BankAccount::deleteRec(vector<int> arrAccountNumber, vector<string> arrFirstName, vector<string> arrLastName, vector<float> &arrTotalBalnce)
{
    int option, i;
    ifstream inFile;
    fstream tmpfile;
    inFile.open("record.bank", ios::binary);
    if(!inFile) {
        cout << "\nError in opening! File Not Found! Cause no account" << endl;
        return;
    }
    if (!BankAccount::numberOfAccounts) {
        cout << "There are no account right now" << endl;
        return;
    }
    updateData(arrAccountNumber, arrFirstName, arrLastName, arrTotalBalnce);
    inFile.seekg(0,ios::end);
    cout << "\nThere are " << BankAccount::numberOfAccounts << " record in the file\n";
    for (i = 0; i <= BankAccount::numberOfAccounts; i++) {
        if (i == BankAccount::numberOfAccounts) {
            cout << i + 1 << "---> Cancel Delete" << endl; 
        } else {
            cout << i + 1 << "---> Account number: " << arrAccountNumber[i] << ", Name: " << arrFirstName[i] << " " << arrLastName[i] << endl;
        }
    }
    cout << "\nEnter Record Number to Delete: ";
    cin >> option;
    if (option == i) {
        cout << "cancel Delete" << endl;
        return;
    } else {
        tmpfile.open("tmpfile.bank", ios::out|ios::binary);
        inFile.seekg(0);
        for(int i=0; i < arrAccountNumber.size(); i++) {
            inFile.read(reinterpret_cast<char*>(this),sizeof(*this));
            if(i == (option - 1)) {
                continue;
            }
            tmpfile.write(reinterpret_cast<char*>(this), sizeof(*this));
        }
        inFile.close();
        tmpfile.close();
        remove("record.bank");
        rename("tmpfile.bank", "record.bank");
        this->numberOfAccounts--;
    }
}

int main()
{
    BankAccount A;
    vector<int> arrAccountNumber;
    vector<string> arrFirstName;
    vector<string> arrLastName;
    vector<float> arrTotalBalance;
    A.updateData(arrAccountNumber, arrFirstName, arrLastName, arrTotalBalance);
    BankAccount::numberOfAccounts = arrAccountNumber.size();
    int option;
    cout << "Account information system\n";
    while(true) {
        cout << "\nThere are " << BankAccount::numberOfAccounts << " account in the bank right now";
        cout << "\nSelect ine option below" << endl;
        cout << "1--->Add record to file" << endl;
        cout << "2--->Show record from file" << endl;
        cout << "3--->Search Record from file" << endl;
        cout << "4--->Update Record" << endl;
        cout << "5--->Delete Records" << endl;
        cout << "6--->Quit" << endl;
        cout << "Enter your choice: ";
        cin >> option;
        switch(option)
        {
        case 1:
            cout << endl;
            A.writeRec(arrAccountNumber);
            break;
        case 2:
            A.readRec();
            break;
        case 3:
            A.searchRec(arrAccountNumber, arrFirstName, arrLastName, arrTotalBalance);
            break;
        case 4:
            A.editRec(arrAccountNumber, arrFirstName, arrLastName, arrTotalBalance);
            break;
        case 5:
            A.deleteRec(arrAccountNumber, arrFirstName, arrLastName, arrTotalBalance);
            break;
        case 6:
            exit(0);
            break;
        default:
            cout<<"\nEnter corret choice";
            exit(0);
        }
    }
}