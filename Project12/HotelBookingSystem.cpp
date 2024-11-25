#include "HotelBookingSystem.h"
#include <algorithm>
#include <numeric>

// Ініціалізація статичного поля
int BookingRequest::totalBookings = 0;

BookingRequest::BookingRequest() : clientName(""), roomCapacity(0), roomClass(""),
stayDuration(0), isConfirmed(false), prepayment(0.0), totalCost(0.0)/*, bookingID(0),
bookedRoom(*(new Room(0, "", 0)))*/ {
    incrementBookings();
}
// Конструктори та методи BookingRequest
BookingRequest::BookingRequest(string clientName, int roomCapacity, string roomClass, int stayDuration, double totalCost/*, int id, const Room& room*/)
    : clientName(clientName), roomCapacity(roomCapacity), roomClass(roomClass), stayDuration(stayDuration),
    isConfirmed(false), prepayment(0.0), totalCost(totalCost) {
    incrementBookings();

    cout << "BookingRequest constructor called: " << clientName << endl;
}

// Конструктор копіювання для BookingRequest
BookingRequest::BookingRequest(const BookingRequest& other) : clientName(other.clientName), roomCapacity(other.roomCapacity), roomClass(other.roomClass),
stayDuration(other.stayDuration), isConfirmed(other.isConfirmed), prepayment(other.prepayment), totalCost(other.totalCost),
additionalServices(other.additionalServices)/*, bookingID(other.bookingID),
bookedRoom(other.bookedRoom)*/ {}  // Копіювання вектора послуг

//BookingRequest::BookingRequest(int id, const Room& room) : bookingID(id), bookedRoom(room) {}


// Деструктор класу BookingRequest, який видаляє об'єкт clientName
BookingRequest::~BookingRequest() {
    /*     cout << "Client " << clientName << " destructor" << endl;*/
    cout << "BookingRequest destructor called: " << clientName << endl;
}

int BookingRequest::getTotalBookings() {
    return totalBookings;
}

void BookingRequest::incrementBookings() {
    totalBookings++;
}

string BookingRequest::getClientName() const {
    return clientName;
}
int BookingRequest::getRoomCapacity() const {
    return roomCapacity;
}
string BookingRequest::getRoomClass() const {
    return roomClass;
}
int BookingRequest::getStayDuration() const {
    return stayDuration;
}
double BookingRequest::getPrepayment() const {
    return prepayment;
}
double BookingRequest::getTotalCost() const {
    return totalCost;
}
bool BookingRequest::getIsConfirmed() const {
    return isConfirmed;
}

//int BookingRequest::getBookingID() const {
//    return bookingID;
//}
//const Room& BookingRequest::getBookedRoom() const {
//    return bookedRoom;
//}
void BookingRequest::confirmBooking() {
    isConfirmed = true;
}
void BookingRequest::cancelBooking() {
    isConfirmed = false;
}

void BookingRequest::setPrepayment(double amount) {
    if (amount <= totalCost) {
        prepayment = amount;
    }
}

void BookingRequest::addService(const Service& service) {
    additionalServices.push_back(service);
    totalCost += service.getPrice();
}

void BookingRequest::displayBookingInfo() const {
    cout << "Client: " << clientName << endl;
    cout << "Room Capacity: " << roomCapacity << ", Class: " << roomClass << endl;
    cout << "Stay Duration: " << stayDuration << " nights" << endl;
    cout << "Total Cost: $" << totalCost << ", Prepayment: $" << prepayment << endl;
    cout << "Confirmed: " << (isConfirmed ? "Yes" : "No") << endl;

    if (!additionalServices.empty()) {
        cout << "\n Additional Services: " << endl;
        for (const auto& service : additionalServices) {
            cout << " - " << service.getServiceName() << " ($" << service.getPrice() << ")" << endl;
        }
    }
    cout << endl;
}
// Метод для обрахунку суми що залишилася.
void displayBookingSummary(const BookingRequest& booking) {
    cout << "Booking Summary:" << endl;
    cout << "Client Name: " << booking.getClientName() << endl;
    cout << "Room Capacity: " << booking.getRoomCapacity() << endl;
    cout << "Room Class: " << booking.getRoomClass() << endl;
    cout << "Stay Duration: " << booking.getStayDuration() << " nights" << endl;
    cout << "Total Cost: $" << booking.getTotalCost() << endl;
    cout << "Prepayment: $" << booking.getPrepayment() << endl;

    double remainingAmount = booking.getTotalCost() - booking.getPrepayment();
    cout << "Remaining Amount: $" << remainingAmount << endl;

    cout << "Booking Confirmed: " << (booking.getIsConfirmed() ? "Yes" : "No") << endl;
    cout << endl;
}
// Цей ж метод, але для обрахунку суми зі знижкою
void displayBookingSummary(const BookingRequest& booking, double discountRate) {
    cout << "Booking Summary with Discount:" << endl;
    cout << "Client Name: " << booking.getClientName() << endl;
    cout << "Room Capacity: " << booking.getRoomCapacity() << endl;
    cout << "Room Class: " << booking.getRoomClass() << endl;
    cout << "Stay Duration: " << booking.getStayDuration() << " nights" << endl;

    double discountedCost = booking.getTotalCost() * (1 - discountRate);
    cout << "Original Total Cost: $" << booking.getTotalCost() << endl;
    cout << "Discount Rate: " << discountRate * 100 << "%" << endl;
    cout << "Discounted Total Cost: $" << discountedCost << endl;
    cout << "Prepayment: $" << booking.getPrepayment() << endl;

    double remainingAmount = discountedCost - booking.getPrepayment();
    cout << "Remaining Amount after Discount: $" << remainingAmount << endl;

    cout << "Booking Confirmed: " << (booking.getIsConfirmed() ? "Yes" : "No") << endl;
    cout << endl;
}
string BookingRequest::getBookingSummary() const {
    return "Client: " + clientName + ", Room Class: " + roomClass + ", Duration: " + to_string(stayDuration) + " nights, Total Cost: $" + to_string(totalCost);
}
string BookingRequest::toFileFormat() const {
    string result = clientName + "," + to_string(roomCapacity) + "," + roomClass + "," +
        to_string(stayDuration) + "," + to_string(totalCost) + "," +
        to_string(prepayment) + "," + (isConfirmed ? "Confirmed" : "Not Confirmed");
    for (const auto& service : additionalServices) {
        result += "," + service.getServiceName() + "($" + to_string(service.getPrice()) + ")";
    }
    return result;
}

void BookingRequest::loadClientsFromFile(BookingRequest clients[], int n, const string& filename) {
    ifstream readFile(filename);
    if (readFile.fail()) {
        cerr << "Помилка відкриття файлу!" << endl;
        return;
    }

    for (int i = 0; i < n; i++) {
        string clientName;
        int roomCapacity;
        string roomClass;
        int stayDuration;
        double totalCost;
        //int bookingID; // Додайте ID бронювання
        //Room; room; // Змініть на правильний об'єкт Room, як ви його визначили

        if (readFile >> clientName >> roomCapacity >> roomClass >> stayDuration >> totalCost) {
            clients[i] = BookingRequest(clientName, roomCapacity, roomClass, stayDuration, totalCost);
        }
    }

    readFile.close();
}

// Конструктори та методи Service
Service::Service() :serviceName(""), price(0.0), serviceType(""), serviceDuration(0), isAvailable(false) {}
Service::Service(const string& name, double price, const string& type, int duration, bool available)
    : serviceName(name), price(price), serviceType(type), serviceDuration(duration), isAvailable(available) {
    cout << "Service constructor called: " << serviceName << endl;
}
//Віртуальний деструктор класу Service
Service:: ~Service() {
    //cout << "Services destructor" << endl;
    cout << "Service destructor called: " << serviceName << endl;
}
// Конструктор копіювання для Service
Service::Service(const Service& other) : serviceName(other.serviceName), price(other.price), serviceType(other.serviceType),
serviceDuration(other.serviceDuration), isAvailable(other.isAvailable) {}


/*  string Service::getServiceInfo() const {
      return serviceName + " (" + serviceType + ")";
  }*/
string Service::getServiceInfo() const {
    return serviceName + " (" + serviceType + "), Duration: " + to_string(serviceDuration) + " minutes, Price: " + to_string(price);
}
string Service::getServiceName() const {
    return serviceName;
}
double Service::getPrice() const {
    return price;
}
// Метод для отримання типу (назви) послуги
string Service::getServiceType() const {
    return serviceName; // Повертає назву послуги
}
// Метод запису даних у файл
void Service::Write(ofstream& outFile) const {
    if (!outFile.is_open()) throw runtime_error("File is not open for writing");
    outFile << serviceName << endl;
    outFile << price << endl;
}
// Метод зчитування даних з файлу
void Service::Read(ifstream& inFile) {
    if (!inFile.is_open()) throw runtime_error("File is not open for reading");
    getline(inFile, serviceName);
    inFile >> price;
    if (inFile.fail()) throw runtime_error("Error reading price from file");
    inFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Пропуск рядка після зчитування
}

// 5.9
void Service::displayService() const {
    cout << getServiceInfo() << endl;
    /*   cout << "base service info " << endl;*/
}
double Service::calculateCost() const {
    return price;
}


// PremiumService class
PremiumService::PremiumService(const string& name, double price, const string& staff, int points, bool personalized)
    : Service(name, price, "Premium"), staffMember(staff), loyaltyPoints(points), isPersonalized(personalized) {
    cout << "PremiumService constructor called: " << loyaltyPoints << endl;
}
PremiumService::~PremiumService() {
    cout << "PremiumService destructor called: " << loyaltyPoints << endl;
}

string PremiumService::getServiceInfo() const {
    return /*serviceName*/ getServiceName() + " - Premium (" + staffMember + ")";
}

double PremiumService::calculateDiscountedPrice(double discount) const {
    return /*price*/ getPrice() * (1 - discount);
}
int PremiumService::getLoyaltyBonus() const {
    return loyaltyPoints;
}
//Room::Room() :roomCapacity(0), roomClass(""), pricePerNight(0.0), isAvailable(false) {}
// Конструктори та методи Room
Room::Room(int roomCapacity, const string& classType, double price, bool available, const string& view, int floor, double price_room)
    : roomCapacity(roomCapacity), roomClass(classType), pricePerNight(price), isAvailable(available), view(view), floor(floor), price_room(price_room) {
    cout << "Room constructor called: " << roomClass << endl;
}

// Конструктор копіювання для Room
Room::Room(const Room& other) : roomCapacity(other.roomCapacity), roomClass(other.roomClass),
pricePerNight(other.pricePerNight), isAvailable(other.isAvailable), view(other.view), floor(other.floor), price_room(other.price_room) {}

// Деструктор класу Room
/* Room::~Room() {
     cout << "Room destructor called: " << roomClass << endl;
 }*/

bool Room::checkAvailability() const {
    return isAvailable;
}

double Room::calculateCost(int nights) const {
    return nights * pricePerNight;
}

void Room::bookRoom() {
    isAvailable = false;
}

void Room::releaseRoom() {
    isAvailable = true;
}

void Room::displayRoomInfo() const {
    cout << "Room Class: " << roomClass << ", Capacity: " << roomCapacity << ", Floor: " << floor << endl;

    cout << "Room Capacity: " << roomCapacity << ", Class: " << roomClass << endl;
    cout << "Price per Night: $" << pricePerNight << endl;
    cout << "Availability: " << (isAvailable ? "Available" : "Not Available") << endl;
    cout << endl;
}

Room createDeluxeRoom() {
    // Створення об'єкта Room з параметрами Deluxe
    Room deluxeRoom(2, "Deluxe", 100.0); // Місткість: 2 людини, клас: Deluxe, ціна за ніч: 100.0

    // Використання методу класу для бронювання кімнати
    deluxeRoom.bookRoom();
    deluxeRoom.displayRoomInfo();

    // Повертаємо об'єкт Room
    return deluxeRoom;
}
// 7.3
// Геттер для отримання ціни
double Room::getPrice() const {
    return price_room;
}

// Сеттер для встановлення нової ціни
void Room::setPrice(double newPrice) {
    price_room = newPrice;
}

// SuiteRoom class
SuiteRoom::SuiteRoom(int capacity, double price, bool jacuzzi, bool balcony, const string& category)
    : Room(capacity, "Suite", price), hasJacuzzi(jacuzzi), hasBalcony(balcony), suiteCategory(category) {
    cout << "SuiteRoom constructor called: " << hasJacuzzi << endl;
}
SuiteRoom::~SuiteRoom() {
    cout << "SuiteRoom destructor called: " << hasJacuzzi << endl;
}
double SuiteRoom::calculateCost(int nights) const {
    return pricePerNight * nights * 1.2;
}

void SuiteRoom::displayRoomInfo() const {
    Room::displayRoomInfo();
    cout << "Category: " << getSuiteCategory() << ", Jacuzzi: " << (hasJacuzzi ? "Yes" : "No") << endl;
}
string SuiteRoom::getSuiteCategory() const {
    return suiteCategory;
}
Customer::Customer() : name(""), contactInfo(""), loyaltyPoints(0), hasDiscount(false) {}
Customer::Customer(const string& name, const string& contactInfo, int points, bool discount)
    : name(name), contactInfo(contactInfo), loyaltyPoints(points), hasDiscount(discount) {
    cout << "Customer constructor called: " << contactInfo << endl;
}
Customer::~Customer() {
    cout << "Customer destructor called: " << contactInfo << endl;
}
// Віртуальний метод для відображення інформації про клієнта
void Customer::displayCustomerInfo() const {
    cout << "Customer Name: " << name << endl;
    cout << "Contact Info: " << contactInfo << endl;
    cout << "Loyalty Points: " << loyaltyPoints << endl;
    cout << "Discount Available: " << (hasDiscount ? "Yes" : "No") << endl;
}
int Customer::getLoyaltyPoints() const {
    return loyaltyPoints;
}

// 6.4
void Customer::setName(const string& newName) {
    name = newName;
}
void Customer::setLoyaltyPoints(const int& points) {
    loyaltyPoints = points;
}
void Customer::setHasDiscount(const bool& discount) {
    hasDiscount = discount;
}

// VIPCustomer class
VIPCustomer::VIPCustomer() : vipLevel(0), personalAssistant(""), accessToLounge(false), assistentIndex(0) {}
VIPCustomer::VIPCustomer(const string& name, const string& contactInfo, int level, const string& assistant, bool lounge, int assistentindex)
    : Customer(name, contactInfo), vipLevel(level), personalAssistant(assistant), accessToLounge(lounge), assistentIndex(assistentindex) {
    cout << "VIPCustomer constructor called: " << vipLevel << endl;
}

//5.3
VIPCustomer:: ~VIPCustomer() {
    cout << "VIPCustomer destructor called: " << vipLevel << endl;
}
// Перевизначений метод для відображення інформації про VIP-клієнта
void VIPCustomer::displayCustomerInfo() const {
    // Викликаємо метод базового класу
    Customer::displayCustomerInfo();
    cout << "VIP Level: " << vipLevel << endl;
    cout << "Personal Assistant: " << getPersonalAssistant(2) << endl;
    cout << "Access to Lounge: " << (accessToLounge ? "Yes" : "No") << endl;
}

// Метод для застосування знижки
double VIPCustomer::applyDiscount(double basePrice) const {
    double discountRate = 0.2; // Знижка для VIP-клієнта
    return basePrice * (1 - discountRate);
}
void VIPCustomer::showBaseClassAccess() const {
    cout << "\n--- Base Class Access Demonstration ---\n";
    // Доступ до приватних членів базового класу через методи
    cout << "Name (from Base Class): " << name << endl;
    cout << "Contact Info (from Base Class): " << contactInfo << endl;
    cout << "Loyalty Points (using getLoyaltyPoints() from Base Class): " << getLoyaltyPoints() << endl;
    cout << "Discount Status (from Base Class): " << (hasDiscount ? "Yes" : "No") << "\n" << endl;
}
string VIPCustomer::getPersonalAssistant(int assistentindex) const {
    string* personalAssistant = new string[5];
    personalAssistant[0] = "Oleg Smoke";
    personalAssistant[1] = "Olga Snike";
    personalAssistant[2] = "Anton Walker";
    personalAssistant[3] = "Maria Mamontova";
    personalAssistant[4] = "Sarah Wiren";

    return personalAssistant[assistentindex];
}
SpecialCustomer::SpecialCustomer(const string& name, const string& contactInfo, int points, bool discount, const string& serviceName, double price, bool privileges)
    : Customer(name, contactInfo, points, discount), Service(serviceName, price), loyaltyPoints(points), hasSpecialPrivileges(privileges) {
    cout << "SpecialCustomer constructor called: " << hasSpecialPrivileges << endl;
}
SpecialCustomer::~SpecialCustomer() {
    cout << "SpecialCustomer destructor called: " << hasSpecialPrivileges << endl;
}
void SpecialCustomer::displaySpecialCustomerInfo() const {
    Customer::displayCustomerInfo();
    cout << "Service: " << Service::getServiceName() << " ($" << Service::getPrice() << ")" << endl;
    cout << "Loyalty Points: " << loyaltyPoints << endl;
    cout << "Special Privileges: " << (hasSpecialPrivileges ? "Yes" : "No") << endl;
    cout << "Status Loyalty " << getLoyaltyStatus() << endl;

}
double  SpecialCustomer::calculateDiscountedServicePrice(double discountRate) const {
    return Service::getPrice() * (1 - discountRate);
}
string SpecialCustomer::getLoyaltyStatus() const {
    if (loyaltyPoints > 100) {
        return "Gold";
    }
    else if (loyaltyPoints > 50) {
        return "Silver";
    }
    else {
        return "Bronze";
    }
}

Hotel::Hotel(string hotelName) : hotelName(hotelName) {
    cout << "Hotel constructor called: " << hotelName << endl;
}
Hotel::~Hotel() {
    cout << "Hotel destructor called: " << hotelName << endl;
}
void Hotel::addRoom(const Room& room) {
    rooms.push_back(room);
}

void Hotel::displayRooms() const {
    cout << "Rooms in " << hotelName << ":" << endl;
    for (const auto& room : rooms) {
        room.displayRoomInfo();
    }
}
// 7.1
// Методи для роботи зі списком клієнтів
void Hotel::addCustomerToQueue(const string& customer) {
    customerQueue.push_back(customer);
}
void Hotel::displayCustomerQueue() const {
    cout << "Customer queue for hotel " << hotelName << ":\n";
    for (const auto& customer : customerQueue) {
        cout << customer << endl;
    }
}
// Методи для роботи зі стеком історії бронювань
void Hotel::addBookingToHistory(const string& booking) {
    bookingHistory.push(booking);
}
void Hotel::displayBookingHistory() const {
    cout << "Booking history for hotel " << hotelName << ":\n";
    stack<string> tempStack = bookingHistory;
    while (!tempStack.empty()) {
        cout << tempStack.top() << endl;
        tempStack.pop();
    }
}
// 7.2
void Hotel::demonstrateInputIterator() const {
    cout << "Input Iterator (reading customer queue):\n";
    list<string>::const_iterator it = customerQueue.begin();
    while (it != customerQueue.end()) {
        cout << *it << endl;
        ++it;
    }
}

void Hotel::demonstrateOutputIterator() {
    cout << "Output Iterator (adding 'Guest' prefix to customer queue):\n";
    list<string>::iterator it = customerQueue.begin();
    while (it != customerQueue.end()) {
        *it = "Guest: " + *it; // Оновлення елемента через ітератор
        ++it;
    }
}

void Hotel::demonstrateForwardIterator() const {
    cout << "Forward Iterator (reading customer queue):\n";
    for (auto it = customerQueue.cbegin(); it != customerQueue.cend(); ++it) {
        cout << *it << endl;
    }
}

void Hotel::demonstrateBidirectionalIterator() const {
    cout << "Bidirectional Iterator (reading queue in reverse):\n";
    for (auto it = customerQueue.crbegin(); it != customerQueue.crend(); ++it) {
        cout << *it << endl;
    }
}

void Hotel::demonstrateRandomAccessIterator() const {
    cout << "Random Access Iterator (rooms):\n";
    for (auto it = rooms.cbegin(); it != rooms.cend(); ++it) {
        it->displayRoomInfo(); // Виклик методу Room через ітератор
    }
}

// 7.3
void Hotel::sortRoomsByPrice() {
    std::sort(rooms.begin(), rooms.end(), [](const Room& a, const Room& b) {
        return a.getPrice() < b.getPrice();
        });
    cout << "Rooms sorted by price:\n";
    displayRooms();
}

void Hotel::findCustomerInQueue(const string& customer) const {
    auto it = std::find(customerQueue.begin(), customerQueue.end(), customer);
    if (it != customerQueue.end()) {
        cout << "Customer " << customer << " found in the queue.\n";
    }
    else {
        cout << "Customer " << customer << " not found in the queue.\n";
    }
}

void Hotel::countCustomersWithName(const string& name) const {
    int count = std::count(customerQueue.begin(), customerQueue.end(), name);
    cout << "Number of customers with the name " << name << ": " << count << endl;
}

void Hotel::increaseRoomPrices(double percentage) {
    std::for_each(rooms.begin(), rooms.end(), [percentage](Room& room) {
        double newPrice = room.getPrice() * (1 + percentage / 100); // Обчислення нової ціни
        room.setPrice(newPrice); // Оновлення ціни
        });
    cout << "Room prices increased by " << percentage << "%:\n";
    displayRooms(); // Відображення оновлених цін
}

void Hotel::reverseCustomerQueue() {
    customerQueue.reverse();
    cout << "Customer queue reversed:\n";
    displayCustomerQueue();
}



Booking::Booking(const BookingRequest& bookingRequest, const Room& room)
    : bookingRequest(bookingRequest), room(room) {}

void Booking::addService(const Service& service) {
    services.push_back(service);
}

void Booking::displayBookingDetails() const {
    bookingRequest.displayBookingInfo();
    room.displayRoomInfo();
    cout << "Services: " << endl;
    for (const auto& service : services) {
        cout << " - " << service.getServiceName() << " ($" << service.getPrice() << ")" << endl;
    }
}

//Віртуальний деструктор класу LoyaltyProgram
LoyaltyProgram:: ~LoyaltyProgram() {
    cout << "LoyaltyProgram destructor called" << endl;
}
Person::~Person() {}