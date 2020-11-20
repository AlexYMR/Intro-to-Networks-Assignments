## Writing a Custom TCP Application with Sockets

### Getting Started
If you cloned the repository for assignment 1, you should be able to run `git pull` to download this new project folder. If you forked the repository, you can try `git fetch upstream` to download the folder.


Inside `02-sockets/`, set up a `build/` directory for CMake as before:

```
mkdir build
cd build
```

Inside the build directory we can run CMake to generate a Makefile for the project.
```
cmake ..
```

Since this is a client-server application, running `make` will generate two programs: `client` and `server`. Try starting the server in one terminal `./server` and then running the client in another terminal `./client`. Make sure that you start the server first, as otherwise the client will complain about there not being a server.

### Next steps
Take a look at [src/client.cpp](https://github.com/BradMcDanel/cps373-assignments/blob/main/02-sockets/src/client.cpp) and [src/server.cpp](https://github.com/BradMcDanel/cps373-assignments/blob/main/02-sockets/src/server.cpp) for an example of how to send messages using sockets.

Note that for this assignment we are running everything locally (both client and server are running on the same computer). However, the code we write for this assignment should work over the internet!
