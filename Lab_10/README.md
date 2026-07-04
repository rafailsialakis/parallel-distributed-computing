# Multithreaded TCP Services

## Overview
This lab continues the TCP server work with two multithreaded services: a broadcast-style message server and a remote pi-computation server with optional result caching.

## Exercise 1: Broadcast Server
- `TCPMultithreadedServer.c` accepts multiple clients on port `3002`.
- Keeps a shared list of connected sockets protected by a mutex.
- Each client connection is served by a pthread handler.
- Received messages are broadcast to the other connected clients.
- `TCPEchoClient2.c` starts a receive thread so the client can print broadcasts while still accepting user input.

## Exercise 2: PI Computation Server
- `TCPMultithreadedServer.c` accepts numeric requests and computes pi using that number of intervals.
- Each client is handled by a detached pthread.
- `TCPMultithreadedServerWithChecks.c` adds a linked-list cache keyed by interval count.
- Cache reads and writes are protected by a mutex so repeated requests can reuse previous results.
- `TCPEchoClient.c` provides a simple interactive TCP client for sending interval counts.

## Build
```bash
gcc -pthread Exercise_1/TCPMultithreadedServer.c -o broadcast_server
gcc -pthread Exercise_1/TCPEchoClient2.c -o broadcast_client
gcc -pthread Exercise_2/TCPMultithreadedServerWithChecks.c -o pi_server_cached
gcc Exercise_2/TCPEchoClient.c -o pi_client
```

Run the corresponding server before starting one or more clients.
