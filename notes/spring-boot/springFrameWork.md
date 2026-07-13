# Part 9 – Spring Framework (Core)

## 1. Spring Architecture Overview

Spring is a modular framework built around one central idea: *Inversion of Control (IoC). Instead of your code creating and wiring objects, the **Spring IoC Container* does it for you.

*Core modules:*
•⁠  ⁠*Spring Core* – IoC container, DI, bean lifecycle
•⁠  ⁠*Spring AOP* – cross-cutting concerns (logging, transactions, security) without cluttering business logic
•⁠  ⁠*Spring Data Access* – JDBC, ORM (JPA/Hibernate) integration
•⁠  ⁠*Spring MVC* – web layer, REST controllers
•⁠  ⁠*Spring Security* – authentication/authorization
•⁠  ⁠*Spring Boot* – auto-configuration + embedded servers, built on top of all the above

---

## 2. IoC (Inversion of Control) & the Container

*Traditional approach:* Your class controls its own dependencies (⁠ new StripeGateway() ⁠ inside ⁠ OrderService ⁠).
*Inverted approach:* An external container creates and injects dependencies into your class — control is "inverted" from the class to the container.

### The Container
Spring's container is represented by ⁠ ApplicationContext ⁠ (the modern, feature-rich interface) — it:
1.⁠ ⁠Reads configuration (annotations, XML, or Java ⁠ @Configuration ⁠ classes)
2.⁠ ⁠Creates beans (objects managed by Spring)
3.⁠ ⁠Wires dependencies between beans
4.⁠ ⁠Manages their full lifecycle

⁠ java
ApplicationContext context = new AnnotationConfigApplicationContext(AppConfig.class);
OrderService service = context.getBean(OrderService.class);
 ⁠
In a Spring Boot app, ⁠ SpringApplication.run() ⁠ bootstraps this container for you automatically.

---

## 3. Dependency Injection Types

⁠ java
@Component
public class OrderService {

    // 1. Constructor Injection — RECOMMENDED
    private final PaymentGateway gateway;
    public OrderService(PaymentGateway gateway) {
        this.gateway = gateway;
    }

    // 2. Setter Injection — good for optional dependencies
    private NotificationService notifier;
    @Autowired
    public void setNotifier(NotificationService notifier) {
        this.notifier = notifier;
    }

    // 3. Field Injection — discouraged (hard to test, hides dependencies)
    @Autowired
    private AuditLogger logger;
}
 ⁠

*Why constructor injection wins:* Immutable dependencies (⁠ final ⁠), explicit required dependencies, testable without a Spring context (just call ⁠ new OrderService(mockGateway) ⁠), and fails fast at startup rather than at runtime with an NPE.

---

## 4. Stereotype Annotations

| Annotation | Purpose |
|---|---|
| ⁠ @Component ⁠ | Generic Spring-managed bean |
| ⁠ @Service ⁠ | Business logic layer (semantically clearer, same behavior as ⁠ @Component ⁠) |
| ⁠ @Repository ⁠ | Data access layer — additionally translates DB exceptions into Spring's ⁠ DataAccessException ⁠ hierarchy |
| ⁠ @Controller ⁠ | Web layer, returns view names |
| ⁠ @RestController ⁠ | ⁠ @Controller ⁠ + ⁠ @ResponseBody ⁠ combined — returns JSON/XML directly |

⁠ java
@Repository
public interface UserRepository extends JpaRepository<User, Long> {}

@Service
public class UserService {
    private final UserRepository repo;
    public UserService(UserRepository repo) { this.repo = repo; }
}

@RestController
@RequestMapping("/api/users")
public class UserController {
    private final UserService service;
    public UserController(UserService service) { this.service = service; }

    @GetMapping("/{id}")
    public UserDto getUser(@PathVariable Long id) {
        return service.findById(id);
    }
}
 ⁠

---

## 5. Bean Lifecycle


Instantiate → Populate Properties (DI) → BeanNameAware/BeanFactoryAware callbacks
    → @PostConstruct → InitializingBean.afterPropertiesSet() → custom init-method
    → Bean is ready for use
    → (on shutdown) @PreDestroy → DisposableBean.destroy() → custom destroy-method


⁠ java
@Component
public class CacheManager {

    @PostConstruct
    public void init() {
        System.out.println("Loading cache...");
    }

    @PreDestroy
    public void cleanup() {
        System.out.println("Clearing cache...");
    }
}
 ⁠

*Why lifecycle hooks matter:* ⁠ @PostConstruct ⁠ is where you do setup that depends on all dependencies already being injected (constructor injection alone can't always do heavy initialization cleanly); ⁠ @PreDestroy ⁠ ensures graceful resource cleanup (closing connections, flushing caches) on shutdown.

---

## 6. Bean Scopes

| Scope | Behavior |
|---|---|
| ⁠ singleton ⁠ (default) | One instance per Spring container |
| ⁠ prototype ⁠ | New instance every time it's requested |
| ⁠ request ⁠ | One instance per HTTP request (web apps) |
| ⁠ session ⁠ | One instance per HTTP session |

⁠ java
@Component
@Scope("prototype")
public class ReportGenerator { ... }
 ⁠

*Common mistake:* Injecting a ⁠ prototype ⁠-scoped bean into a ⁠ singleton ⁠-scoped bean via normal field injection — the prototype bean is only created ONCE (when the singleton is created), defeating the purpose. Fix: use ⁠ ObjectProvider<T> ⁠ or ⁠ @Lookup ⁠ to fetch a fresh instance on each use.

---

## 7. Configuration: Annotations vs Java Config

⁠ java
@Configuration
public class AppConfig {

    @Bean
    public PaymentGateway paymentGateway() {
        return new StripeGateway();
    }

    @Bean
    @Profile("dev")
    public DataSource devDataSource() {
        return new EmbeddedDatabaseBuilder().setType(EmbeddedDatabaseType.H2).build();
    }
}
 ⁠

•⁠  ⁠⁠ @Configuration ⁠ classes replace old XML bean definitions.
•⁠  ⁠⁠ @Bean ⁠ methods define beans explicitly (useful for third-party classes you can't annotate directly).
•⁠  ⁠⁠ @ComponentScan ⁠ tells Spring where to look for ⁠ @Component ⁠/⁠ @Service ⁠/etc. (Spring Boot does this automatically from the main class package).

### @Qualifier — disambiguating multiple implementations
⁠ java
public interface PaymentGateway {}

@Component("stripe")
class StripeGateway implements PaymentGateway {}

@Component("paypal")
class PaypalGateway implements PaymentGateway {}

@Service
public class OrderService {
    public OrderService(@Qualifier("stripe") PaymentGateway gateway) { ... }
}
 ⁠

### @Profile — environment-specific beans
⁠ java
@Bean
@Profile("prod")
public DataSource prodDataSource() { ... }
 ⁠
Activated via ⁠ application.yml ⁠: ⁠ spring.profiles.active=prod ⁠, or ⁠ -Dspring.profiles.active=prod ⁠ at runtime.

### @ConfigurationProperties — type-safe external config
⁠ java
@ConfigurationProperties(prefix = "app.mail")
@Component
public class MailProperties {
    private String host;
    private int port;
    // getters/setters
}
 ⁠
⁠ yaml
app:
  mail:
    host: smtp.example.com
    port: 587
 ⁠

---

## 8. Spring AOP (Aspect-Oriented Programming) — Brief Overview

*Why it exists:* Cross-cutting concerns (logging, security checks, transaction management) would otherwise be duplicated across many methods. AOP lets you define this logic once and apply it declaratively.

⁠ java
@Aspect
@Component
public class LoggingAspect {

    @Before("execution(* com.example.service.*.*(..))")
    public void logBefore(JoinPoint jp) {
        System.out.println("Calling: " + jp.getSignature().getName());
    }
}
 ⁠
⁠ @Transactional ⁠ (heavily used with Spring Data JPA) is itself implemented as an AOP proxy — Spring wraps your method call with begin/commit/rollback logic transparently.

---

## 9. Interview Questions — Spring Core

1.⁠ ⁠*What is the difference between ⁠ BeanFactory ⁠ and ⁠ ApplicationContext ⁠?*
   ⁠ BeanFactory ⁠ is the basic container (lazy bean initialization); ⁠ ApplicationContext ⁠ extends it with eager initialization by default, event publishing, AOP integration, and internationalization — it's what real applications use.

2.⁠ ⁠*Why is the default bean scope ⁠ singleton ⁠, and how is that different from the GoF Singleton pattern?*
   Spring's singleton is one instance *per container* (per ⁠ ApplicationContext ⁠), not one instance per JVM like the classic Singleton pattern — a JVM could theoretically have multiple Spring containers, each with its own "singleton" instance.

3.⁠ ⁠*What happens if there are two beans implementing the same interface and you ⁠ @Autowired ⁠ without a qualifier?*
   Spring throws ⁠ NoUniqueBeanDefinitionException ⁠ unless one bean is marked ⁠ @Primary ⁠ or you specify ⁠ @Qualifier ⁠.

4.⁠ ⁠*Explain circular dependency in Spring and how it's resolved.*
   If Bean A needs Bean B and Bean B needs Bean A, constructor injection fails immediately (⁠ BeanCurrentlyInCreationException ⁠) because neither can be fully constructed first. Fixable via setter injection (allows a partially-constructed bean reference) or refactoring to break the cycle — constructor circular dependencies are generally considered a design smell.

5.⁠ ⁠*What's the difference between ⁠ @Component ⁠ and ⁠ @Bean ⁠?*
   ⁠ @Component ⁠ is a class-level annotation letting Spring auto-detect and instantiate your own classes via component scanning; ⁠ @Bean ⁠ is a method-level annotation inside a ⁠ @Configuration ⁠ class used typically for third-party classes you don't own/can't annotate, or when custom construction logic is needed.

6.⁠ ⁠*How does ⁠ @Transactional ⁠ work internally?*
   Spring creates a proxy (JDK dynamic proxy for interfaces, or CGLIB subclass proxy for classes) around the bean; the proxy intercepts the method call, starts a transaction before it, and commits/rolls back after, based on whether an exception was thrown.

## 10. Best Practices
•⁠  ⁠Prefer constructor injection everywhere; reserve field injection for quick prototypes/tests only.
•⁠  ⁠Keep ⁠ @Configuration ⁠ classes focused — one per concern (DataSourceConfig, SecurityConfig, etc.) rather than one giant config class.
•⁠  ⁠Use ⁠ @ConfigurationProperties ⁠ over scattered ⁠ @Value("${...}") ⁠ injections for related settings — it's type-safe and groups config logically.
•⁠  ⁠Don't overuse ⁠ prototype ⁠ scope — most beans should remain stateless singletons; introduce prototype scope only when a bean genuinely holds per-use mutable state.
•⁠  ⁠Keep AOP aspects narrow and well-documented — overuse makes code flow hard to trace ("magic" behavior).



# Part 9 – Spring Framework

## 1. Spring Architecture

Spring is a **modular, layered Java framework** built around one core idea: instead of your objects creating and wiring their own dependencies, a container does it for you. This is called **Inversion of Control (IoC)**.

Core modules:
- **Core Container** – Beans, Core, Context, SpEL (Spring Expression Language)
- **Data Access/Integration** – JDBC, ORM, JMS, Transactions
- **Web** – Spring MVC, WebFlux (reactive)
- **AOP** – Aspect-Oriented Programming (cross-cutting concerns like logging, security)
- **Test** – Support for JUnit/TestNG integration testing

**Why it exists:** Plain Java apps end up tightly coupled — `new OrderService(new OrderRepository())` scattered everywhere. Spring centralizes object creation, configuration, and lifecycle management so classes only depend on abstractions, not concrete instantiation logic.

---

## 2. IoC (Inversion of Control)

**Definition:** Control of object creation and wiring is inverted — instead of your code controlling it, the **Spring IoC Container** does.

Without IoC:
```java
class OrderService {
    private OrderRepository repo = new OrderRepository(); // tight coupling
}
```

With IoC:
```java
class OrderService {
    private final OrderRepository repo;
    OrderService(OrderRepository repo) { this.repo = repo; } // injected
}
```

The container (`ApplicationContext`) reads configuration (annotations, XML, or Java config), creates beans, resolves their dependencies, and hands you a fully wired object graph.

**Real-world analogy:** A restaurant kitchen doesn't have each chef go shopping for their own ingredients — a supply manager (the container) delivers exactly what's needed, when it's needed.

---

## 3. Dependency Injection (DI)

DI is the **mechanism** that implements IoC. Three types:

| Type | Example | Notes |
|---|---|---|
| Constructor Injection | `public OrderService(OrderRepository r)` | **Preferred** — supports immutability, makes dependencies explicit, works well with `final` fields |
| Setter Injection | `@Autowired public void setRepo(...)` | Useful for optional dependencies |
| Field Injection | `@Autowired private OrderRepository repo;` | Easiest to write, but harder to test/mock, hides dependencies — avoid in production code |

**Why constructor injection is best practice:** it makes the class fail fast at construction if a required bean is missing, allows fields to be `final` (thread-safe, immutable), and makes unit testing trivial (`new OrderService(mockRepo)` — no Spring needed).

---

## 4. Bean Lifecycle

A **Bean** is any object managed by the Spring IoC container.

Lifecycle sequence:
1. Container starts → reads bean definitions
2. Instantiate bean (constructor call)
3. Populate properties (dependency injection)
4. `Aware` interfaces invoked (e.g., `BeanNameAware`, `ApplicationContextAware`)
5. `@PostConstruct` / `InitializingBean.afterPropertiesSet()`
6. Custom `init-method` (if configured)
7. Bean is **ready to use**
8. On shutdown: `@PreDestroy` / `DisposableBean.destroy()` / custom `destroy-method`

```java
@Component
class CacheManager {
    @PostConstruct
    void init() { System.out.println("Cache warming up..."); }

    @PreDestroy
    void cleanup() { System.out.println("Cache flushing..."); }
}
```

**Common interview question:** *"What's the difference between `@PostConstruct` and a constructor?"* — The constructor runs before dependencies are injected (for field/setter injection); `@PostConstruct` runs **after** all dependencies are injected, so it's safe to use injected fields there.

---

## 5. Bean Scopes

| Scope | Meaning |
|---|---|
| `singleton` (default) | One instance per Spring container |
| `prototype` | New instance every time it's requested |
| `request` | One instance per HTTP request (web apps) |
| `session` | One instance per HTTP session |
| `application` | One instance per ServletContext |

```java
@Component
@Scope("prototype")
class ReportGenerator { }
```

**Common mistake:** Injecting a `prototype` bean into a `singleton` bean directly — the prototype bean gets created only once (at singleton wiring time) and reused forever, defeating its purpose. Fix: use `ObjectProvider<T>` or a proxy scope.

---

## 6. Stereotype Annotations: `@Component`, `@Service`, `@Repository`, `@Controller`

All four are specializations of `@Component` — they mark a class as a Spring-managed bean, but carry **semantic meaning**:

- `@Component` – generic bean
- `@Service` – business/service layer logic
- `@Repository` – data access layer; additionally enables **exception translation** (converts JDBC/JPA exceptions into Spring's unified `DataAccessException` hierarchy)
- `@Controller` – web layer (MVC); handles HTTP requests
- `@RestController` – `@Controller` + `@ResponseBody` combined; returns data (JSON) directly, not view names

```java
@Repository
interface UserRepository extends JpaRepository<User, Long> {}

@Service
class UserService {
    private final UserRepository repo;
    UserService(UserRepository repo) { this.repo = repo; }
}

@RestController
class UserController {
    private final UserService service;
    UserController(UserService service) { this.service = service; }

    @GetMapping("/users/{id}")
    User getUser(@PathVariable Long id) { return service.findById(id); }
}
```

---

## 7. `@Configuration` and `@Bean`

Java-based configuration (replaces old XML config):

```java
@Configuration
class AppConfig {
    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }
}
```

Use `@Bean` when you need to register a **third-party class** (you can't annotate `RestTemplate` itself with `@Component` since you don't own the source).

---

## 8. `@Autowired` and `@Qualifier`

`@Autowired` tells Spring to inject a dependency by **type**. If multiple beans of the same type exist, Spring throws `NoUniqueBeanDefinitionException` unless disambiguated:

```java
interface PaymentGateway {}

@Component("razorpay")
class RazorpayGateway implements PaymentGateway {}

@Component("stripe")
class StripeGateway implements PaymentGateway {}

@Service
class PaymentService {
    private final PaymentGateway gateway;

    PaymentService(@Qualifier("stripe") PaymentGateway gateway) {
        this.gateway = gateway;
    }
}
```

`@Primary` is an alternative — marks one bean as the default choice when multiple candidates exist, without needing `@Qualifier` at every injection point.

---

## 9. `@Profile`

Lets you register beans **conditionally**, based on the active environment (dev, test, prod).

```java
@Configuration
class DataSourceConfig {

    @Bean
    @Profile("dev")
    DataSource devDataSource() { return new H2DataSource(); }

    @Bean
    @Profile("prod")
    DataSource prodDataSource() { return new PostgresDataSource(); }
}
```

Activate via `application.properties`: `spring.profiles.active=dev`, or `-Dspring.profiles.active=prod` at runtime.

---

## 10. `@ConfigurationProperties`

Type-safe way to bind external configuration (YAML/properties) to a Java object, instead of scattering `@Value("${...}")` everywhere.

```yaml
app:
  mail:
    host: smtp.example.com
    port: 587
```

```java
@ConfigurationProperties(prefix = "app.mail")
@Component
class MailProperties {
    private String host;
    private int port;
    // getters/setters
}
```

**Why prefer this over `@Value`:** centralizes related config into one class, supports validation (`@Validated` + JSR-303 annotations), and is easier to test and refactor.

---

## Common Interview Questions – Part 9

1. What is the difference between BeanFactory and ApplicationContext? (ApplicationContext is a superset — adds event publishing, internationalization, AOP integration, and eager singleton initialization.)
2. Why is constructor injection preferred over field injection?
3. What happens if two beans qualify for the same `@Autowired` field?
4. Explain the Spring Bean lifecycle in order.
5. Difference between `@Component`, `@Service`, and `@Repository` — functionally almost identical, but `@Repository` adds exception translation.
6. What is a circular dependency and how does Spring handle/report it?