# Part 11 – Spring Data JPA

## 1. What JPA Is (and Isn't)

**JPA (Jakarta Persistence API)** is a **specification** — a set of interfaces defining how Java objects map to relational database tables (ORM). **Hibernate** is the most common **implementation** of that spec. **Spring Data JPA** sits on top of Hibernate/JPA and eliminates boilerplate repository code.

Layer stack: `Spring Data JPA` → `JPA (spec)` → `Hibernate (implementation)` → `JDBC` → `Database`

---

## 2. Entity Mapping

```java
@Entity
@Table(name = "products")
class Product {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(name = "product_name", nullable = false, length = 100)
    private String name;

    @Column(precision = 10, scale = 2)
    private BigDecimal price;

    @Enumerated(EnumType.STRING)
    private Category category;

    @Temporal(TemporalType.TIMESTAMP)
    private Date createdAt;
}
```

`GenerationType` strategies: `IDENTITY` (DB auto-increment), `SEQUENCE` (DB sequence object, preferred for Postgres/Oracle — allows batching), `AUTO` (JPA provider picks), `TABLE` (rarely used, simulates sequence via a table).

---

## 3. Relationships

### One-to-Many / Many-to-One

```java
@Entity
class Department {
    @Id @GeneratedValue Long id;

    @OneToMany(mappedBy = "department", cascade = CascadeType.ALL, orphanRemoval = true)
    private List<Employee> employees = new ArrayList<>();
}

@Entity
class Employee {
    @Id @GeneratedValue Long id;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "department_id")
    private Department department;
}
```

- `mappedBy` – indicates the **inverse (non-owning)** side; the owning side (with `@JoinColumn`) controls the foreign key.
- Always prefer mapping `@ManyToOne` as the **owning side** — it directly holds the FK column, avoiding an extra query/join table.

### Many-to-Many

```java
@Entity
class Student {
    @ManyToMany
    @JoinTable(
        name = "student_course",
        joinColumns = @JoinColumn(name = "student_id"),
        inverseJoinColumns = @JoinColumn(name = "course_id"))
    private Set<Course> courses = new HashSet<>();
}
```

### One-to-One

```java
@OneToOne(cascade = CascadeType.ALL)
@JoinColumn(name = "profile_id")
private UserProfile profile;
```

---

## 4. Fetch Types: LAZY vs EAGER

- **LAZY** – related data loaded only when accessed (proxy object returned initially). Default for `@OneToMany` and `@ManyToMany`.
- **EAGER** – related data loaded immediately with the parent. Default for `@ManyToOne` and `@OneToOne`.

**Why LAZY is generally preferred:** loading entire object graphs eagerly causes performance issues (fetching unnecessary data) and is a common source of the **N+1 query problem**.

**LazyInitializationException:** happens when you try to access a lazy-loaded collection **outside** an active Hibernate session (e.g., after the transaction/service method has returned, in the view layer). Fixes: fetch eagerly with a JOIN FETCH query when you know you'll need it, use DTO projections, or restructure to access lazy fields within the transactional boundary.

---

## 5. Cascade Types

Determines what operations propagate from parent to child entity:

| Type | Effect |
|---|---|
| `PERSIST` | Saving parent also saves child |
| `MERGE` | Updating parent also updates child |
| `REMOVE` | Deleting parent also deletes child |
| `REFRESH` | Refreshing parent also refreshes child |
| `DETACH` | Detaching parent also detaches child |
| `ALL` | All of the above |

`orphanRemoval = true` – additionally deletes a child entity when it's **removed from the parent's collection**, even if not explicitly deleted (used with `@OneToMany`/`@OneToOne`).

---

## 6. Transactions

```java
@Service
class OrderService {

    @Transactional
    void placeOrder(Order order) {
        inventoryService.reserveStock(order);
        paymentService.charge(order);
        orderRepository.save(order);
        // if any step throws a RuntimeException, everything rolls back
    }
}
```

**Key facts:**
- `@Transactional` uses **AOP proxies** — like `@Async`, self-invocation within the same class bypasses the proxy, so the transaction won't apply.
- By default, only **unchecked exceptions** (`RuntimeException`) trigger rollback. Checked exceptions do **not**, unless you specify `rollbackFor = Exception.class`.
- Propagation types: `REQUIRED` (default — join existing transaction or create new), `REQUIRES_NEW` (always starts a fresh transaction, suspending any existing one), `NESTED`, `MANDATORY`, `SUPPORTS`, `NOT_SUPPORTED`, `NEVER`.
- Isolation levels: `READ_UNCOMMITTED`, `READ_COMMITTED` (most common default), `REPEATABLE_READ`, `SERIALIZABLE`.

---

## 7. Query Methods (Derived Queries)

Spring Data JPA generates SQL from method names automatically:

```java
interface ProductRepository extends JpaRepository<Product, Long> {

    List<Product> findByCategory(Category category);
    List<Product> findByPriceGreaterThan(BigDecimal price);
    List<Product> findByNameContainingIgnoreCase(String keyword);
    Optional<Product> findByNameAndCategory(String name, Category category);
    List<Product> findTop5ByOrderByPriceDesc();
    long countByCategory(Category category);
    boolean existsByName(String name);
}
```

---

## 8. JPQL (Java Persistence Query Language)

Object-oriented query language — queries **entities and their fields**, not table/column names directly.

```java
@Query("SELECT p FROM Product p WHERE p.price > :minPrice AND p.category = :cat")
List<Product> findExpensiveProducts(@Param("minPrice") BigDecimal minPrice,
                                     @Param("cat") Category cat);

@Query("SELECT p FROM Product p JOIN FETCH p.reviews WHERE p.id = :id")
Optional<Product> findByIdWithReviews(@Param("id") Long id);
```

`JOIN FETCH` is the standard fix for **N+1 query problems** — it eagerly loads the association in a single query instead of triggering a separate query per row.

Native SQL is also supported: `@Query(value = "SELECT * FROM products WHERE price > ?1", nativeQuery = true)`.

---

## 9. Criteria API

Type-safe, programmatic way to build dynamic queries at runtime (useful when filter conditions vary based on user input).

```java
CriteriaBuilder cb = entityManager.getCriteriaBuilder();
CriteriaQuery<Product> query = cb.createQuery(Product.class);
Root<Product> root = query.from(Product.class);

List<Predicate> predicates = new ArrayList<>();
if (minPrice != null) predicates.add(cb.greaterThan(root.get("price"), minPrice));
if (category != null) predicates.add(cb.equal(root.get("category"), category));

query.where(predicates.toArray(new Predicate[0]));
List<Product> results = entityManager.createQuery(query).getResultList();
```

---

## 10. Specifications

Spring Data JPA's higher-level, reusable wrapper around Criteria API — composable filters.

```java
class ProductSpecifications {
    static Specification<Product> hasCategory(Category cat) {
        return (root, query, cb) -> cb.equal(root.get("category"), cat);
    }
    static Specification<Product> priceGreaterThan(BigDecimal price) {
        return (root, query, cb) -> cb.greaterThan(root.get("price"), price);
    }
}

interface ProductRepository extends JpaRepository<Product, Long>, JpaSpecificationExecutor<Product> {}

// usage
Specification<Product> spec = Specification.where(hasCategory(Category.BOOKS))
                                            .and(priceGreaterThan(BigDecimal.TEN));
List<Product> results = productRepository.findAll(spec);
```

---

## 11. Pagination and Sorting

```java
Pageable pageable = PageRequest.of(0, 20, Sort.by("price").descending());
Page<Product> page = productRepository.findAll(pageable);

page.getContent();       // list of items
page.getTotalElements(); // total count across all pages
page.getTotalPages();
```

Method-level derived query with paging: `Page<Product> findByCategory(Category cat, Pageable pageable);`

---

## 12. Optimistic Locking

Prevents lost updates in concurrent scenarios **without** locking rows in the database (unlike pessimistic locking).

```java
@Entity
class Product {
    @Id @GeneratedValue Long id;

    @Version
    private Long version;
}
```

How it works: each update checks that `version` in the DB still matches what was read. If another transaction updated the row in between, the version mismatch causes `OptimisticLockException` — the app can retry or notify the user of a conflict.

**Pessimistic locking** (alternative), locks the row at the DB level immediately:
```java
@Lock(LockModeType.PESSIMISTIC_WRITE)
@Query("SELECT p FROM Product p WHERE p.id = :id")
Product findByIdForUpdate(@Param("id") Long id);
```

---

## Common Interview Questions – Part 11

1. Difference between JPA, Hibernate, and Spring Data JPA.
2. What is the N+1 query problem and how do you fix it?
3. LAZY vs EAGER fetching — when would you choose each?
4. What causes `LazyInitializationException` and how do you avoid it?
5. Difference between `@OneToMany(mappedBy=...)` and using `@JoinColumn` directly.
6. Explain optimistic vs pessimistic locking with an example.
7. Why does `@Transactional` fail silently when called via self-invocation?
8. Difference between JPQL and native SQL queries — when would you use native?
9. What's the difference between `save()` and `saveAndFlush()`?