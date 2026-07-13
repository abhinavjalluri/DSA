# Part 3 – Core Java

---

## 1. Access Modifiers

| Modifier | Same Class | Same Package | Subclass (diff. package) | World |
|---|:---:|:---:|:---:|:---:|
| `private` | ✅ | ❌ | ❌ | ❌ |
| *default* (no modifier) | ✅ | ✅ | ❌ | ❌ |
| `protected` | ✅ | ✅ | ✅ | ❌ |
| `public` | ✅ | ✅ | ✅ | ✅ |

```java
public class Account {
    private double balance;      // only within Account
    protected String owner;      // package + subclasses
    String branch;               // default: package only
    public String accountNo;     // everywhere
}
```

**Best practice**: fields `private`, expose via `public` getters/setters; keep classes/methods only as visible as needed (principle of least privilege).

---

## 2. Arrays

Fixed-size, index-based, homogeneous data structure. Stored on the **Heap**, even for arrays of primitives.

```java
int[] arr = new int[5];              // default-initialized to 0
int[] nums = {1, 2, 3, 4, 5};
int[][] matrix = new int[3][3];      // 2D array (array of arrays)

Arrays.sort(nums);
int idx = Arrays.binarySearch(nums, 3);
int[] copy = Arrays.copyOf(nums, 10);
System.out.println(Arrays.toString(nums));
List<Integer> list = Arrays.asList(1, 2, 3);  // fixed-size backed list
```

- `array.length` is a **field**, not a method (unlike `String.length()` or `List.size()`).
- Arrays are **covariant** (`Object[] arr = new String[3];` compiles) but this can cause `ArrayStoreException` at runtime.

---

## 3. Enums

Type-safe set of named constants. Implicitly extend `java.lang.Enum`, can have fields, constructors, and methods.

```java
enum Day {
    MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY;
}

enum Planet {
    MERCURY(3.3e23, 2.4e6), EARTH(5.9e24, 6.4e6);

    final double mass, radius;
    Planet(double mass, double radius) { this.mass = mass; this.radius = radius; }
    double gravity() { return 6.67e-11 * mass / (radius * radius); }
}

Day d = Day.MONDAY;
switch (d) {
    case MONDAY -> System.out.println("Start of week");
    default -> System.out.println("Other day");
}
for (Day day : Day.values()) System.out.println(day.ordinal() + ": " + day.name());
```

Enums can implement interfaces and even have per-constant method bodies (anonymous class per constant) — useful for the Strategy pattern.

---

## 4. Records (Java 16+)

Compact syntax for immutable data carrier classes — compiler auto-generates constructor, `equals()`, `hashCode()`, `toString()`, and accessors.

```java
public record Point(int x, int y) { }

Point p = new Point(3, 4);
p.x();            // accessor (not getX())
p.toString();     // Point[x=3, y=4]
p.equals(new Point(3,4));  // true (value-based)
```

Custom validation via **compact constructor**:
```java
public record Range(int min, int max) {
    public Range {                 // compact constructor
        if (min > max) throw new IllegalArgumentException("min > max");
    }
}
```

Records are implicitly `final`, all fields are `private final`. Great for DTOs — commonly used with Spring Boot REST APIs.

---

## 5. Inner Classes

Non-static class defined **inside** another class — has access to the enclosing instance's members, including `private` ones.

```java
class Outer {
    private int x = 10;
    class Inner {
        void show() { System.out.println(x); }   // accesses Outer's private field
    }
}
Outer.Inner inner = new Outer().new Inner();   // needs an Outer instance
```

---

## 6. Nested Classes

Umbrella term for any class defined within another. Two kinds:
- **Static nested class**: doesn't need an outer instance, can't access outer's instance members directly.
- **Inner (non-static) class**: tied to an outer instance (see above).

```java
class Outer {
    static class StaticNested {
        void show() { System.out.println("Static nested"); }
    }
}
Outer.StaticNested obj = new Outer.StaticNested();  // no Outer instance needed
```

`Map.Entry` and Builder-pattern helper classes are commonly implemented as static nested classes.

---

## 7. Anonymous Classes

A class without a name, declared and instantiated in a single expression — typically to override a method on the fly.

```java
Runnable r = new Runnable() {
    @Override
    public void run() { System.out.println("Running anonymously"); }
};
new Thread(r).start();

Comparator<String> byLength = new Comparator<String>() {
    public int compare(String a, String b) { return a.length() - b.length(); }
};
```

Largely superseded by **lambda expressions** for functional interfaces since Java 8, but still used for multi-method interfaces/abstract classes.

---

## 8. Object Class

Root of the class hierarchy — every class implicitly extends `Object`. Key methods:

| Method | Purpose |
|---|---|
| `equals(Object o)` | logical equality (default: reference equality `==`) |
| `hashCode()` | integer hash used by hash-based collections |
| `toString()` | string representation |
| `getClass()` | runtime `Class` object |
| `clone()` | shallow copy (must implement `Cloneable`) |
| `wait()/notify()/notifyAll()` | thread coordination (monitor methods) |
| `finalize()` | deprecated, called before GC (unreliable) |

```java
class Person {
    String name; int age;
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Person p)) return false;    // pattern matching instanceof
        return age == p.age && Objects.equals(name, p.name);
    }
    @Override
    public int hashCode() { return Objects.hash(name, age); }
}
```

**Contract**: if `a.equals(b)` is true, then `a.hashCode() == b.hashCode()` **must** be true (violating this breaks `HashMap`/`HashSet`).

---

## 9. Wrapper Classes

Object representations of primitives: `Integer, Long, Double, Float, Short, Byte, Character, Boolean`.

```java
int primitive = 10;
Integer wrapped = Integer.valueOf(primitive);   // manual boxing
int back = wrapped.intValue();                  // manual unboxing

Integer a = 127, b = 127;
System.out.println(a == b);   // true  -- Integer Cache (-128 to 127)
Integer c = 200, d = 200;
System.out.println(c == d);   // false -- outside cache range, different objects
```

Needed because Collections (`List<Integer>`) can't hold primitives directly (generics work only with objects).

---

## 10. Autoboxing / Unboxing

Automatic conversion between primitives and their wrapper classes, done by the compiler.

```java
List<Integer> list = new ArrayList<>();
list.add(5);            // autoboxing: int -> Integer
int x = list.get(0);    // auto-unboxing: Integer -> int
```

**Pitfall**: unboxing a `null` throws `NullPointerException`:
```java
Integer val = null;
int x = val;    // NPE!
```
**Pitfall**: unnecessary autoboxing in loops (e.g. `Long sum = 0L; sum += i;` in a loop) creates excessive object churn — prefer primitive accumulators in hot loops.

---

## 11. Generics

Enables types (classes, interfaces, methods) to be parameterized — compile-time type safety, eliminates casts.

```java
class Box<T> {
    private T value;
    void set(T value) { this.value = value; }
    T get() { return value; }
}
Box<String> box = new Box<>();
box.set("hello");

// Generic method
<T> void printAll(List<T> list) { for (T t : list) System.out.println(t); }

// Bounded type parameter
<T extends Number> double sum(List<T> list) {
    double total = 0;
    for (T t : list) total += t.doubleValue();
    return total;
}

// Wildcards
void printList(List<?> list) { ... }                        // unknown type
void addNumbers(List<? super Integer> list) { list.add(5); } // lower bound (contravariant)
double sumList(List<? extends Number> list) { ... }          // upper bound (covariant)
```

**PECS rule**: *Producer Extends, Consumer Super* — use `? extends T` when you only read from the structure, `? super T` when you only write to it.

**Type Erasure**: generic type info exists only at **compile time**; at runtime, `List<String>` and `List<Integer>` both become raw `List` — this is why you can't do `new T()` or `T[] arr = new T[10]` directly.

---

## 12. Exceptions

Java's mechanism for handling runtime errors, keeping normal code separate from error-handling code.

```
Throwable
├── Error (JVM-level, unrecoverable: OutOfMemoryError, StackOverflowError)
└── Exception
    ├── Checked (must be declared/caught: IOException, SQLException)
    └── RuntimeException (unchecked: NullPointerException, ArrayIndexOutOfBoundsException)
```

```java
try {
    int[] arr = new int[5];
    arr[10] = 1;                          // throws ArrayIndexOutOfBoundsException
} catch (ArrayIndexOutOfBoundsException e) {
    System.out.println("Caught: " + e.getMessage());
} catch (Exception e) {
    System.out.println("Generic catch");
} finally {
    System.out.println("Always runs (cleanup)");
}

// Custom exception
class InsufficientFundsException extends Exception {
    InsufficientFundsException(String msg) { super(msg); }
}
void withdraw(double amt, double balance) throws InsufficientFundsException {
    if (amt > balance) throw new InsufficientFundsException("Not enough funds");
}

// try-with-resources (auto-closes resources implementing AutoCloseable)
try (BufferedReader br = new BufferedReader(new FileReader("file.txt"))) {
    System.out.println(br.readLine());
} catch (IOException e) {
    e.printStackTrace();
}

// multi-catch
try { ... } catch (IOException | SQLException e) { ... }
```

**Checked vs Unchecked**: Checked exceptions represent recoverable conditions the caller is *forced* to handle (compile-time check). Unchecked (RuntimeException) represent programming errors, not mandatory to declare/catch.

**Best practice**: don't swallow exceptions silently; catch specific exceptions before generic ones; use custom exceptions for domain-specific errors; prefer unchecked exceptions for library APIs to avoid forcing boilerplate on callers (a common Spring/modern-Java design choice).

---

## 13. Reflection

API (`java.lang.reflect`) to inspect/modify classes, methods, fields **at runtime**, even `private` ones.

```java
Class<?> clazz = Class.forName("com.example.Person");
// or: Person.class  /  personObj.getClass()

Method[] methods = clazz.getDeclaredMethods();
Field field = clazz.getDeclaredField("name");
field.setAccessible(true);              // bypass access checks
Object instance = clazz.getDeclaredConstructor().newInstance();
field.set(instance, "John");
System.out.println(field.get(instance));
```

**Used by**: Spring (dependency injection, `@Autowired`), Hibernate/JPA (populating entities), JUnit (test discovery), Jackson (serialization). Powerful but has performance overhead and breaks encapsulation — use sparingly in application code.

---

## 14. Annotations

Metadata attached to code — processed at compile-time or runtime, don't directly affect program logic.

```java
@Retention(RetentionPolicy.RUNTIME)   // available via reflection at runtime
@Target(ElementType.METHOD)            // where it can be applied
public @interface LogExecutionTime { }

class Service {
    @LogExecutionTime
    void process() { ... }
}

// Reading it via reflection:
Method m = Service.class.getMethod("process");
if (m.isAnnotationPresent(LogExecutionTime.class)) {
    System.out.println("Should log timing");
}
```

**Built-in annotations**: `@Override`, `@Deprecated`, `@SuppressWarnings`, `@FunctionalInterface`.

**Meta-annotations**: `@Retention` (SOURCE/CLASS/RUNTIME), `@Target`, `@Inherited`, `@Documented`.

Spring/Spring Boot is built heavily on annotations: `@Component`, `@Autowired`, `@RestController`, `@Transactional`, etc. — all processed via reflection at container startup.