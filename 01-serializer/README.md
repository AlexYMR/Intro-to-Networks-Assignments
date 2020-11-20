## Writing a Custom Serializer and Deserializer

### Getting Started
There are two requirements before getting started on this project: a C++11 compiler (likely the case for any computer < 5 years old) and [CMake](https://cmake.org/download/) (3.0 or greater). Follow the instructions on the website to install CMake. Afterwards, when opening a new terminal, typing cmake should prompt you with a usage message.

After installing these prerequisites, you should be able to compile the code provided in this repositoy. First, you need to clone this to your machine:

```
git clone git@github.com:BradMcDanel/cps373-assignments.git
cd cps373-assignments/01-serializer/
```

Now, we need to set up a `build/` directory for CMake:

```
mkdir build
cd build
```

Inside the build directory we can run CMake to generate a Makefile for the project.
```
cmake ..
```

Now, we can run make as usual to generate our executable (`main` in this case). Afterwards, we can run `main` from inside the build directory.
```
make
./main
```

Assuming all of that worked, you should see output printed to the terminal corresponding to [data/small.txt](https://github.com/BradMcDanel/cps373-assignments/blob/main/01-serializer/data/small.txt).

### Next steps
Take a look at [include/bakery.hpp](https://github.com/BradMcDanel/cps373-assignments/blob/main/01-serializer/include/bakery.hpp) and [src/bakery.cpp](https://github.com/BradMcDanel/cps373-assignments/blob/main/01-serializer/src/bakery.cpp) to get a sense of what changes you will need to make to implement the additional functions described in the write-up. I have provided function interfaces for each of these.
