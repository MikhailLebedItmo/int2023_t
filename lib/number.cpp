#include "number.h"

#include <cstring>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vcruntime.h>

int2023_t::int2023_t() {
    for (size_t i = 0; i < kDataSize; ++i) {
        data[i] = 0;
    }
}

int2023_t from_int(int32_t i) {
    return int2023_t();
}

int2023_t from_string(const char* buff) {
    auto result = int2023_t();
    bool is_negative = false;
    if (buff[0] == '-') {
        is_negative = true;
    }
    for (size_t i = static_cast<size_t>(is_negative); i < std::strlen(buff); ++i) {
        result *= static_cast<uint8_t>(10);
        result += static_cast<uint8_t>(buff[i] - '0'); 
    }
    if (is_negative) {
        return -result;
    }
    return result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    uint32_t carry = 0;
    auto result = int2023_t();
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + lhs.data[i] + rhs.data[i];
        result.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase); 
        carry = sum / int2023_t::kSystemBase;
    }
    return result;
}

int2023_t& operator+=(int2023_t& lhs, const int2023_t& rhs) {
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + lhs.data[i] + rhs.data[i];
        lhs.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase); 
        carry = sum / int2023_t::kSystemBase;
    }
    return lhs;
}

int2023_t operator+(const int2023_t& lhs, uint8_t rhs) {
    uint32_t carry = 0;
    auto result = int2023_t();
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + lhs.data[i] + rhs;
        result.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase);
        carry = sum / int2023_t::kSystemBase;
    }
    return result;
}

int2023_t& operator+=(int2023_t& lhs, uint8_t rhs) {
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + lhs.data[i] + rhs;
        lhs.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase);
        carry = sum / int2023_t::kSystemBase;
        rhs = 0;
        if (carry == 0) return lhs;
    }
    return lhs;
}

int2023_t& operator++(int2023_t& rhs) {
    return rhs += 1;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    return lhs + (-rhs);
}

int2023_t operator-(const int2023_t rhs) {
    auto result = int2023_t();
    for (size_t i = 0; i < int2023_t::kDataSize; ++i) {
        result.data[i] = ~rhs.data[i];
    }
    ++result;
    return result;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    auto result = int2023_t();
    for(int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        auto num = lhs * rhs.data[i];  // change name!
        size_t shift = int2023_t::kDataSize - 1 - i;
        // num *= 256 ** shift
        for (size_t t = 0; t < int2023_t::kDataSize - shift; ++t) {
            num.data[t] = num.data[t + shift];
        }
        for (size_t t = int2023_t::kDataSize - shift; t < int2023_t::kDataSize; ++t) {
            num.data[t] = static_cast<uint8_t>(0);
        }
        result += num;
    }
    return result;
}

int2023_t operator*(const int2023_t& lhs, uint8_t rhs) {
    auto result = int2023_t();
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + static_cast<uint32_t>(rhs) * lhs.data[i];
        result.data[i] = sum % int2023_t::kSystemBase;
        carry = sum / int2023_t::kSystemBase;
    }
    return result;
}

int2023_t& operator*=(int2023_t& lhs, uint8_t rhs) {
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + static_cast<uint32_t>(rhs) * lhs.data[i];
        lhs.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase);
        carry = sum / int2023_t::kSystemBase;
    }
    return lhs;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    return int2023_t();
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (size_t i = 0; i < int2023_t::kDataSize; ++i) {
        if (lhs.data[i] != rhs.data[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    for (size_t i = 0; i < int2023_t::kDataSize; ++i) {
        stream << value.data[i] << ' ';
    }
    stream << '\n';

    return stream;
}


// int main() {
//     // auto num1 = from_string("1235802546456486");
//     // int i = 0;
//     // while (num1.data[i] == 0) ++i;
//     // while (i < int2023_t::kDataSize) {
//     //     std::cout << (int)num1.data[i++] << ' ';
//     // }
//     // std::cout << '\n' << '\n';
//     // auto num2 = from_string("111451145611841111");
//     // auto res = num1 + num2;
//     // i = 0;
//     // while (res.data[i] == 0) ++i;
//     // while (i < int2023_t::kDataSize) {
//     //     std::cout << (int)res.data[i++] << ' ';
//     // }

//     // auto num = int2023_t();
//     // num.data[int2023_t::kDataSize - 1] = 100;
//     // num.data[int2023_t::kDataSize - 2] = 255;
//     // num += static_cast<uint8_t>(156);
//     // int i = 0;
//     // while (num.data[i] == 0) ++i;
//     // while (i < int2023_t::kDataSize) {
//     //     std::cout << (int)num.data[i++] << ' ';
//     // }
//     std::string num_str = "-7427455033789100857024454337721870341258421789008102987451722074227385252557329101";
//     auto num = from_string(num_str.data());
//     int i = 0;
//     while (num.data[i] == 0) ++i;
//     while (i < int2023_t::kDataSize) {
//         std::cout << (int)num.data[i++] << ' ';
//     }
// }
