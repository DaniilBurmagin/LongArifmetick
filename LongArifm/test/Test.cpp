#include <tuple>
#include "../include/BigInt.hpp"
#include <cassert>
#include "Test.hpp"

using TestCase = std::tuple<std::string, std::string, std::string>;
using namespace big_int;

void TestAdd(const std::vector<TestCase>& cases) {
    for (const auto& cur_case: cases) {
        assert(BigInt(std::get<0>(cur_case)) + BigInt(std::get<1>(cur_case)) == BigInt(std::get<2>(cur_case)));
    }
}
void TestSub(const std::vector<TestCase>& cases) {
    for (const auto& cur_case : cases) {
        assert(BigInt(std::get<0>(cur_case)) - BigInt(std::get<1>(cur_case)) == BigInt(std::get<2>(cur_case)));
    }
}
void TestMult(const std::vector<TestCase>& cases) {
    for (const auto& cur_case : cases) {
        assert(BigInt(std::get<0>(cur_case)) * BigInt(std::get<1>(cur_case)) == BigInt(std::get<2>(cur_case)));
    }
}
void TestDiv(const std::vector<TestCase>& cases, int divPrec) {
    BigInt pres("1");
    for (int i = 0; i < divPrec; ++i) {
        pres *= BigInt("0.1");
    }
    for (const auto& cur_case : cases) {
        BigInt result = BigInt::divide(BigInt(std::get<0>(cur_case)), BigInt(std::get<1>(cur_case)), divPrec);
        BigInt expected = BigInt(std::get<2>(cur_case));
        assert(result - expected < pres || expected - result < pres);
    }
}

void plus() {
    std::vector<TestCase> data = {
            {"888", "111", "999"},
            {"6.0", "-1", "5"},
            {"-100.0000", "-50.00", "-150.0"},
            {"-1.0001", "-0.9999", "-2"},
            {
             "3131251289737817312398128921892130919823831213.18238192899213902390381289123324",
                    "8984350922139218389239848324092341023102943085.213332525231932940219002391090132",
                    "12115602211877035701637977245984471942926774298.395714454224071964122815282323372"
            }
    };
    TestAdd(data);
}

void minus() {
    std::vector<TestCase> data = {
            {"2500",      "1",        "2499"},
            {"10",        "-1",       "11"},
            {"-110.0000", "10.0",     "-120"},
            {"-5.689",    "1.011",    "-6.7"},
            {"123.123",   "72.56789", "50.55511"},
            {
             "9802344039820894890148938094143109123313221452.182381928992139023",
                          "343454387892437471978187391238219004500394562.213332525231932940219002391090132",
                                      "9458889651928457418170750702904890118812826889.969049403760206082780997608909868"
            }
    };
    TestSub(data);
}

void multiply() {
    std::vector<TestCase> data = {
            {"0.1",        "0.0009",       "0.00009"},
            {"-1111.101",  "1.00",         "-1111.101"},
            {"-0.2",       "-250000",      "50000"},
            {"1",          "9934201.231",  "9934201.2310"},
            {"98249.1212", "1.2133246432", "119208.07992470355584"},
            {
             "8123129923942392304112389128989.123828948910211293120938",
                           "919221.98219389120381239218389120",
                                           "7466959590304838547285393291390446476.3113933175456759204128974386759869660092570339456"
            }
    };
    TestMult(data);
}

void div() {
    std::vector<TestCase> data = {
            {"1",       "7",           "0.1428571428"},
            {"-1",      "99",          "-0.0101010101"},
            {"219.789", "4893.405",    "0.044915350354201215717"},
            {"1.2421",  "832784.1",    "0.00000149150301981"},
            {"0",       "21313.21321", "0"},
            {
             "-9289012312312893217429341091231231249.12938192389109381293243241",
                        "-87832313147593.213123102311",
                                       "105758484314350900703765.77490296347985508642379892"
            }
    };
    TestDiv(data, 10);
}

