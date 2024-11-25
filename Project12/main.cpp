#include "HotelBookingSystem.h"
#include <iostream>
#include <locale.h>

void saveClientsToFile(const BookingRequest clients[], int size, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (int i = 0; i < size; i++) {
            outFile << clients[i].toFileFormat() << endl;
        }
        outFile.close();
        cout << "\n Clients saved to " << filename << "\n" << endl;
    }
    else {
        cerr << "Unable to open file " << filename << endl;
    }
}

//5.7
// Функція для запису і зчитування об'єкта Service з файлу
void ServiceFileOperations(Service& service, const string& filename) {
    try {
        ofstream outFile(filename, ios::app); // Відкриття у режимі додавання
        if (!outFile) throw runtime_error("Unable to open file for writing");

        // Запис об'єкта у файл
        service.Write(outFile);
        outFile.close();

        ifstream inFile(filename); // Відкриття для читання
        if (!inFile) throw runtime_error("Unable to open file for reading");

        // Створення нового об'єкта для зчитування даних
        Service serviceCopy;
        serviceCopy.Read(inFile);
        inFile.close();

        // Демонстрація успішного зчитування даних
        cout << endl;
        cout << "_________________________" << endl;
        cout << "Service read from file: " << endl;
        cout << "Service Name: " << serviceCopy.getServiceName() << endl;
        cout << "Price: $" << serviceCopy.getPrice() << endl;
    }
    catch (const ios_base::failure& exception) {
        cerr << "I/O error: " << exception.what() << endl;
    }
    catch (const runtime_error& exception) {
        cerr << "Runtime error: " << exception.what() << endl;
    }
    catch (const bad_alloc& exception) {
        cerr << "Memory allocation error: " << exception.what() << endl;
    }
    catch (const exception& exception) {
        cerr << "General error: " << exception.what() << endl;
    }
}


// Видалення з динамічного масиву за певним критерієм
void Service_Dynamic(Service*& services, int& size, double priceLimit) {
    try {
        if (size == 0) {
            // Динамічний масив порожній. Немає елементів для видалення
            throw runtime_error("Dynamic array is empty. No items to delete.");
        }

        int indexToDelete = -1;
        for (int i = 0; i < size; i++) {
            if (services[i].getPrice() > priceLimit) {
                indexToDelete = i;
                break;
            }
        }

        if (indexToDelete == -1) {
            // Не знайдено послуг із зазначеними критеріями
            throw runtime_error("No service found with the specified criteria.");
        }

        Service* newServices = new Service[size - 1];
        for (int i = 0, j = 0; i < size; i++) {
            if (i != indexToDelete) {
                newServices[j++] = services[i];
            }
        }

        delete[] services;
        services = newServices;
        size--;
    }
    catch (const bad_alloc& exception) {
        cerr << "Memory allocation error: " << exception.what() << endl;
    }
    catch (const runtime_error& exception) {
        cerr << "Runtime error: " << exception.what() << endl;
    }
}

// Видалення з статичного масиву за певним критерієм
void Service_Static(Service services[], int& size, double priceLimit) {
    try {
        if (size == 0) {
            // Статичний масив порожній. Немає елементів для видалення
            throw runtime_error("Static array is empty. No items to delete.");
        }

        int indexToDelete = -1;
        for (int i = 0; i < size; i++) {
            if (services[i].getPrice() > priceLimit) {
                indexToDelete = i;
                break;
            }
        }

        if (indexToDelete == -1) {
            // Не знайдено послуг із зазначеними критеріями
            throw runtime_error("No service found with the specified criteria.");
        }

        for (int i = indexToDelete; i < size - 1; i++) {
            services[i] = services[i + 1];
        }
        size--;
    }
    catch (const runtime_error& exception) {
        cerr << "Runtime error: " << exception.what() << endl;
    }
}

// 5.8
void displayMenu() {
    cout << "\n--- Hotel Booking System Menu ---\n";
    cout << "1. Create a Booking Request\n";
    cout << "2. Delete a Booking Request\n";
    cout << "3. Create a Service\n";
    cout << "4. Delete a Service\n";
    cout << "5. Exit\n";
    cout << "Select an option: ";
}

// 5.11
// Метод для демонстрації обходу віртуальних функцій
void displayServiceInfo(Service* service) {
    // Виклик одного методу, але для різних типів об'єктів
    service->displayService();
}

int main(int argument_сount, char* argument_vector[]) {
    setlocale(LC_ALL, "ukr");


    //5.7
    // cd ... Laboratory_Project_OOP.exe service.txt
    // Перевірка наявності аргументу командного рядка
    if (argument_сount < 2) {
        cerr << "Please provide the filename as a command-line argument." << endl;
        return 1;
    }

    string filename = argument_vector[1];

    // Приклад створення об'єкта Service
    Service service("Room Cleaning", 25.0);
    // Виклик функції з роботи з файлом
    ServiceFileOperations(service, filename);
    cout << "_________________________" << endl;
    cout << endl;

// 5.8
    cout << "___________________________________" << endl;
    bool isRunning = true;
    int userChoice;

    vector<BookingRequest> bookingRequests;
    vector<Service> services;

    while (isRunning) {
        displayMenu();
        cin >> userChoice;

        try {
            switch (userChoice) {
            case 1: {
                // Додавання нового BookingRequest
                string name;
                int guests, nights;
                double cost;
                string roomType;

                cout << "Enter guest name: ";
                cin >> name;
                cout << "Enter number of guests: ";
                cin >> guests;
                cout << "Enter room type: ";
                cin >> roomType;
                cout << "Enter number of nights: ";
                cin >> nights;
                cout << "Enter cost per night: ";
                cin >> cost;

                BookingRequest newRequest(name, guests, roomType, nights, cost);
                bookingRequests.push_back(newRequest);

                cout << "Booking Request created successfully." << endl;
                break;
            }
            case 2: {
                // Видалення BookingRequest за індексом
                if (bookingRequests.empty()) {
                    cout << "No booking requests available to delete.\n";
                    break;
                }

                int index;
                cout << "Enter the index of the Booking Request to delete (0 to " << bookingRequests.size() - 1 << "): ";
                cin >> index;

                // Перевіряємо, чи є введений індекс у межах діапазону
                if (cin.fail() || index < 0 || index >= bookingRequests.size()) {
                    cin.clear(); // Очищуємо стан cin, якщо ввід некоректний
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Видаляємо залишки вводу
                    cout << "Invalid index. Unable to delete Booking Request.\n";
                }
                else {
                    bookingRequests.erase(bookingRequests.begin() + index);
                    cout << "Booking Request deleted successfully.\n";
                }
                break;
            }
            case 3: {
                // Додавання нового Service
                string serviceName;
                double price;

                cout << "Enter service name: ";
                cin >> serviceName;
                cout << "Enter service price: ";
                cin >> price;

                Service newService(serviceName, price);
                services.push_back(newService);

                cout << "Service created successfully." << endl;
                break;
            }
            case 4: {
                // видалення Service за індексом
                if (services.empty()) {
                    cout << "No services available to delete.\n";
                    break;
                }

                int index;
                cout << "Enter the index of the Service to delete (0 to " << services.size() - 1 << "): ";
                cin >> index;

                // Перевіряємо коректність індексу
                if (cin.fail() || index < 0 || index >= services.size()) {
                    cin.clear(); // Очищуємо стан cin від помилкового вводу
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Видаляємо залишки вводу
                    cout << "Invalid index. Unable to delete Service.\n";
                }
                else {
                    services.erase(services.begin() + index);
                    cout << "Service deleted successfully.\n";
                }
                break;
            }

            case 5: {
                // Вихід із меню 
                isRunning = false;
                cout << "Exit the menu..." << endl << endl;
                break;
            }
            default: {
                cerr << "Invalid choice. Please try again." << endl;
                break;
            }
            }
        }
        catch (const out_of_range& exception) {
            cerr << "Error: " << exception.what() << "\n";
        }
        catch (const exception& exception) {
            cerr << "An unexpected error occurred: " << exception.what() << "\n";
        }
    }
    cout << "___________________________________" << endl;
    cout << endl;

    // 6.1
    cout << "_____________6.1_______________" << endl;
    cout << "\nDemonstrating Unary Operators (++ and --):\n" << endl;

    // Створюємо кімнату
    Room demoRoom(5, "Luxury", 150.0);
    cout << "Initial Room Info:\n";
    demoRoom.displayRoomInfo();

    // Префіксний ++
    ++demoRoom;
    cout << "\nAfter Prefix ++ (Increase price by 10):" << endl;
    demoRoom.displayRoomInfo();

    // Постфіксний ++
    demoRoom++;
    cout << "\nAfter Postfix ++ (Increase price by 10):" << endl;
    demoRoom.displayRoomInfo();

    // Префіксний --
    --demoRoom;
    cout << "\nAfter Prefix -- (Decrease price by 10):" << endl;
    demoRoom.displayRoomInfo();

    // Постфіксний --
    demoRoom--;
    cout << "\nAfter Postfix -- (Decrease price by 10):" << endl;
    demoRoom.displayRoomInfo();

    // 6.2 
    cout << "_____________6.2_______________" << endl;
    Room deluxeRoom(5, "Deluxe", 250.0);
    cout << "Original Room Info:" << endl;
    deluxeRoom.displayRoomInfo();

    // Послідовність операцій
    deluxeRoom + 50.0;  // Додаємо $50 до ціни за добу
    cout << "After Adding $50:" << endl;
    deluxeRoom.displayRoomInfo();

    deluxeRoom - 30.0;  // Віднімаємо $30 від зміненої ціни
    cout << "After Subtracting $30:" << endl;
    deluxeRoom.displayRoomInfo();

    // 6.3
    cout << "_____________6.3_______________" << endl;
    Room demoRoom1(3, "Deluxe", 150.0);
    Room demoRoom2(3, "Suite", 250.0);

    cout << "Original Room Info:" << endl;
    demoRoom1.displayRoomInfo();
    demoRoom2.displayRoomInfo();

    // Перевантаження оператора *
    Room discountedRoom = demoRoom1 * 0.8;  // Знижка 20%
    cout << "\nDiscounted Room Info (20% off):" << endl;
    discountedRoom.displayRoomInfo();

    // Перевантаження оператора =
    demoRoom2 = demoRoom1;
    cout << "\nAfter Assignment (demoRoom2 = demoRoom1):" << endl;
    demoRoom2.displayRoomInfo();

    // Перевантаження оператора +=
    demoRoom1 += 50.0;  // Додати $50 до ціни
    cout << "\nAfter Adding $50 to demoRoom1:" << endl;
    demoRoom1.displayRoomInfo();

    // Перевантаження оператора -=
    demoRoom1 -= 30.0;  // Відняти $30 від ціни
    cout << "\nAfter Subtracting $30 from demoRoom1:" << endl;
    demoRoom1.displayRoomInfo();

    // Перевантаження оператора *=
    demoRoom1 *= 1.1;  // Підняти ціну на 10%
    cout << "\nAfter Increasing Price by 10%:" << endl;
    demoRoom1.displayRoomInfo();

    // Перевантаження оператора []
    cout << "\nRoom Class Character at Index 2: " << demoRoom1[2] << endl;

    try {
        cout << "Room Class Character at Invalid Index: " << demoRoom1[100] << endl;
    }
    catch (const out_of_range& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    // 6.4
    cout << "_____________6.4_______________" << endl;
    Customer update_customer("Viktor Philips", "My.Philips@email.com", 127, true);

    cout << "Original Customer Info:" << endl;
    update_customer.displayCustomerInfo();

    cout << "\nUpdating Customer Name to 'Oleg Philips'..." << endl;
    // Оновлюємо ім'я клієнта
    update_customer.setName("Oleg Philips");

    cout << "Updating Loyalty Points to 268..." << endl;
    // Оновлюємо кількість балів клієнта 
    update_customer.setLoyaltyPoints(268);

    cout << "Updating Discount Status to false..." << endl;
    // Оновлюємо статус знижки
    update_customer.setHasDiscount(false);

    // Оновлена інформація клієнта
    cout << "\nUpdated Customer Info:" << endl;
    update_customer.displayCustomerInfo();

    // 6.5
    cout << "_____________6.5_______________" << endl;
    // Створюємо об'єкт класу Administrator
    vector<int> roomDatabase = { 144, 564, 405, 489, 515, 538, 249, 362, 382, 396, 105, 282, 364, 244, 406 }; // База номерів кімнат
    vector<int> availableRooms = { 538, 406, 144, 244 }; // Вільні кімнати

    Administrator<string, string, vector<int>, vector<int>, string> admin(
        "Andriy Dubenko", // Ім'я адміністратора
        "+380672549804", // Номер телефону рецепції
        roomDatabase, // База номерів кімнат
        availableRooms, // Вільні кімнати
        "Katerina Sokolovska " // Ім'я нового клієнта
    );

    // Виводимо інформацію адміністратора
    cout << "Administrator Info:" << endl;
    admin.displayAdministratorInfo();

    /*Геттери — це функції, які повертають значення закритих змінних-членів класу*/
    // Отримання окремих даних через геттери
    cout << "\nGetting individual data:" << endl;
    cout << "Administrator Name: " << admin.getAdminName() << endl;
    cout << "Reception Phone: " << admin.getReceptionPhone() << endl;

    cout << "Room Database: ";
    for (const auto& room : admin.getRoomDatabase()) {
        cout << room << " ";
    }
    cout << endl;

    cout << "Available Rooms: ";
    for (const auto& room : admin.getAvailableRooms()) {
        cout << room << " ";
    }
    cout << endl;

    cout << "New Client Name: " << admin.getNewClientName() << endl;

    // 6.6
      // Створюємо послуги
    cout << "_____________6.6_______________" << endl;
    Service spaService("Spa Treatment", 100.0, "Luxury", 120, true);
    Service massageService("Massage", 50.0, "Relaxation", 90, true);
    Service facialService("Facial", 60.0, "Beauty", 75, true);

    // Створюємо менеджер для послуг
    ServiceManager<Service> manager;

    // Додаємо послуги до менеджера
    manager.addService(spaService);
    manager.addService(massageService);
    manager.addService(facialService);

    // Виведення інформації про всі послуги
    cout << "All Services:" << endl;
    manager.displayAllServices();

    // Розрахунок загальної вартості послуг
    cout << "\nTotal Cost: " << manager.calculateTotalCost() << endl;

    // 7.1
    // Створюємо об'єкт готелю
    cout << "_____________7.1_______________" << endl;
    Hotel new_hotel("Grand Hotel");

    // Створення кількох кімнат
    Room room_1(4, "Standard", 100.0);
    Room room_2(3, "Delux", 150.0);
    Room room_3(2, "Lux", 300.0);
    Room room_4(3, "Delux", 150.0);

    // Додаємо кімнати в готель
    new_hotel.addRoom(room_1);
    new_hotel.addRoom(room_2);
    new_hotel.addRoom(room_3);

    // Виводимо всі кімнати
    new_hotel.displayRooms();

    // Додаємо клієнтів до черги
    new_hotel.addCustomerToQueue("John Doe");
    new_hotel.addCustomerToQueue("Alice Smith");
    new_hotel.addCustomerToQueue("Bob Johnson");
    // Виводимо чергу клієнтів
    new_hotel.displayCustomerQueue();

    // Додаємо бронювання до історії
    new_hotel.addBookingToHistory("John Doe booked Room Standard");
    new_hotel.addBookingToHistory("Alice Smith booked Room Delux");
    new_hotel.addBookingToHistory("Bob Johnson booked Room Lux");

    // Виводимо історію бронювань
    new_hotel.displayBookingHistory();

    // 7.2
    // Демонстрація ітераторів
    cout << "_____________7.2_______________" << endl;
    new_hotel.addRoom(Room(4, "Standard", 100.0));
    new_hotel.addRoom(Room(3, "Delux", 150.0));
    new_hotel.addRoom(Room(2, "Lux", 300.0));


    new_hotel.demonstrateInputIterator();
    new_hotel.demonstrateOutputIterator();
    new_hotel.displayCustomerQueue(); // Вивід оновленої черги
    new_hotel.demonstrateForwardIterator();
    new_hotel.demonstrateBidirectionalIterator();
    new_hotel.demonstrateRandomAccessIterator();

    //7.3
     // 1. Сортування кімнат за ціною
    cout << "_____________7.3_______________" << endl;
    new_hotel.sortRoomsByPrice();
    cout << endl;
    // 2. Пошук клієнта в черзі
    new_hotel.findCustomerInQueue("Alice Smith");
    cout << endl;
    // 3. Підрахунок клієнтів із певним іменем
    new_hotel.countCustomersWithName("John Doe");
    cout << endl;
    // 4. Збільшення ціни кімнат
    new_hotel.increaseRoomPrices(13);
    cout << endl;
    // 5. Реверс черги клієнтів
    new_hotel.reverseCustomerQueue();

    // 7.4
    // Демонстрація роботи функціональних об'єктів
    cout << "_____________7.4_______________" << endl;
    Functor functors;

    // Демонстрація арифметичних операцій
    Functor::Plus plus;
    cout << "476 + 1579 = " << plus(476, 1579) << endl;

    Functor::Minus minus;
    cout << "52398 - 24356 = " << minus(52398, 24356) << endl;

    Functor::Multiply multiply;
    cout << "457 * 751 = " << multiply(457, 751) << endl;

    Functor::Divide divide;
    try {
        cout << "422 / 2 = " << divide(422, 2) << endl;
        cout << "10 / 0 = " << divide(10, 0) << endl;  // Операція з діленням на 0 викличе виключення
    }
    catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }

    // Демонстрація порівняльних операцій
    Functor::EqualTo equalTo;
    cout << "156 == 156: " << equalTo(5, 5) << endl;

    Functor::NotEqualTo notEqualTo;
    cout << "5 != 3: " << notEqualTo(5, 3) << endl;

    Functor::GreaterThan greaterThan;
    cout << "42353 > 11255: " << greaterThan(42353, 11255) << endl;

    Functor::LessThan lessThan;
    cout << "474 < 557: " << lessThan(474, 557) << endl;

    // Демонстрація логічних операцій
    Functor::LogicalAnd logicalAnd;
    cout << "true AND false: " << logicalAnd(true, false) << endl;

    Functor::LogicalOr logicalOr;
    cout << "true OR false: " << logicalOr(true, false) << endl;

    Functor::LogicalNot logicalNot;
    cout << "NOT true: " << logicalNot(true) << endl;

    // 5.11
    cout << "_____________5.11_______________" << endl;
    Service* room_service = new RoomService("Mini bar", 10.0);
    // Відображаємо обхід віртуальної функції
    cout << "We display the bypass of the virtual function:" << endl;
    // Демонстрація виклику через базовий клас
    displayServiceInfo(room_service);  // Викликається displayService() для RoomService

    // 5.10
    cout << "_____________5.10_______________" << endl;
    RoomService roomService("Breakfast on the terrace", 55.0);
    roomService.displayService();  // Раннє зв'язування (Статичне)
    cout << "Cost: " << roomService.calculateCost() << endl;

    // 5.9, 5.10
    // Спосіб 1: Використання вказівників на базовий клас
    cout << "_____________5.9_______________" << endl;
    Service* maintenance1 = new RoomService("Breakfast in Room", 20.0);
    Service* maintenance2 = new RoomService("Dinner Service", 45.0);

    maintenance1->displayService(); // Пізнє зв'язування (Динамічне)

    // Спосіб 2: Використання масиву вказівників на базовий клас
    Service* maintenance[2];
    maintenance[0] = new RoomService("Dinner Service", 45.0);
    maintenance[1] = new RoomService("Breakfast in Room", 20.0);

    for (int i = 0; i < 2; ++i) {
        maintenance[i]->displayService();  // Виклик поліморфних методів
    }

    // Спосіб 3: Перевизначення методу та демонстрація зміни форми об'єкта
    Service* dynamicService = new RoomService("Late Checkout", 25.0);
    dynamicService->displayService();

    // Зміна форми об'єкта за допомогою поліморфізму
    delete dynamicService;

    dynamicService = new RoomService("Cleaning of the room", 55.0);
    dynamicService->displayService();
    cout << "____________________________" << endl;
    cout << endl;

    //5.6
    // Динамічний масив Service
    int dynamicSize = 5;
    Service* dynamicServices = new Service[dynamicSize]{
        Service("Breakfast in room", 15.0),
        Service("Transfer", 30.0),
        Service("Pool Access", 20.0),
        Service("Spa Treatment", 50.0),
        Service("Dinner Package", 40.0)
    };
    cout << endl;
    // Статичний масив Service
    int staticSize = 5;
    Service staticServices[5] = {
        Service("Breakfast in room", 15.0),
        Service("Transfer", 30.0),
        Service("Pool Access", 20.0),
        Service("Spa Treatment", 50.0),
        Service("Dinner Package", 40.0)
    };

    cout << "_________________________________________" << endl;
    // Використання методів видалення з критерієм ціни: якщо ціна послуги більша за 25, послуга видаляється 
    double priceLimit = 25.0;

    cout << "Removing from dynamic array..." << endl;
    Service_Dynamic(dynamicServices, dynamicSize, priceLimit);

    cout << "Removing from static array..." << endl << endl;
    Service_Static(staticServices, staticSize, priceLimit);
    cout << "_________________________________________" << endl;

    // 5.4
    // Віртуальний метод для отримання короткої інформації про бронювання
    BookingRequest getbookingsummary("Andrii Kharakternyk", 2, "Luxury", 5, 1200.0);
    cout << getbookingsummary.getBookingSummary() << endl << endl;

    VIPCustomer personalAssistant;
    // Вививід усіх асистентів
    cout << "List of Personal Assistants:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Personal Assistant " << (i + 1) << ": " << personalAssistant.getPersonalAssistant(i) << endl;
    }
    cout << endl;

    // 5.1,5.2
    double basePrice = 200.0;

    SilverProgram silver;
    GoldProgram gold;
    PlatinumProgram platinum;
    cout << "|__Abstract class hierarchy__|";
    cout << "\nOriginal Price: $" << basePrice << endl;

    // Використання SilverProgram
    silver.displayProgramInfo();
    cout << "Price after Silver discount: $" << silver.calculateDiscount(basePrice) << endl;

    // Використання GoldProgram
    gold.displayProgramInfo();
    cout << "Price after Gold discount: $" << gold.calculateDiscount(basePrice) << endl;

    // Використання PlatinumProgram
    platinum.displayProgramInfo();
    cout << "Price after Platinum discount: $" << platinum.calculateDiscount(basePrice) << "\n" << endl;



    // 5,6
    // Створюємо об'єкт VIPCustomer
    VIPCustomer vipCustomer("Alice Johnson", "alice@vip.com", 3, " ");
    cout << "\n----- VIP Customer Info -----" << endl;
    vipCustomer.displayCustomerInfo();

    // Демонстрація застосування знижки
    double originalPrice = 500.0;
    double discountedPrice = vipCustomer.applyDiscount(originalPrice);
    cout << "Original Price: $" << originalPrice << endl;
    cout << "Discounted Price for VIP Customer: $" << discountedPrice << "\n" << endl;
    // Використання функції showBaseClassAccess для доступу до членів базового класу
    vipCustomer.showBaseClassAccess();



    // 4
    // Виводимо результат множиного наслідування

        // Створення об'єкта SpecialCustomer
    cout << "Creating a  SpecialCustomer object...\n";
    SpecialCustomer specialCustomer("John Smith", "john@special.com", 150, true, "VIP Lounge Access", 25.0, true);
    cout << "Exiting main...\n";
    cout << "\n----- Special Customer Info -----" << endl;
    specialCustomer.displaySpecialCustomerInfo();

    double discountedPrice_ = specialCustomer.calculateDiscountedServicePrice(0.2); // 20% знижка
    cout << "Discounted Service Price: $" << discountedPrice_ << endl << endl;


    cout << "*********New_updates********* " << endl;
    // Створення об'єкта VIPCustomer
    cout << "Creating a  VIPCustomer object...\n";
    VIPCustomer vipClient("Alice Johnson", "alice@vip.com", 5, "Emily Smith", true);
    cout << "Exiting main...\n";
    vipClient.displayCustomerInfo();
    cout << endl;

    // Створення об'єкта PremiumService
    PremiumService vipService("Personal Spa Session", 100.0, "Sarah Connor", 50);
    cout << vipService.getServiceInfo() << ", Price: $" << vipService.calculateDiscountedPrice(0.1) << endl << endl;

    // Створення об'єкта SuiteRoom
    SuiteRoom suiteRoom(4, 200.0, true, true, "Executive Suite");
    suiteRoom.displayRoomInfo();

    // Створення та виведення інформації про бронювання VIP-клієнта
    BookingRequest bookingRequest("Alice Johnson", 2, "Suite", 3, suiteRoom.calculateCost(3));
    cout << "Total Bookings: " << BookingRequest::getTotalBookings() << endl << endl;

    cout << "***************************** " << endl;

    // Створення об'єкта послуги
    Service breakfast("Breakfast in room", 15.0);
    // Отримання інформації про послугу
    cout << "Service Name: " << breakfast.getServiceName() << endl;
    cout << "Service Price: $" << breakfast.getPrice() << endl;
    cout << "Service Type: " << breakfast.getServiceType() << "\n" << endl;
    cout << "\n********************************" << endl;
    cout << "************11,12,13************" << endl;
    // Створення об'єктів Customer
    Customer customer("John Doe", "john@example.com");
    customer.displayCustomerInfo();

    // Створення об'єктів Room
    Room rooms1(2, "Standard", 50.0);
    Room rooms2(3, "Deluxe", 80.0);

    // Створення об'єкта Hotel та додавання кімнат
    Hotel hotel("Grand Hotel");
    hotel.addRoom(rooms1);
    hotel.addRoom(rooms2);
    hotel.displayRooms();

    // Створення запиту на бронювання
    BookingRequest bookingRequest_("John Doe", 2, "Standard", 3, rooms1.calculateCost(3));
    Booking booking(bookingRequest, rooms1);

    // Додавання послуг до бронювання
    Service breakfast_t("Breakfast", 15.0);
    Service transfer_r("Airport Transfer", 30.0);

    booking.addService(breakfast_t);
    booking.addService(transfer_r);

    // Виведення деталей бронювання
    booking.displayBookingDetails();
    cout << "********************************\n" << endl;



    // Створюємо і повертаємо об'єкт Room за допомогою функції createDeluxeRoom
    Room myRoom = createDeluxeRoom();
    // Додатково викликаємо метод для перевірки
    if (!myRoom.checkAvailability()) {
        cout << "The room has been booked successfully" << endl;
    }
    cout << "\n---------Simple method---------" << endl;
    // 1. Простий спосіб
    Room simpleRoom(2, "Standard", 50.0);  // Просте створення об'єкта
    simpleRoom.displayRoomInfo();
    cout << "---------Explicit method---------" << endl;
    // 2. Явний спосіб
    Room explicitRoom = Room(3, "Deluxe", 80.0);  // Явний виклик конструктора
    explicitRoom.displayRoomInfo();
    cout << "---------Shorted method---------" << endl;
    // 3. Скорочений спосіб
    Room shortRoom{ 4, "Suite", 120.0 };  // Список ініціалізації
    shortRoom.displayRoomInfo();

    cout << "\n\n *|___System Hotel___|*" << endl << endl;
    // Додаємо інформацію про кімнати
    Room room1(2, "Standard", 50.0);
    Room room2(3, "Deluxe", 80.0);
    Room room3(4, "Suite", 120.0);


    // 1. Створення динамічного масиву послуг
    try {
        Service* services = nullptr;

        // Використання try-catch для обробки винятків у випадку невдачі виділення пам'яті
        try {
            services = new Service[5];
        }
        catch (bad_alloc& exception) {
            //Помилка виділення пам'яті для масиву послуг
            cerr << "Error allocating memory for services array: " << exception.what() << endl;
            return 1;
        }
        services[0] = Service("Breakfast in room", 15.0);
        services[1] = Service("Transfer", 30.0);
        services[2] = Service("Pool Access", 20.0);
        services[3] = Service("Spa Treatment", 50.0);
        services[4] = Service("Dinner Package", 40.0);

        // 2. Перевірка на переповнення індексу масиву
        try {
            int index = 5; // Неприпустимий індекс
            if (index < 0 || index >= 5) {
                // Iндекс масиву виходить за межi допустимого значення
                throw out_of_range("The index of the array is out of range\n");
            }
            cout << "Service: " << services[index].getServiceName() << endl;
        }
        catch (const out_of_range& exception) {
            cerr << "\n\nException: " << exception.what() << endl;
        }



        // Створюємо масив із 5 клієнтів
        BookingRequest static  clients[5] = {
            BookingRequest("Alice Smith", 2, "Standard", 3, room1.calculateCost(3)),
            BookingRequest("Bob Johnson", 3, "Deluxe", 4, room2.calculateCost(4)),
            BookingRequest("Charlie Brown", 2, "Standard", 2, room1.calculateCost(2)),
            BookingRequest("David Williams", 4, "Suite", 1, room3.calculateCost(1)),
            BookingRequest("Eve Miller", 3, "Deluxe", 5, room2.calculateCost(5))
        };


        // 4 Пункт
        //Room room1(2, "Standard", 50.0);
        //BookingRequest request("Alice Smith", 2, "Standard", 3, room1.calculateCost(3), 101, room1);
        //
        //
        //// Виведення інформації про бронювання
        //request.displayBookingInfo();
        //cout << "Booking ID: " << request.getBookingID() << endl;
        //cout << "Booked Room Info:" << endl;
        //request.getBookedRoom().displayRoomInfo();

        //  Вивід загальної кількості бронювань
        cout << "------------------" << endl;
        cout << "Total Bookings: " << BookingRequest::getTotalBookings() << endl;
        cout << "------------------" << endl;
        // Виведіть підсумки бронювань
        for (int i = 0; i < 5; i++) {
            displayBookingSummary(clients[i]);
        }
        // Виклик перевантаженої функції з коефіцієнтом знижки
        for (int i = 0; i < 5; i++) {
            displayBookingSummary(clients[i], 0.15); // Наприклад, 15% знижка
        }
        // Копіюємо один з елементів масиву 
        BookingRequest copiedClient = clients[0];
        // Виводимо інформацію про оригінальний та скопійований запитити на бронювання
        cout << "__________________________________" << endl;
        cout << "Original Booking:" << endl;
        clients[0].displayBookingInfo();
        cout << "\nCopied Booking:" << endl;
        copiedClient.displayBookingInfo();
        cout << "__________________________________\n\n" << endl;



        // Виведення інформації про кожного клієнта до підтвердження бронювання
        cout << "Booking Requests Before Confirmation:\n" << endl;
        for (int i = 0; i < 5; i++) {
            clients[i].displayBookingInfo();
        }
        cout << endl;

        // Випадкове додавання послуг для кожного клієнта
        for (int i = 0; i < 5; i++) {
            int randomServiceCount = rand() % 5; // Випадкове число від 0 до 2 (кількість послуг для клієнта)

            if (randomServiceCount > 0) clients[i].addService(services[0]); // Додаємо breakfast, якщо > 0
            if (randomServiceCount > 1) clients[i].addService(services[1]); // Додаємо transfer, якщо > 1
            if (randomServiceCount > 3) clients[i].addService(services[3]); // Додаємо Spa Treatment якщо > 3
            if (randomServiceCount > 1) clients[i].addService(services[4]); // Додаємо Dinner Package якщо > 1
            if (rand() % 2 == 0) clients[i].addService(services[2]);        // Додаємо poolAccess випадково (50% шанс)
        }


        // Підтвердження бронювання для всіх клієнтів та виведення інформації після підтвердження
        cout << "\nBooking Requests After Confirmation:" << endl;
        for (int i = 0; i < 5; i++) {
            clients[i].confirmBooking();
        }

        // Збереження інформації у файл
        saveClientsToFile(clients, 5, "clients.txt");

        // Завантаження клієнтів з файлу та виведення інформації
        cout << "Loaded Booking Requests:" << endl;
        BookingRequest::loadClientsFromFile(clients, 5, "clients.txt");
        for (int i = 0; i < 5; i++) {
            clients[i].displayBookingInfo();
        }



        // 3. Видалення масиву з обробкою виключення для запобігання видалення нульового покажчика
        try {
            delete[] services;
            services = nullptr; // Обнулення покажчика

            // Повторне видалення нульового покажчика
            if (!services) {
                // Спроба видалення нульового покажчика
                throw runtime_error("Attempting to delete a null pointer");
            }
            delete[] services;
        }
        catch (const runtime_error& exception) {
            cerr << "\nException: " << exception.what() << endl << endl;
        }
    }
    catch (const exception& exception) {
        // Загальна помилка
        cerr << "Common error: " << exception.what() << endl;
    }

    // Звільнення динамічної пам'яті
    delete[] dynamicServices;
    delete room_service;
    // 5.9
    // Звільнення пам'яті
    delete maintenance1;
    delete maintenance2;
    delete maintenance[0];
    delete maintenance[1];
    delete dynamicService;
    return 0;
}