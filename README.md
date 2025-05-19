# llm-cli

A command-line interface for an experimental large language model (LLM), written in C++. This project is a personal exploration into building an LLM from the ground up — a brainchild of curiosity, research, and raw code.

## 🚀 Goals

- Build a minimalist LLM engine in C++
- Learn and apply principles of tokenization, attention mechanisms, and model inference
- Develop a robust and extensible CLI for interacting with the model
- Maintain cross-platform compatibility (Linux & Windows)

## 📁 Project Structure

llm-cli/
├── src/              # C++ source files
├── include/          # C++ header files
├── build/            # CMake build output (ignored by git)
├── CMakeLists.txt    # CMake build script
├── .gitignore        # Git ignore rules
└── README.md         # Project overview (this file)

## 🛠️ Build Instructions

### Requirements

- CMake 3.10+
- C++17-compatible compiler (GCC, Clang, or MSVC)
- Git (for version control)

### Linux / WSL / macOS

git clone https://github.com/Evan-Montano/llm-cli.git
cd llm-cli
mkdir build && cd build
cmake ..
make
./llm_cli

### Windows (with WSL or native CMake/MSVC)

If using WSL, follow the Linux steps above.  
If using native tools like Visual Studio, open the project with the CMake GUI or configure it via terminal.

## 🧪 Current Status

✅ Basic CLI working  
🚧 Model architecture and logic in progress  
🧠 All logic is being built from scratch (no external LLM frameworks)

## 🧠 Why This Project?

This project is meant as a deep-dive into how LLMs actually work — not just from an API perspective, but from the metal up. It is a hands-on attempt to engineer intelligence through systems-level programming.

## 📄 License

MIT License — free to use and modify, but attribution is appreciated.

---

> This is a personal project — please expect frequent changes, occasional breakage, and lots of learning.
