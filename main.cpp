#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class BankAccount 
{
    private:
        int accountNumber;
        string firstName;
        string lastName;
        float totalBalance;
    public:
        int getAccountNumber() {
            return accountNumber;
        }
        string getFirstName() {
            return firstName;
        }
        string getLastName() {
            return lastName;
        }
        float getTotalBalance() {
            return totalBalance;
        }
        void insertData();
        void showData();
        void writeRec();
        void readRec();
        void searchRec();
        void editRec();
        void deleteRec();
};

void BankAccount::insertData()
{
    cout << "Enter Account Number: ";
    cin >> this->accountNumber;
    cout << "Enter First Name: ";
    cin >> this->firstName;
    cout << "Enter Last Name: ";
    cin >> this->lastName;
    cout << "Enter Balance: ";
    cin >> this->totalBalance;
}

void BankAccount::showData()
{
    cout << "-------------------------------" << endl;
    cout << "Account Number: " << this->getAccountNumber() << endl;
    cout << "First Name: " << this->getFirstName() << endl;
    cout << "Last Name: " << this->getLastName() << endl;
    cout << "Current Balance: " << this->getTotalBalance() << endl;
    cout << "-------------------------------" << endl;
}

void BankAccount::writeRec()
{
    ofstream outFile;
    outFile.open("record.bank", ios::binary|ios::app);
    this->insertData();
    outFile.write(reinterpret_cast<char *>(this), sizeof(*this)); // Change to pointer this to char* for writing to file
    outFile.close();
}

void BankAccount::readRec()
{
    ifstream inFile;
    inFile.open("record.bank", ios::binary);
    if (!inFile) {
        cout << "Error in opening! File not found" << endl;
        return;
    }
    cout << "\n****Data from file****" << endl;
    while (!inFile.eof()) {
        if(inFile.read(reinterpret_cast<char*>(this), sizeof(*this))) {
            this->showData();
        }
    }
    inFile.close();
}

void BankAccount::searchRec()
{
    int option;
    ifstream inFile;
    inFile.open("record.bank", ios::binary);
    if (!inFile) {
        cout << "Error in opening! File not found" << endl;
        return;
    }
    inFile.seekg(0, ios::end);
    int count = inFile.tellg() / sizeof(*this);
    cout << "\n There are " << count << " record in the file";
    cout << "\n Enter Record Number to Search: ";
    cin >> option;
    inFile.seekg((option-1)*sizeof(*this));
    inFile.read(reinterpret_cast<char*>(this), sizeof(*this));
    this->showData();
}

void BankAccount::editRec()
{
    int option;
    fstream iofile;
    iofile.open("record.bank", ios::in|ios::binary);
    if(!iofile) {
        cout<<"\nError in opening! File Not Found!!"<<endl;
        return;
    }
    iofile.seekg(0, ios::end);
    int count = iofile.tellg() / sizeof(*this);
    cout << "\n There are " << count << " record in the file";
    cout << "\n Enter Record Number to edit: ";
    cin >> option;
    iofile.seekg((option-1)*sizeof(*this));
    iofile.read(reinterpret_cast<char*>(this), sizeof(*this));
    cout << "Record " << option << " has following data" << endl;
    this->showData();
    iofile.close();
    iofile.open("record.bank", ios::out|ios::in|ios::binary);
    iofile.seekp((option-1)*sizeof(*this));
    cout << "\nEnter data to Modify " << endl;
    this->insertData();
    iofile.write(reinterpret_cast<char*>(this), sizeof(*this));
}

void BankAccount::deleteRec()
{
    int option;
    ifstream inFile;
    inFile.open("record.bank", ios::binary);
    if(!inFile) {
        cout << "\nError in opening! File Not Found!!" << endl;
        return;
    }
    inFile.seekg(0,ios::end);
    int count = inFile.tellg()/sizeof(*this);
    cout << "\n There are " << count << " record in the file";
    cout << "\n Enter Record Number to Delete: ";
    cin >> option;
    fstream tmpfile;
    tmpfile.open("tmpfile.bank", ios::out|ios::binary);
    inFile.seekg(0);
    for(int i=0; i<count; i++) {
        inFile.read(reinterpret_cast<char*>(this),sizeof(*this));
        if(i == (option-1)) {
            continue;
        }
        tmpfile.write(reinterpret_cast<char*>(this), sizeof(*this));
    }
    inFile.close();
    tmpfile.close();
    remove("record.bank");
    rename("tmpfile.bank", "record.bank");
}

int main()
{
    BankAccount A;
    int option;
    cout << "Account information system" << endl;
    while(true) {
        cout << "Select ine option below" << endl;
        cout << "1--->Add record to file" << endl;
        cout << "2--->Show record from file" << endl;
        cout << "3--->Search Record from file" << endl;
        cout << "4--->Update Record" << endl;
        cout << "5--->Delete Records" << endl;
        cout << "6--->Quit" << endl;
        cout<<"Enter your choice: ";
        cin >> option;
        switch(option)
        {
        case 1:
            A.writeRec();
            break;
        case 2:
            A.readRec();
            break;
        case 3:
            A.searchRec();
            break;
        case 4:
            A.editRec();
            break;
        case 5:
            A.deleteRec();
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