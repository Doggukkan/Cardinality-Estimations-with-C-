# Probabilistic Data Structures

This repository contains implementations of various probabilistic data structures for efficient estimation of set cardinality and membership testing. It includes HyperLogLog for cardinality estimation, Bloom Filter for membership testing, and K-Minimum Values (KMinVal) for another approach to cardinality estimation. These data structures are particularly useful for large datasets where exact computations are impractical due to high memory requirements.

## Features

- **HyperLogLog**: Estimates the number of unique elements in a dataset with configurable accuracy.
- **Bloom Filter**: Tests whether an element is likely part of a set, with a definable false positive rate.
- **K-Minimum Values (KMinVal)**: Estimates the cardinality of a dataset by tracking the minimum hash values.
- Implementation of two hash function strategies: a simple `ax + b` hash and MurmurHash.

## Getting Started

### Dependencies

- C++11 or newer
- No external libraries required beyond the Standard Template Library (STL)
