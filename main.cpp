#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
#include <iomanip>
using namespace std;

struct MenuItem {
    string name;
    double price;
    int sold;
    bool available;
};

struct Order {
    int orderID;
    string items;
    double totalCost;
    string status;
};

int main() {
    vector<MenuItem> menu = {
        {"Espresso", 20000, 0, true},
        {"Latte", 25000, 0, true},
        {"Cappuccino", 30000, 0, true},
    };

    int sales[100][24] = {0}; // Sales data: [menu index][hour]
    queue<Order> orderQueue; // Queue for managing orders

    int role;
    do {
        cout << "Welcome to Coffee Shop Operations Management System (COMS)!\n";
        cout << "Select your role:\n1. Owner\n2. Staff\n3. Exit\nChoice: ";
        cin >> role;

        if (role == 1) {
            int choice;
            do {
                cout << "\nOwner Menu:\n";
                cout << "1. Add New Menu\n";
                cout << "2. Edit Menu Details\n";
                cout << "3. View Best-Selling Menu\n";
                cout << "4. View Daily Sales\n";
                cout << "5. Exit\n";
                cout << "Choice: ";
                cin >> choice;

                if (choice == 1) {
                    cin.ignore(); // Clear input buffer
                    string name;
                    double price;

                    cout << "\nEnter new menu name: ";
                    getline(cin, name);
                    cout << "Enter price for " << name << ": Rp ";
                    cin >> price;

                    menu.push_back({name, price, 0, true});
                    cout << name << " has been added to the menu with price Rp " << price << ".\n";
                } else if (choice == 2) {
                    cout << "\nMenu:\n";
                    for (size_t i = 0; i < menu.size(); ++i) {
                        cout << i + 1 << ". " << menu[i].name << " - Rp " << menu[i].price;
                        if (menu[i].available) {
                            cout << " (Available)";
                        } else {
                            cout << " (Not Available)";
                        }
                        cout << endl;
                    }

                    int menuChoice;
                    cout << "\nSelect menu item to edit (0 to exit): ";
                    cin >> menuChoice;

                    if (menuChoice > 0 && menuChoice <= menu.size()) {
                        int editChoice;
                        do {
                            cout << "\nEditing " << menu[menuChoice - 1].name << ":\n";
                            cout << "1. Edit Name\n";
                            cout << "2. Edit Price\n";
                            cout << "3. Update Availability\n";
                            cout << "4. Exit Editing\n";
                            cout << "Choice: ";
                            cin >> editChoice;

                            if (editChoice == 1) {
                                cin.ignore();
                                string newName;
                                cout << "Enter new name: ";
                                getline(cin, newName);
                                menu[menuChoice - 1].name = newName;
                                cout << "Name updated to " << newName << ".\n";
                            } else if (editChoice == 2) {
                                double newPrice;
                                cout << "Enter new price: Rp ";
                                cin >> newPrice;
                                menu[menuChoice - 1].price = newPrice;
                                cout << "Price updated to Rp " << newPrice << ".\n";
                            } else if (editChoice == 3) {
                                menu[menuChoice - 1].available = !menu[menuChoice - 1].available;
                                cout << "Availability updated to " << (menu[menuChoice - 1].available ? "Available" : "Not Available") << ".\n";
                            } else if (editChoice != 4) {
                                cout << "Invalid choice. Try again.\n";
                            }
                        } while (editChoice != 4);
                    } else if (menuChoice != 0) {
                        cout << "Invalid choice.\n";
                    }
                } else if (choice == 3) {
                    const MenuItem* bestSelling = nullptr;
                    for (const auto& item : menu) {
                        if (!bestSelling || item.sold > bestSelling->sold) {
                            bestSelling = &item;
                        }
                    }
                    if (bestSelling) {
                        cout << "\nBest-Selling Menu Item: " << bestSelling->name << " (" << bestSelling->sold << " sold)\n";
                    } else {
                        cout << "No sales data available.\n";
                    }
                } else if (choice == 4) {
                    cout << "\nHourly Sales:\n";
                    for (int hour = 0; hour < 24; ++hour) {
                        bool hasSales = false;
                        for (size_t i = 0; i < menu.size(); ++i) {
                            if (sales[i][hour] > 0) {
                                hasSales = true;
                                cout << "Hour " << hour << ": " << menu[i].name << " - " << sales[i][hour] << " sold\n";
                            }
                        }
                        if (!hasSales) {
                            cout << "Hour " << hour << ": No sales\n";
                        }
                    }
                } else if (choice == 5) {
                    cout << "Exiting Owner Menu.\n";
                } else {
                    cout << "Invalid choice. Try again.\n";
                }
            } while (choice != 5);
        } else if (role == 2) {
            int choice;
            do {
                cout << "\nStaff Menu:\n";
                cout << "1. Input Order\n";
                cout << "2. Update Order Status\n";
                cout << "3. View Order Queue\n";
                cout << "4. Exit\n";
                cout << "Choice: ";
                cin >> choice;

                if (choice == 1) {
                    cout << "\nMenu:\n";
                    for (size_t i = 0; i < menu.size(); ++i) {
                        cout << i + 1 << ". " << menu[i].name << " - Rp " << menu[i].price;
                        if (menu[i].available) {
                            cout << " (Available)";
                        } else {
                            cout << " (Not Available)";
                        }
                        cout << endl;
                    }

                    cin.ignore(); // Clear input buffer
                    string menuInput;
                    cout << "\nEnter the menu item numbers (e.g., 1,2,3): ";
                    getline(cin, menuInput);

                    stringstream ss(menuInput);
                    string item;

                    int hour;
                    cout << "Enter the order hour (0-23): ";
                    cin >> hour;

                    if (hour < 0 || hour > 23) {
                        cout << "Invalid hour.\n";
                        continue;
                    }

                    double totalCost = 0;
                    string itemsOrdered;
                    static int orderID = 1;

                    while (getline(ss, item, ',')) {
                        int menuChoice = stoi(item);
                        if (menuChoice < 1 || menuChoice > menu.size() || !menu[menuChoice - 1].available) {
                            cout << "Invalid or unavailable menu item: " << menuChoice << "\n";
                            continue;
                        }

                        int quantity;
                        cout << "Enter quantity for " << menu[menuChoice - 1].name << ": ";
                        cin >> quantity;

                        menu[menuChoice - 1].sold += quantity;
                        sales[menuChoice - 1][hour] += quantity;

                        double cost = menu[menuChoice - 1].price * quantity;
                        totalCost += cost;
                        itemsOrdered += menu[menuChoice - 1].name + " x" + to_string(quantity) + ", ";
                    }

                    itemsOrdered.pop_back(); // Remove trailing comma
                    itemsOrdered.pop_back();

                    cout << "\nOrder finalized. Total Cost: Rp " << totalCost << endl;

                    orderQueue.push({orderID++, itemsOrdered, totalCost, "Pending"});
                } else if (choice == 2) {
                    if (orderQueue.empty()) {
                        cout << "\nNo orders to update.\n";
                        continue;
                    }

                    Order currentOrder = orderQueue.front();
                    orderQueue.pop();

                    cout << "\nUpdating status for Order ID " << currentOrder.orderID << ":\n";
                    cout << "Items: " << currentOrder.items << "\n";
                    cout << "Total Cost: Rp " << currentOrder.totalCost << "\n";
                    cout << "Current Status: " << currentOrder.status << "\n";

                    cout << "Enter new status (e.g., Completed, In Progress):";

                    cin.ignore();
                    getline(cin, currentOrder.status);

                    cout << "Order ID " << currentOrder.orderID << " updated to status: " << currentOrder.status << "\n";
                } else if (choice == 3) {
                    if (orderQueue.empty()) {
                        cout << "\nNo orders in the queue.\n";
                        continue;
                    }

                    cout << "\nCurrent Order Queue:\n";
                    queue<Order> tempQueue = orderQueue; // Copy queue to iterate through it
                    while (!tempQueue.empty()) {
                        Order currentOrder = tempQueue.front();
                        tempQueue.pop();

                        cout << "Order ID: " << currentOrder.orderID << "\n";
                        cout << "Items: " << currentOrder.items << "\n";
                        cout << "Total Cost: Rp " << currentOrder.totalCost << "\n";
                        cout << "Status: " << currentOrder.status << "\n";
                        cout << "-------------------\n";
                    }
                } else if (choice == 4) {
                    cout << "Exiting Staff Menu.\n";
                } else {
                    cout << "Invalid choice. Try again.\n";
                }
            } while (choice != 4);
        } else if (role == 3) {
            cout << "Exiting system. Goodbye!\n";
        } else {
            cout << "Invalid role. Please try again.\n";
        }
    } while (role != 3);

    return 0;
}

