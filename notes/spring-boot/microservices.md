# Part 12 – Microservices

## 1. What Are Microservices (and Why)

A **microservices architecture** breaks a monolithic application into small, independently deployable services, each owning its own data and business capability (e.g., `OrderService`, `PaymentService`, `InventoryService`), communicating over the network (HTTP/REST, messaging).

**Why:** independent scaling, independent deployment (no "big bang" releases), fault isolation (one service failing doesn't necessarily bring down the whole system), technology flexibility per service.

**Trade-offs to know for interviews:** distributed system complexity, network latency, data consistency across services (no single ACID transaction spanning services), harder debugging/tracing, operational overhead (need service discovery, monitoring, centralized logging).

---

## 2. API Gateway

A single entry point that sits in front of all microservices, routing client requests to the appropriate backend service.

**Responsibilities:** routing, authentication/authorization, rate limiting, request/response transformation, load balancing, aggregating responses from multiple services.

Common implementations: **Spring Cloud Gateway**, Netflix Zuul (legacy), Kong, NGINX.

```yaml
# Spring Cloud Gateway route config
spring:
  cloud:
    gateway:
      routes:
        - id: order-service
          uri: lb://ORDER-SERVICE
          predicates:
            - Path=/api/orders/**
        - id: payment-service
          uri: lb://PAYMENT-SERVICE
          predicates:
            - Path=/api/payments/**
```

**Why it matters:** without a gateway, clients need to know every service's address individually, and cross-cutting concerns (auth, logging) must be duplicated in every service.

---

## 3. Service Discovery — Eureka

In a dynamic environment (containers scaling up/down, IPs changing), services can't hardcode each other's addresses. **Eureka** (Netflix, via Spring Cloud Netflix) is a **service registry**:

1. Each service instance **registers itself** with Eureka on startup (name + host + port)
2. Services **discover** each other by querying Eureka by logical name, not hardcoded IP
3. Eureka does periodic **heartbeat checks**; if a service stops responding, it's removed from the registry

```java
@EnableEurekaServer
@SpringBootApplication
class EurekaServerApplication {}
```

```java
@EnableEurekaClient
@SpringBootApplication
class OrderServiceApplication {}
```

```yaml
eureka:
  client:
    serviceUrl:
      defaultZone: http://localhost:8761/eureka/
```

Alternative modern approach: Kubernetes' built-in service discovery (DNS-based) often replaces Eureka in containerized deployments.

---

## 4. Config Server

Centralizes configuration for **all microservices** in one place (typically backed by a Git repo), instead of each service maintaining its own `application.yml`.

```java
@EnableConfigServer
@SpringBootApplication
class ConfigServerApplication {}
```

Each client service points to it:
```yaml
spring:
  config:
    import: "optional:configserver:http://localhost:8888"
  application:
    name: order-service
```

**Why:** consistent config across environments, ability to change config **without redeploying** services (combined with Spring Cloud Bus for refresh propagation), single source of truth for secrets/settings.

---

## 5. Feign Client (Declarative REST Client)

Lets you call another microservice as if it were a local Java interface method — no manual `RestTemplate`/`WebClient` boilerplate.

```java
@FeignClient(name = "inventory-service")
interface InventoryClient {
    @GetMapping("/api/inventory/{productId}")
    InventoryDto checkStock(@PathVariable Long productId);
}

@Service
class OrderService {
    private final InventoryClient inventoryClient;

    void placeOrder(Order order) {
        InventoryDto stock = inventoryClient.checkStock(order.getProductId());
        // ...
    }
}
```

Under the hood: integrates with service discovery (Eureka) to resolve `inventory-service` to an actual instance, and can be combined with load balancing and Resilience4j for fault tolerance.

---

## 6. Messaging: RabbitMQ vs Kafka

Both enable **asynchronous, decoupled communication** between services (instead of synchronous REST calls that couple services' uptime together).

| | RabbitMQ | Kafka |
|---|---|---|
| Model | Message broker (queue-based, push) | Distributed log (pull-based, partitioned) |
| Use case | Task queues, request/reply, complex routing | High-throughput event streaming, event sourcing, replay-able history |
| Message retention | Deleted after consumption (typically) | Retained for a configurable period regardless of consumption |
| Ordering | Per-queue | Per-partition |
| Throughput | Moderate | Very high |

**RabbitMQ example (Spring AMQP):**
```java
@RabbitListener(queues = "order.queue")
void handleOrder(OrderEvent event) { ... }

rabbitTemplate.convertAndSend("order.exchange", "order.created", event);
```

**Kafka example (Spring Kafka):**
```java
@KafkaListener(topics = "order-events", groupId = "inventory-group")
void consume(OrderEvent event) { ... }

kafkaTemplate.send("order-events", event);
```

**When to use which (common interview answer):** Kafka for high-volume event streaming and when consumers need replay/audit capability; RabbitMQ for classic task-queue patterns and complex routing logic (exchanges: direct, topic, fanout, headers).

---

## 7. Docker

Packages an application with all its dependencies into a **container image** — runs identically across dev, test, and prod environments.

```dockerfile
FROM eclipse-temurin:21-jre
COPY target/order-service.jar app.jar
ENTRYPOINT ["java", "-jar", "/app.jar"]
```

```bash
docker build -t order-service:1.0 .
docker run -p 8080:8080 order-service:1.0
```

**Why it matters for microservices:** each service can be built, versioned, and deployed independently as a container image — solves the classic "works on my machine" problem.

---

## 8. Kubernetes

Orchestrates containers at scale — handles deployment, scaling, self-healing (restarting failed containers), load balancing, and rolling updates across a cluster of machines.

Key concepts:
- **Pod** – smallest deployable unit, usually one container (or tightly coupled containers)
- **Deployment** – manages desired state (e.g., "keep 3 replicas of order-service running")
- **Service** – stable network endpoint/load balancer in front of a set of pods
- **ConfigMap / Secret** – externalized configuration and sensitive values

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: order-service
spec:
  replicas: 3
  selector:
    matchLabels:
      app: order-service
  template:
    metadata:
      labels:
        app: order-service
    spec:
      containers:
        - name: order-service
          image: order-service:1.0
          ports:
            - containerPort: 8080
```

**Why Kubernetes over plain Docker:** Docker runs containers; Kubernetes manages **fleets** of containers — auto-restart on crash, horizontal scaling based on load, zero-downtime rolling deployments, service discovery built in.

---

## 9. Resilience4j

A fault-tolerance library for handling failures gracefully in distributed calls — replaces the now-deprecated Netflix Hystrix.

Core modules: **Circuit Breaker**, **Rate Limiter**, **Retry**, **Bulkhead**, **Time Limiter**.

```java
@CircuitBreaker(name = "inventoryService", fallbackMethod = "fallbackStock")
@Retry(name = "inventoryService")
InventoryDto checkStock(Long productId) {
    return inventoryClient.checkStock(productId);
}

InventoryDto fallbackStock(Long productId, Throwable t) {
    return new InventoryDto(productId, 0, "UNAVAILABLE");
}
```

---

## 10. Circuit Breaker Pattern

Prevents a failing downstream service from **cascading failure** through the whole system by "tripping" after repeated failures and short-circuiting further calls temporarily.

Three states:
- **Closed** – normal operation, calls go through, failures are counted
- **Open** – failure threshold exceeded; calls **fail fast** (don't even try to reach the downstream service) for a configured wait period, and the fallback is invoked instead
- **Half-Open** – after the wait period, a limited number of trial calls are allowed through to check if the downstream service has recovered; success → back to Closed, failure → back to Open

**Real-world analogy:** an electrical circuit breaker trips to prevent an overload from damaging the whole system, and can be reset once the fault is cleared.

```yaml
resilience4j:
  circuitbreaker:
    instances:
      inventoryService:
        failureRateThreshold: 50
        waitDurationInOpenState: 10s
        slidingWindowSize: 10
```

**Why it's essential in microservices:** without it, if `InventoryService` becomes slow/unresponsive, every thread in `OrderService` calling it could block waiting for a timeout, exhausting the thread pool and taking down `OrderService` too — a single point of failure cascading outward.

---

## Common Interview Questions – Part 12

1. What problem does an API Gateway solve that a load balancer alone doesn't?
2. Explain how Eureka enables service discovery, step by step.
3. Kafka vs RabbitMQ — when would you pick one over the other?
4. Explain the three states of a circuit breaker.
5. Difference between Docker and Kubernetes.
6. How does Resilience4j's Retry differ from Circuit Breaker — could you use both together, and why?
7. What is the "distributed transaction" problem in microservices, and what patterns address it (e.g., Saga pattern)?
8. Why is Config Server important for microservices specifically, more than for a monolith?