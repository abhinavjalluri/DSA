# Part 5 – Java 8+ (Functional Programming)

---

## 1. Lambda Expressions

Anonymous function — concise way to implement a **functional interface** (interface with exactly one abstract method).

```java
// Before Java 8
Runnable r1 = new Runnable() {
    public void run() { System.out.println("Old way"); }
};

// Java 8 lambda
Runnable r2 = () -> System.out.println("Lambda way");

Comparator<Integer> cmp = (a, b) -> a - b;
BiFunction<Integer, Integer, Integer> add = (a, b) -> a + b;
```

Syntax: `(parameters) -> expression` or `(parameters) -> { statements; }`. Types are usually inferred.

Lambdas can access **effectively final** local variables from the enclosing scope (variable must not be reassigned after being captured).

```java
int factor = 10;                 // effectively final
Function<Integer,Integer> multiply = x -> x * factor;   // OK
// factor = 20;  // would break compilation -> "not effectively final"
```

---

## 2. Functional Interface

An interface with exactly **one abstract method** (can have default/static methods too). Annotated with `@FunctionalInterface` (optional but recommended — compiler enforces the single-abstract-method rule).

```java
@FunctionalInterface
interface Calculator {
    int calculate(int a, int b);
}
Calculator add = (a, b) -> a + b;
Calculator multiply = (a, b) -> a * b;
System.out.println(add.calculate(2, 3));       // 5
```

**Built-in functional interfaces** (`java.util.function`): `Function<T,R>`, `Predicate<T>`, `Consumer<T>`, `Supplier<T>`, `BiFunction<T,U,R>`, `UnaryOperator<T>`, `BinaryOperator<T>`.

---

## 3. Method Reference

Shorthand for a lambda that just calls an existing method. Four kinds:

```java
// 1. Static method reference
Function<String, Integer> parse = Integer::parseInt;          // s -> Integer.parseInt(s)

// 2. Instance method reference on a particular object
String prefix = "Hello";
Predicate<String> startsWith = prefix::startsWith;             // s -> prefix.startsWith(s)

// 3. Instance method reference on an arbitrary object of a type
Function<String, Integer> len = String::length;                // s -> s.length()

// 4. Constructor reference
Supplier<ArrayList<String>> listSupplier = ArrayList::new;     // () -> new ArrayList<>()
```

```java
List<String> names = List.of("Charlie", "Alice", "Bob");
names.forEach(System.out::println);            // instance method ref
List<Integer> lengths = names.stream().map(String::length).toList();
```

---

## 4. Streams

Sequence of elements supporting functional-style **pipeline** operations: `source → intermediate ops (lazy) → terminal op (eager)`.

```java
List<String> names = List.of("Alice", "Bob", "Charlie", "Dave", "Eve");

List<String> result = names.stream()
    .filter(n -> n.length() > 3)      // intermediate (lazy)
    .map(String::toUpperCase)         // intermediate (lazy)
    .sorted()                         // intermediate (lazy)
    .toList();                        // terminal (triggers execution)

// Numeric streams (avoids boxing)
int sum = IntStream.rangeClosed(1, 10).sum();

// Parallel stream (splits work across ForkJoinPool.commonPool threads)
long count = names.parallelStream().filter(n -> n.startsWith("A")).count();
```

**Key properties**:
- **Lazy**: intermediate operations don't execute until a terminal operation is invoked.
- **Single-use**: a stream can only be consumed once — reusing a consumed stream throws `IllegalStateException`.
- **Non-mutating**: streams don't modify the underlying source (unless you explicitly do so inside a lambda, which is discouraged).

Common intermediate ops: `filter, map, flatMap, sorted, distinct, limit, skip, peek`.
Common terminal ops: `collect, forEach, reduce, count, anyMatch, allMatch, noneMatch, findFirst, findAny, toList`.

```java
// flatMap - flattening nested structures
List<List<Integer>> nested = List.of(List.of(1,2), List.of(3,4));
List<Integer> flat = nested.stream().flatMap(List::stream).toList();  // [1,2,3,4]

// reduce
int product = Stream.of(1,2,3,4).reduce(1, (a, b) -> a * b);   // 24
```

---

## 5. Optional

A container object that may or may not hold a non-null value — designed to reduce `NullPointerException`s and make "value might be absent" explicit in method signatures.

```java
Optional<String> opt = Optional.of("value");
Optional<String> empty = Optional.empty();
Optional<String> nullable = Optional.ofNullable(getMaybeNullValue());

opt.isPresent();
opt.ifPresent(v -> System.out.println(v));
String val = opt.orElse("default");
String val2 = opt.orElseGet(() -> computeDefault());
String val3 = opt.orElseThrow(() -> new NoSuchElementException());

Optional<Integer> len = opt.map(String::length);      // chainable, avoids null checks
```

**Best practice**: use `Optional` as a **return type** to signal "may be absent" (e.g. Repository `findById`), not as a field type or method parameter — that adds unnecessary wrapping overhead.

---

## 6. Predicate

`Predicate<T>` — represents a boolean-valued function: `boolean test(T t)`.

```java
Predicate<Integer> isEven = n -> n % 2 == 0;
Predicate<Integer> isPositive = n -> n > 0;

Predicate<Integer> combined = isEven.and(isPositive);
Predicate<Integer> either = isEven.or(isPositive);
Predicate<Integer> negated = isEven.negate();

List<Integer> nums = List.of(-4, -3, 2, 5, 8);
List<Integer> evenPositive = nums.stream().filter(combined).toList();  // [2, 8]
```

Widely used with `Stream.filter()` and in Spring Data JPA `Specification`s.

---

## 7. Function

`Function<T, R>` — takes an argument of type `T`, returns a result of type `R`: `R apply(T t)`.

```java
Function<String, Integer> toLength = String::length;
Function<Integer, Integer> square = n -> n * n;

Function<String, Integer> lengthSquared = toLength.andThen(square);  // apply toLength then square
Function<Integer, Integer> squareOfLength = square.compose(toLength); // apply toLength first, then square (same here)

System.out.println(lengthSquared.apply("hello"));   // 25
```

`andThen()` runs *after* the current function; `compose()` runs *before*.

---

## 8. Consumer

`Consumer<T>` — takes an input, returns nothing (side-effect only): `void accept(T t)`.

```java
Consumer<String> printer = System.out::println;
Consumer<String> logger = s -> System.out.println("LOG: " + s);

Consumer<String> combined = printer.andThen(logger);
combined.accept("Hello");   // prints, then logs

List.of("a", "b").forEach(printer);   // forEach takes a Consumer
```

---

## 9. Supplier

`Supplier<T>` — takes no input, returns a value: `T get()`. Useful for lazy evaluation / deferred computation.

```java
Supplier<Double> randomValue = Math::random;
Supplier<List<String>> listFactory = ArrayList::new;

// Lazy evaluation - only called if needed
Optional<String> opt = Optional.empty();
String result = opt.orElseGet(() -> computeExpensiveDefault());   // only invoked if opt is empty
```

---

## 10. Collectors

Utility class with factory methods for `Stream.collect()` — reduces a stream into a collection, map, string, or summary statistic.

```java
List<String> names = List.of("Alice", "Bob", "Charlie", "Dave");

List<String> list = names.stream().collect(Collectors.toList());
Set<String> set = names.stream().collect(Collectors.toSet());
String joined = names.stream().collect(Collectors.joining(", ", "[", "]"));  // [Alice, Bob, ...]

Map<Integer, List<String>> byLength = names.stream()
    .collect(Collectors.groupingBy(String::length));
// {3=[Bob], 4=[Dave], 5=[Alice], 7=[Charlie]}

Map<Boolean, List<String>> partitioned = names.stream()
    .collect(Collectors.partitioningBy(n -> n.length() > 4));

double avgLength = names.stream().collect(Collectors.averagingInt(String::length));
IntSummaryStatistics stats = names.stream().collect(Collectors.summarizingInt(String::length));

// groupingBy + downstream collector
Map<Integer, Long> countByLength = names.stream()
    .collect(Collectors.groupingBy(String::length, Collectors.counting()));
```

**Real-world use**: transforming JPA entity lists into DTO Maps grouped by category — extremely common in Spring Boot service layers.