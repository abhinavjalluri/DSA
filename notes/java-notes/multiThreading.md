# Part 6 – Multithreading & Concurrency

## 1. Thread Basics

### What is a Thread?
A thread is the smallest unit of execution within a process. A Java program starts with one thread — the *main thread* — and can spawn more.

*Why it exists:* To allow concurrent execution — better CPU utilization, responsiveness (e.g., UI doesn't freeze), and parallel processing of independent tasks.

### Creating Threads

*Way 1: Extend Thread*
```java
class MyThread extends Thread {
    public void run() {
        System.out.println("Running in: " + Thread.currentThread().getName());
    }
}
MyThread t = new MyThread();
t.start(); // NOT t.run() — start() creates a new OS thread; run() just calls the method normally
```

*Way 2: Implement Runnable (preferred — allows extending other classes)*
```java
class MyTask implements Runnable {
    public void run() {
        System.out.println("Task running");
    }
}
Thread t = new Thread(new MyTask());
t.start();
```

*Way 3: Lambda (Java 8+, since Runnable is a functional interface)*
```java
Thread t = new Thread(() -> System.out.println("Lambda thread"));
t.start();
```

### Extra Example: Why `run()` vs `start()` Actually Matters
```java
public class RunVsStartDemo {
    public static void main(String[] args) {
        Runnable task = () -> System.out.println("Executed by: " + Thread.currentThread().getName());

        Thread t = new Thread(task);
        t.run();    // prints "Executed by: main" — no new thread created!
        t.start();  // prints "Executed by: Thread-0" — actual new thread
    }
}
```
This is a classic interview trap — calling `run()` silently "works" (no compile error, no exception) but defeats the entire purpose of threading, which is why it's dangerous rather than obviously wrong.

### Thread Lifecycle

NEW → RUNNABLE → RUNNING → (BLOCKED / WAITING / TIMED_WAITING) → TERMINATED

- **NEW** – thread created, `start()` not yet called
- **RUNNABLE** – eligible to run, waiting for CPU
- **BLOCKED** – waiting to acquire a lock
- **WAITING / TIMED_WAITING** – waiting indefinitely or for a timeout (`wait()`, `join()`, `sleep()`)
- **TERMINATED** – run() completed

### Extra Example: Observing Thread States
```java
public class ThreadStateDemo {
    public static void main(String[] args) throws InterruptedException {
        Thread worker = new Thread(() -> {
            try { Thread.sleep(2000); } catch (InterruptedException e) {}
        });

        System.out.println(worker.getState()); // NEW
        worker.start();
        Thread.sleep(100);
        System.out.println(worker.getState()); // TIMED_WAITING (inside sleep)
        worker.join();
        System.out.println(worker.getState()); // TERMINATED
    }
}
```

### Common Mistakes
- Calling `run()` instead of `start()` (runs on the current thread, no concurrency).
- Forgetting `join()` when the main thread needs to wait for a worker thread to finish.
- Assuming thread execution order — the JVM/OS scheduler decides order; never rely on it.

### Extra Example: Why `join()` Matters
```java
public class JoinDemo {
    public static void main(String[] args) throws InterruptedException {
        Thread downloader = new Thread(() -> {
            System.out.println("Downloading file...");
            try { Thread.sleep(1000); } catch (InterruptedException e) {}
            System.out.println("Download complete");
        });

        downloader.start();
        // Without join(), this line can print BEFORE "Download complete"
        downloader.join(); // main thread waits here until downloader finishes
        System.out.println("Processing downloaded file"); // guaranteed to run after
    }
}
```

---

## 2. Runnable vs Callable

| Feature | Runnable | Callable<V> |
|---|---|---|
| Return value | No (`void run()`) | Yes (`V call()`) |
| Checked exceptions | Cannot throw | Can throw |
| Introduced | Java 1.0 | Java 5 |
| Used with | Thread, ExecutorService | ExecutorService (via `submit`) |

```java
Callable<Integer> task = () -> {
    Thread.sleep(1000);
    return 42;
};
ExecutorService executor = Executors.newSingleThreadExecutor();
Future<Integer> future = executor.submit(task);
Integer result = future.get(); // blocks until result is ready
executor.shutdown();
```

*When to use Callable:* When the task computes a result or may throw a checked exception (e.g., a computation that reads a file).

### Extra Example: Handling Exceptions from a Callable
```java
Callable<Integer> risky = () -> {
    if (Math.random() > 0.5) throw new IOException("Simulated failure");
    return 100;
};

Future<Integer> future = executor.submit(risky);
try {
    Integer result = future.get(); // exception is wrapped and re-thrown here
} catch (ExecutionException e) {
    System.out.println("Task failed: " + e.getCause().getMessage()); // real exception is the cause
} catch (InterruptedException e) {
    Thread.currentThread().interrupt(); // restore interrupt status — a common best practice
}
```
**Why this matters:** with `Runnable`, an uncaught exception is swallowed or sent to the thread's default handler silently; with `Callable` + `Future`, exceptions are captured and surfaced when you call `get()` — much easier to detect failures.

---

## 3. ExecutorService — The Thread Pool Framework

Manually creating threads for every task is expensive and hard to manage. `ExecutorService` manages a pool of reusable threads.

### Why it exists
Creating an OS thread is costly (memory + context switching). A pool reuses threads across many short tasks, controls concurrency limits, and provides lifecycle management (shutdown, task queuing).

### Common Executor Types
```java
ExecutorService fixed = Executors.newFixedThreadPool(4);
ExecutorService cached = Executors.newCachedThreadPool();       // grows/shrinks dynamically
ExecutorService single = Executors.newSingleThreadExecutor();   // sequential execution guaranteed
ExecutorService scheduled = Executors.newScheduledThreadPool(2); // delayed/periodic tasks
```

### Submitting Tasks
```java
executor.execute(() -> System.out.println("fire and forget"));      // Runnable, no result
Future<String> f = executor.submit(() -> "result");                 // returns a Future

executor.shutdown();          // graceful — finishes queued tasks, rejects new ones
executor.shutdownNow();       // attempts to interrupt running tasks
executor.awaitTermination(5, TimeUnit.SECONDS);
```

### Extra Example: Processing a Batch of Tasks and Collecting Results
```java
ExecutorService executor = Executors.newFixedThreadPool(4);
List<Callable<Integer>> tasks = List.of(
    () -> squareSlowly(2),
    () -> squareSlowly(3),
    () -> squareSlowly(4)
);

List<Future<Integer>> futures = executor.invokeAll(tasks); // runs all, blocks until all complete
int total = 0;
for (Future<Integer> f : futures) {
    total += f.get();
}
System.out.println("Total: " + total);
executor.shutdown();
```
`invokeAll()` is a convenient way to fan out multiple independent tasks and gather every result, without manually looping over `submit()` calls.

### Extra Example: Scheduled Tasks
```java
ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);

// Run once, after a 5-second delay
scheduler.schedule(() -> System.out.println("Delayed task"), 5, TimeUnit.SECONDS);

// Run repeatedly every 10 seconds, starting immediately
scheduler.scheduleAtFixedRate(() -> System.out.println("Heartbeat"), 0, 10, TimeUnit.SECONDS);
```
**Real-world use:** health-check pings, cache eviction sweeps, retry-with-backoff jobs.

### Best Practices
- Prefer `Executors.newFixedThreadPool` for CPU-bound predictable work; **avoid `newCachedThreadPool` in production** without bounds — it can create unbounded threads under load.
- In production, prefer building a `ThreadPoolExecutor` explicitly so you control queue size and rejection policy:
```java
new ThreadPoolExecutor(
    4, 8, 60, TimeUnit.SECONDS,
    new LinkedBlockingQueue<>(100),
    new ThreadPoolExecutor.CallerRunsPolicy()
);
```
- Always call `shutdown()` — leaking executors leaks threads.

### Extra Example: Sizing a Thread Pool Correctly
A common interview/practical question: *"How many threads should my pool have?"*

```java
// CPU-bound work (e.g., heavy computation, image processing):
int cpuBoundSize = Runtime.getRuntime().availableProcessors();

// I/O-bound work (e.g., DB calls, HTTP calls) — threads spend most time waiting, not computing:
// A common formula: threads = cores * (1 + waitTime/computeTime)
int ioBoundSize = Runtime.getRuntime().availableProcessors() * 4; // rough starting estimate
```
**Why it matters:** too few threads under-utilizes I/O-bound workloads (threads sit idle waiting on the network while requests queue up); too many threads for CPU-bound work just causes excessive context switching without more actual throughput.

---

## 4. CompletableFuture (Java 8+)

Solves the problem of chaining async computations without blocking, and combining multiple async results.

```java
CompletableFuture<String> future = CompletableFuture
    .supplyAsync(() -> fetchUser())
    .thenApply(user -> user.getName())
    .thenApply(String::toUpperCase)
    .exceptionally(ex -> "DEFAULT");

future.thenAccept(System.out::println);

// Combining two independent futures
CompletableFuture<Integer> a = CompletableFuture.supplyAsync(() -> 10);
CompletableFuture<Integer> b = CompletableFuture.supplyAsync(() -> 20);
CompletableFuture<Integer> sum = a.thenCombine(b, Integer::sum);

// Waiting for multiple futures
CompletableFuture.allOf(a, b).join();
```

*Real-world use:* In Spring Boot, calling three microservices in parallel and combining results before responding to a client — instead of calling them sequentially and adding up latencies.

### Extra Example: Parallel Microservice Calls (the real-world use case, in full)
```java
CompletableFuture<UserDto> userFuture = CompletableFuture.supplyAsync(() -> userClient.getUser(id));
CompletableFuture<List<OrderDto>> ordersFuture = CompletableFuture.supplyAsync(() -> orderClient.getOrders(id));
CompletableFuture<CreditScoreDto> creditFuture = CompletableFuture.supplyAsync(() -> creditClient.getScore(id));

CompletableFuture<CustomerProfile> combined = CompletableFuture.allOf(userFuture, ordersFuture, creditFuture)
    .thenApply(v -> new CustomerProfile(
        userFuture.join(),
        ordersFuture.join(),
        creditFuture.join()
    ));

CustomerProfile profile = combined.get(5, TimeUnit.SECONDS); // overall timeout for the whole fan-out
```
**Why this beats sequential calls:** if each call takes 200ms and you make them sequentially, the total is 600ms; run in parallel via `CompletableFuture`, the total is roughly 200ms (the slowest one), because all three fire at once.

### Extra Example: Error Handling with `handle()` vs `exceptionally()`
```java
CompletableFuture<Integer> future = CompletableFuture
    .supplyAsync(() -> { throw new RuntimeException("boom"); })
    .handle((result, ex) -> {
        if (ex != null) {
            System.out.println("Recovered from: " + ex.getMessage());
            return -1;
        }
        return result;
    });
```
`exceptionally()` only handles the failure case and must return the same type; `handle()` receives **both** the result and the exception (one will be null), giving you a single place to branch on success/failure — useful when you want unified post-processing regardless of outcome.

---

## 5. Synchronization

### The Problem: Race Condition
Two threads modifying shared state at the same time produce incorrect results because operations like `count++` are *not atomic* (they're read-modify-write).

```java
class Counter {
    private int count = 0;
    public void increment() { count++; } // NOT thread-safe
}
```

### Extra Example: Proving the Race Condition Exists
```java
public class RaceConditionDemo {
    private static int counter = 0;

    public static void main(String[] args) throws InterruptedException {
        Runnable increment = () -> {
            for (int i = 0; i < 100_000; i++) counter++;
        };

        Thread t1 = new Thread(increment);
        Thread t2 = new Thread(increment);
        t1.start(); t2.start();
        t1.join(); t2.join();

        System.out.println("Final count: " + counter);
        // Expected 200000, but actual output varies (e.g., 187342) — proof of the race condition
    }
}
```

### Fix 1: synchronized keyword
```java
class Counter {
    private int count = 0;
    public synchronized void increment() { count++; } // method-level lock (on 'this')
}

// Or block-level, locking on a specific object (finer control)
public void increment() {
    synchronized (this) {
        count++;
    }
}
```

*How it works internally:* Every object has an intrinsic monitor lock. `synchronized` makes a thread acquire the lock before entering, and release it on exit (even if an exception is thrown).

### Fix 2: Explicit Locks (java.util.concurrent.locks)
```java
private final ReentrantLock lock = new ReentrantLock();

public void increment() {
    lock.lock();
    try {
        count++;
    } finally {
        lock.unlock(); // MUST be in finally
    }
}
```

*Why prefer Lock over synchronized sometimes:*
- `tryLock()` — attempt to acquire without blocking forever
- Fairness policies (`new ReentrantLock(true)`)
- `ReadWriteLock` — allow multiple concurrent readers, exclusive writer

```java
ReadWriteLock rwLock = new ReentrantReadWriteLock();
rwLock.readLock().lock();   // many threads can hold this simultaneously
rwLock.writeLock().lock();  // exclusive
```

### Extra Example: `tryLock()` to Avoid Blocking Forever
```java
ReentrantLock lock = new ReentrantLock();

public boolean tryUpdateInventory() {
    try {
        if (lock.tryLock(500, TimeUnit.MILLISECONDS)) {
            try {
                // update shared inventory state
                return true;
            } finally {
                lock.unlock();
            }
        } else {
            System.out.println("Could not acquire lock — system busy, skipping this attempt");
            return false;
        }
    } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
        return false;
    }
}
```
**Real-world use:** a checkout system that shouldn't let a customer hang indefinitely if inventory is locked by another transaction — fail fast and show "please try again" instead.

### Extra Example: `ReadWriteLock` for a Cache
```java
class SimpleCache<K, V> {
    private final Map<K, V> map = new HashMap<>();
    private final ReadWriteLock lock = new ReentrantReadWriteLock();

    public V get(K key) {
        lock.readLock().lock();
        try {
            return map.get(key); // many threads can read concurrently
        } finally {
            lock.readLock().unlock();
        }
    }

    public void put(K key, V value) {
        lock.writeLock().lock();
        try {
            map.put(key, value); // exclusive — blocks readers and other writers
        } finally {
            lock.writeLock().unlock();
        }
    }
}
```
**Why this beats plain `synchronized`:** a cache is read far more often than written; `synchronized` would serialize even simple reads, while `ReadWriteLock` lets unlimited readers proceed concurrently and only blocks during the rare write.

### Fix 3: Atomic Classes (lock-free, CAS-based)
```java
AtomicInteger count = new AtomicInteger(0);
count.incrementAndGet();      // atomic, no lock needed
count.compareAndSet(5, 10);   // CAS: Compare-And-Swap
```

*Internal working:* Uses CPU-level Compare-And-Swap instructions instead of OS locks — much faster under low-to-moderate contention since there's no context switching.

### Extra Example: Fixing the Earlier Race Condition with AtomicInteger
```java
public class AtomicCounterDemo {
    private static AtomicInteger counter = new AtomicInteger(0);

    public static void main(String[] args) throws InterruptedException {
        Runnable increment = () -> {
            for (int i = 0; i < 100_000; i++) counter.incrementAndGet();
        };

        Thread t1 = new Thread(increment);
        Thread t2 = new Thread(increment);
        t1.start(); t2.start();
        t1.join(); t2.join();

        System.out.println("Final count: " + counter.get()); // always exactly 200000
    }
}
```

### volatile keyword
```java
private volatile boolean running = true;
```
Guarantees *visibility* (changes by one thread are immediately visible to others) but *not atomicity*. `volatile int x; x++;` is still unsafe — use `AtomicInteger` for that.

*When to use volatile:* Simple flags read/written by multiple threads without compound operations (e.g., a "stop" flag for a worker loop).

### Extra Example: `volatile` Flag Controlling a Worker Loop
```java
class Worker implements Runnable {
    private volatile boolean running = true;

    public void run() {
        while (running) {
            // do work
        }
        System.out.println("Worker stopped cleanly");
    }

    public void stop() { running = false; } // called from another thread
}
```
Without `volatile`, the JVM/CPU might cache `running` in a register for the worker thread, and the update from `stop()` (called on another thread) might never become visible — the loop runs forever even though `stop()` was called. This is a real, common bug in hand-rolled background workers.

---

## 6. Deadlock & Race Conditions

### Deadlock Example
```java
// Thread 1: locks A then B
// Thread 2: locks B then A
// → both wait forever
synchronized (lockA) {
    synchronized (lockB) { ... }
}
```

### Extra Example: A Runnable Deadlock Demo (Classic "Dining" Style)
```java
public class DeadlockDemo {
    private static final Object lockA = new Object();
    private static final Object lockB = new Object();

    public static void main(String[] args) {
        Thread t1 = new Thread(() -> {
            synchronized (lockA) {
                System.out.println("Thread 1: holding lockA...");
                sleep(100);
                synchronized (lockB) {
                    System.out.println("Thread 1: acquired lockB");
                }
            }
        });

        Thread t2 = new Thread(() -> {
            synchronized (lockB) {
                System.out.println("Thread 2: holding lockB...");
                sleep(100);
                synchronized (lockA) {
                    System.out.println("Thread 2: acquired lockA");
                }
            }
        });

        t1.start(); t2.start();
        // Program hangs forever — classic deadlock
    }

    private static void sleep(long ms) {
        try { Thread.sleep(ms); } catch (InterruptedException e) {}
    }
}
```

### Extra Example: The Fix — Consistent Lock Ordering
```java
// Both threads now acquire locks in the SAME order: A before B — deadlock impossible
Thread t1 = new Thread(() -> {
    synchronized (lockA) {
        synchronized (lockB) { /* work */ }
    }
});

Thread t2 = new Thread(() -> {
    synchronized (lockA) { // same order as t1, not B-then-A
        synchronized (lockB) { /* work */ }
    }
});
```
**Why this is the standard fix:** deadlock requires *circular wait*. If every thread in the system agrees to acquire shared locks in a single global order, a cycle can never form.

*Conditions for deadlock (all four must hold):*
1. Mutual exclusion
2. Hold and wait
3. No preemption
4. Circular wait

*Prevention:* Always acquire locks in the same global order across all threads; use `tryLock()` with timeout; avoid nested locks where possible.

### Race Condition
Occurs when the outcome depends on unpredictable timing of thread execution. Fixed via synchronization, atomics, or immutability.

---

## 7. Producer-Consumer with BlockingQueue

```java
BlockingQueue<Integer> queue = new LinkedBlockingQueue<>(10);

// Producer
new Thread(() -> {
    for (int i = 0; i < 100; i++) {
        try { queue.put(i); } catch (InterruptedException e) {}
    }
}).start();

// Consumer
new Thread(() -> {
    while (true) {
        try {
            Integer val = queue.take();
            System.out.println("Consumed: " + val);
        } catch (InterruptedException e) {}
    }
}).start();
```
`put()` blocks if the queue is full; `take()` blocks if it's empty — no manual `wait/notify` needed.

### Extra Example: Multiple Producers and Consumers (a Realistic Work Queue)
```java
BlockingQueue<String> jobQueue = new ArrayBlockingQueue<>(50);
ExecutorService producers = Executors.newFixedThreadPool(2);
ExecutorService consumers = Executors.newFixedThreadPool(3);

// 2 producers adding jobs
for (int i = 0; i < 2; i++) {
    producers.submit(() -> {
        for (int j = 0; j < 20; j++) {
            jobQueue.put("job-" + j);
        }
    });
}

// 3 consumers processing jobs concurrently
for (int i = 0; i < 3; i++) {
    consumers.submit(() -> {
        while (true) {
            String job = jobQueue.take();
            processJob(job);
        }
    });
}
```
**Real-world use:** this is exactly the internal pattern behind Spring's `@Async` thread pool executors, message queue consumers (RabbitMQ/Kafka listener containers), and image/video processing pipelines.

### Extra Example: Manual `wait()`/`notify()` (What BlockingQueue Replaces)
It's worth seeing the low-level mechanism `BlockingQueue` hides from you:
```java
class SimpleBuffer {
    private final Queue<Integer> queue = new LinkedList<>();
    private final int capacity = 5;

    public synchronized void produce(int value) throws InterruptedException {
        while (queue.size() == capacity) {
            wait(); // releases the lock and waits until notified
        }
        queue.add(value);
        notifyAll(); // wake up any waiting consumers
    }

    public synchronized int consume() throws InterruptedException {
        while (queue.isEmpty()) {
            wait();
        }
        int value = queue.poll();
        notifyAll(); // wake up any waiting producers
        return value;
    }
}
```
**Why `BlockingQueue` is preferred in real code:** this hand-rolled version is easy to get subtly wrong (e.g., using `if` instead of `while` around `wait()`, forgetting `notifyAll()`, missing `synchronized`). `BlockingQueue` implementations handle all of this correctly and are heavily tested — always prefer them over manual `wait/notify` in production code.

---

## 8. Virtual Threads (Java 21 — Project Loom)

Traditional threads map 1:1 to OS threads (expensive, limited to thousands). Virtual threads are lightweight, JVM-managed threads — millions can run concurrently.

```java
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    executor.submit(() -> {
        System.out.println("Running in: " + Thread.currentThread());
    });
}
```

*When to use:* I/O-bound workloads (e.g., a web server handling thousands of blocking DB/network calls) — each request gets its own virtual thread without the memory cost of platform threads.
*When NOT to use:* CPU-bound heavy computation — virtual threads don't add more CPU parallelism; use a fixed platform thread pool sized to CPU cores instead.

### Extra Example: Platform Threads vs Virtual Threads at Scale
```java
// Platform threads — this will likely crash or severely degrade performance at 100,000 threads
// due to each thread reserving ~1MB of stack memory by default
for (int i = 0; i < 100_000; i++) {
    new Thread(() -> simulateBlockingIO()).start(); // OutOfMemoryError territory
}

// Virtual threads — handles 100,000 concurrent tasks comfortably
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    for (int i = 0; i < 100_000; i++) {
        executor.submit(() -> simulateBlockingIO());
    }
}
```
**Why virtual threads scale so much better:** they don't consume a dedicated OS thread while blocked on I/O — the JVM "parks" the virtual thread and frees the underlying OS (carrier) thread to run other virtual threads in the meantime. This is transparent to your code — you write normal blocking-style code, but get async-level scalability.

---

## 9. Interview Questions — Multithreading

1. **Difference between `wait()` and `sleep()`?**
   `wait()` releases the monitor lock and must be called inside `synchronized`; `sleep()` does not release any lock and can be called anywhere.

2. **Why is `run()` different from `start()`?**
   `start()` creates a new thread and the JVM calls `run()` on it; calling `run()` directly just executes the method on the current thread — no concurrency.

3. **What is a daemon thread?**
   A background thread (e.g., GC) that doesn't prevent JVM shutdown. Set via `thread.setDaemon(true)` before `start()`.

4. **Difference between `HashMap` and `ConcurrentHashMap` under concurrency?**
   `HashMap` is not thread-safe (can corrupt internal structure or infinite-loop under concurrent modification); `ConcurrentHashMap` uses segment/bucket-level locking (Java 8+ uses CAS + synchronized on bins) so multiple threads can read/write safely with high throughput.

5. **What does `volatile` guarantee vs not guarantee?**
   Guarantees visibility and ordering (happens-before); does NOT guarantee atomicity for compound actions like `x++`.

6. **How does `ExecutorService.submit()` differ from `execute()`?**
   `submit()` returns a `Future` (works with `Callable` or `Runnable`) and captures exceptions inside the Future; `execute()` takes only `Runnable`, returns nothing, and uncaught exceptions go to the thread's uncaught exception handler.

7. **What is thread starvation?**
   Low-priority threads never get CPU time because higher-priority threads keep executing — different from deadlock (no circular wait, just scheduling unfairness).

8. **Why prefer `ConcurrentHashMap` over `Collections.synchronizedMap()`?**
   `synchronizedMap` locks the entire map for every operation (coarse-grained); `ConcurrentHashMap` allows much higher concurrency via finer-grained locking/CAS.

9. **Why does `while (queue.isEmpty()) wait();` use `while`, not `if`?**
   Spurious wakeups can occur (a thread may return from `wait()` without an actual `notify()`), and with multiple waiting consumers, another thread might have already consumed the item between notify and this thread waking up. Rechecking the condition in a loop guards against both.

10. **What's the practical benefit of virtual threads over increasing a platform thread pool size?**
    Platform threads are expensive (stack memory, OS scheduling overhead), capping realistic pool sizes in the low thousands. Virtual threads let you write simple blocking-style code that scales to handle massive I/O-bound concurrency (hundreds of thousands of concurrent tasks) without switching to a reactive/async programming model.

---

## 10. Best Practices Summary
- Prefer higher-level concurrency utilities (`ExecutorService`, `CompletableFuture`, `java.util.concurrent` collections) over raw `Thread`/`wait/notify`.
- Keep synchronized blocks as small as possible — lock only what needs protecting.
- Prefer immutability where possible — immutable objects are inherently thread-safe.
- Always release locks in a `finally` block.
- Never rely on thread execution order or priority for correctness.
- Use virtual threads for I/O-bound concurrency at scale (Java 21+); use fixed thread pools for CPU-bound work.
- When protecting a wait condition, always re-check it in a `while` loop, not `if`.
- Acquire multiple locks in a globally consistent order to prevent deadlock.