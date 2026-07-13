# Part 8 – Design Patterns

Design patterns are proven, reusable solutions to common software design problems. They are grouped into *Creational* (object creation), *Structural* (object composition), and *Behavioral* (object interaction) — plus *Dependency Injection*, which underlies the entire Spring framework.

---

## 1. Singleton Pattern

*Intent:* Ensure a class has only one instance, and provide a global access point to it.

*Why it exists:* Some objects should exist exactly once — configuration managers, connection pools, loggers — creating multiple instances would waste resources or cause inconsistent state.

### Thread-Safe Implementation (Double-Checked Locking)
⁠ java
public class ConfigManager {
    private static volatile ConfigManager instance;
    private ConfigManager() {} // private constructor prevents external instantiation

    public static ConfigManager getInstance() {
        if (instance == null) {
            synchronized (ConfigManager.class) {
                if (instance == null) {
                    instance = new ConfigManager();
                }
            }
        }
        return instance;
    }
}
 ⁠
*Why ⁠ volatile ⁠ is required:* Prevents instruction reordering during object construction — without it, another thread could see a non-null reference to a partially-constructed object.

### Best Modern Approach: Enum Singleton
⁠ java
public enum ConfigManager {
    INSTANCE;
    public void doSomething() { ... }
}
// Usage: ConfigManager.INSTANCE.doSomething();
 ⁠
Enum singletons are inherently thread-safe, serialization-safe, and reflection-attack-proof — the JVM guarantees only one instance per enum constant.

*Spring context:* Every ⁠ @Component ⁠/⁠ @Service ⁠/⁠ @Bean ⁠ is a *Singleton by default* within the Spring container (one instance per container, not per JVM) — Spring manages this for you, so manual Singleton pattern code is rarely needed in Spring apps.

*Common mistake:* Non-thread-safe lazy initialization (⁠ if (instance == null) instance = new X(); ⁠ without synchronization) — causes multiple instances under concurrent access.

---

## 2. Factory Pattern

*Intent:* Delegate object creation to a factory method instead of using ⁠ new ⁠ directly, so the calling code doesn't depend on concrete classes.

⁠ java
interface Notification {
    void notifyUser();
}
class EmailNotification implements Notification {
    public void notifyUser() { System.out.println("Email sent"); }
}
class SMSNotification implements Notification {
    public void notifyUser() { System.out.println("SMS sent"); }
}

class NotificationFactory {
    public static Notification create(String type) {
        return switch (type) {
            case "EMAIL" -> new EmailNotification();
            case "SMS" -> new SMSNotification();
            default -> throw new IllegalArgumentException("Unknown type");
        };
    }
}

// Usage
Notification n = NotificationFactory.create("EMAIL");
n.notifyUser();
 ⁠

*Why it exists:* Decouples client code from concrete implementations — adding a new notification type doesn't require changing calling code, only the factory.

*Spring context:* ⁠ BeanFactory ⁠/⁠ ApplicationContext ⁠ itself is a factory that creates and manages beans. ⁠ @Bean ⁠ factory methods in ⁠ @Configuration ⁠ classes are a direct application of this pattern.

---

## 3. Builder Pattern

*Intent:* Construct complex objects step-by-step, avoiding telescoping constructors (constructors with many optional parameters).

⁠ java
public class User {
    private final String name;
    private final int age;
    private final String email;

    private User(Builder builder) {
        this.name = builder.name;
        this.age = builder.age;
        this.email = builder.email;
    }

    public static class Builder {
        private String name;
        private int age;
        private String email;

        public Builder name(String name) { this.name = name; return this; }
        public Builder age(int age) { this.age = age; return this; }
        public Builder email(String email) { this.email = email; return this; }
        public User build() { return new User(this); }
    }
}

// Usage
User user = new User.Builder()
    .name("Alice")
    .age(30)
    .email("alice@mail.com")
    .build();
 ⁠

*Why it exists:* When a class has many optional fields, constructor overloading becomes unmanageable (⁠ new User(name, age, null, null, true, false, ...) ⁠ is unreadable and error-prone).

*Modern shortcut:* Lombok's ⁠ @Builder ⁠ annotation auto-generates this boilerplate.

*Real-world use:* ⁠ HttpRequest.newBuilder()...build() ⁠ in the Java HTTP client; ⁠ StringBuilder ⁠ itself is a variant of this pattern.

---

## 4. Strategy Pattern

*Intent:* Define a family of interchangeable algorithms, encapsulate each one, and select one at runtime.

⁠ java
interface PaymentStrategy {
    void pay(double amount);
}
class CreditCardPayment implements PaymentStrategy {
    public void pay(double amount) { System.out.println("Paid " + amount + " via Credit Card"); }
}
class PayPalPayment implements PaymentStrategy {
    public void pay(double amount) { System.out.println("Paid " + amount + " via PayPal"); }
}

class ShoppingCart {
    private PaymentStrategy strategy;
    public ShoppingCart(PaymentStrategy strategy) { this.strategy = strategy; }
    public void checkout(double amount) { strategy.pay(amount); }
}

// Usage
new ShoppingCart(new PayPalPayment()).checkout(250.0);
 ⁠

*Why it exists:* Avoids giant ⁠ if-else ⁠/⁠ switch ⁠ chains for choosing behavior; each algorithm is isolated, testable, and swappable.

*Spring context:* Injecting different implementations of an interface based on a ⁠ @Qualifier ⁠ or ⁠ @Profile ⁠ is Strategy pattern applied via Dependency Injection.

---

## 5. Observer Pattern

*Intent:* Define a one-to-many dependency so that when one object (subject) changes state, all its dependents (observers) are notified automatically.

⁠ java
interface Observer {
    void update(String event);
}
class EmailSubscriber implements Observer {
    public void update(String event) { System.out.println("Email notified: " + event); }
}

class EventPublisher {
    private List<Observer> observers = new ArrayList<>();
    public void subscribe(Observer o) { observers.add(o); }
    public void publish(String event) {
        observers.forEach(o -> o.update(event));
    }
}

// Usage
EventPublisher publisher = new EventPublisher();
publisher.subscribe(new EmailSubscriber());
publisher.publish("Order Placed");
 ⁠

*Why it exists:* Decouples the subject from observers — the subject doesn't need to know concrete observer types, just that they implement the interface.

*Spring context:* Spring's ⁠ ApplicationEventPublisher ⁠ and ⁠ @EventListener ⁠ are a built-in implementation of this pattern (e.g., publishing an ⁠ OrderPlacedEvent ⁠ that multiple listeners react to independently).

---

## 6. Adapter Pattern

*Intent:* Convert the interface of a class into another interface clients expect, allowing incompatible interfaces to work together.

⁠ java
interface MediaPlayer {
    void play(String fileName);
}
class LegacyPlayer {
    void playOldFormat(String file) { System.out.println("Playing old format: " + file); }
}
class MediaAdapter implements MediaPlayer {
    private LegacyPlayer legacyPlayer = new LegacyPlayer();
    public void play(String fileName) {
        legacyPlayer.playOldFormat(fileName); // adapts the call
    }
}
 ⁠

*Real-world use:* ⁠ Arrays.asList() ⁠ adapts an array to the ⁠ List ⁠ interface; JDBC drivers adapt database-specific protocols to the common ⁠ java.sql ⁠ API.

---

## 7. Decorator Pattern

*Intent:* Attach additional responsibilities to an object dynamically, without altering its class.

⁠ java
interface Coffee {
    double cost();
}
class SimpleCoffee implements Coffee {
    public double cost() { return 2.0; }
}
class MilkDecorator implements Coffee {
    private Coffee coffee;
    public MilkDecorator(Coffee coffee) { this.coffee = coffee; }
    public double cost() { return coffee.cost() + 0.5; }
}

// Usage
Coffee order = new MilkDecorator(new SimpleCoffee());
System.out.println(order.cost()); // 2.5
 ⁠

*Real-world use:* Java I/O streams are a textbook Decorator example — ⁠ new BufferedReader(new FileReader("file.txt")) ⁠ wraps a basic reader with buffering behavior.

---

## 8. Dependency Injection (DI)

*Intent:* Instead of a class creating its own dependencies, they are "injected" from outside — inverting control of object creation.

⁠ java
// Without DI — tightly coupled
class OrderService {
    private PaymentGateway gateway = new StripeGateway(); // hard-coded dependency
}

// With DI — loosely coupled
class OrderService {
    private final PaymentGateway gateway;
    public OrderService(PaymentGateway gateway) { // dependency injected via constructor
        this.gateway = gateway;
    }
}
 ⁠

*Why it exists:* Decouples object creation from business logic, making classes easier to test (inject a mock) and swap implementations without modifying the class.

### Spring's DI in Practice
⁠ java
@Service
public class OrderService {
    private final PaymentGateway gateway;

    @Autowired // constructor injection (recommended over field injection)
    public OrderService(PaymentGateway gateway) {
        this.gateway = gateway;
    }
}

@Component
public class StripeGateway implements PaymentGateway { ... }
 ⁠
Spring's ⁠ ApplicationContext ⁠ (the IoC container) scans, creates, and wires these beans together automatically.

*Why constructor injection is preferred over field injection (⁠ @Autowired ⁠ on a field):*
•⁠  ⁠Makes dependencies explicit and immutable (⁠ final ⁠ fields)
•⁠  ⁠Easier to unit test without a Spring context (just call the constructor)
•⁠  ⁠Fails fast at startup if a dependency is missing, rather than causing a ⁠ NullPointerException ⁠ later

---

## 9. Design Pattern Selection — Quick Reference

| Problem | Pattern |
|---|---|
| Need exactly one instance | Singleton |
| Object creation logic should be centralized/abstracted | Factory |
| Object has many optional constructor parameters | Builder |
| Need to swap algorithms at runtime | Strategy |
| One change should notify many dependents | Observer |
| Two incompatible interfaces need to work together | Adapter |
| Need to add behavior without modifying existing class | Decorator |
| Want loosely-coupled, testable classes | Dependency Injection |

---

## 10. Interview Questions

1.⁠ ⁠*Why is Enum Singleton considered the best Singleton implementation?*
   It's inherently thread-safe, and immune to breaking via reflection or serialization, unlike lazy-initialized class-based singletons.

2.⁠ ⁠*Difference between Factory and Abstract Factory?*
   Factory creates one type of product via a single method; Abstract Factory creates families of related products (e.g., a ⁠ GUIFactory ⁠ producing matching ⁠ Button ⁠ + ⁠ Checkbox ⁠ for Windows vs Mac).

3.⁠ ⁠*How does the Decorator pattern differ from inheritance?*
   Inheritance adds behavior statically at compile time for a whole subclass; Decorator adds behavior dynamically at runtime to individual object instances, and can be stacked/combined.

4.⁠ ⁠*Why does Spring prefer constructor injection over field injection?*
   Immutability, explicit dependencies, easier testing without reflection hacks, and fail-fast behavior at startup rather than runtime NPEs.

5.⁠ ⁠*What problem does the Strategy pattern solve that a simple ⁠ if-else ⁠ doesn't?*
   Open/Closed Principle — new algorithms/behaviors can be added as new classes without modifying existing code, whereas ⁠ if-else ⁠ chains must be edited (and re-tested) for every new case.

## 11. Best Practices
•⁠  ⁠Don't force-fit patterns — use them when the problem genuinely matches, not "because it's a pattern".
•⁠  ⁠Favor composition (Strategy, Decorator, DI) over inheritance for flexibility.
•⁠  ⁠In Spring apps, let the framework's IoC container handle Singleton and Factory concerns — don't hand-roll them.
•⁠  ⁠Keep constructors focused; use Builder when parameter counts exceed ~4, especially with optional fields.