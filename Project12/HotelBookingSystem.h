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
    virtual void displayInfo() const = 0; // ����������� �����
    ~Person();

    string getName() const {
        return name;
    }
    string getContactInfo() const {
        return contactInfo;
    }
};
// ����� �� ����������
class BookingRequest {
private:
    string clientName;
    int roomCapacity; // ʳ������ ���� � �����
    string roomClass; // ���� ������
    int stayDuration; // ���������� �����������
    bool isConfirmed; // ϳ����������� ����������
    double prepayment; // �����������
    double totalCost; // �������� �������
    vector<Service> additionalServices; // �������� �������

    // �������� ���� ��� ��������� ������� ���������
    static int totalBookings;
    //const int bookingID;         // ���������� ���� ��� ID ����������
    ////static Room defaultRoom;   // ��������� ��'��� Room �� �������������
    //const Room& bookedRoom;      // ����-��������� �� ��'��� Room
    static void incrementBookings(); // ��������� ����� ��� ��������� ������� ���������

public:
    BookingRequest(); // ����������� ��� ���������
    BookingRequest(string clientName, int roomCapacity, string roomClass, int stayDuration, double totalCost/*, int id, const Room& room*/); // ����������� � �����������
    BookingRequest(const BookingRequest& other); // ����������� ���������
    /* BookingRequest(int id, const Room& room);*/
    ~BookingRequest();

    string getClientName() const;
    int getRoomCapacity() const;
    string getRoomClass() const;
    int getStayDuration() const;
    double getPrepayment() const;
    double getTotalCost() const;
    bool  getIsConfirmed() const;

    static int getTotalBookings(); // ��������� ����� ��� ��������� ������� ���������

    //5.4
    // ³��������� ����� ��� ��������� ������� ���������� ��� ����������
    virtual string getBookingSummary() const;


    //int getBookingID() const;             // ������ ��� bookingID
    //const Room& getBookedRoom() const;    // ������ ��� bookedRoom

    void confirmBooking(); // ϳ��������� ����������
    void cancelBooking(); // ³������� ����������
    void setPrepayment(double amount); // ���������� ����������
    void addService(const Service& service); // ������ �������
    void displayBookingInfo() const; // ³���������� ���������� ��� ����������
    string toFileFormat() const;  // ����� ����� ��� ������������ ���������� � ������ �����

    /* void loadClientsFromFile(BookingRequest clients[], int n, const string& filename);*/
    static void loadClientsFromFile(BookingRequest clients[], int size, const string& filename);
};

class Service {
private:
    string serviceName;
    double price;

    string serviceType;
    int serviceDuration; // ��������� � ��������
    bool isAvailable;    // ���������� �������
public:
    Service();
    /* Service(string serviceName, double price);*/
    Service(const string& name, double price, const string& type = "Standard", int duration = 60, bool available = true);
    virtual ~Service();  // ³��������� ����������
    virtual string getServiceInfo() const;
    virtual double getPrice() const;
    Service(const Service& other);  // ����������� ���������

    string getServiceName() const; // �������� ��'� �������
    /*double getPrice() const;*/ // �������� ���� ������� 
    string getServiceType() const;  // ����� ��� ��������� ���� (�����) �������
    void Write(ofstream& outFile) const;
    void Read(ifstream& inFile);


    // 5.9
    virtual void displayService() const;
    virtual double calculateCost() const;
};

// 6.6
// ��������� ���� ServiceManager
template <typename T>
class ServiceManager {
private:
    vector<T> services; // ������ ������

public:
    // ��������� �������
    void addService(const T& service) {
        services.push_back(service);
    }

    // ��������� ��� ������
    void displayAllServices() const {
        for (const auto& service : services) {
            service.displayService();
        }
    }

    // ���������� �������� ������� ��� ������
    double calculateTotalCost() const {
        double totalCost = 0.0;
        for (const auto& service : services) {
            totalCost += service.calculateCost();
        }
        return totalCost;
    }
};


// ϳ����� PremiumService
class PremiumService : public Service {
private:
    bool isPersonalized;  // �������������� �������
    string staffMember;   // ������������ ���������
    int loyaltyPoints;    // ������ ���� �� �������

public:
    PremiumService(const string& name, double price, const string& staff, int points, bool personalized = true);
    ~PremiumService();
    string getServiceInfo() const override;
    double calculateDiscountedPrice(double discount) const;
    virtual int getLoyaltyBonus() const;
};

// 5.9
// �������� ���� ��� RoomService
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
    int roomCapacity; // ̳������ ������
    string roomClass; // ���� ������
    double pricePerNight; // ֳ�� �� ����
    bool isAvailable; // �����, ���� ������ �� � � �������� ���� ������
    string view; // �����������
    int floor; // ������
    double price_room; // ֳ�� ������


public:
    /*  Room();*/
    Room(int capacity, const string& classType, double price, bool available = true, const string& view = "Standard", int floor = 1, double price_room = 1);

    virtual ~Room() = default; // ³��������� ���������� �� �������������

    virtual double calculateCost(int nights) const;
    virtual void displayRoomInfo() const;
    Room(const Room& other);  // ����������� ���������
    /* ~Room();*/
    bool checkAvailability() const; // ��������� ��������

    /* double calculateCost(int nights) const;*/ // ���������� �������
    void bookRoom(); // 
    void releaseRoom();

    /*void displayRoomInfo() const;*/ // ����� ��� ����������� ���������� ��� ������
    //static

    // 7.3
    double getPrice() const;
    void setPrice(double newPrice);

    // 6.3
    // �������������� ��������� *
    Room& operator*(double multiplier) {
        this->pricePerNight *= multiplier;
        return *this;
    }

    // �������������� ��������� =
    Room& operator=(const Room& other) {
        if (this != &other) {
            this->roomClass = other.roomClass;
            this->pricePerNight = other.pricePerNight;
        }
        return *this;
    }
    // �������������� ��������� +=
    Room& operator+=(double value) {
        this->pricePerNight += value;
        return *this;
    }

    // �������������� ��������� -=
    Room& operator-=(double value) {
        this->pricePerNight -= value;
        return *this;
    }

    // �������������� ��������� *=
    Room& operator*=(double multiplier) {
        this->pricePerNight *= multiplier;
        return *this;
    }

    // �������������� ��������� []
    char operator[](size_t index) const {
        if (index >= roomClass.size()) {
            throw out_of_range("Index out of range");
        }
        return roomClass[index];
    }


    // 6.2
    // �������������� ��������� +
    Room& operator+(double value) {
        this->pricePerNight += value;
        return *this;
    }
    // �������������� ��������� -
    Room& operator-(double value) {
        this->pricePerNight -= value;
        return *this;
    }

    // 6.1
     // ���������� �������� ++ (������ ���� �� 10)
    Room& operator++() {
        pricePerNight += 10.0; // �������� ���� �� 10
        return *this;
    }

    // ����������� �������� ++ (������ ���� �� 10)
    Room operator++(int) {
        Room temp = *this;
        pricePerNight += 10.0; // �������� ���� �� 10
        return temp;
    }

    // ���������� �������� -- (������ ���� �� 10)
    Room& operator--() {
        if (pricePerNight > 10.0) {
            pricePerNight -= 10.0; // �������� ���� �� 10
        }
        return *this;
    }

    // ����������� �������� -- (������ ���� �� 10)
    Room operator--(int) {
        Room temp = *this;
        if (pricePerNight > 10.0) {
            pricePerNight -= 10.0; // �������� ���� �� 10
        }
        return temp;
    }

};
// ϳ����� SuiteRoom
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


Room createDeluxeRoom();  // �������, ��� ������� � ������� ��'��� Room
void displayBookingSummary(const BookingRequest& booking);
// ������ ������
void displayBookingSummary(const BookingRequest& booking, double discountRate);  // ��������������





// 1. ����������
class Customer {
protected:
    string name;
    string contactInfo;
    int loyaltyPoints;
    bool hasDiscount; // �������� ������

public:
    Customer();
    Customer(const string& name, const string& contactInfo, int points = 0, bool discount = false);
    virtual ~Customer(); // ³��������� ����������
    virtual void displayCustomerInfo() const;
    int getLoyaltyPoints() const;

    /* void displayCustomerInfo() const;*/

    // 6.4
    void setName(const string& newName);
    void setLoyaltyPoints(const int& points);
    void setHasDiscount(const bool& discount);

    // �������� ������� ��� ��������� ������������
    template <typename Any_Type>
    void updateProperty(void (Customer::* setter)(const Any_Type&), const Any_Type& newValue) {
        (this->*setter)(newValue); // ��������� ��������� ������
        cout << "Property updated successfully!" << endl;
    }
    /* void updateProperty(Any_Type& property, const Any_Type& newValue) {
         property = newValue;
         cout << "Property updated successfully!" << endl;
     }*/
};
// ϳ����� VIPCustomer
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
    ~VIPCustomer() override; // ������������� ���������� ������� VIPCustomer 

    // ������ �� ������ getLoyaltyPoints �������� ����� ����� �������� using
    using Customer::getLoyaltyPoints;
};


// ������������ ���������� �����������
class SpecialCustomer : public Customer, public Service {
private:
    int loyaltyPoints;
    bool hasSpecialPrivileges;
public:
    SpecialCustomer(const string& name, const string& contactInfo, int points, bool discount, const string& serviceName, double price, bool privileges);
    virtual ~SpecialCustomer(); // ³��������� ����������
    void displaySpecialCustomerInfo() const;
    double calculateDiscountedServicePrice(double discountRate) const;
    virtual string getLoyaltyStatus() const;
};

// 2. ���������
class Hotel {
private:
    string hotelName;
    vector<Room> rooms; // ���������: Hotel �� Rooms
    // 7.1
    list<string> customerQueue; // ������ �볺���, �� ������� �� ���������
    stack<string> bookingHistory; // ���� ��� ����� ������������� �����

public:
    Hotel(string hotelName);
    ~Hotel();
    void addRoom(const Room& room);
    void displayRooms() const;

    // 7.1
     // ������ ��� ������ � ������� �볺���
    void addCustomerToQueue(const string& customer);
    void displayCustomerQueue() const;
    // ������ ��� ������ � ������ ����� ���������
    void addBookingToHistory(const string& booking);
    void displayBookingHistory() const;

    // 7.2
     // ������ ������������ ������ ���������
    void demonstrateInputIterator() const;
    void demonstrateOutputIterator();
    void demonstrateForwardIterator() const;
    void demonstrateBidirectionalIterator() const;
    void demonstrateRandomAccessIterator() const;

    // 7.3
     // ������ ��� ������������ ���������
    void sortRoomsByPrice();
    void findCustomerInQueue(const string& customer) const;
    void countCustomersWithName(const string& name) const;
    void increaseRoomPrices(double percentage);
    void reverseCustomerQueue();


};

// 3. ����������
class Booking {
private:
    BookingRequest bookingRequest; // ����������: Booking ������ BookingRequest
    Room room;
    vector<Service> services;

public:
    Booking(const BookingRequest& bookingRequest, const Room& room);
    void addService(const Service& service);
    void displayBookingDetails() const;
};


// ����������� ���� LoyaltyProgram
class LoyaltyProgram {
protected:
    string programName;

public:
    LoyaltyProgram(const string& name) : programName(name) {}

    // ����� �������� ������
    virtual double calculateDiscount(double basePrice) const = 0;
    virtual void displayProgramInfo() const = 0;

    virtual ~LoyaltyProgram(); // ³��������� ����������
};

// ���� SilverProgram
class SilverProgram : virtual public LoyaltyProgram {
public:
    SilverProgram() : LoyaltyProgram("Silver Program") {}

    double calculateDiscount(double basePrice) const override {
        return basePrice * 0.95; // 5% ������
    }

    void displayProgramInfo() const override {
        cout << "Loyalty Program: " << programName << " (5% discount)" << endl;
    }
};

// ���� GoldProgram
class GoldProgram : virtual public LoyaltyProgram {
public:
    GoldProgram() : LoyaltyProgram("Gold Program") {}

    double calculateDiscount(double basePrice) const override {
        return basePrice * 0.9; // 10% ������
    }

    void displayProgramInfo() const override {
        cout << "Loyalty Program: " << programName << " (10% discount)" << endl;
    }
};

// ���� PlatinumProgram
class PlatinumProgram : virtual public LoyaltyProgram {
public:
    PlatinumProgram() : LoyaltyProgram("Platinum Program") {}

    double calculateDiscount(double basePrice) const override {
        return basePrice * 0.85; // 15% ������
    }

    void displayProgramInfo() const override {
        cout << "Loyalty Program: " << programName << " (15% discount)" << endl;
    }

};
// 6.5
template <typename admin_1, typename admin_2, typename admin_3, typename admin_4, typename admin_5>
class Administrator {
private:
    admin_1 adminName; // ��'� ������������
    admin_2 receptionPhone; // ����� �������� ��������
    admin_3 roomDatabase; // ���� ������ ����� ������
    admin_4 availableRooms; // ³��� ������ ������
    admin_5 newClientName; // ��'� ������ �볺���

public:
    // �����������
    Administrator(admin_1 adminName, admin_2 receptionPhone, admin_3 roomDatabase, admin_4 availableRooms, admin_5 newClientName)
        : adminName(adminName), receptionPhone(receptionPhone), roomDatabase(roomDatabase),
        availableRooms(availableRooms), newClientName(newClientName) {}

    // ³��������� ����������
    virtual ~Administrator() {};
    // ������ ��� ��������� �����
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

    // ����� ��� ��������� ���������� ��� ������������
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
    // �������� ���������, ��������, ��������, ������
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

    // �������� ���������
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

    // ����� ��������
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