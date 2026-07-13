# Part 14 – Interview Preparation

> Note: A literal 300-question file would run thousands of lines and lose usefulness as a study tool. Instead, this file gives you a **curated, high-yield set** organized by category, each with a concise model answer — the kind of depth interviewers actually probe for. Use Parts 1–13 for full explanations behind any answer here.

---

## A. Tricky Core Java Questions

**1. Why is `String` immutable in Java?**
Security (used in classloading, network connections — can't be altered mid-use), thread-safety (safely shared across threads without synchronization), and enables the **String pool** (identical literals reuse the same object, saving memory). Once created, its internal `char[]`/`byte[]` cannot be changed.

**2. What's the output?**
```java
String a = "hello";
String b = "hello";
String c = new String("hello");
System.out.println(a == b);   // true  — both point to the same String pool literal
System.out.println(a == c);   // false — new String() forces heap allocation, bypassing the pool
System.out.println(a.equals(c)); // true — equals() compares content
```

**3. Difference between `==` and `.equals()`?**
`==` compares references (memory addresses) for objects, or actual values for primitives. `.equals()` compares logical/content equality, and its behavior depends on whether the class overrides it (default `Object.equals()` is just `==`).

**4. Why override `hashCode()` whenever you override `equals()`?**
Hash-based collections (`HashMap`, `HashSet`) rely on the contract: **equal objects must have equal hash codes.** If you override `equals()` without `hashCode()`, two "equal" objects could land in different buckets, breaking lookups — `map.get(key)` might fail to find an entry that's logically present.

**5. What is the difference between `final`, `finally`, and `finalize()`?**
- `final` — keyword; makes a variable constant, a method non-overridable, or a class non-extendable.
- `finally` — block that always executes after try/catch, regardless of exception, used for cleanup.
- `finalize()` — deprecated method called by the GC before object destruction (unreliable timing — avoid using it; use try-with-resources instead).

**6. Why does `ConcurrentModificationException` occur?**
```java
List<String> list = new ArrayList<>(List.of("a", "b", "c"));
for (String s : list) {
    if (s.equals("b")) list.remove(s); // throws ConcurrentModificationException
}
```
The enhanced for-loop uses an `Iterator` internally, which tracks a `modCount`. Structurally modifying the list directly (not via `Iterator.remove()`) invalidates that count, and the next `next()` call detects the mismatch and fails fast. Fix: use `Iterator.remove()` or `list.removeIf(...)`.

**7. Difference between `HashMap` and `ConcurrentHashMap`?**
`HashMap` is not thread-safe — concurrent modification can corrupt internal structure or cause infinite loops in old JDK versions. `ConcurrentHashMap` achieves thread safety via lock striping (segments/bucket-level locking in modern versions), allowing concurrent reads and limited concurrent writes without locking the entire map.

**8. What's the difference between checked and unchecked exceptions?**
Checked exceptions (`IOException`, `SQLException`) extend `Exception` and must be declared/caught at compile time — represent recoverable conditions the caller should anticipate. Unchecked exceptions (`RuntimeException` and subclasses) don't require explicit handling — usually represent programming errors (`NullPointerException`, `IllegalArgumentException`).

**9. Why does autoboxing cause subtle bugs?**
```java
Integer a = 127, b = 127;
System.out.println(a == b); // true — Integer cache (-128 to 127)

Integer c = 200, d = 200;
System.out.println(c == d); // false — outside cache range, different objects
```
Always use `.equals()` for wrapper class comparison, never `==`.

**10. What is the diamond problem, and how does Java solve it for interfaces with default methods?**
If a class implements two interfaces with the same default method signature, Java forces the class to explicitly override and choose (or combine) the implementation — it won't silently pick one, avoiding the ambiguity C++ multiple inheritance faces.

**11. Difference between `Comparable` and `Comparator`?**
`Comparable` (`compareTo`) defines a class's **natural ordering**, implemented within the class itself (one strategy only). `Comparator` (`compare`) defines **external, pluggable** ordering strategies — you can have multiple comparators for the same class without modifying it.

**12. What happens when you call a static method via an instance reference (`obj.staticMethod()`)?**
It compiles and works, but is considered bad practice/a code smell — static methods are resolved at compile time based on the reference type, not dynamically dispatched, so calling them via an instance is misleading.

**13. Why can't you override `static` or `private` methods?**
Overriding relies on **dynamic dispatch (runtime polymorphism)**. `static` methods are bound at compile time (class-level, not instance-level) and `private` methods aren't visible/inherited by subclasses at all — both are resolved statically, so the concept of "override" doesn't apply; you can only **hide** a static method by declaring one with the same signature in the subclass.

---

## B. Spring Boot Interview Questions

**14. Explain how a request flows through a Spring Boot MVC application end to end.**
`DispatcherServlet` receives the HTTP request → consults `HandlerMapping` to find the right controller method → `HandlerAdapter` invokes it, resolving `@PathVariable`/`@RequestBody`/etc. arguments via `HttpMessageConverter`s → controller delegates to service/repository layers → return value is converted (e.g., to JSON via Jackson) → response sent back through any configured filters/interceptors.

**15. How does `@Transactional` actually work under the hood?**
Spring creates a **proxy** (CGLIB or JDK dynamic proxy) around the bean. When a `@Transactional` method is called externally, the proxy intercepts the call, starts a transaction, invokes the real method, then commits or rolls back based on the outcome. This is why calling a `@Transactional` method from **within the same class** bypasses the proxy entirely — no transaction is applied.

**16. What is the difference between `@RequestParam` and `@PathVariable`?**
`@PathVariable` extracts values from the URI path itself (`/users/{id}`); `@RequestParam` extracts query string parameters (`/users?id=5`) or form data.

**17. How would you handle a scenario with two beans of the same interface type?**
Use `@Qualifier("beanName")` at the injection point, or mark one implementation `@Primary` as the default, or inject a `List<InterfaceType>`/`Map<String, InterfaceType>` if you genuinely need all implementations (e.g., strategy pattern dispatch).

**18. What's the actual difference between `@Component`, `@Service`, and `@Repository` functionally?**
All three register a Spring-managed bean identically via component scanning. `@Repository` adds one real functional difference: Spring wraps it with a `PersistenceExceptionTranslationPostProcessor`, converting low-level JDBC/Hibernate exceptions into Spring's unified `DataAccessException` hierarchy. `@Service` and `@Component` have no additional runtime behavior — they're primarily for semantic clarity to the developer.

**19. How do you secure a REST API used by a mobile app and a web app, given both use different needs?**
Typically: stateless JWT-based authentication (`SessionCreationPolicy.STATELESS`) so no server-side session state is needed; issue access + refresh tokens; validate the JWT in a custom filter before it reaches Spring Security's authentication chain; use role/scope claims within the token for authorization (`@PreAuthorize("hasRole('ADMIN')")`).

**20. What's the difference between `@Valid` and `@Validated`?**
`@Valid` (from `jakarta.validation`) triggers standard bean validation on method parameters/fields. `@Validated` (Spring's own annotation) additionally supports **validation groups** and works at the class level to enable method-parameter validation on non-`@RequestBody` types.

---

## C. Coding Standards & Practical Scenarios

**21. How would you design a REST API to handle "get all orders for a user" with 100,000+ orders?**
Never return the whole list. Use pagination (`Pageable`, `Page<T>`), consider cursor-based pagination for very large/frequently changing datasets (more stable than offset-based when data is inserted concurrently), add filtering/sorting query params, and consider caching frequent queries.

**22. How do you prevent duplicate order submissions from a flaky client retry?**
Use an **idempotency key**: client generates a unique key per logical operation and sends it in a header; server checks if that key has already been processed (e.g., stored with a short TTL in Redis or a DB unique constraint) before creating a new order.

**23. Walk through how you'd design exception handling for a production REST API.**
Custom exception hierarchy for domain errors → centralized `@RestControllerAdvice` mapping each exception type to an appropriate HTTP status and a consistent `ErrorResponse` body (timestamp, status, message, path) → log at appropriate level (client errors like 400s at `WARN`, server errors at `ERROR` with stack trace) → never leak internal stack traces or DB details to the client response.

**24. How would you avoid the N+1 problem in a real project — walk through detecting and fixing it?**
Detect: enable `show-sql: true` / use a tool like p6spy or Hibernate statistics in dev, and notice one query per row instead of one query total for a list endpoint. Fix: rewrite the repository query with `JOIN FETCH` for the needed association, or use a DTO projection (`@Query("SELECT new com.app.dto.OrderSummary(o.id, o.total, c.name) FROM Order o JOIN o.customer c")`) to fetch only what's needed in a single query.

---

## D. Real Project / Design-Style Questions (What Interviewers Actually Probe)

**25. "Tell me about a challenging bug you fixed."**
Structure your answer: (1) symptom observed, (2) how you narrowed down root cause (logs, debugger, reproducing locally), (3) the actual fix, (4) what you changed afterward to prevent recurrence (test added, monitoring/alerting added). Interviewers care more about your **debugging process** than the specific bug.

**26. "How would you design an e-commerce order system?"**
Talk through: `Order`, `OrderItem`, `Product`, `Customer` entities and their relationships; separating `OrderService` (business logic) from `OrderRepository` (persistence); using `@Transactional` to ensure stock reservation + payment + order creation are atomic; using events/messaging (Kafka/RabbitMQ) to decouple downstream concerns like sending confirmation emails or updating analytics, so the core order-placement path stays fast; idempotency for payment retries; how you'd scale it into microservices if needed (Order Service, Inventory Service, Payment Service, communicating via REST + async events).

**27. "How do you approach writing tests for a new feature?"**
Unit tests for business logic in isolation (mock dependencies with Mockito), `@WebMvcTest` for controller-layer contract testing, `@DataJpaTest` for repository query correctness against an in-memory DB, and a smaller set of `@SpringBootTest` integration tests covering critical end-to-end flows. Emphasize testing edge cases and failure paths, not just the happy path.

**28. "How would you version a public REST API?"**
Common approaches: URI versioning (`/api/v1/orders`), header versioning (`Accept: application/vnd.myapp.v1+json`), or query param versioning. URI versioning is simplest and most common in practice; discuss trade-offs (URI versioning is visible/cacheable but "pollutes" the URI; header versioning is cleaner REST-wise but harder to test/discover).

---

## How to Use This File

1. Read Parts 9–13 first for the underlying concepts.
2. Try to answer each question above **out loud, unaided**, before checking the model answer.
3. For any question you struggle with, go back to the relevant Part and re-read that section — don't just memorize the answer here.
4. Practice explaining answers with a **concrete code example**, not just definitions — interviewers weight this heavily for mid/senior roles.