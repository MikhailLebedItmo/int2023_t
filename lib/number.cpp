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


int2023_t::int2023_t(const int2023_t& other) {
    for (size_t i = 0; i < int2023_t::kDataSize; ++i) {
        data[i] = other.data[i];
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
    size_t buff_len = std::strlen(buff);
    for (size_t i = static_cast<size_t>(is_negative); i < buff_len; ++i) {
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

int2023_t shift_digits_to_left(int2023_t value, uint8_t shift) {
    // return value * 256^shift
    for (size_t t = 0; t < int2023_t::kDataSize - shift; ++t) {
            value.data[t] = value.data[t + shift];
    }
    for (size_t t = int2023_t::kDataSize - shift; t < int2023_t::kDataSize; ++t) {
        value.data[t] = static_cast<uint8_t>(0);
    }
    return value;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    auto result = int2023_t();
    for(int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        size_t shift = int2023_t::kDataSize - 1 - i;
        result += shift_digits_to_left(lhs * rhs.data[i], shift);
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

void sum_with_shift(int2023_t& lhs, const int2023_t& rhs, uint8_t shift) {
    // lhs = lhs + rhs * 256^shift
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - static_cast<int>(shift) - 1; i >= 0; --i) {
        uint32_t sum = carry + lhs.data[i] + rhs.data[i + static_cast<int>(shift)];
        lhs.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase); 
        carry = sum / int2023_t::kSystemBase;
    }
}

size_t int2023_t::len() const {
    size_t i = 0;
    while(i < int2023_t::kDataSize && data[i] == 0) ++i;
    return kDataSize - i;
}


bool is_negative(const int2023_t value) {
    int char_size = 8;

    return static_cast<bool>((value.data[0] >> (char_size - 1)) & 1);
}


int2023_t abs(int2023_t value) {
    if (is_negative(value)) {
        return -value;
    }
    return value;
}


uint8_t get_first_non_zero_digit(const int2023_t& value) {
    int i = 0;
    while (i < int2023_t::kDataSize && value.data[i] == 0) {
        ++i;
    }
    return value.data[i];
}







// int2023_t NumShift(int2023_t a, int k) {
//     int2023_t ans = a;
//     for (int i = 0; i + k < 253; ++i) {
//         ans.data[i] = ans.data[i + k];
//     }
//     for (int i = 253 - k; i < 253; ++i) {
//         ans.data[i] = 0;
//     }
//     return ans;
// }


// bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
//     for (int i = 0; i < 253; ++i) {
//         if (lhs.data[i] > rhs.data[i]) {
//             return true;
//         }
//         if (lhs.data[i] < rhs.data[i]) {
//             return false;
//         }
//     }
//     return true;
// }


// int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
//     bool flag = true;
//     if ((lhs.data[0] >= 128) && (rhs.data[0] < 128) || (lhs.data[0] < 128) && (rhs.data[0] >= 128)) {
//         flag = false;
//     }
//     int2023_t a = abs(lhs);
//     int2023_t b = abs(rhs);
//     int2023_t c = from_string("1");
//     int2023_t ans;
//     int k = -1;
//     while (abs(lhs) >= b) {
//         k++;
//         b = NumShift(rhs, k);
//         if (k > 252) {
//             break;
//         }
//     }
//     k--;
//     b = NumShift(abs(rhs), k);
//     while (k >= 0) {
//         while (a >= b) {
//             a = a - b;
//             ans = ans + NumShift(c, k);
//         }
//         k--;
//         b = NumShift(rhs, k);
//     }
//     if (flag) {
//         return ans;
//     }
    
//     return -ans;
// }


int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    auto result = int2023_t();
    size_t result_len = 0;
    auto lhs_abs = abs(lhs);
    // std::cout << "LOOOK";
    // std::cout << lhs_abs;
    // return result;
    auto rhs_abs = abs(rhs);
    // std::cout << "LOOOK";
    // std::cout << rhs_abs << '\n';
    // return result;
    // std::cout << "-rhs = " << -rhs_abs << '\n';
    size_t rhs_abs_len = rhs_abs.len();
    // std::cout << "LOOOK";
    // std::cout << rhs_abs_len << '\n';
    // return result;

    // std::cout << "lhs.len = " << lhs.len() << '\n';
    // std::cout << "rhs.len = " << rhs.len() << '\n';
    // std::cout << "-rhs = " << -rhs << '\n';
    //  - static_cast<size_t>(rhs.data[1] >= static_cast<uint8_t>(128))
    // bool skip_first_itera
    for (size_t i = lhs_abs.len() - static_cast<size_t>(lhs_abs.data[0] != 0 && get_first_non_zero_digit(rhs_abs) >= static_cast<uint8_t>(128)); i > 0; --i) {
        // std::cout << "lhs = " << lhs_copy << '\n';
        std::cout << "LOOOK";
        std::cout << i << '\n';
        return result;
        if (rhs_abs_len > i) {
            // result.data[result_len++] = 0;
            // std::cout << "digit = " << 0 << '\n';
            continue;
        }
        for (uint8_t digit = 1; digit <= static_cast<uint8_t>(int2023_t::kSystemBase - 1); ++digit) {
            sum_with_shift(lhs_abs, -rhs_abs, i - rhs_abs_len);
            // std::cout << "after shift = " << lhs_abs << '\n';
            if (is_negative(lhs_abs)) {
                sum_with_shift(lhs_abs, rhs_abs, i - rhs_abs_len);
                // std::cout << "digit = " << (int)digit << '\n';
                result.data[result_len++] = digit - 1;
                break;
            }
        }
    }
    // std::cout << "result = " << result << '\n';
    // std::cout << "result_len = " << result_len << '\n';
    for (size_t i = static_cast<int>(result_len); i > 0; --i) {
        result.data[int2023_t::kDataSize - result_len + i - 1] = result.data[i - 1];
        result.data[i - 1] = 0;
    }
    if (is_negative(lhs) != is_negative(rhs)) {

        return -result;
    }

    return result;
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
    stream << '(';
    bool is_first_digit = true;
    for (size_t i = 0; i < int2023_t::kDataSize - 1; ++i) {
        // if (is_first_digit && value.data[i] == 0) {
        //     continue;
        // }
        is_first_digit = false;
        stream << static_cast<int>(value.data[i]) << ' ';
    }
    stream << static_cast<int>(value.data[int2023_t::kDataSize - 1]);
    stream << ')';

    return stream;
}


int main() {
    // auto num = from_string("123456789");
    // std::cout << num << '\n';
    // auto num2= from_string("256");
    // std::cout << -(num * num2) << '\n';
    // 248 164 50 235 0
    auto r = from_string("256");
    auto num1 = from_string("-296606085336141709279692590556287014562458493203356299902672776719780753965195937305895366014651113513936294507091596783559942054561537842495415677935983511086226947117039613479735376508167473398524467034716403388685241002083099556462837471923326652232222155093037209819064072444880601618019702465298400501523730936258154528732059011191174264823479203625188983194370615274032273865453623372471181512404292624800359834705156869338968689442834586738597571088393020011010449161952454998236458193073404029946501567376922223297329987702335805064130245993684749809851309315420199991273648092885102586288391411929603");
    // std::cout << "num1 = " << num1 << '\n';
    auto num2 = from_string("3074695033");
    // std::cout  << "256 * num2 = "<< (-num2) * r<< '\n'; 
    // std::cout << "num2 = " << num2 << '\n';
    // auto num1 = from_string("168496141");
    // auto num2 = from_string("2570");
    // sum_with_shift(num1, num2, 0);
    // std::cout << num1;
    std::cout << num1 << '\n' << num2 << '\n';
    std::cout << num1 / num2;

    // auto num1 = from_string("1235802546456486");
    // int i = 0;
    // while (num1.data[i] == 0) ++i;
    // while (i < int2023_t::kDataSize) {
    //     std::cout << (int)num1.data[i++] << ' ';
    // }
    // std::cout << '\n' << '\n';
    // auto num2 = from_string("111451145611841111");
    // auto res = num1 + num2;
    // i = 0;
    // while (res.data[i] == 0) ++i;
    // while (i < int2023_t::kDataSize) {
    //     std::cout << (int)res.data[i++] << ' ';
    // }

    // auto num = int2023_t();
    // num.data[int2023_t::kDataSize - 1] = 100;
    // num.data[int2023_t::kDataSize - 2] = 255;
    // num += static_cast<uint8_t>(156);
    // int i = 0;
    // while (num.data[i] == 0) ++i;
    // while (i < int2023_t::kDataSize) {
    //     std::cout << (int)num.data[i++] << ' ';
    // }
    // std::string num_str = "-7427455033789100857024454337721870341258421789008102987451722074227385252557329101";
    // auto num = from_string(num_str.data());
    // int i = 0;
    // while (num.data[i] == 0) ++i;
    // while (i < int2023_t::kDataSize) {
    //     std::cout << (int)num.data[i++] << ' ';
    // }
}
