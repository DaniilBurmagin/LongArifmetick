#pragma once
#include "../include/BigInt.hpp"
#include <cassert>
#include <tuple>

using TestCase = std::tuple<std::string, std::string, std::string>;
using namespace big_int;

void TestAdd(const std::vector<TestCase>& cases);
void TestSub(const std::vector<TestCase>& cases);
void TestMult(const std::vector<TestCase>& cases);
void TestDiv(const std::vector<TestCase>& cases, int divPrec);
void plus();
void minus();
void multiply();
void div();