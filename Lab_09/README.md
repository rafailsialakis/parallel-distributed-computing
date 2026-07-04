# TCP Client-Server Applications

## Overview
This lab introduces TCP socket programming in C. It starts with single-client services and then extends the same request-processing logic to multithreaded servers that can handle multiple clients.

## Exercise 1: Text Transformation Service
- Implements a TCP client and server on port `3002`.
- Requests use the format `<type>#<text>#<key>`.
- Supported operations are lowercase, uppercase, Caesar cipher encode, and Caesar cipher decode.
- The client sends interactive messages until `EXIT`.

## Exercise 2: Calculator Service
- Implements a TCP calculator server on port `3000`.
- Parses expressions in the form `<num><operator><num>`.
- Supports `+`, `-`, `*`, and `/`.
- Handles invalid formats and division by zero with error messages.

## Exercise 3: Multithreaded Servers
- Adds one-thread-per-client versions of the text transformation and calculator services.
- Each accepted connection is assigned to a detached pthread handler.
- Includes matching clients and terminal screenshots under the `Images` directories.
- Demonstrates how the server can accept new clients while existing clients continue sending requests.

## Build
```bash
gcc Exercise_1/TCPEchoServer.c -o text_server
gcc Exercise_1/TCPEchoClient.c -o text_client
gcc Exercise_3/CalcParallelServer.c -pthread -o calc_parallel_server
gcc Exercise_3/CalcParallelClient.c -o calc_parallel_client
```

Run a server first, then connect with the matching client:
```bash
./text_server
./text_client 127.0.0.1 3002
```
