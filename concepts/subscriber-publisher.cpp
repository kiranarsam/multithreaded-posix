#include <iostream>
#include <vector>

// Abstract base class for subscribers
class Subscriber
{
public:
    virtual void update(const std::string &message) = 0;
};

// Concrete class representing a specific subscriber
class ConcreteSubscriber : public Subscriber
{
public:
    ConcreteSubscriber(const std::string &name) : name(name) {}

    // Implementation of the update method
    void update(const std::string &message) override
    {
        std::cout << "Subscriber " << name << " received message: " << message << std::endl;
    }

private:
    std::string name;
};

// Publisher class
class Publisher
{
public:
    // Method to add subscribers
    void addSubscriber(Subscriber *subscriber)
    {
        subscribers.push_back(subscriber);
    }

    // Method to publish a message to all subscribers
    void publish(const std::string &message)
    {
        for (auto subscriber : subscribers)
        {
            subscriber->update(message);
        }
    }

private:
    std::vector<Subscriber *> subscribers;
};

int main()
{
    // Create a publisher
    Publisher publisher;

    // Create subscribers
    ConcreteSubscriber subscriber1("Subscriber1");
    ConcreteSubscriber subscriber2("Subscriber2");

    // Add subscribers to the publisher
    publisher.addSubscriber(&subscriber1);
    publisher.addSubscriber(&subscriber2);

    // Publish a message
    publisher.publish("Hello, subscribers!");

    return 0;
}
