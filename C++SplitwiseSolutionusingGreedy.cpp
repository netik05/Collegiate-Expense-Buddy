#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <array>
#include <limits>
#include <sstream>

using namespace std;

void line(int n) {
    while (n--) cout << "-";
}

struct Expense {
    string payer;
    string category;
    float amount;
    vector<string> participants;
};

struct Transfer {
    string from;
    string to;
    float amount;
};

class Group {
    string groupName;
    int number_of_persons;
    vector<vector<float> > graph;
    map<string, int> peopleArr;
    map<string, int> peopleIncludedInExpenseArr;
    vector<Expense> expenseHistory;
    vector<Transfer> transferHistory;

public:
    Group(string, int, map<string, int>);
    int get_Minimum(const vector<float>& array);
    int get_Maximum(const vector<float>& array);
    float minimum_of_two(float number1, float number2);
    string findNameForIndex(int index);
    void minCashFlowRecursion(vector<float>& amount);
    void minCashFlow();
    void settle();
    void transfer();
    void addExpense();
    void generateReport();
    void editExpense();
    void deleteExpense();
    void editTransfer();
    void deleteTransfer();
};

Group::Group(string s, int num_people, map<string, int> m) : graph(num_people, vector<float>(num_people, 0.0f)) {
    groupName = s;
    number_of_persons = num_people;
    peopleArr = m;

    // Initialize graph values to 0
    for (int i = 0; i < number_of_persons; i++)
        for (int j = 0; j < number_of_persons; j++)
            graph[i][j] = 0;
}

int Group::get_Minimum(const vector<float>& array) {
    int min_Index = 0;
    for (int i = 1; i < number_of_persons; i++) {
        if (array[i] < array[min_Index])
            min_Index = i;
    }
    return min_Index;
}

int Group::get_Maximum(const vector<float>& array) {
    int max_Index = 0;
    for (int i = 1; i < number_of_persons; i++) {
        if (array[i] > array[max_Index])
            max_Index = i;
    }
    return max_Index;
}

float Group::minimum_of_two(float number1, float number2) {
    return (number1 < number2) ? number1 : number2;
}

string Group::findNameForIndex(int index) {
    for (auto it_type = peopleArr.begin(); it_type != peopleArr.end(); it_type++) {
        if (it_type->second == index) {
            return it_type->first;
        }
    }
    return "";
}

void Group::minCashFlowRecursion(vector<float>& amount) {
    int maxCredit = get_Maximum(amount), maxDebit = get_Minimum(amount);

    if (amount[maxCredit] <= 0.01 && amount[maxDebit] <= 0.01)
        return;

    float minimum = minimum_of_two(-amount[maxDebit], amount[maxCredit]);
    amount[maxCredit] -= minimum;
    amount[maxDebit] += minimum;

    cout << findNameForIndex(maxDebit) << " pays " << minimum << " to " << findNameForIndex(maxCredit) << "." << endl;

    minCashFlowRecursion(amount);
}

void Group::minCashFlow() {
    vector<float> amount(number_of_persons, 0.0f);

    for (int p = 0; p < number_of_persons; p++) {
        for (int i = 0; i < number_of_persons; i++) {
            amount[p] += (graph[i][p] - graph[p][i]);
        }
    }

    minCashFlowRecursion(amount);
}

void Group::settle() {
    line(64);
    cout << endl << endl;
    minCashFlow();
    cout << endl << endl;
    line(64);
    cout << endl << endl;
}

void Group::transfer() {
    cout << "Who transferred: ";
    string personWhoTransferred;
    cin >> personWhoTransferred;

    if (peopleArr.find(personWhoTransferred) == peopleArr.end()) {
        cout << "Person not found in the group. Transfer cancelled." << endl;
        return;
    }

    string transferredTo;
    cout << "Transferred to: ";
    cin >> transferredTo;

    if (peopleArr.find(transferredTo) == peopleArr.end()) {
        cout << "Person not found in the group. Transfer cancelled." << endl;
        return;
    }

    cout << "How much transferred: ";
    float transferMoney;
    while (!(cin >> transferMoney) || transferMoney <= 0) {
        cout << "Invalid input. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    graph[peopleArr[transferredTo]][peopleArr[personWhoTransferred]] += transferMoney;

    Transfer transfer = {personWhoTransferred, transferredTo, transferMoney};
    transferHistory.push_back(transfer);
}

void Group::addExpense() {
    cout << "Enter expense amount to group " << groupName << " : ";
    float expense;
    while (!(cin >> expense) || expense <= 0) {
        cout << "Invalid input. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Who paid: ";
    string personWhoPaid;
    cin >> personWhoPaid;

    if (peopleArr.find(personWhoPaid) == peopleArr.end()) {
        cout << "\nWrong name entered. Expense addition cancelled. \n";
        return;
    }

    int n;
    cout << "Enter the number of people paid for: ";
    while (!(cin >> n) || n <= 0 || n > number_of_persons) {
        cout << "Invalid input. Please enter a number between 1 and " << number_of_persons << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter the category of the expense: ";
    string category;
    cin >> category;

    vector<string> participants;
    for (int i = 0; i < n; i++) {
        string peop;
        cout << "Enter " << i + 1 << " person: ";
        cin >> peop;
        if (peopleArr.find(peop) == peopleArr.end()) {
            cout << "Wrong name entered. Enter again.\n\n";
            return;
        }
        participants.push_back(peop);
    }

    float eachPersonExpense = expense / n;

    for (const auto& participant : participants) {
        graph[peopleArr[participant]][peopleArr[personWhoPaid]] += eachPersonExpense;
    }

    Expense newExpense = {personWhoPaid, category, expense, participants};
    expenseHistory.push_back(newExpense);
}

void Group::generateReport() {
    cout << "\n********** Expense Report and Transfer History for Group " << groupName << " **********\n\n";

    if (!expenseHistory.empty()) {
        cout << "Expense History:\n";
        for (size_t i = 0; i < expenseHistory.size(); ++i) {
            const Expense& exp = expenseHistory[i];
            cout << "Expense " << i + 1 << ":\n";
            cout << "  Payer: " << exp.payer << "\n";
            cout << "  Category: " << exp.category << "\n";
            cout << "  Amount: " << exp.amount << "\n";
            cout << "  Participants: ";
            for (const auto& participant : exp.participants) {
                cout << participant << " ";
            }
            cout << "\n\n";
        }
    } else {
        cout << "No expenses recorded.\n";
    }

    if (!transferHistory.empty()) {
        cout << "Transfer History:\n";
        for (size_t i = 0; i < transferHistory.size(); ++i) {
            const Transfer& transfer = transferHistory[i];
            cout << "Transfer " << i + 1 << ":\n";
            cout << "  From: " << transfer.from << "\n";
            cout << "  To: " << transfer.to << "\n";
            cout << "  Amount: " << transfer.amount << "\n\n";
        }
    } else {
        cout << "No transfers recorded.\n";
    }
}

void Group::editExpense() {
    generateReport();
    cout << "Enter the index of the expense to edit: ";
    int index;
    while (!(cin >> index) || index < 1 || index > expenseHistory.size()) {
        cout << "Invalid input. Please enter a valid index: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Expense& exp = expenseHistory[index - 1];
    cout << "Editing Expense " << index << ":\n";
    cout << "  Current Payer: " << exp.payer << "\n";
    cout << "  Enter new payer (or press Enter to keep the same): ";
    string newPayer;
    cin.ignore();
    getline(cin, newPayer);
    if (!newPayer.empty()) {
        exp.payer = newPayer;
    }

    cout << "  Current Category: " << exp.category << "\n";
    cout << "  Enter new category (or press Enter to keep the same): ";
    string newCategory;
    getline(cin, newCategory);
    if (!newCategory.empty()) {
        exp.category = newCategory;
    }

    cout << "  Current Amount: " << exp.amount << "\n";
    cout << "  Enter new amount (or press Enter to keep the same): ";
    string newAmountStr;
    getline(cin, newAmountStr);
    if (!newAmountStr.empty()) {
        float newAmount = stof(newAmountStr);
        exp.amount = newAmount;
    }

    cout << "  Current Participants: ";
    for (const auto& participant : exp.participants) {
        cout << participant << " ";
    }
    cout << "\n";
    cout << "  Enter new participants separated by space (or press Enter to keep the same): ";
    string newParticipantsStr;
    getline(cin, newParticipantsStr);
    if (!newParticipantsStr.empty()) {
        vector<string> newParticipants;
        stringstream ss(newParticipantsStr);
        string participant;
        while (ss >> participant) {
            newParticipants.push_back(participant);
        }
        exp.participants = newParticipants;
    }
}

void Group::deleteExpense() {
    generateReport();
    int index;
    cout << "Enter the index of the expense to delete: ";
    while (!(cin >> index) || index < 1 || index > expenseHistory.size()) {
        cout << "Invalid input. Please enter a valid index: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    expenseHistory.erase(expenseHistory.begin() + index - 1);
}

void Group::editTransfer() {
    generateReport();
    cout << "Enter the index of the transfer to edit: ";
    int index;
    while (!(cin >> index) || index < 1 || index > transferHistory.size()) {
        cout << "Invalid input. Please enter a valid index: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Transfer& transfer = transferHistory[index - 1];
    cout << "Editing Transfer " << index << ":\n";
    cout << "  Current Sender: " << transfer.from << "\n";
    cout << "  Enter new sender (or press Enter to keep the same): ";
    string newSender;
    cin.ignore();
    getline(cin, newSender);
    if (!newSender.empty()) {
        transfer.from = newSender;
    }

    cout << "  Current Receiver: " << transfer.to << "\n";
    cout << "  Enter new receiver (or press Enter to keep the same): ";
    string newReceiver;
    getline(cin, newReceiver);
    if (!newReceiver.empty()) {
        transfer.to = newReceiver;
    }

    cout << "  Current Amount: " << transfer.amount << "\n";
    cout << "  Enter new amount (or press Enter to keep the same): ";
    string newAmountStr;
    getline(cin, newAmountStr);
    if (!newAmountStr.empty()) {
        float newAmount = stof(newAmountStr);
        transfer.amount = newAmount;
    }
}

void Group::deleteTransfer() {
    generateReport();
    int index;
    cout << "Enter the index of the transfer to delete: ";
    while (!(cin >> index) || index < 1 || index > transferHistory.size()) {
        cout << "Invalid input. Please enter a valid index: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    transferHistory.erase(transferHistory.begin() + index - 1);
}

map<string, Group*> groups;

void displayMenu(string group) {
    Group* g1 = groups[group];
    int option = 0;
    do {
        cout << "\n\n******** MAIN MENU ********";
        cout << "\n\n 1. Add expense to " << group;
        cout << "\n 2. Transfer in " << group;
        cout << "\n 3. Settle up " << group;
        cout << "\n 4. Generate and Display Expense Report and Transfer History";
        cout << "\n 5. Edit an Expense";
        cout << "\n 6. Delete an Expense";
        cout << "\n 7. Edit a Transfer";
        cout << "\n 8. Delete a Transfer";
        cout << "\n 9. Exit \n";
        line(64);
        cout << "\nEnter your option: ";
        while (!(cin >> option) || option < 1 || option > 9) {
            cout << "Invalid input. Please enter a number between 1 and 9: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (option) {
        case 1:
            g1->addExpense();
            break;
        case 2:
            g1->transfer();
            break;
        case 3:
            g1->settle();
            break;
        case 4:
            g1->generateReport();
            break;
        case 5:
            g1->editExpense();
            break;
        case 6:
            g1->deleteExpense();
            break;
        case 7:
            g1->editTransfer();
            break;
        case 8:
            g1->deleteTransfer();
            break;
        default:
            break;
        }

    } while (option != 9);
    cout << "\n\n\n\n";
}

void formNewGroup() {
    string group;
    do {
        cout << "Enter name of the group: ";
        cin >> group;

        if (groups.find(group) != groups.end()) {
            cout << "Group already exists. Enter a new group: ";
        }
    } while (groups.find(group) != groups.end());

    int num_people;
    cout << "Enter the number of people in group " << group << " : ";
    while (!(cin >> num_people) || num_people <= 0) {
        cout << "Invalid input. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string name;
    map<string, int> people;
    cout << "Enter the names of people in group " << group << " : ";
    for (int i = 0; i < num_people; i++) {
        cin >> name;
        people[name] = i;
    }

    Group* g = new Group(group, num_people, people);
    groups[group] = g;

    displayMenu(group);
}

void enterOldGroup() {
    if (groups.empty()) {
        cout << "\n\nNo groups formed. Form a new group first.\n";
        return;
    }

    string name;

    do {
        cout << "\n\nEnter group name: ";
        cin >> name;
        if (groups.find(name) == groups.end()) {
            cout << "No such group exists. Enter again: ";
        }
    } while (groups.find(name) == groups.end());
    displayMenu(name);
}

void initialDisplayMenu() {
    int choice;
    do {
        line(64);
        cout << "\n WELCOME TO SPLITTER \n";
        line(64);

        cout << "\n\n1. Add a new group";
        cout << "\n2. Enter in old group";
        cout << "\n3. Exit\n";
        line(64);
        cout << "\nEnter your choice: ";
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Invalid input. Please enter a number between 1 and 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
        case 1:
            formNewGroup();
            break;
        case 2:
            enterOldGroup();
            break;
        default:
            break;
        }

    } while (choice != 3);
}

int main() {
    initialDisplayMenu();
    return 0;
}
