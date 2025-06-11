# Kv — A Persistent Key-Value Store

**Kv** is a lightweight, persistent key-value store inspired by [Facebook's RocksDB](http://rocksdb.org/) and [Google's LevelDB](https://github.com/google/leveldb).

> ⚠️ **Warning**  
> This project is a work-in-progress and is intended primarily as a learning exercise.  
> It’s not production-ready, take it with a pinch of salt.

## Overview

Kv aims to provide a simple and efficient storage engine with a minimal interface. While it shares conceptual similarities with RocksDB and LevelDB, it’s not nearly as feature-rich or optimized. Think of it as a fun, educational take on building a key-value store from scratch.

## Features (Planned or WIP)

- Persistent storage of key-value pairs  
- Command-line interface for basic operations  
- Modular codebase written in C  
- Simple log-structured storage design

## Why?

This project was born out of curiosity, to understand how real-world systems like RocksDB and LevelDB work under the hood. It's a playground to learn about:

- File-based storage design  
- Memory management in C  
- Serialization and data integrity  
- Low-level I/O

## Getting Started

```sh
git clone https://github.com/Kemphire/kv
cd kv
make
./kv --help
```

## Installation

```sh
git clone https://github.com/Kemphire/kv
cd kv
make sys
kv --help
```

## Contributions

Pull requests are welcome, but this project is still in early stages and may change frequently.  
Feel free to fork and experiment!
