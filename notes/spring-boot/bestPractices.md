# Part 13 – Best Practices

## 1. Coding Standards

- Follow consistent naming: `camelCase` for variables/methods, `PascalCase` for classes, `UPPER_SNAKE_CASE` for constants.
- One class = one responsibility (Single Responsibility Principle — see SOLID below).
- Keep methods short and focused — a method should ideally do one thing and be readable without scrolling.
- Avoid deeply nested `if/else` — prefer early returns (guard clauses).

```java
// Avoid
void process(Order order) {
    if (order != null) {
        if (order.isValid()) {
            if (order.getItems().size() > 0) {
                // actual logic buried 3 levels deep
            }
        }
    }
}

// Prefer
void process(Order order) {
    if (order == null || !order.isValid()) return;
    if (order.getItems().isEmpty()) return;
    // actual logic, flat and readable
}
```

---

## 2. Package Structure

Two common approaches:

**Layered (technical) packaging:**
```
com.myapp
 ├── controller
 ├── service
 ├── repository
 ├── dto
 └── entity
```

**Feature-based (domain) packaging** — generally preferred for larger apps:
```
com.myapp
 ├── order
 │    ├── OrderController
 │    ├── OrderService
 │    ├── OrderRepository
 │    └── OrderDto
 ├── payment
 │    ├── PaymentController
 │    ├── PaymentService
 │    └── ...
```

**Why feature-based scales better:** related code stays together, easier to understand a feature in isolation, easier to eventually extract into a separate microservice if needed.

---

## 3. Naming Conventions

- Classes: nouns (`OrderService`, not `ProcessOrder`)
- Methods: verbs (`calculateTotal()`, not `totalCalculation()`)
- Booleans: `isActive`, `hasPermission`, `canEdit` — read naturally in `if` statements
- Avoid abbreviations that aren't universally understood (`calc`, `mgr` — prefer `calculate`, `manager`)
- Constants: `MAX_RETRY_COUNT`, not `maxRetryCount`

---

## 4. DTO Mapping

Manual mapping is verbose; for larger projects use **MapStruct** (compile-time, fast, no reflection):

```java
@Mapper(componentModel = "spring")
interface ProductMapper {
    ProductDto toDto(Product entity);
    Product toEntity(ProductDto dto);
    List<ProductDto> toDtoList(List<Product> entities);
}
```

**Why MapStruct over ModelMapper:** MapStruct generates actual Java code at compile time (fast, type-safe, easy to debug with a breakpoint); ModelMapper uses reflection at runtime (slower, errors surface only at runtime).

---

## 5. Logging Best Practices

- Use appropriate log levels: `ERROR` (something broke, needs attention), `WARN` (unexpected but recoverable), `INFO` (high-level business events), `DEBUG` (detailed diagnostic info, off in production).
- Never log sensitive data (passwords, tokens, credit card numbers, PII) — a common security/compliance failure.
- Use structured/parameterized logging: `log.info("Order {} created for user {}", orderId, userId);` — not string concatenation.
- Include correlation/trace IDs in distributed systems so a single request can be traced across services (e.g., via MDC — Mapped Diagnostic Context — or a tool like Sleuth/Micrometer Tracing).

---

## 6. Exception Design

- Create a small hierarchy of **custom, meaningful exceptions** rather than throwing generic `RuntimeException` everywhere:

```java
class ResourceNotFoundException extends RuntimeException {
    ResourceNotFoundException(String message) { super(message); }
}

class InsufficientStockException extends RuntimeException {
    InsufficientStockException(String message) { super(message); }
}
```

- Don't catch exceptions just to swallow them silently:

```java
// Bad — hides real problems
try {
    riskyOperation();
} catch (Exception e) {}

// Good
try {
    riskyOperation();
} catch (IOException e) {
    log.error("Failed to process file", e);
    throw new FileProcessingException("Could not process uploaded file", e);
}
```

- Fail fast — validate inputs early and throw clear exceptions rather than letting bad data propagate deep into the system.
- Use `@RestControllerAdvice` (see Part 10) to centralize translation of exceptions into HTTP responses.

---

## 7. Clean Code Principles

- **DRY (Don't Repeat Yourself)** — extract repeated logic into shared methods/utilities.
- **KISS (Keep It Simple, Stupid)** — prefer the simplest solution that works; avoid unnecessary abstraction/cleverness.
- **YAGNI (You Aren't Gonna Need It)** — don't build flexibility/features for hypothetical future requirements that don't exist yet.
- Meaningful names over comments — code should mostly explain itself; comments explain **why**, not **what**.
- Keep functions small; a function that needs a comment to explain what it does often should be split/renamed instead.

---

## 8. SOLID Principles (Applied Practically)

- **S – Single Responsibility:** a class should have only one reason to change. `OrderService` shouldn't also handle email sending — extract `NotificationService`.
- **O – Open/Closed:** open for extension, closed for modification. Use interfaces/strategy pattern so new behavior can be added without editing existing tested code.
- **L – Liskov Substitution:** subclasses must be substitutable for their base class without breaking correctness — e.g., an `Ostrich extends Bird` that throws `UnsupportedOperationException` on `fly()` violates this.
- **I – Interface Segregation:** many small, specific interfaces are better than one large general-purpose interface — clients shouldn't be forced to implement methods they don't use.
- **D – Dependency Inversion:** depend on abstractions (interfaces), not concrete implementations — this is exactly what Spring's DI enables.

---

## 9. Performance Best Practices

- Avoid N+1 queries (see Part 11) — use `JOIN FETCH` or DTO projections.
- Use pagination for large result sets — never return unbounded lists from an API.
- Use appropriate indexes on frequently queried DB columns.
- Prefer `StringBuilder` over string concatenation in loops (see Part 1 — immutability of `String` means each `+=` creates a new object).
- Use connection pooling (HikariCP, Spring Boot's default) — never open raw DB connections manually.
- Cache expensive/frequently-read, rarely-changed data (`@Cacheable`) rather than recomputing/refetching every request.
- Use async/non-blocking calls for I/O-heavy operations that don't need to block the caller.

---

## 10. Security Best Practices

- Never hardcode secrets/credentials in source code — use environment variables, `application.yml` + external config, or a secrets manager (Vault, AWS Secrets Manager).
- Always hash passwords with a strong algorithm (`BCryptPasswordEncoder`) — never store plaintext or reversible-encrypted passwords.
- Validate and sanitize all external input to prevent SQL injection (use parameterized queries / JPA, never string-concatenated SQL) and XSS.
- Apply the principle of least privilege — a service account should only have the DB/API permissions it actually needs.
- Keep dependencies updated — many vulnerabilities come from outdated third-party libraries (use tools like OWASP Dependency-Check).
- Use HTTPS everywhere; never transmit tokens/credentials over plain HTTP.
- Set proper CORS policy — don't default to `Access-Control-Allow-Origin: *` in production APIs handling sensitive data.

---

## Common Interview Questions – Part 13

1. Explain each SOLID principle with a real code example.
2. What's the difference between DRY and premature abstraction — how do you avoid over-engineering?
3. Why shouldn't you swallow exceptions silently? Give a real scenario where this caused a bug.
4. How would you structure packages for a large Spring Boot application, and why?
5. What are common security mistakes you've seen in REST APIs, and how do you prevent them?
6. How do you decide what to log at INFO vs DEBUG vs ERROR level?