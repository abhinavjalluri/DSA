# Part 2 – Object-Oriented Programming (Detailed)

---

## 1. Class

A blueprint/template that defines the state (fields) and behavior (methods) of objects.

```java
public class Car {
    // fields (state)
    String brand;
    int speed;

    // methods (behavior)
    void accelerate() { speed += 10; }
}
```

No memory is allocated for a class itself (only for its objects) — except the class metadata stored once in the Method Area.

---

## 2. Object

A runtime **instance** of a class, created using `new`, living on the **Heap**. Has three characteristics: **state**, **behavior**, **identity**.

```java
Car myCar = new Car();     // 'myCar' -> Stack (reference), Car object -> Heap
myCar.brand = "Toyota";
myCar.accelerate();
```

Creating an object involves: memory allocation → default field initialization → instance initializer blocks run → constructor runs.

---

## 3. Encapsulation

Bundling data (fields) and methods that operate on it into a single unit, and **restricting direct access** to internal state using access modifiers + getters/setters.

```java
public class BankAccount {
    private double balance;   // hidden from outside

    public double getBalance() { return balance; }
    public void deposit(double amt) {
        if (amt > 0) balance += amt;   // controlled/validated mutation
    }
}
```

**Why**: protects invariants (e.g. balance can never go negative through direct field manipulation), improves maintainability, allows internal implementation to change without affecting external callers.

---

## 4. Abstraction

Hiding implementation details and exposing only essential features. Achieved via **abstract classes** and **interfaces**.

```java
abstract class Shape {
    abstract double area();          // WHAT, not HOW
    void describe() { System.out.println("Area = " + area()); }
}
class Circle extends Shape {
    double radius;
    Circle(double r) { radius = r; }
    double area() { return Math.PI * radius * radius; }  // HOW
}
```

**Encapsulation vs Abstraction** (classic interview question): Encapsulation is about *data hiding* (how it's protected); Abstraction is about *implementation hiding* (what is shown to the user). Encapsulation is a technique; abstraction is a design goal.

---

## 5. Inheritance

A class (subclass/child) acquires fields and methods of another class (superclass/parent) using `extends`. Promotes code reuse.

```java
class Animal {
    void eat() { System.out.println("eating"); }
}
class Dog extends Animal {
    void bark() { System.out.println("barking"); }
}
Dog d = new Dog();
d.eat();   // inherited
d.bark();  // own
```

Java supports **single inheritance** for classes (no multiple class inheritance — avoids the Diamond Problem) but **multiple inheritance of type** via interfaces.

Types: Single, Multilevel (`A → B → C`), Hierarchical (`B, C, D extends A`). Multiple inheritance (via interfaces only).

---

## 6. Polymorphism

"Many forms" — the same operation behaves differently on different objects.

- **Compile-time (static) polymorphism**: Method Overloading.
- **Runtime (dynamic) polymorphism**: Method Overriding, resolved via **dynamic method dispatch**.

```java
Animal a = new Dog();   // reference type Animal, object type Dog
a.sound();               // Dog's overridden version runs — decided at RUNTIME
```

Runtime polymorphism works through the **vtable (virtual method table)** mechanism internally — each object carries a pointer to its actual class's method table.

---

## 7. Method Overloading

Same method name, **different parameter list** (number/type/order) in the **same class**. Return type alone is NOT sufficient to overload.

```java
class Calculator {
    int add(int a, int b) { return a + b; }
    double add(double a, double b) { return a + b; }
    int add(int a, int b, int c) { return a + b + c; }
}
```

Resolved at **compile-time** based on the reference/argument types (static binding).

---

## 8. Method Overriding

Subclass provides a **specific implementation** of a method already defined in its superclass — same signature (name + parameters + return type, or covariant return type).

```java
class Animal {
    void sound() { System.out.println("Some sound"); }
}
class Cat extends Animal {
    @Override
    void sound() { System.out.println("Meow"); }
}
```

Rules:
- Access modifier can't be more restrictive than the parent's.
- Can't override `static`, `final`, or `private` methods.
- Must have the same or a **covariant** return type.
- Can widen (but not narrow) the checked exceptions thrown.

Resolved at **runtime** (dynamic binding) via the actual object type.

---

## 9. Interface

A contract of abstract methods (pre-Java 8) that implementing classes must fulfill. From Java 8+: can also have `default`, `static` and (Java 9+) `private` methods.

```java
interface Drivable {
    void drive();                                  // abstract
    default void honk() { System.out.println("Beep!"); }  // default
    static Drivable create() { return () -> System.out.println("driving"); } // static
}
class Car implements Drivable {
    public void drive() { System.out.println("Car driving"); }
}
```

- A class can `implement` **multiple** interfaces (this is how Java achieves multiple inheritance of type).
- All fields in an interface are implicitly `public static final`.
- **Diamond problem** with default methods: if two interfaces provide the same default method, the implementing class **must** override it explicitly.

---

## 10. Abstract Class

A class declared `abstract` — cannot be instantiated, may have both abstract (no body) and concrete methods.

```java
abstract class Vehicle {
    abstract void move();              // must be implemented by subclass
    void fuelUp() { System.out.println("Fueling..."); }  // concrete, shared
}
```

**Abstract class vs Interface**:

| | Abstract Class | Interface |
|---|---|---|
| Methods | Abstract + concrete | Abstract, default, static, private |
| Fields | Any type | `public static final` only |
| Constructor | Yes | No |
| Multiple inheritance | No (single `extends`) | Yes (multiple `implements`) |
| Use when | Sharing common code + state among closely related classes ("is-a" with shared implementation) | Defining a capability/contract across unrelated classes |

---

## 11. Association, Aggregation, Composition, Dependency

These describe **relationships between classes**, in increasing order of "coupling":

- **Association**: general relationship where one object *uses* another — objects have independent lifecycles.
  ```java
  class Teacher { }
  class Student { Teacher teacher; }  // Student "has a" Teacher (association)
  ```
- **Aggregation** ("has-a", weak ownership): whole-part relationship where the part **can exist independently** of the whole.
  ```java
  class Department { List<Professor> professors; }  // Professor can exist without Department
  ```
- **Composition** ("has-a", strong ownership): whole-part relationship where the part **cannot exist without** the whole — lifecycle is bound.
  ```java
  class House {
      private final Room room = new Room();   // Room dies when House dies
  }
  ```
- **Dependency** ("uses-a"): one class temporarily uses another, e.g. as a method parameter — weakest coupling.
  ```java
  class Printer { void print(Document d) { ... } }  // Printer depends on Document temporarily
  ```

---

## 12. IS-A vs HAS-A

- **IS-A** → Inheritance (`Dog extends Animal` → "a Dog IS-A(n) Animal").
- **HAS-A** → Composition/Aggregation (`Car has-a Engine`).

**Guideline**: Favor composition over inheritance where possible — it's more flexible and avoids tight coupling/fragile base class problems.

---

## 13. Object Lifecycle

1. **Loading** — class loaded into memory by ClassLoader (once).
2. **Instantiation** — `new` allocates memory on heap.
3. **Initialization** — default values → instance initializer blocks → constructor body executes.
4. **In use** — object referenced and used by the program.
5. **Unreachable** — no live references point to it.
6. **Garbage Collected** — memory reclaimed by GC (non-deterministic timing).

```java
class Demo {
    { System.out.println("Instance initializer block"); }  // runs before constructor body
    Demo() { System.out.println("Constructor"); }
}
new Demo();
// Output:
// Instance initializer block
// Constructor
```

---

## 14. SOLID Principles

| Principle | Meaning | Example |
|---|---|---|
| **S** – Single Responsibility | A class should have only one reason to change | Separate `InvoiceCalculator` from `InvoicePrinter` |
| **O** – Open/Closed | Open for extension, closed for modification | Use interfaces/strategy pattern instead of `if-else` chains for new types |
| **L** – Liskov Substitution | Subclasses must be substitutable for their base type without breaking behavior | A `Square extends Rectangle` that changes both sides on `setWidth()` violates LSP |
| **I** – Interface Segregation | Prefer many small, specific interfaces over one fat interface | Split `Worker` into `Workable` and `Eatable` instead of forcing robots to implement `eat()` |
| **D** – Dependency Inversion | Depend on abstractions, not concrete implementations | Inject `PaymentGateway` interface, not `StripeGateway` directly (enables Spring's DI) |

```java
// Dependency Inversion example
interface PaymentGateway { void pay(double amount); }
class StripeGateway implements PaymentGateway {
    public void pay(double amount) { System.out.println("Paid via Stripe: " + amount); }
}
class CheckoutService {
    private final PaymentGateway gateway;         // depends on abstraction
    CheckoutService(PaymentGateway gateway) { this.gateway = gateway; }  // injected
    void checkout(double amt) { gateway.pay(amt); }
}
```

SOLID is the theoretical foundation behind Spring's Dependency Injection and much of enterprise Java design — expect this to come up heavily in interviews for mid/senior roles.