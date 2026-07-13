# Part 10 – Spring Boot

## 1. Why Spring Boot Exists

Classic Spring required extensive XML/Java configuration — data sources, view resolvers, dispatcher servlets, embedded server setup — before writing a single line of business logic. Spring Boot removes this ceremony via *auto-configuration, **starter dependencies, and an **embedded server*, so you can run a production-ready app with a single class.

⁠ java
@SpringBootApplication
public class MyApp {
    public static void main(String[] args) {
        SpringApplication.run(MyApp.class, args);
    }
}
 ⁠

⁠ @SpringBootApplication ⁠ is a meta-annotation combining:
•⁠  ⁠⁠ @Configuration ⁠ – marks it as a config class
•⁠  ⁠⁠ @EnableAutoConfiguration ⁠ – triggers Spring Boot's auto-configuration engine
•⁠  ⁠⁠ @ComponentScan ⁠ – scans the current package and sub-packages for beans

---

## 2. Auto-Configuration — How It Works Internally

Spring Boot inspects what's on the classpath and what beans already exist, then conditionally configures beans you'd otherwise write by hand.

*Mechanism:* ⁠ @EnableAutoConfiguration ⁠ triggers loading of classes listed in ⁠ META-INF/spring/org.springframework.boot.autoconfigure.AutoConfiguration.imports ⁠ (Boot 3+; older versions used ⁠ spring.factories ⁠). Each auto-config class is annotated with conditional annotations:

⁠ java
@Configuration
@ConditionalOnClass(DataSource.class)          // only if this class is on the classpath
@ConditionalOnMissingBean(DataSource.class)    // only if the user hasn't defined their own
public class DataSourceAutoConfiguration {
    @Bean
    public DataSource dataSource() { ... }
}
 ⁠

*Example:* Add ⁠ spring-boot-starter-data-jpa ⁠ to your classpath → Boot detects Hibernate + a ⁠ DataSource ⁠ → auto-configures ⁠ EntityManagerFactory ⁠, ⁠ TransactionManager ⁠, etc. — all without you writing config, unless you override it.

*Debugging auto-configuration:* run with ⁠ --debug ⁠ flag to print an auto-configuration report showing which configs were applied/excluded and why.

---

## 3. Starter Dependencies

Starters are curated dependency bundles that pull in everything needed for a feature, with compatible versions.

| Starter | Provides |
|---|---|
| ⁠ spring-boot-starter-web ⁠ | Spring MVC, embedded Tomcat, Jackson (JSON) |
| ⁠ spring-boot-starter-data-jpa ⁠ | Spring Data JPA, Hibernate, JDBC |
| ⁠ spring-boot-starter-security ⁠ | Spring Security |
| ⁠ spring-boot-starter-test ⁠ | JUnit 5, Mockito, AssertJ, Spring Test |
| ⁠ spring-boot-starter-validation ⁠ | Bean Validation (Hibernate Validator) |
| ⁠ spring-boot-starter-actuator ⁠ | Production monitoring endpoints (health, metrics) |

*Why this matters:* Without starters, you'd need to manually resolve dozens of compatible library versions — starters guarantee a tested, working combination.

---

## 4. Configuration: application.yml / application.properties

⁠ yaml
server:
  port: 8081

spring:
  datasource:
    url: jdbc:mysql://localhost:3306/mydb
    username: root
    password: secret
  jpa:
    hibernate:
      ddl-auto: update
    show-sql: true

app:
  mail:
    host: smtp.example.com
 ⁠

⁠ java
@Value("${server.port}")
private int port;

// or type-safe binding
@ConfigurationProperties(prefix = "app.mail")
public class MailProperties {
    private String host;
}
 ⁠

*Profile-specific configs:* ⁠ application-dev.yml ⁠, ⁠ application-prod.yml ⁠ — activated via ⁠ spring.profiles.active=dev ⁠.

*Best practice:* Use YAML over ⁠ .properties ⁠ for nested/hierarchical config — it's more readable; use ⁠ @ConfigurationProperties ⁠ over scattered ⁠ @Value ⁠ for related settings.

---

## 5. Building a REST API

⁠ java
// Entity
@Entity
public class Product {
    @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String name;
    private double price;
    // getters/setters
}

// DTO — never expose entities directly over the wire
public record ProductDto(Long id, String name, double price) {}

// Repository
public interface ProductRepository extends JpaRepository<Product, Long> {}

// Service
@Service
public class ProductService {
    private final ProductRepository repo;
    public ProductService(ProductRepository repo) { this.repo = repo; }

    public ProductDto findById(Long id) {
        Product p = repo.findById(id)
            .orElseThrow(() -> new ProductNotFoundException(id));
        return new ProductDto(p.getId(), p.getName(), p.getPrice());
    }
}

// Controller
@RestController
@RequestMapping("/api/products")
public class ProductController {
    private final ProductService service;
    public ProductController(ProductService service) { this.service = service; }

    @GetMapping("/{id}")
    public ResponseEntity<ProductDto> getProduct(@PathVariable Long id) {
        return ResponseEntity.ok(service.findById(id));
    }

    @PostMapping
    public ResponseEntity<ProductDto> create(@Valid @RequestBody CreateProductRequest req) {
        ProductDto created = service.create(req);
        return ResponseEntity.status(HttpStatus.CREATED).body(created);
    }
}
 ⁠

*Why DTOs matter:* Exposing JPA entities directly risks leaking internal fields, causes lazy-loading serialization errors (⁠ LazyInitializationException ⁠), and tightly couples your API contract to your database schema.

---

## 6. Validation

⁠ java
public record CreateProductRequest(
    @NotBlank(message = "Name is required") String name,
    @Positive(message = "Price must be positive") double price
) {}

@PostMapping
public ResponseEntity<?> create(@Valid @RequestBody CreateProductRequest req) {
    // if validation fails, MethodArgumentNotValidException is thrown automatically
}
 ⁠
Common annotations: ⁠ @NotNull ⁠, ⁠ @NotBlank ⁠, ⁠ @NotEmpty ⁠, ⁠ @Size ⁠, ⁠ @Min ⁠/⁠ @Max ⁠, ⁠ @Email ⁠, ⁠ @Pattern ⁠, ⁠ @Positive ⁠.

---

## 7. Global Exception Handling

⁠ java
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(ProductNotFoundException.class)
    public ResponseEntity<ErrorResponse> handleNotFound(ProductNotFoundException ex) {
        return ResponseEntity.status(HttpStatus.NOT_FOUND)
            .body(new ErrorResponse(ex.getMessage()));
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<Map<String, String>> handleValidation(MethodArgumentNotValidException ex) {
        Map<String, String> errors = new HashMap<>();
        ex.getBindingResult().getFieldErrors()
          .forEach(e -> errors.put(e.getField(), e.getDefaultMessage()));
        return ResponseEntity.badRequest().body(errors);
    }
}
 ⁠

*Why centralize this:* Without ⁠ @RestControllerAdvice ⁠, every controller must catch exceptions individually, leading to duplicated, inconsistent error responses across the API.

---

## 8. Logging

⁠ java
@Slf4j // Lombok — generates a `log` field
@Service
public class ProductService {
    public void process() {
        log.info("Processing started");
        log.error("Failed to process order {}", orderId, exception);
    }
}
 ⁠
Spring Boot uses Logback by default. Configure levels in ⁠ application.yml ⁠:
⁠ yaml
logging:
  level:
    root: INFO
    com.example.app: DEBUG
 ⁠
*Best practice:* Never log sensitive data (passwords, tokens); use parameterized logging (⁠ log.info("User {}", id) ⁠) instead of string concatenation for performance.

---

## 9. OpenAPI / Swagger Documentation

⁠ xml
<dependency>
    <groupId>org.springdoc</groupId>
    <artifactId>springdoc-openapi-starter-webmvc-ui</artifactId>
</dependency>
 ⁠
Auto-generates interactive API docs at ⁠ /swagger-ui.html ⁠, reading ⁠ @Operation ⁠, ⁠ @Parameter ⁠, and schema annotations from your controllers.

---

## 10. Spring Security Basics

⁠ java
@Configuration
@EnableWebSecurity
public class SecurityConfig {

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            .csrf(csrf -> csrf.disable())
            .authorizeHttpRequests(auth -> auth
                .requestMatchers("/api/public/**").permitAll()
                .requestMatchers("/api/admin/**").hasRole("ADMIN")
                .anyRequest().authenticated())
            .sessionManagement(sm -> sm.sessionCreationPolicy(SessionCreationPolicy.STATELESS))
            .addFilterBefore(jwtAuthFilter, UsernamePasswordAuthenticationFilter.class);
        return http.build();
    }

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }
}
 ⁠

### JWT Authentication Flow
1.⁠ ⁠Client sends credentials to ⁠ /login ⁠.
2.⁠ ⁠Server validates, generates a signed JWT (containing user identity + expiry) and returns it.
3.⁠ ⁠Client sends the JWT in the ⁠ Authorization: Bearer <token> ⁠ header on subsequent requests.
4.⁠ ⁠A custom ⁠ OncePerRequestFilter ⁠ intercepts each request, validates the JWT signature/expiry, and sets the ⁠ SecurityContext ⁠ if valid.

⁠ java
public class JwtAuthFilter extends OncePerRequestFilter {
    protected void doFilterInternal(HttpServletRequest req, HttpServletResponse res, FilterChain chain) {
        String token = extractToken(req);
        if (token != null && jwtUtil.validateToken(token)) {
            Authentication auth = jwtUtil.getAuthentication(token);
            SecurityContextHolder.getContext().setAuthentication(auth);
        }
        chain.doFilter(req, res);
    }
}
 ⁠
*Why stateless sessions with JWT:* Enables horizontal scaling — no server-side session storage means any instance behind a load balancer can handle any request.

---

## 11. Caching

⁠ java
@EnableCaching
@Configuration
public class CacheConfig {}

@Service
public class ProductService {

    @Cacheable(value = "products", key = "#id")
    public ProductDto findById(Long id) { ... } // cached after first call

    @CacheEvict(value = "products", key = "#id")
    public void update(Long id, ...) { ... } // invalidates cache on update

    @CachePut(value = "products", key = "#result.id")
    public ProductDto create(...) { ... } // always executes, updates cache
}
 ⁠
*Why it matters:* Reduces redundant expensive operations (DB calls, external API calls) for frequently-requested, rarely-changing data.

---

## 12. Scheduling & Async

⁠ java
@EnableScheduling
@Configuration
public class SchedulingConfig {}

@Component
public class ReportJob {
    @Scheduled(cron = "0 0 1 * * *") // 1 AM daily
    public void generateDailyReport() { ... }

    @Scheduled(fixedRate = 60000) // every 60 seconds
    public void healthCheck() { ... }
}

@EnableAsync
@Service
public class EmailService {
    @Async
    public void sendEmail(String to) { ... } // runs on a separate thread pool
}
 ⁠

---

## 13. Testing in Spring Boot

⁠ java
// Unit test — no Spring context, fast
@ExtendWith(MockitoExtension.class)
class ProductServiceTest {
    @Mock private ProductRepository repo;
    @InjectMocks private ProductService service;

    @Test
    void shouldReturnProduct() {
        when(repo.findById(1L)).thenReturn(Optional.of(new Product(1L, "Phone", 500)));
        ProductDto result = service.findById(1L);
        assertEquals("Phone", result.name());
    }
}

// Integration test — loads full Spring context
@SpringBootTest
@AutoConfigureMockMvc
class ProductControllerIT {
    @Autowired private MockMvc mockMvc;

    @Test
    void shouldReturn200() throws Exception {
        mockMvc.perform(get("/api/products/1"))
               .andExpect(status().isOk())
               .andExpect(jsonPath("$.name").value("Phone"));
    }
}

// Repository test — only loads JPA layer, uses in-memory DB
@DataJpaTest
class ProductRepositoryTest {
    @Autowired private ProductRepository repo;

    @Test
    void shouldSaveProduct() {
        Product saved = repo.save(new Product(null, "Laptop", 1000));
        assertNotNull(saved.getId());
    }
}
 ⁠

*Why different test slices:* ⁠ @SpringBootTest ⁠ loads the entire context (slow, thorough); ⁠ @DataJpaTest ⁠/⁠ @WebMvcTest ⁠ load only relevant layers (fast, focused) — choosing the right slice keeps the test suite fast without sacrificing coverage.

---

## 14. Actuator (Production Monitoring)

⁠ yaml
management:
  endpoints:
    web:
      exposure:
        include: health, metrics, info
 ⁠
Exposes endpoints like ⁠ /actuator/health ⁠, ⁠ /actuator/metrics ⁠, ⁠ /actuator/env ⁠ — used by load balancers, Kubernetes liveness/readiness probes, and monitoring tools (Prometheus/Grafana).

---

## 15. Interview Questions — Spring Boot

1.⁠ ⁠*How does Spring Boot decide which beans to auto-configure?*
   Via conditional annotations (⁠ @ConditionalOnClass ⁠, ⁠ @ConditionalOnMissingBean ⁠, ⁠ @ConditionalOnProperty ⁠) evaluated against the classpath, existing beans, and configuration properties at startup.

2.⁠ ⁠*Difference between ⁠ @Component ⁠, ⁠ @Service ⁠, ⁠ @Repository ⁠, ⁠ @Controller ⁠?*
   Functionally similar (all register a bean), but ⁠ @Repository ⁠ additionally enables exception translation to Spring's ⁠ DataAccessException ⁠ hierarchy, and the others communicate architectural intent/layer for readability and tooling.

3.⁠ ⁠*Why use DTOs instead of returning JPA entities directly from a controller?*
   Prevents leaking internal persistence details, avoids ⁠ LazyInitializationException ⁠ on unfetched associations, and decouples the API contract from the database schema so either can evolve independently.

4.⁠ ⁠*What's the difference between ⁠ @SpringBootTest ⁠ and ⁠ @WebMvcTest ⁠?*
   ⁠ @SpringBootTest ⁠ boots the entire application context (all beans, real or test DB); ⁠ @WebMvcTest ⁠ loads only the web layer (controllers, filters) with mocked service dependencies — much faster for controller-focused tests.

5.⁠ ⁠*How does Spring Boot's embedded server work (e.g., Tomcat)?*
   Boot bundles the server as a library dependency rather than requiring external deployment — ⁠ SpringApplication.run() ⁠ starts an embedded Tomcat instance programmatically inside the same JVM as the application, producing a self-contained runnable JAR.

6.⁠ ⁠*What's the purpose of ⁠ spring-boot-starter-actuator ⁠?*
   Exposes production-ready operational endpoints (health checks, metrics, environment info) used for monitoring and orchestration (e.g., Kubernetes probes) without writing custom endpoints yourself.

## 16. Best Practices
•⁠  ⁠Never expose JPA entities directly in REST responses — always map to DTOs.
•⁠  ⁠Centralize exception handling with ⁠ @RestControllerAdvice ⁠.
•⁠  ⁠Use constructor injection; keep controllers thin — business logic belongs in the service layer.
•⁠  ⁠Externalize all environment-specific config (⁠ application-{profile}.yml ⁠); never hard-code secrets — use environment variables or a secrets manager.
•⁠  ⁠Enable Actuator health/metrics endpoints in every production service.
•⁠  ⁠Write a mix of unit tests (fast, mocked) and a smaller number of integration tests (slow, real context) — the "test pyramid".


# Part 10 – Spring Boot

## 1. What Problem Spring Boot Solves

Classic Spring required a lot of manual XML/Java configuration — DataSource setup, DispatcherServlet registration, dependency version alignment, embedded server setup. **Spring Boot** removes this ceremony via:

- **Auto Configuration** – sensible defaults, configured automatically based on classpath contents
- **Starter Dependencies** – curated dependency bundles with compatible versions
- **Embedded Servers** – Tomcat/Jetty/Undertow bundled in, no external WAR deployment needed
- **Opinionated defaults** – convention over configuration

---

## 2. Auto Configuration

Spring Boot scans the classpath at startup and **conditionally** registers beans.

Example: if `spring-boot-starter-data-jpa` and a JDBC driver are on the classpath, Spring Boot auto-configures a `DataSource`, `EntityManagerFactory`, and `TransactionManager` — you don't write that config yourself.

Mechanism: `@Conditional` annotations family —
- `@ConditionalOnClass` – bean created only if a certain class is present
- `@ConditionalOnMissingBean` – bean created only if the user hasn't defined their own
- `@ConditionalOnProperty` – bean created only if a property is set to a value

```java
@Configuration
@ConditionalOnClass(DataSource.class)
class DataSourceAutoConfiguration {
    @Bean
    @ConditionalOnMissingBean
    DataSource dataSource() { ... }
}
```

This is why you can override any auto-configured bean simply by defining your own bean of the same type — Spring Boot backs off.

The entry point: `@SpringBootApplication` = `@Configuration` + `@EnableAutoConfiguration` + `@ComponentScan` combined.

---

## 3. Starter Dependencies

Starters are dependency descriptors, e.g.:
- `spring-boot-starter-web` – MVC + embedded Tomcat + Jackson
- `spring-boot-starter-data-jpa` – Hibernate + Spring Data JPA + JDBC
- `spring-boot-starter-security` – Spring Security
- `spring-boot-starter-test` – JUnit, Mockito, AssertJ, Spring Test

**Why they matter:** they eliminate "dependency hell" — you don't manually pick compatible versions of 10 libraries; the starter's parent `spring-boot-starter-parent` (or BOM) manages versions consistently.

---

## 4. `application.yml` / `application.properties`

Central external configuration file.

```yaml
server:
  port: 8081

spring:
  datasource:
    url: jdbc:postgresql://localhost:5432/mydb
    username: postgres
    password: secret
  jpa:
    hibernate:
      ddl-auto: update
    show-sql: true

logging:
  level:
    root: INFO
    com.myapp: DEBUG
```

YAML is generally preferred over `.properties` for readability with nested structures. Profile-specific files: `application-dev.yml`, `application-prod.yml`, activated via `spring.profiles.active`.

---

## 5. Building a REST API

```java
@RestController
@RequestMapping("/api/products")
class ProductController {

    private final ProductService service;
    ProductController(ProductService service) { this.service = service; }

    @GetMapping
    List<ProductDto> getAll() { return service.findAll(); }

    @GetMapping("/{id}")
    ProductDto getById(@PathVariable Long id) { return service.findById(id); }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    ProductDto create(@Valid @RequestBody ProductDto dto) { return service.create(dto); }

    @PutMapping("/{id}")
    ProductDto update(@PathVariable Long id, @Valid @RequestBody ProductDto dto) {
        return service.update(id, dto);
    }

    @DeleteMapping("/{id}")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    void delete(@PathVariable Long id) { service.delete(id); }
}
```

Key annotations: `@RequestBody` (deserialize JSON into object), `@PathVariable`, `@RequestParam` (query params), `@ResponseStatus`.

---

## 6. DTO vs Entity

- **Entity** – maps directly to a database table (`@Entity`); represents persistence structure.
- **DTO (Data Transfer Object)** – plain object used to transfer data across layers/API boundaries.

**Why separate them:** exposing entities directly in your API leaks database structure, risks over-fetching sensitive fields, and creates tight coupling — a DB schema change breaks your API contract. DTOs let you control exactly what's exposed and evolve API and DB independently.

```java
@Entity
class Product {
    @Id @GeneratedValue Long id;
    String name;
    BigDecimal cost;       // internal field, never exposed
    BigDecimal sellingPrice;
}

record ProductDto(Long id, String name, BigDecimal price) {}
```

Mapping is usually done via MapStruct, ModelMapper, or manual mapper classes/methods.

---

## 7. Validation

Using JSR-380 (Bean Validation) annotations:

```java
record ProductDto(
    Long id,
    @NotBlank(message = "Name is required") String name,
    @Positive(message = "Price must be positive") BigDecimal price
) {}
```

Triggered via `@Valid` on the controller parameter. Common annotations: `@NotNull`, `@NotBlank`, `@NotEmpty`, `@Size`, `@Min`, `@Max`, `@Email`, `@Pattern`.

---

## 8. Global Exception Handling

```java
@RestControllerAdvice
class GlobalExceptionHandler {

    @ExceptionHandler(ResourceNotFoundException.class)
    ResponseEntity<ErrorResponse> handleNotFound(ResourceNotFoundException ex) {
        return ResponseEntity.status(HttpStatus.NOT_FOUND)
                .body(new ErrorResponse(ex.getMessage()));
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    ResponseEntity<Map<String, String>> handleValidation(MethodArgumentNotValidException ex) {
        Map<String, String> errors = new HashMap<>();
        ex.getBindingResult().getFieldErrors()
          .forEach(e -> errors.put(e.getField(), e.getDefaultMessage()));
        return ResponseEntity.badRequest().body(errors);
    }
}
```

**Why this matters:** centralizes error handling logic instead of try/catch in every controller method — consistent error response shape across the whole API.

---

## 9. Logging

Spring Boot uses **SLF4J** as the logging facade, with **Logback** as the default implementation.

```java
private static final Logger log = LoggerFactory.getLogger(ProductService.class);

log.info("Creating product: {}", dto.name());
log.error("Failed to save product", exception);
```

Best practice: use parameterized logging (`{}`) instead of string concatenation for performance — avoids building the string if the log level is disabled.

---

## 10. OpenAPI / Swagger

`springdoc-openapi` auto-generates interactive API docs from your controllers/DTOs at `/swagger-ui.html`.

```xml
<dependency>
    <groupId>org.springdoc</groupId>
    <artifactId>springdoc-openapi-starter-webmvc-ui</artifactId>
</dependency>
```

Annotate for richer docs: `@Operation`, `@ApiResponse`, `@Schema`.

---

## 11. Security Basics

`spring-boot-starter-security` secures all endpoints by default with basic auth. Typical custom setup:

```java
@Configuration
@EnableWebSecurity
class SecurityConfig {

    @Bean
    SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http.csrf(csrf -> csrf.disable())
            .authorizeHttpRequests(auth -> auth
                .requestMatchers("/api/public/**").permitAll()
                .anyRequest().authenticated())
            .sessionManagement(sm -> sm.sessionCreationPolicy(SessionCreationPolicy.STATELESS))
            .addFilterBefore(jwtFilter, UsernamePasswordAuthenticationFilter.class);
        return http.build();
    }

    @Bean
    PasswordEncoder passwordEncoder() { return new BCryptPasswordEncoder(); }
}
```

---

## 12. JWT (JSON Web Token) Authentication

Flow:
1. User logs in with credentials → server validates → issues a signed JWT
2. Client stores token, sends it in `Authorization: Bearer <token>` header on subsequent requests
3. A filter intercepts requests, validates the token signature/expiry, and sets the `SecurityContext`

**Why JWT over sessions:** stateless — no server-side session store needed, scales horizontally more easily, works well for microservices where multiple services need to verify identity independently.

```java
class JwtUtil {
    private final Key key = Keys.hmacShaKeyFor(secret.getBytes());

    String generateToken(String username) {
        return Jwts.builder()
                .setSubject(username)
                .setIssuedAt(new Date())
                .setExpiration(new Date(System.currentTimeMillis() + 3600_000))
                .signWith(key)
                .compact();
    }
}
```

---

## 13. Caching

```java
@EnableCaching
@Configuration
class CacheConfig {}

@Service
class ProductService {
    @Cacheable(value = "products", key = "#id")
    ProductDto findById(Long id) { ... }

    @CacheEvict(value = "products", key = "#id")
    void update(Long id, ProductDto dto) { ... }
}
```

Backed by simple in-memory maps by default, or Redis/EhCache in production via appropriate starter.

---

## 14. Scheduling

```java
@EnableScheduling
@Configuration
class SchedulerConfig {}

@Component
class ReportJob {
    @Scheduled(cron = "0 0 1 * * *") // 1 AM daily
    void generateDailyReport() { ... }

    @Scheduled(fixedRate = 60000) // every 60 seconds
    void healthCheck() { ... }
}
```

---

## 15. Async Processing

```java
@EnableAsync
@Configuration
class AsyncConfig {}

@Service
class NotificationService {
    @Async
    void sendEmail(String to) {
        // runs on a separate thread, doesn't block caller
    }
}
```

**Common mistake:** calling an `@Async` method from within the same class — Spring's proxy-based AOP won't intercept self-invocation, so the method runs synchronously. It must be called from a different bean.

---

## 16. Testing in Spring Boot

- `@SpringBootTest` – loads the full application context (integration tests)
- `@WebMvcTest` – loads only the web layer (controller tests, mocks service layer)
- `@DataJpaTest` – loads only JPA-related components (repository tests, uses in-memory DB)
- `@MockBean` – replaces a bean in the context with a Mockito mock

```java
@WebMvcTest(ProductController.class)
class ProductControllerTest {

    @Autowired MockMvc mockMvc;
    @MockBean ProductService service;

    @Test
    void shouldReturnProduct() throws Exception {
        when(service.findById(1L)).thenReturn(new ProductDto(1L, "Pen", BigDecimal.TEN));

        mockMvc.perform(get("/api/products/1"))
               .andExpect(status().isOk())
               .andExpect(jsonPath("$.name").value("Pen"));
    }
}
```

---

## Common Interview Questions – Part 10

1. How does Spring Boot auto-configuration actually work internally? (`spring.factories` / `AutoConfiguration.imports`, `@Conditional*` annotations)
2. Difference between `@SpringBootApplication` and `@EnableAutoConfiguration`.
3. How do you override an auto-configured bean?
4. Difference between `@RestController` and `@Controller`.
5. Why use DTOs instead of exposing entities directly?
6. Explain the JWT authentication flow end to end.
7. Why doesn't `@Async` work when called from within the same class?
8. Difference between `@WebMvcTest`, `@DataJpaTest`, and `@SpringBootTest`.