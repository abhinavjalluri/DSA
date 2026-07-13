# Part 7 – File Handling, NIO, Serialization

## 1. java.io.File

⁠ File ⁠ represents a path on the filesystem — it does NOT read/write content itself, only metadata and path operations.

⁠ java
File file = new File("data.txt");
file.exists();       // does it exist?
file.createNewFile(); // create if absent
file.delete();
file.isDirectory();
file.length();        // size in bytes
file.getAbsolutePath();
 ⁠

### Reading/Writing with Streams (classic I/O)
⁠ java
// Text — character streams
try (BufferedReader reader = new BufferedReader(new FileReader("data.txt"))) {
    String line;
    while ((line = reader.readLine()) != null) {
        System.out.println(line);
    }
}

try (BufferedWriter writer = new BufferedWriter(new FileWriter("out.txt"))) {
    writer.write("Hello World");
}

// Binary — byte streams
try (FileInputStream in = new FileInputStream("image.png");
     FileOutputStream out = new FileOutputStream("copy.png")) {
    byte[] buffer = new byte[4096];
    int len;
    while ((len = in.read(buffer)) != -1) {
        out.write(buffer, 0, len);
    }
}
 ⁠

*Why ⁠ try-with-resources ⁠:* Streams implement ⁠ AutoCloseable ⁠; this guarantees ⁠ close() ⁠ is called even if an exception occurs, preventing file-handle/resource leaks.

*Common mistake:* Using ⁠ FileReader ⁠ for binary files (corrupts data due to character encoding conversion) — always use byte streams (⁠ FileInputStream ⁠/⁠ FileOutputStream ⁠) for binary data.

---

## 2. Java NIO (New I/O) — java.nio.file

Introduced in Java 7, NIO.2 provides a modern, more powerful file API centered around ⁠ Path ⁠ and ⁠ Files ⁠.

### Why NIO exists
Classic ⁠ File ⁠ API had poor error handling (returned ⁠ false ⁠ instead of throwing exceptions), no symbolic link support, and no efficient directory-walking. NIO fixes all of this and adds non-blocking I/O via channels/buffers.

⁠ java
Path path = Paths.get("data.txt");

// Reading
List<String> lines = Files.readAllLines(path);
String content = Files.readString(path); // Java 11+

// Writing
Files.writeString(path, "Hello NIO", StandardOpenOption.CREATE);
Files.write(path, "data".getBytes());

// Copy / Move / Delete
Files.copy(Paths.get("a.txt"), Paths.get("b.txt"), StandardCopyOption.REPLACE_EXISTING);
Files.move(Paths.get("a.txt"), Paths.get("archive/a.txt"));
Files.deleteIfExists(path);

// Metadata
Files.exists(path);
Files.size(path);
Files.isDirectory(path);

// Walking a directory tree
try (Stream<Path> stream = Files.walk(Paths.get("."))) {
    stream.filter(Files::isRegularFile)
          .forEach(System.out::println);
}
 ⁠

### Channels & Buffers (low-level NIO)
⁠ java
try (FileChannel channel = FileChannel.open(path, StandardOpenOption.READ)) {
    ByteBuffer buffer = ByteBuffer.allocate(1024);
    int bytesRead = channel.read(buffer);
    buffer.flip(); // switch from writing to reading mode
    while (buffer.hasRemaining()) {
        System.out.print((char) buffer.get());
    }
}
 ⁠
*Used for:* high-performance file/network I/O (e.g., memory-mapped files, non-blocking socket servers).

### File vs NIO.2 Path — Quick Comparison

| Feature | java.io.File | java.nio.file.Path/Files |
|---|---|---|
| Error handling | Returns boolean/null | Throws descriptive exceptions |
| Symbolic links | Not supported | Fully supported |
| Directory streaming | Manual recursion | ⁠ Files.walk() ⁠, ⁠ Files.list() ⁠ |
| Performance | Slower for large ops | Optimized, supports async channels |

---

## 3. Serialization

Serialization converts an object's in-memory state into a byte stream (for storage or transfer); deserialization reconstructs it.

### Why it exists
To persist objects to disk, send them over a network, or cache them — without manually writing every field to a custom format.

⁠ java
class User implements Serializable {
    private static final long serialVersionUID = 1L;
    private String name;
    private transient String password; // excluded from serialization
    // constructor, getters...
}

// Serialize
try (ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream("user.ser"))) {
    out.writeObject(new User("Alice", "secret"));
}

// Deserialize
try (ObjectInputStream in = new ObjectInputStream(new FileInputStream("user.ser"))) {
    User user = (User) in.readObject();
}
 ⁠

### Key Points
•⁠  ⁠The class *must implement ⁠ Serializable ⁠* (a marker interface — no methods to implement).
•⁠  ⁠⁠ transient ⁠ fields are skipped during serialization (e.g., passwords, non-serializable fields like ⁠ Thread ⁠).
•⁠  ⁠⁠ serialVersionUID ⁠ — a version identifier; if missing, the JVM auto-generates one based on class structure, which can break deserialization across different compiler versions if the class changes. *Best practice: always declare it explicitly.*
•⁠  ⁠Fields that are ⁠ static ⁠ are never serialized (they belong to the class, not the instance).

### Common Mistakes
•⁠  ⁠Forgetting ⁠ serialVersionUID ⁠, causing ⁠ InvalidClassException ⁠ after minor code changes.
•⁠  ⁠Serializing objects with non-serializable fields without marking them ⁠ transient ⁠ → ⁠ NotSerializableException ⁠.
•⁠  ⁠Using Java serialization for cross-language communication — it's JVM-specific. For interop, use *JSON (Jackson)* or *Protocol Buffers* instead.

### Modern Alternative: JSON Serialization (common in Spring Boot)
⁠ java
ObjectMapper mapper = new ObjectMapper();
String json = mapper.writeValueAsString(user);     // serialize
User user2 = mapper.readValue(json, User.class);   // deserialize
 ⁠
*Why preferred in real projects:* Human-readable, language-agnostic, and what REST APIs use by default (Spring Boot uses Jackson under the hood for ⁠ @RestController ⁠ responses).

---

## 4. Real-World / Spring Boot Usage
•⁠  ⁠File uploads in Spring Boot use ⁠ MultipartFile ⁠, internally backed by NIO temp file storage.
•⁠  ⁠Config files (⁠ application.yml ⁠) are read via classpath resource streams, not raw ⁠ File ⁠ paths, because the app may run from a JAR.
•⁠  ⁠Logging frameworks (Logback/Log4j2) use buffered I/O internally for performance.
•⁠  ⁠Object caching (Redis, Ehcache) often relies on serialization (Java or JSON) to store objects as byte arrays.

---

## 5. Interview Questions

1.⁠ ⁠*Why use try-with-resources for file streams?*
   Guarantees ⁠ close() ⁠ is called automatically, preventing file handle leaks even when exceptions occur.

2.⁠ ⁠*Difference between ⁠ FileReader ⁠ and ⁠ FileInputStream ⁠?*
   ⁠ FileReader ⁠ reads characters (text, with encoding conversion); ⁠ FileInputStream ⁠ reads raw bytes — required for binary files like images.

3.⁠ ⁠*What happens if a class has a non-serializable field and you forget ⁠ transient ⁠?*
   Throws ⁠ NotSerializableException ⁠ at runtime when ⁠ writeObject() ⁠ is called.

4.⁠ ⁠*Why does NIO's ⁠ Files ⁠ class throw exceptions instead of returning ⁠ false ⁠ like ⁠ File ⁠?*
   Better error diagnostics — ⁠ File.delete() ⁠ returning ⁠ false ⁠ doesn't tell you why it failed (permission? doesn't exist? in use?); NIO throws a specific exception explaining the cause.

5.⁠ ⁠*What's the purpose of ⁠ serialVersionUID ⁠?*
   Ensures a serialized object can only be deserialized by a compatible class version; prevents silent corruption when class structure changes.

## 6. Best Practices
•⁠  ⁠Prefer NIO.2 (⁠ Path ⁠/⁠ Files ⁠) over legacy ⁠ File ⁠ for new code.
•⁠  ⁠Always use try-with-resources for any stream/channel.
•⁠  ⁠Use buffered streams (⁠ BufferedReader ⁠/⁠ BufferedWriter ⁠/⁠ BufferedInputStream ⁠) to avoid slow, unbuffered disk I/O.
•⁠  ⁠Prefer JSON/Protobuf over Java native serialization for anything crossing service or language boundaries.
•⁠  ⁠Never trust user-uploaded file paths directly — sanitize to prevent path traversal attacks (⁠ ../../etc/passwd ⁠).