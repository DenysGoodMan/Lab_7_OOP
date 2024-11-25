#pragma once
#ifndef HOTELBOOKINGSYSTEM_H
#define HOTELBOOKINGSYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <list>
#include <stack>
#include <functional>
using namespace std;

class Room;
class Service;


class Person {
protected:
    string name;
    string contactInfo;

public:
    Person(const string& name, const string& contactInfo) /*: name(name), contactInfo(contactInfo)*/ {}
    virtual void displayInfo() const = 0; // Абстрактний метод
    ~Person();

    string getName() const {
        return name;
    }
    string getContactInfo() const {
        return contactInfo;
    }
};
// Запит на бронювання
class BookingRequest {
private:
    string clientName;
    int roomCapacity; // Кількість місць у кімнаті
    string roomClass; // Клас кімнати
    int stayDuration; // Тривальсть перебування
    bool isConfirmed; // Підтвердження бронювання
    double prepayment; // Передоплата
    double totalCost; // Загальна вартість
    vector<Service> additionalServices; // Додаткові послуги

    // Статичне поле для підрахунку кількості бронювань
    static int totalBookings;
    //const int bookingID;         // Константне поле для ID бронювання
    ////static Room defaultRoom;   // Статичний об'єкт Room за замовчуванням
    //const Room& bookedRoom;      // Поле-посилання на об'єкт Room
    static void incrementBookings(); // Статичний метод для збільшення кількості бронювань

public:
    BookingRequest(); // Конструктор без параметрів
    BookingRequest(string clientName, int roomCapacity, string roomClass, int stayDuration, double totalCost/*, int id, const Room& room*/); // Конструктор з параметрами
    BookingRequest(const BookingRequest& other); // Конструктор копіювання
    /* BookingRequest(int id, const Room& room);*/
    ~BookingRequest();

    string getClientName() const;
    int getRoomCapacity() const;
    string getRoomClass() const;
    int getStayDuration() const;
    double getPrepayment() const;
    double getTotalCost() const;
    bool  getIsConfirmed() const;

    static int getTotalBookings(); // Статичний метод для отримання кількості бронювань

    //5.4
    // Віртуальний метод для отримання короткої інформації про бронювання
    virtual string getBookingSummary() const;


    //int getBookingID() const;             // Геттер для bookingID
    //const Room& getBookedRoom() const;    // Геттер для bookedRoom

    void confirmBooking(); // Підтвердити бронювання
    void cancelBooking(); // Відхилити бронювання
    void setPrepayment(double amount); // Встановити предоплату
    void addService(const Service& service); // Додати послугу
    void displayBookingInfo() const; // Відображення інформації про бронювання
    string toFileFormat() const;  // Новий метод для форматування інформації у вигляді рядка

    /* void loadClientsFromFile(BookingRequest clients[], int n, const string& filename);*/
    static void loadClientsFromFile(BookingRequest clients[], int size, const string& filename);
};

class Service {
private:
    string serviceName;
    double price;

    string serviceType;
    int serviceDuration; // тривалість у хвилинах
    bool isAvailable;    // доступність послуги
public:
    Service();
    /* Service(string serviceName, double price);*/
    Service(const string& name, double price, const string& type = "Standard", int duration = 60, bool available = true);
    virtual ~Service();  // Віртуальний деструктро
    virtual string getServiceInfo() const;
    virtual double getPrice() const;
    Service(const Service& other);  // Конструктор копіювання

    string getServiceName() const; // Отримати ім'я послуги
    /*double getPrice() const;*/ // Отримати ціну послуги 
    string getServiceType() const;  // Метод для отримання типу (назви) послуги
    void Write(ofstream& outFile) const;
    void Read(ifstream& inFile);


    // 5.9
    virtual void displayService() const;
    virtual double calculateCost() const;
};

// 6.6
// Шаблонний клас ServiceManager
template <typename T>
class ServiceManager {
private:
    vector<T> services; // Список послуг

public:
    // Додавання послуги
    void addService(const T& service) {
        services.push_back(service);
    }

    // Виведення всіх послуг
    void displayAllServices() const {
        for (const auto& service : services) {
            service.displayService();
        }
    }

    // Розрахунок загальної вартості всіх послуг
    double calculateTotalCost() const {
        double totalCost = 0.0;
        for (const auto& service : services) {
            totalCost += service.calculateCost();
        }
        return totalCost;
    }
};


// Підклас PremiumService
class PremiumService : public Service {
private:
    bool isPersonalized;  // персоналізована послуга
    string staffMember;   // відповідальний працівник
    int loyaltyPoints;    // бонусні бали за послугу

public:
    PremiumService(const string& name, double price, const string& staff, int points, bool personalized = true);
    ~PremiumService();
    string getServiceInfo() const override;
    double calculateDiscountedPrice(double discount) const;
    virtual int getLoyaltyBonus() const;
};

// 5.9
// Похідний клас для RoomService
class RoomService : public Service {
private:
    string serviceName;
    double cost;
public:
    RoomService(const string& name, double c) : serviceName(name), cost(c) {}

    void displayService() const override {
        cout << "Room Service: " << serviceName << " - Cost: " << cost << endl;
    }
    double calculateCost() const override {
        return cost;
    }
};



class Room {
protected:
    int roomCapacity; // Місткість кімнати
    string roomClass; // Клас кімнати
    double pricePerNight; // Ціна за добу
    bool isAvailable; // Метод, який показує чи є в наявності така кімната
    string view; // Переглянути
    int floor; // Поверх
    double price_room; // Ціна кімнати


public:
    /*  Room();*/
    Room(int capacity, const string& classType, double price, bool available = true, const string& view = "Standard", int floor = 1, double price_room = 1);

    virtual ~Room() = default; // Віртуальний деструктор за замовчуванням

    virtual double calculateCost(int nights) const;
    virtual void displayRoomInfo() const;
    Room(const Room& other);  // Конструктор копіювання
    /* ~Room();*/
    bool checkAvailability() const; // Перевірити наявність

    /* double calculateCost(int nights) const;*/ // Розрахунок вартості
    void bookRoom(); // 
    void releaseRoom();

    /*void displayRoomInfo() const;*/ // Метод для відображення інформації про кімнату
    //static

    // 7.3
    double getPrice() const;
    void setPrice(double newPrice);

    // 6.3
    // Перевантаження оператора *
    Room& operator*(double multiplier) {
        this->pricePerNight *= multiplier;
        return *this;
    }

    // Перевантаження оператора =
    Room& operator=(const Room& other) {
        if (this != &other) {
            this->roomClass = other.roomClass;
            this->pricePerNight = other.pricePerNight;
        }
        return *this;
    }
    // Перевантаження оператора +=
    Room& operator+=(double value) {
        this->pricePerNight += value;
        return *this;
    }

    // Перевантаження оператора -=
    Room& operator-=(double value) {
        this->pricePerNight -= value;
        return *this;
    }

    // Перевантаження оператора *=
    Room& operator*=(double multiplier) {
        this->pricePerNight *= multiplier;
        return *this;
    }

    // Перевантаження оператора []
    char operator[](size_t index) const {
        if (index >= roomClass.size()) {
            throw out_of_range("Index out of range");
        }
        return roomClass[index];
    }


    // 6.2
    // Перевантаження оператора +
    Room& operator+(double value) {
        this->pricePerNight += value;
        return *this;
    }
    // Перевантаження оператора -
    Room& operator-(double value) {
        this->pricePerNight -= value;
        return *this;
    }

    // 6.1
     // Префіксний оператор ++ (збільшує ціну на 10)
    Room& operator++() {
        pricePerNight += 10.0; // Збільшуємо ціну на 10
        return *this;
    }

    // Постфіксний оператор ++ (збільшує ціну на 10)
    Room operator++(int) {
        Room temp = *this;
        pricePerNight += 10.0; // Збільшуємо ціну на 10
        return temp;
    }

    // Префіксний оператор -- (зменшує ціну на 10)
    Room& operator--() {
        if (pricePerNight > 10.0) {
            pricePerNight -= 10.0; // Зменшуємо ціну на 10
        }
        return *this;
    }

    // Постфіксний оператор -- (зменшує ціну на 10)
    Room operator--(int) {
        Room temp = *this;
        if (pricePerNight > 10.0) {
            pricePerNight -= 10.0; // Зменшуємо ціну на 10
        }
        return temp;
    }

};
// Підклас SuiteRoom
class SuiteRoom : private Room {
private:
    bool hasJacuzzi;
    bool hasBalcony;
    string suiteCategory;

public:
    SuiteRoom(int capacity, double price, bool jacuzzi, bool balcony, const string& category = "VIP Suite");
    ~SuiteRoom();
    double calculateCost(int nights) const override;
    void displayRoomInfo() const override;
    virtual string getSuiteCategory() const;
};


Room createDeluxeRoom();  // Функція, яка створює і повертає об'єкт Room
void displayBookingSummary(const BookingRequest& booking);
// Додаємо знижку
void displayBookingSummary(const BookingRequest& booking, double discountRate);  // Перевантаження





// 1. Ассоціація
class Customer {
protected:
    string name;
    string contactInfo;
    int loyaltyPoints;
    bool hasDiscount; // наявність знижки

public:
    Customer();
    Customer(const string& name, const string& contactInfo, int points = 0, bool discount = false);
    virtual ~Customer(); // Віртуальний деструктор
    virtual void displayCustomerInfo() const;
    int getLoyaltyPoints() const;

    /* void displayCustomerInfo() const;*/

    // 6.4
    void setName(const string& newName);
    void setLoyaltyPoints(const int& points);
    void setHasDiscount(const bool& discount);

    // Шаблонна функція для оновлення властивостей
    template <typename Any_Type>
    void updateProperty(void (Customer::* setter)(const Any_Type&), const Any_Type& newValue) {
        (this->*setter)(newValue); // Викликаємо відповідний сеттер
        cout << "Property updated successfully!" << endl;
    }
    /* void updateProperty(Any_Type& property, const Any_Type& newValue) {
         property = newValue;
         cout << "Property updated successfully!" << endl;
     }*/
};
// Підклас VIPCustomer
class VIPCustomer : public Customer {
private:
    int vipLevel;
    string personalAssistant;
    bool accessToLounge;
    int assistentIndex;


public:
    VIPCustomer();
    VIPCustomer(const string& name, const string& contactInfo, int level, const string& assistant, bool lounge = true, int assistentindex = 0);

    void displayCustomerInfo() const override;
    double applyDiscount(double basePrice) const;
    void showBaseClassAccess() const;
    virtual string getPersonalAssistant(int assistentindex) const;

    // 5.3
    ~VIPCustomer() override; // Перевизначаємо деструктор підкласу VIPCustomer 

    // Доступ до методу getLoyaltyPoints базового класу через оператор using
    using Customer::getLoyaltyPoints;
};


// Демонстрація множинного наслідування
class SpecialCustomer : public Customer, public Service {
private:
    int loyaltyPoints;
    bool hasSpecialPrivileges;
public:
    SpecialCustomer(const string& name, const string& contactInfo, int points, bool discount, const string& serviceName, double price, bool privileges);
    virtual ~SpecialCustomer(); // Віртуальний деструктор
    void displaySpecialCustomerInfo() const;
    double calculateDiscountedServicePrice(double discountRate) const;
    virtual string getLoyaltyStatus() const;
};

// 2. Агрегація
class Hotel {
private:
    string hotelName;
    vector<Room> rooms; // Агрегація: Hotel має Rooms
    // 7.1
    list<string> customerQueue; // Список клієнтів, які чекають на реєстрацію
    stack<string> bookingHistory; // Стек для історії заброньованих кімнат

public:
    Hotel(string hotelName);
    ~Hotel();
    void addRoom(const Room& room);
    void displayRooms() const;

    // 7.1
     // Методи для роботи зі списком клієнтів
    void addCustomerToQueue(const string& customer);
    void displayCustomerQueue() const;
    // Методи для роботи зі стеком історії бронювань
    void addBookingToHistory(const string& booking);
    void displayBookingHistory() const;

    // 7.2
     // Методи демонстрації роботи ітераторів
    void demonstrateInputIterator() const;
    void demonstrateOutputIterator();
    void demonstrateForwardIterator() const;
    void demonstrateBidirectionalIterator() const;
    void demonstrateRandomAccessIterator() const;

    // 7.3
     // Методи для демонстрації алгоритмів
    void sortRoomsByPrice();
    void findCustomerInQueue(const string& customer) const;
    void countCustomersWithName(const string& name) const;
    void increaseRoomPrices(double percentage);
    void reverseCustomerQueue();


};

// 3. Композиція
class Booking {
private:
    BookingRequest bookingRequest; // Композиція: Booking містить BookingRequest
    Room room;
    vector<Service> services;

public:
    Booking(const BookingRequest& bookingRequest, const Room& room);
    void addService(const Service& service);
    void displayBookingDetails() const;
};


// Абстрактний клас LoyaltyProgram
class LoyaltyProgram {
protected:
    string programName;

public:
    LoyaltyProgram(const string& name) : programName(name) {}

    // Чисто віртуальні методи
    virtual double calculateDiscount(double basePrice) const = 0;
    virtual void displayProgramInfo() const = 0;

    virtual ~LoyaltyProgram(); // Віртуальний деструктор
};

// Клас SilverProgram
class SilverProgram : virtual public LoyaltyProgram {
public:
    SilverProgram() : LoyaltyProgram("Silver Program") {}

    double calculateDiscount(double basePrice) const override {
        return basePrice * 0.95; // 5% знижка
    }

    void displayProgramInfo() const override {
        cout << "Loyalty Program: " << programName << " (5% discount)" << endl;
    }
};

// Клас GoldProgram
class GoldProgram : virtual public LoyaltyProgram {
public:
    GoldProgram() : LoyaltyProgram("Gold Program") {}

    double calculateDiscount(double basePrice) const override {
        return basePrice * 0.9; // 10% знижка
    }

    void displayProgramInfo() const override {
        cout << "Loyalty Program: " << programName << " (10% discount)" << endl;
    }
};

// Клас PlatinumProgram
class PlatinumProgram : virtual public LoyaltyProgram {
public:
    PlatinumProgram() : LoyaltyProgram("Platinum Program") {}

    double calculateDiscount(double basePrice) const override {
        return basePrice * 0.85; // 15% знижка
    }

    void displayProgramInfo() const override {
        cout << "Loyalty Program: " << programName << " (15% discount)" << endl;
    }

};
// 6.5
template <typename admin_1, typename admin_2, typename admin_3, typename admin_4, typename admin_5>
class Administrator {
private:
    admin_1 adminName; // Ім'я адміністратора
    admin_2 receptionPhone; // Номер телефону рецепції
    admin_3 roomDatabase; // База номерів кімнат готелю
    admin_4 availableRooms; // Вільні кімнати готелю
    admin_5 newClientName; // Ім'я нового клієнта

public:
    // Конструктор
    Administrator(admin_1 adminName, admin_2 receptionPhone, admin_3 roomDatabase, admin_4 availableRooms, admin_5 newClientName)
        : adminName(adminName), receptionPhone(receptionPhone), roomDatabase(roomDatabase),
        availableRooms(availableRooms), newClientName(newClientName) {}

    // Віртуальний деструктор
    virtual ~Administrator() {};
    // Методи для отримання даних
    admin_1 getAdminName() const {
        return adminName;
    }
    admin_2 getReceptionPhone() const {
        return receptionPhone;
    }
    admin_3 getRoomDatabase() const {
        return roomDatabase;
    }
    admin_4 getAvailableRooms() const {
        return availableRooms;
    }
    admin_5 getNewClientName() const {
        return newClientName;
    }

    // Метод для виведення інформації про адміністратора
    void displayAdministratorInfo() const {
        cout << "Administrator Name: " << adminName << endl;
        cout << "Reception Phone: " << receptionPhone << endl;

        cout << "Room Database: ";
        for (const auto& room : roomDatabase) {
            cout << room << " ";
        }
        cout << endl;

        cout << "Available Rooms: ";
        for (const auto& room : availableRooms) {
            cout << room << " ";
        }
        cout << endl;

        cout << "New Client Name: " << newClientName << endl;
    }


};

// 7.4
class Functor {
public:
    // Операції додавання, віднімання, множення, ділення
    class Plus {
    public:
        int operator()(int a, int b) const {
            return a + b;
        }
    };

    class Minus {
    public:
        int operator()(int a, int b) const {
            return a - b;
        }
    };

    class Multiply {
    public:
        int operator()(int a, int b) const {
            return a * b;
        }
    };

    class Divide {
    public:
        int operator()(int a, int b) const {
            if (b == 0) {
                throw invalid_argument("Division by zero");
            }
            return a / b;
        }
    };

    class Modulo {
    public:
        int operator()(int a, int b) const {
            if (b == 0) {
                throw invalid_argument("Division by zero");
            }
            return a % b;
        }
    };

    // Операції порівняння
    class EqualTo {
    public:
        bool operator()(int a, int b) const {
            return a == b;
        }
    };

    class NotEqualTo {
    public:
        bool operator()(int a, int b) const {
            return a != b;
        }
    };

    class GreaterThan {
    public:
        bool operator()(int a, int b) const {
            return a > b;
        }
    };

    class LessThan {
    public:
        bool operator()(int a, int b) const {
            return a < b;
        }
    };

    class GreaterThanOrEqual {
    public:
        bool operator()(int a, int b) const {
            return a >= b;
        }
    };

    class LessThanOrEqual {
    public:
        bool operator()(int a, int b) const {
            return a <= b;
        }
    };

    // Логічні операції
    class LogicalAnd {
    public:
        bool operator()(bool a, bool b) const {
            return a && b;
        }
    };

    class LogicalOr {
    public:
        bool operator()(bool a, bool b) const {
            return a || b;
        }
    };

    class LogicalNot {
    public:
        bool operator()(bool a) const {
            return !a;
        }
    };
};

#endif // HOTELBOOKINGSYSTEM_H