# Part 1 – Java Fundamentals

---

## 1. Java History

- Created by **James Gosling** at Sun Microsystems, released in **1995**.
- Originally called "Oak", designed for embedded/set-top-box devices.
- Goals (from the Java whitepaper): **Simple, Object-Oriented, Portable, Platform-Independent, Robust, Secure, Multithreaded, High Performance, Dynamic**.
- "Write Once, Run Anywhere" (WORA) — achieved through the JVM + bytecode.
- Owned by **Oracle** since 2010 (acquired Sun Microsystems).
- Release cadence: until Java 8, releases were irregular. From **Java 9 onward**, a new version ships **every 6 months** (March/September), with **LTS (Long Term Support)** releases every 2–3 years: Java 8, 11, 17, 21 (LTS).

---

## 2. JDK vs JRE vs JVM

| Component | Full Form | Purpose |
|---|---|---|
| **JVM** | Java Virtual Machine | Executes bytecode, provides platform independence |
| **JRE** | Java Runtime Environment | JVM + core libraries needed to *run* Java apps |
| **JDK** | Java Development Kit | JRE + compiler (`javac`), debugger, tools needed to *develop* Java apps |

```
JDK = JRE + Development Tools (javac, javadoc, jar, jdb, etc.)
JRE = JVM + Core Class Libraries (java.lang, java.util, etc.)
JVM = Bytecode interpreter/execution engine (platform-specific)
```

**Analogy**: JVM is the engine, JRE is the car (engine + fuel system needed to drive), JDK is the car + the workshop tools needed to build/modify it.

**Interview tip**: To *run* a `.class` file you only need a JRE. To *compile* `.java` → `.class` you need a JDK. Modern JDK distributions (9+) no longer ship a separate JRE package — the JDK includes everything.

---

## 3. Compilation Process

```
MyClass.java  --(javac)-->  MyClass.class (bytecode)  --(JVM)-->  Machine Code (via JIT)
```

Steps:
1. **Source code** (`.java`) is written by the developer.
2. **`javac`** (Java compiler) compiles it into **bytecode** (`.class` file) — a platform-independent intermediate representation.
3. The **JVM's Class Loader** loads the `.class` file into memory.
4. The **Bytecode Verifier** checks it for security/correctness.
5. The **Execution Engine** (Interpreter + JIT Compiler) converts bytecode into native machine code and runs it.

```bash
javac MyClass.java   # produces MyClass.class
java MyClass         # JVM loads and runs MyClass.class
```

Unlike C/C++ (compiled directly to machine code, platform-specific), Java's two-step process (compile once to bytecode, then JIT-compile per platform at runtime) is what gives it portability.

---

## 4. Bytecode

- Intermediate, platform-independent instruction set stored in `.class` files.
- Each instruction (opcode) is 1 byte, hence "bytecode".
- Not directly understood by hardware — the JVM interprets or JIT-compiles it into native machine code.
- You can inspect bytecode using `javap -c MyClass`.

```java
// Sample.java
public class Sample {
    public static void main(String[] args) {
        int a = 5;
        int b = 10;
        System.out.println(a + b);
    }
}
```
```bash
javac Sample.java
javap -c Sample
```
```
0: iconst_5
1: istore_1
2: bipush        10
4: istore_2
5: getstatic     #2  // System.out
8: iload_1
9: iload_2
10: iadd
11: invokevirtual #3  // println
```

**Why it matters**: Bytecode is what makes "Write Once, Run Anywhere" possible — any device with a JVM can run the same `.class` file.

---

## 5. ClassLoader

The ClassLoader subsystem loads `.class` files into JVM memory. It follows the **Delegation Hierarchy Model**.

| ClassLoader | Loads |
|---|---|
| **Bootstrap ClassLoader** | Core Java classes (`java.lang.*`, `java.util.*`) from `rt.jar`/`java.base` module — written in native code |
| **Extension/Platform ClassLoader** | Classes from `ext` directory / platform modules |
| **Application/System ClassLoader** | Classes from the application classpath |
| **Custom ClassLoader** | User-defined, for plugins, hot-reloading, etc. |

**Delegation model**: A class loader delegates loading to its parent *first*. Only if the parent fails does it try to load the class itself. This prevents core classes like `java.lang.String` from being overridden maliciously.

```java
public class ClassLoaderDemo {
    public static void main(String[] args) {
        System.out.println(String.class.getClassLoader());       // null (Bootstrap)
        System.out.println(ClassLoaderDemo.class.getClassLoader()); // AppClassLoader
    }
}
```

ClassLoader phases: **Loading → Linking (Verification, Preparation, Resolution) → Initialization**.

---

## 6. JVM Memory (Runtime Data Areas)

```
+-------------------------------------------------------+
|                        JVM Memory                     |
|  +----------------+  +----------------+  +----------+ |
|  |   Method Area   |  |      Heap      |  |  Stack   | |
|  | (class metadata,|  | (objects,      |  | (per     | |
|  |  static vars)   |  |  instance vars)|  |  thread) | |
|  +----------------+  +----------------+  +----------+ |
|  +----------------+  +----------------------------+   |
|  |  PC Registers   |  |    Native Method Stack     |   |
|  +----------------+  +----------------------------+   |
+-------------------------------------------------------+
```

- **Method Area** (shared): class-level data — runtime constant pool, static variables, method bytecode.
- **Heap** (shared): all objects and instance variables live here. Divided into **Young Generation** (Eden + S0 + S1) and **Old Generation** for GC purposes.
- **Stack** (per-thread): stores frames for method calls — local variables, partial results, return addresses.
- **PC (Program Counter) Register** (per-thread): holds address of currently executing instruction.
- **Native Method Stack** (per-thread): supports native (non-Java, e.g. C/C++ via JNI) method calls.

---

## 7. Stack vs Heap

| Aspect | Stack | Heap |
|---|---|---|
| Stores | Method calls, local variables, primitives, object references | Objects, instance variables |
| Scope | Per-thread | Shared across threads |
| Lifetime | Ends when method returns | Managed by Garbage Collector |
| Speed | Faster (LIFO) | Slower |
| Size | Smaller, fixed (`-Xss`) | Larger, configurable (`-Xms`, `-Xmx`) |
| Error | `StackOverflowError` | `OutOfMemoryError` |

```java
void method() {
    int x = 10;                 // primitive -> Stack
    Person p = new Person();    // reference 'p' -> Stack, actual object -> Heap
}
```

---

## 8. Garbage Collection (GC)

Automatic memory management — reclaims heap memory occupied by objects that are no longer reachable.

**Generational hypothesis**: most objects die young → heap split into generations.

- **Young Generation**: Eden + 2 Survivor spaces (S0, S1). New objects created here. Minor GC is frequent and fast.
- **Old (Tenured) Generation**: objects that survive several Minor GCs are "promoted" here. Major/Full GC is less frequent but more expensive.
- **Metaspace** (Java 8+, replaced PermGen): stores class metadata, grows dynamically in native memory.

**GC Algorithms**: Serial, Parallel, CMS (deprecated), **G1 (default since Java 9)**, **ZGC** and **Shenandoah** (low-latency, for Java 11/15+).

```java
Object obj = new Object();
obj = null;   // object now eligible for GC (unreachable)
System.gc();  // *hint* to JVM to run GC — not guaranteed
```

Objects become eligible for GC when unreachable via any live thread, static reference, or JNI reference. `finalize()` is deprecated — use `try-with-resources`/`Cleaner` instead.

---

## 9. Variables

Three types:
1. **Local variables** — declared inside a method, no default value, must be initialized before use.
2. **Instance variables** — declared in a class, outside methods; one copy per object; get default values.
3. **Static (class) variables** — declared with `static`; one copy shared across all objects.

```java
public class Variables {
    static int staticVar = 100;   // static/class variable
    int instanceVar = 10;         // instance variable

    void method() {
        int localVar = 5;         // local variable
        System.out.println(localVar + instanceVar + staticVar);
    }
}
```

---

## 10. Data Types

**Primitive (8)**: `byte`(1B), `short`(2B), `int`(4B), `long`(8B), `float`(4B), `double`(8B), `char`(2B, Unicode), `boolean`(JVM-dependent, logically 1 bit).

**Reference types**: Classes, Interfaces, Arrays, Enums — store the *address* of the object, not the object itself.

```java
byte b = 127;
short s = 32000;
int i = 100000;
long l = 100000L;
float f = 3.14f;
double d = 3.14159;
char c = 'A';
boolean flag = true;
```

Default values (instance/static only, not local): numeric types → `0`/`0.0`, `char` → `'\u0000'`, `boolean` → `false`, reference types → `null`.

---

## 11. Operators

| Category | Operators |
|---|---|
| Arithmetic | `+ - * / %` |
| Relational | `== != > < >= <=` |
| Logical | `&& \|\| !` |
| Bitwise | `& \| ^ ~ << >> >>>` |
| Assignment | `= += -= *= /= %=` |
| Ternary | `condition ? a : b` |
| instanceof | type check |

```java
int a = 10, b = 3;
System.out.println(a % b);     // 1
System.out.println(a >> 1);    // 5  (arithmetic right shift)
System.out.println(-8 >>> 1);  // 2147483644 (unsigned right shift)
String s = (a > b) ? "a wins" : "b wins";
```

`>>>` is unique to Java (no sign extension) — a common interview trap vs `>>`.

---

## 12. Control Statements

```java
// if-else
if (a > b) { ... } else if (a == b) { ... } else { ... }

// switch (traditional)
switch (day) {
    case 1: System.out.println("Mon"); break;
    default: System.out.println("Unknown");
}

// switch expression (Java 14+)
String name = switch (day) {
    case 1 -> "Mon";
    case 2 -> "Tue";
    default -> "Unknown";
};

// loops
for (int i = 0; i < 5; i++) { ... }
for (String item : list) { ... }     // enhanced for
while (cond) { ... }
do { ... } while (cond);
```

`break`, `continue`, and **labeled** break/continue for nested loops:
```java
outer:
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (j == 1) continue outer;
        System.out.println(i + "," + j);
    }
}
```

---

## 13. Methods

```java
accessModifier returnType methodName(parameters) throws Exceptions {
    // body
    return value;
}
```

- **Method overloading**: same name, different parameter list (compile-time polymorphism).
- **Varargs**: `void print(String... items)` — internally treated as an array.
- **Pass by value**: Java always passes arguments by value. For objects, the *value of the reference* is passed (so mutating the object's state is visible, but reassigning the parameter inside the method is not).

```java
void modify(Person p) {
    p.setName("Changed");  // affects caller's object
    p = new Person();      // does NOT affect caller's reference
}
```

---

## 14. Constructors

- Special method, same name as class, no return type, invoked at object creation.
- **Default constructor**: auto-provided by compiler only if no constructor is defined.
- **Constructor overloading**: multiple constructors with different parameters.
- **`this()`**: calls another constructor in the same class (must be first statement).

```java
public class Person {
    String name; int age;

    Person() { this("Unknown", 0); }               // default -> delegates
    Person(String name) { this(name, 0); }
    Person(String name, int age) { this.name = name; this.age = age; }
}
```

Constructors are **not inherited**, but a subclass constructor implicitly calls `super()` (no-arg) as its first statement unless you call `super(args)`/`this(args)` explicitly.

---

## 15. Packages

- Namespace mechanism to organize related classes/interfaces and avoid naming collisions.
- Convention: reverse domain name, e.g. `com.company.project.module`.

```java
package com.example.service;

import com.example.model.User;      // single import
import com.example.util.*;          // wildcard import

public class UserService { ... }
```

- `import static` allows importing static members: `import static java.lang.Math.PI;`
- Classes in the same package can access each other's package-private (default) members without import.

---

## 16. `static`

Belongs to the **class**, not instances — shared across all objects.

```java
class Counter {
    static int count = 0;               // static variable
    Counter() { count++; }

    static void printCount() {          // static method — no 'this'
        System.out.println(count);
    }

    static { count = 100; }             // static initializer block, runs once at class load
}
```

- Static methods **cannot** access instance (non-static) members directly, and cannot be overridden (they can be *hidden* — resolved by compile-time type, not runtime type).
- Static nested classes, static imports, static blocks all follow the same "belongs to class" idea.

---

## 17. `final`

| Applied to | Effect |
|---|---|
| variable | value cannot be reassigned once initialized (constant) |
| method | cannot be overridden by subclasses |
| class | cannot be subclassed (e.g. `String`, `Integer`) |

```java
final int MAX = 100;          // MAX = 200; -> compile error
class Base { final void show() {} }   // cannot override in subclass
final class Utility { }               // cannot extend Utility
```

Note: `final` on a reference variable means the **reference** can't change, not that the object is immutable:
```java
final List<Integer> list = new ArrayList<>();
list.add(1);          // OK — mutating the object
list = new ArrayList<>(); // Compile error — reassigning reference
```

---

## 18. `this`

Refers to the **current object instance**. Common uses:
```java
class Point {
    int x, y;
    Point(int x, int y) {
        this.x = x;      // disambiguate field vs parameter
        this.y = y;
    }
    Point() { this(0, 0); }         // constructor chaining
    Point moveRight() { this.x++; return this; }  // method chaining / builder pattern
}
```

---

## 19. `super`

Refers to the **immediate parent class**. Used to:
1. Call parent constructor: `super(args)` (must be first line).
2. Access parent's overridden method: `super.methodName()`.
3. Access parent's field (if shadowed): `super.fieldName`.

```java
class Animal {
    String sound() { return "..."; }
}
class Dog extends Animal {
    Dog() { super(); }
    @Override
    String sound() { return "Bark, parent says: " + super.sound(); }
}
```

---

## 20. `String`

- Immutable, stored in the **String Constant Pool (SCP)** (part of heap since Java 7) when created via literals.
- `String s1 = "abc";` reuses pooled instance; `new String("abc")` always creates a new heap object.

```java
String a = "hello";
String b = "hello";
String c = new String("hello");

System.out.println(a == b);          // true  (same pool reference)
System.out.println(a == c);          // false (different heap object)
System.out.println(a.equals(c));     // true  (content equal)
System.out.println(c.intern() == a); // true  (interning puts it back in pool)
```

Because Strings are immutable, every "modification" (`concat`, `replace`, `substring`) creates a **new** object — which is why `StringBuilder` is preferred in loops.

Immutability benefits: thread-safety, safe for use as `HashMap` keys, security (e.g., class loading, network URLs), string pool caching.

---

## 21. `StringBuilder`

- **Mutable**, sequence of characters, **not thread-safe**, faster for single-threaded string manipulation.

```java
StringBuilder sb = new StringBuilder();
sb.append("Hello").append(" ").append("World");
sb.insert(5, ",");
sb.reverse();
System.out.println(sb.toString());
```

Internally backed by a resizable `char[]`/`byte[]` array (capacity grows as needed, default initial capacity 16).

---

## 22. `StringBuffer`

- Same API as `StringBuilder`, but **thread-safe** (methods are `synchronized`) — slightly slower due to locking overhead.

```java
StringBuffer sb = new StringBuffer("data");
sb.append("-safe");   // synchronized method
```

**Comparison**:

| | String | StringBuilder | StringBuffer |
|---|---|---|---|
| Mutability | Immutable | Mutable | Mutable |
| Thread-safe | Yes (immutable) | No | Yes |
| Performance | Slow for concatenation | Fast | Slower than StringBuilder |
| Use case | Fixed/rarely-changed text | Single-threaded heavy manipulation | Multi-threaded manipulation |

**Common interview mistake**: looping string concatenation with `+` in a loop creates many intermediate objects — always use `StringBuilder` inside loops.
```java
// Bad:
String result = "";
for (int i = 0; i < 10000; i++) result += i; // O(n^2), many garbage objects

// Good:
StringBuilder sb = new StringBuilder();
for (int i = 0; i < 10000; i++) sb.append(i); // O(n)
```