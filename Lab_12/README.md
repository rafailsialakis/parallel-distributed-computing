# RPC Calculator Service

## Overview
This lab implements a remote calculator with Sun RPC using `rpcgen` and `libtirpc`. The interface is defined in an `.x` file, generated RPC stubs handle transport and XDR encoding, and custom client/server files implement the calculator behavior.

## Exercise 1: Calculator RPC
- `calculator.x` defines program `CALCULATOR_PROG` with version `CALCULATOR_VERS`.
- The RPC interface exposes `ADD`, `SUBTRACT`, `MULTIPLY`, and `DIVIDE`.
- Each procedure accepts an `intpair` containing operands `a` and `b`.
- `calculator_server.c` implements the service routines.
- `calculator_client.c` parses expressions like `1+2` and invokes the matching remote procedure.
- Generated files include `calculator.h`, `calculator_clnt.c`, `calculator_svc.c`, and `calculator_xdr.c`.

## Build
```bash
cd Exercise_1
make -f Makefile.calculator
```

The makefile regenerates RPC support files with:
```bash
rpcgen -C -I calculator.x
```

## Run
Start the RPC service, then call the client with a host and expression:
```bash
./calculator_server
./calculator_client localhost 10+5
./calculator_client localhost 10/4
```

The project uses `libtirpc`, so systems may need the `rpcbind` service and development headers installed.
