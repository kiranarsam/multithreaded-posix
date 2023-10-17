#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>

// Event type
enum class Event
{
    EventA,
    EventB,
    EventC
};

// Abstract base class for subscribers
class Subscriber
{
public:
    virtual void update(Event event, const std::string &message) = 0;
};

// Concrete class representing a specific subscriber
class ConcreteSubscriber : public Subscriber
{
public:
    ConcreteSubscriber(const std::string &name) : name(name) {}

    // Implementation of the update method
    void update(Event event, const std::string &message) override
    {
        std::cout << "Subscriber " << name << " received Event: ";
        switch (event)
        {
        case Event::EventA:
            std::cout << "EventA";
            break;
        case Event::EventB:
            std::cout << "EventB";
            break;
        case Event::EventC:
            std::cout << "EventC";
            break;
        }
        std::cout << " - Message: " << message << std::endl;
    }

private:
    std::string name;
};

// Publisher class
class Publisher
{
public:
    // Method to add subscribers for a specific event
    void addSubscriber(Event event, Subscriber *subscriber)
    {
        subscribers[event].push_back(subscriber);
    }

    // Method to publish a message for a specific event
    void publish(Event event, const std::string &message)
    {
        if (subscribers.find(event) != subscribers.end())
        {
            for (auto subscriber : subscribers[event])
            {
                subscriber->update(event, message);
            }
        }
    }

private:
    std::unordered_map<Event, std::vector<Subscriber *>> subscribers;
};

int main()
{
    // Create a publisher
    Publisher publisher;

    // Create subscribers
    ConcreteSubscriber subscriber1("Subscriber1");
    ConcreteSubscriber subscriber2("Subscriber2");

    // Add subscribers to specific events
    publisher.addSubscriber(Event::EventA, &subscriber1);
    publisher.addSubscriber(Event::EventB, &subscriber1);
    publisher.addSubscriber(Event::EventB, &subscriber2);
    publisher.addSubscriber(Event::EventC, &subscriber2);

    // Publish messages for specific events
    publisher.publish(Event::EventA, "Hello from EventA!");
    publisher.publish(Event::EventB, "Greetings from EventB!");
    publisher.publish(Event::EventC, "Salutations from EventC!");

    return 0;
}
