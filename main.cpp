#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;


// Class representing an item in the inventory
class Item {
private:
    string id;
    string name;
    int quantity;
    double price;
    string category;

public:
    // Constructor
    Item(string itemId = "", string itemName = "", int itemQuantity = 0, double itemPrice = 0.0, string itemCategory = "")
            : id(itemId), name(itemName), quantity(itemQuantity), price(itemPrice), category(itemCategory) {}

    // Getters and setters for encapsulation
    string getId() const { return id; }
    void setId(string newId) { id = newId; }

    string getName() const { return name; }
    void setName(string newName) { name = newName; }

    int getQuantity() const { return quantity; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }

    double getPrice() const { return price; }
    void setPrice(double newPrice) { price = newPrice; }

    string getCategory() const { return category; }
    void setCategory(string newCategory) { category = newCategory; }

    // Method to display item details (abstraction for the user)
    void displayItem() const {
        cout << "ID: " << id << ", Name: " << name
             << ", Quantity: " << quantity << ", Price: " << price
             << ", Category: " << category << endl;
    }
};

// Base Inventory class
class BaseInventory {
public:
    virtual void addItem() = 0;
    virtual void updateItem() = 0;
    virtual void removeItem() = 0;
    virtual void displayItemsByCategory() = 0;
    virtual void displayAllItems() = 0;
    virtual void searchItem() = 0;
    virtual void sortItems() = 0;
    virtual void displayLowStockItems() = 0;
};

// Class representing the inventory (manages multiple items)
class Inventory : public BaseInventory {
public:
    static const int MAX_ITEMS = 100;  // Fixed array size
    Item items[MAX_ITEMS];             // Array of Item objects
    int itemCount;                     // Track the number of items in inventory

    // Constructor
    Inventory() : itemCount(0) {}

    // Method to validate numeric input
    template<typename T>
    void validateInput(T& value) {
        while (!(cin >> value) || value <= 0) {
            cout << "Invalid input. Please enter a positive value: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Method to add a new item to the inventory
    void addItem() override {
        string id, name, category;
        int quantity;
        double price;
        char choice;

        while (true) {
            cout << "[Back - 0]\n";
            cout << "Input Category (Clothing, Electronics, Entertainment): ";
            cin >> category;

            // Check for exit option
            if (category == "0") {
                return; // Go back to main menu
            }

            // Convert to lowercase for case-insensitive comparison
            for (auto &c : category) c = tolower(c);

            // Validate the category
            if (category != "clothing" && category != "electronics" && category != "entertainment") {
                cout << "Invalid category! Please enter a valid category.\n";
            } else {
                break; // Valid category, exit the loop
            }
        }

        while (true) {
            cout << "Input ID: ";
            cin >> id;

            // Check if the user entered "0" as the item ID
            if (id == "0") {
                cout << "ID cannot be 0. Please enter a valid ID.\n";
                continue; // Prompt for the ID again
            }

            // Check for existing ID
            bool idExists = false;
            for (int i = 0; i < itemCount; i++) {
                if (items[i].getId() == id) {
                    idExists = true;
                    break;
                }
            }

            if (idExists) {
                cout << "Item already in inventory. Please enter a different ID.\n";
            } else {
                break; // ID is unique and valid, exit the loop
            }
        }

        cout << "Input Name: ";
        cin >> name;

        // Validate quantity and price input
        cout << "Quantity: ";
        validateInput(quantity);

        cout << "Price: ";
        validateInput(price);

        // Check if we can add more items
        if (itemCount < MAX_ITEMS) {
            items[itemCount++] = Item(id, name, quantity, price, category);  // Add item to the inventory
            cout << "Item added successfully!\n";
        } else {
            cout << "Cannot add more items. Inventory is full.\n";
        }

        // Ensure only valid inputs for 'add another item'
        do {
            cout << "Add another item? [Y/N]: ";
            cin >> choice;
            choice = toupper(choice);
            if (choice == 'Y') {
                addItem();
                break;
            } else if (choice == 'N') {
                break;
            } else {
                cout << "Invalid input. Please enter 'Y' or 'N'.\n";
            }
        } while (true);
    }

    // Method to update an item's quantity or price
    void updateItem() override {
        if (itemCount == 0) {
            cout << "Please add items first!\n";
            return;
        }

        string id, updateChoice;
        int newQuantity;
        double newPrice;
        char choice;
        bool itemFound;

        do {
            itemFound = false; // Reset for each loop

            cout << "[Back - 0]\n";
            cout << "Input ID: ";
            cin >> id;

            if (id == "0") return; // Exit if user inputs "0"

            // Searching for the item with the given ID
            for (int i = 0; i < itemCount; i++) {
                if (items[i].getId() == id) {
                    itemFound = true;
                    do {
                        cout << "What to update? [Qty/Price]: ";
                        cin >> updateChoice;
                        // Convert to lowercase for case-insensitive comparison
                        for (auto &c : updateChoice) c = tolower(c);

                        if (updateChoice == "qty") {
                            cout << "New Quantity: ";
                            validateInput(newQuantity);

                            if (newQuantity == items[i].getQuantity()) {
                                cout << "The Quantity of " << items[i].getName() << " is already " << newQuantity << endl;
                            } else {
                                cout << items[i].getName() << " Quantity updated from " << items[i].getQuantity();
                                items[i].setQuantity(newQuantity);
                                cout << " --> " << items[i].getQuantity() << endl;
                            }
                        } else if (updateChoice == "price") {
                            cout << "New Price: ";
                            validateInput(newPrice);

                            if (newPrice == items[i].getPrice()) {
                                cout << "The Price of " << items[i].getName() << " is already " << newPrice << endl;
                            } else {
                                cout << items[i].getName() << " Price updated from " << items[i].getPrice();
                                items[i].setPrice(newPrice);
                                cout << " --> " << items[i].getPrice() << endl;
                            }
                        } else {
                            cout << "Invalid choice! Please enter 'Qty' or 'Price'.\n";
                        }
                    } while (updateChoice != "qty" && updateChoice != "price");
                    break;
                }
            }

            if (!itemFound) {
                cout << "Item not found!\n";
            }

            // Ask if they want to update another item
            cout << "Update another item? [Y/N]: ";
            cin >> choice;

        } while (tolower(choice) == 'y');
    }

    // Method to remove an item from the inventory
    void removeItem() override {
        if (itemCount == 0) {
            cout << "Please add items first!\n";
            return;
        }

        string id;
        char choice;
        bool itemFound = false;

        do {
            cout << "[Back - 0]\n";
            cout << "Input ID: ";
            cin >> id;

            if (id == "0") return;  // Exit to main menu if "0" is entered

            // Searching for the item with the given ID
            for (int i = 0; i < itemCount; i++) {
                if (items[i].getId() == id) {
                    itemFound = true;
                    cout << items[i].getName() << " has been removed from the inventory.\n";

                    // Shift all items after the found item to fill the gap
                    for (int j = i; j < itemCount - 1; j++) {
                        items[j] = items[j + 1];
                    }
                    itemCount--;  // Decrease item count after removal
                    break;
                }
            }

            if (!itemFound) {
                cout << "Item not found!\n";
            }

            // Check if the inventory is now empty
            if (itemCount == 0) {
                cout << "Inventory is empty. Returning to main menu...\n";
                return; // Exit to main menu if inventory is empty
            }

            // Ask if the user wants to remove another item
            cout << "Remove another item? [Y/N]: ";
            cin >> choice;

        } while (tolower(choice) == 'y');
    }

    // Method to display items by category
    void displayItemsByCategory() override {
        if (itemCount == 0) {
            cout << "Please add items first!\n";
            return;
        }

        string categoryChoice;
        bool categoryFound = false;

        while (true) {
            cout << "[Back - 0]\n";
            cout << "Input Category (Clothing, Electronics, Entertainment): ";
            cin >> categoryChoice;

            // Check for back option
            if (categoryChoice == "0") {
                return; // Return to main menu if input is '0'
            }

            // Convert to lowercase for case-insensitive comparison
            for (auto &c : categoryChoice) c = tolower(c);

            // Validate the category
            if (categoryChoice != "clothing" && categoryChoice != "electronics" && categoryChoice != "entertainment") {
                cout << "Invalid category! Please enter a valid category.\n";
            } else {
                break; // Valid category, exit the loop
            }
        }

        // Display items in the chosen category
        cout << "ID        ITEM           QTY     PRICE   \n";
        cout << "-----------------------------------------\n";
        bool itemsExist = false;
        for (int i = 0; i < itemCount; i++) {
            if (items[i].getCategory() == categoryChoice) {
                categoryFound = true;
                itemsExist = true;
                cout << left << setw(10) << items[i].getId()
                     << setw(15) << items[i].getName()
                     << setw(8) << items[i].getQuantity()
                     << setw(8) << items[i].getPrice() << endl;
            }
        }

        if (!itemsExist) {
            cout << "No items available in this category.\n";
        }
    }

    // Method to display all items in a formatted table
    void displayAllItems() override {
        if (itemCount == 0) {
            cout << "Please add items first!\n";
            return;
        }

        cout << "ID        ITEM           QTY     PRICE   CATEGORY\n";
        cout << "-------------------------------------------------\n";
        for (int i = 0; i < itemCount; i++) {
            cout << left << setw(10) << items[i].getId()
                 << setw(15) << items[i].getName()
                 << setw(8) << items[i].getQuantity()
                 << setw(8) << items[i].getPrice()
                 << setw(10) << items[i].getCategory() << endl;
        }
    }

    // Method to search for an item by ID
    void searchItem() override {
        if (itemCount == 0) {
            cout << "Please add items first!\n";
            return;
        }

        string id;
        bool itemFound;
        char choice;

        do {
            itemFound = false; // Reset itemFound for each search attempt

            cout << "[Back - 0]\n";
            cout << "Input ID: ";
            cin >> id;

            if (id == "0") return;  // Exit to main menu if "0" is entered

            // Searching for the item with the given ID
            for (int i = 0; i < itemCount; i++) {
                if (items[i].getId() == id) {
                    itemFound = true;

                    // Display item in table format
                    cout << "Item found:\n";
                    cout << "---------------------------------------------------\n";
                    cout << left << setw(10) << "ID"
                         << left << setw(20) << "Name"
                         << left << setw(10) << "Quantity"
                         << left << setw(10) << "Price" << endl;
                    cout << "---------------------------------------------------\n";
                    cout << left << setw(10) << items[i].getId()
                         << left << setw(20) << items[i].getName()
                         << left << setw(10) << items[i].getQuantity()
                         << left << setw(10) << items[i].getPrice() << endl;
                    cout << "---------------------------------------------------\n";
                    break;
                }
            }

            if (!itemFound) {
                cout << "Item not found!\n";
            }

            // Ask the user if they want to search again and validate input
            do {
                cout << "Search item again? [Y/N]: ";
                cin >> choice;
                choice = tolower(choice); // Convert input to lowercase for easier validation
            } while (choice != 'y' && choice != 'n'); // Repeat until 'y' or 'n' is entered

        } while (choice == 'y'); // Continue searching if 'Y' or 'y' is entered
    }

    // Method to sort items using Bubble Sort
    void sortItems() override {
        if (itemCount == 0) {
            cout << "Please add items first!\n";
            return;
        }

        string sortChoice;
        char orderChoice;

        // Display back option
        cout << "[Back - 0]\n";

        // Ask the user for sorting criteria (Name, Price, Quantity)
        while (true) {
            cout << "Sort by [Name/Price/Quantity]: ";
            cin >> sortChoice;
            if (sortChoice == "0") return; // Return to main menu if input is '0'

            for (auto &c : sortChoice) c = tolower(c);  // Convert to lowercase for case-insensitive comparison

            if (sortChoice == "name" || sortChoice == "price" || sortChoice == "quantity") {
                break; // Valid input, exit loop
            } else {
                cout << "Invalid choice! Please enter 'Name', 'Price', or 'Quantity'.\n";
            }
        }

        // Ask the user for sorting order (Ascending or Descending)
        while (true) {
            cout << "Sort in [A-Ascending/D-Descending]: ";
            cin >> orderChoice;
            if (orderChoice == '0') return; // Return to main menu if input is '0'

            orderChoice = tolower(orderChoice);  // Convert input to lowercase for easier validation
            if (orderChoice == 'a' || orderChoice == 'd') {
                break; // Valid input, exit loop
            } else {
                cout << "Invalid choice! Please enter 'A' for Ascending or 'D' for Descending.\n";
            }
        }

        // Bubble Sort implementation
        for (int i = 0; i < itemCount - 1; i++) {
            for (int j = 0; j < itemCount - i - 1; j++) {
                bool swapNeeded = false;
                if (sortChoice == "name") {
                    swapNeeded = (orderChoice == 'a') ?
                                 (items[j].getName() > items[j + 1].getName()) :
                                 (items[j].getName() < items[j + 1].getName());
                } else if (sortChoice == "price") {
                    swapNeeded = (orderChoice == 'a') ?
                                 (items[j].getPrice() > items[j + 1].getPrice()) :
                                 (items[j].getPrice() < items[j + 1].getPrice());
                } else if (sortChoice == "quantity") {
                    swapNeeded = (orderChoice == 'a') ?
                                 (items[j].getQuantity() > items[j + 1].getQuantity()) :
                                 (items[j].getQuantity() < items[j + 1].getQuantity());
                }

                // Swap items if needed
                if (swapNeeded) {
                    swap(items[j], items[j + 1]);
                }
            }
        }

        // Display sorted items in table format
        cout << "Items sorted by " << sortChoice << " in "
             << ((orderChoice == 'a') ? "Ascending" : "Descending") << " order:\n";
        cout << "---------------------------------------------------\n";
        cout << left << setw(10) << "ID"
             << left << setw(20) << "Name"
             << left << setw(10) << "Quantity"
             << left << setw(10) << "Price" << endl;
        cout << "---------------------------------------------------\n";
        for (int i = 0; i < itemCount; i++) {
            cout << left << setw(10) << items[i].getId()
                 << left << setw(20) << items[i].getName()
                 << left << setw(10) << items[i].getQuantity()
                 << left << setw(10) << items[i].getPrice() << endl;
        }
        cout << "---------------------------------------------------\n";
    }

    // Method to display items that are low in stock (quantity <= 5)
    void displayLowStockItems() override {
        if (itemCount == 0) {
            cout << "Please add items first!\n";
            return;
        }

        bool lowStockFound = false;

        cout << "Low stock items (Quantity <= 5):\n";
        cout << "ID        ITEM           QTY     PRICE   CATEGORY\n";
        cout << "-------------------------------------------------\n";

        for (int i = 0; i < itemCount; i++) {
            if (items[i].getQuantity() <= 5) {
                lowStockFound = true;
                cout << left << setw(10) << items[i].getId()
                     << setw(15) << items[i].getName()
                     << setw(8) << items[i].getQuantity()
                     << setw(8) << items[i].getPrice()
                     << setw(10) << items[i].getCategory() << endl;
            }
        }

        if (!lowStockFound) {
            cout << "No low stock items.\n";
        }
    }
};

int main() {
    Inventory inventory;
    int choice;

    // Main menu loop
    do {
        cout << "\nInventory Management System\n";
        cout << "1 - Add Item\n";
        cout << "2 - Update Item\n";
        cout << "3 - Remove Item\n";
        cout << "4 - Display Items by Category\n";
        cout << "5 - Display All Items\n";
        cout << "6 - Search Item\n";
        cout << "7 - Sort Items\n";
        cout << "8 - Display Low Stock Items\n";
        cout << "9 - Exit\n";
        cout << "Enter choice: ";

        // Input validation for choice
        while (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number from 1 to 9: ";
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
        }

        switch (choice) {
            case 1:
                inventory.addItem();
                break;
            case 2:
                inventory.updateItem();
                break;
            case 3:
                inventory.removeItem();
                break;
            case 4:
                inventory.displayItemsByCategory();
                break;
            case 5:
                inventory.displayAllItems();
                break;
            case 6:
                inventory.searchItem();
                break;
            case 7:
                inventory.sortItems();
                break;
            case 8:
                inventory.displayLowStockItems();
                break;
            case 9:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please enter a valid option.\n";
                break;
        }

    } while (choice != 9);

    return 0;
}
