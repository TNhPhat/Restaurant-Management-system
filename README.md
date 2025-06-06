# Restaurant-Management-system

## Build Instructions

This project uses [CMake](https://cmake.org/) for building and [nlohmann/json](https://github.com/nlohmann/json) for JSON support.

### Prerequisites

- CMake 3.16 or newer
- A C++20 compatible compiler (e.g., GCC, Clang, MSVC)

### Build Steps

1. **Clone the repository:**
    ```sh
    git clone https://github.com/TNhPhat/Restaurant-Management-system
    cd Restaurant-Management-system
    ```

2. **Configure the project with CMake:**
    ```sh
    mkdir build
    cd build
    cmake -G “Ninja” ..
    ```

3. **Build the project:**
    ```sh
    cmake --build .
    ```

4. **Run the executable:**
    ```sh
    ./RESTAURANT
    ```
    *(On Windows, use [RESTAURANT.exe](http://_vscodecontentref_/0))*

### Notes

- The project automatically fetches the [nlohmann/json](https://github.com/nlohmann/json) library using CMake's `FetchContent`.
- Source files are located in [src](http://_vscodecontentref_/1) and its subdirectories.
- You can add your own JSON files for testing in the project directory.

---