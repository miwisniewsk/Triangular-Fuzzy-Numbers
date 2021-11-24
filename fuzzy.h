#ifndef FUZZY_H
#define FUZZY_H

#include <bits/stdc++.h>

using namespace std;

using real_t = double;

class TriFuzzyNum {
private:
    real_t l;
    real_t m;
    real_t u;

    constexpr void sort_numbers(real_t a, real_t b, real_t c) {
        real_t numbers[3];

        numbers[0] = a;
        numbers[1] = b;
        numbers[2] = c;

        sort(numbers, numbers + 3);

        l = numbers[0];
        m = numbers[1];
        u = numbers[2];
    }

public:
    constexpr TriFuzzyNum(real_t a, real_t b, real_t c) {
        sort_numbers(a, b, c);
    };
    
    inline TriFuzzyNum(const TriFuzzyNum&) = default;

    inline TriFuzzyNum& operator=(const TriFuzzyNum &) {return *this;}

    inline TriFuzzyNum (TriFuzzyNum&& object) = default;

    inline TriFuzzyNum& operator=(TriFuzzyNum&& object) {
        if (this != &object) {
            l = object.l;
            m = object.m;
            u = object.u;

            object.l = 0;
            object.m = 0;
            object.u = 0;
        }

        return *this;
    }
    
    inline TriFuzzyNum& operator+=(const TriFuzzyNum &object) {
        l = l + object.l;
        m = m + object.m;
        u = u + object.u;
        
        return *this;
    }

    inline TriFuzzyNum& operator-=(const TriFuzzyNum &object) {
        l = l - object.u;
        m = m - object.m;
        u = u - object.l;

        return *this;
    }

    inline TriFuzzyNum& operator*=(const TriFuzzyNum &object) {
        l = l * object.l;
        m = m * object.m;
        u = u * object.u;
        
        real_t a = l;
        real_t b = m;
        real_t c = u;
        
        sort_numbers(a, b, c);
        
        return *this;
    }

    const inline TriFuzzyNum operator+(const TriFuzzyNum &object) const {
        TriFuzzyNum result = *this;
        result += object;
        return result;
    }
        
    const inline TriFuzzyNum operator-(const TriFuzzyNum &object) const {
        TriFuzzyNum result = *this;
        result -= object;
        return result;
    }
    
    const inline TriFuzzyNum operator*(const TriFuzzyNum &object) const {
        TriFuzzyNum result = *this;
        result *= object;
        return result;
    }

    constexpr bool operator==(const TriFuzzyNum& object) const {
        return (m == object.m) && (l == object.l) && (u == object.u);
    };

    constexpr tuple<real_t, real_t, real_t> get_ranks(const TriFuzzyNum& object) const {
        real_t l = object.lower_value();
        real_t m = object.modal_value();
        real_t u = object.upper_value();
            
        real_t z = (u - l) + sqrt(1 + (u - m) * (u - m)) + sqrt(1 + (m - l) * (m - l));
        real_t y = (u - l) / z;
        real_t x = ((u - l) * m + sqrt(1 + (u - m) * (u - m)) * l 
            + sqrt(1 + (m - l) * (m - l)) * u) / z;

        real_t rank1 = x - y / 2;
        real_t rank2 = 1 - y;
        real_t rank3 = object.modal_value();

        return make_tuple(rank1, rank2, rank3);
    };

    constexpr int operator<=>(const TriFuzzyNum& object) const {
        auto res = (get_ranks(*this) <=> get_ranks(object));
        if (res > 0) return 1;
        else if (res < 0) return -1;
        else return 0;
    }
    
    constexpr real_t lower_value() const {return this->l;};

    constexpr real_t modal_value() const {return this->m;};
    
    constexpr real_t upper_value() const {return this->u;};
};

inline ostream& operator<<(ostream &os, const TriFuzzyNum &num) { 
    return os << "(" << num.lower_value() << ", " << num.modal_value() 
        << ", " << num.upper_value() << ")";
}

consteval TriFuzzyNum crisp_number(real_t v) {
    return TriFuzzyNum(v, v, v);
}

constinit TriFuzzyNum crisp_zero = crisp_number(0);

class TriFuzzyNumSet {
private:
    multiset<TriFuzzyNum> collection;

public:
    inline TriFuzzyNumSet(const TriFuzzyNumSet& source) = default;

    inline TriFuzzyNumSet(TriFuzzyNumSet&& source) = default;

    inline TriFuzzyNumSet() = default;

    inline TriFuzzyNumSet(initializer_list<TriFuzzyNum> l) {
        this->collection = l;
    }

    inline void insert(const TriFuzzyNum &source) {
        TriFuzzyNum copy = TriFuzzyNum(source);
        this->collection.insert(copy);
    }

    inline void insert(const TriFuzzyNum &&source) {
        TriFuzzyNum object = TriFuzzyNum(source);
        this->collection.insert(object);
    }

    inline void remove(const TriFuzzyNum &source) {
        TriFuzzyNum copy = TriFuzzyNum(source);
        this->collection.erase(copy);
    }

    inline const TriFuzzyNum arithmetic_mean() const {
        if (this->collection.empty()) {
            throw std::length_error("TriFuzzyNumSet::arithmetic_mean - the set is empty.");
        } else {
            real_t arithmetic_mean_lower = 0;
            real_t arithmetic_mean_modal = 0;
            real_t arithmetic_mean_upper = 0;

            for (auto it = this->collection.begin(); it!= this->collection.end(); it++) {
                arithmetic_mean_lower += (*it).lower_value();
                arithmetic_mean_modal += (*it).modal_value();
                arithmetic_mean_upper += (*it).upper_value();
            }

            arithmetic_mean_lower/=this->collection.size();
            arithmetic_mean_modal/=this->collection.size();
            arithmetic_mean_upper/=this->collection.size();

            return TriFuzzyNum(arithmetic_mean_lower, arithmetic_mean_modal, arithmetic_mean_upper);
        }
    }
};

#endif
