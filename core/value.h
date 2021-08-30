/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>


class value {
public:
    enum class type {
        invalid,
        integral,
        floating,
        doubling
    };

public:
    static const std::unordered_map<std::string, type> STR_TYPE_DICT;
    static const std::unordered_map<type, std::string> TYPE_STR_DICT;

private:
    type                m_type = value::type::invalid;
    std::size_t         m_size = 0;
    bool                m_signed = false;
    std::uint8_t        m_buffer[sizeof(std::uint64_t)];

    mutable bool        m_value_string_valid = false;
    mutable std::string m_value_as_string;

public:
    value() = default;

    value(const value::type p_type, const std::size_t p_size, const bool p_signed, const void* p_buffer);

    value(const value::type p_type, const std::size_t p_size, const bool p_signed, const std::string& p_value);

public:
    bool is_valid() const;

    bool is_signed() const;

    std::size_t get_size() const;

    type get_type() const;

public:
    template <typename TypeValue>
    TypeValue get() const {
        return *((TypeValue*)m_buffer);
    }

    template <>
    std::string get<std::string>() const {
        if (!m_value_string_valid) {
            m_value_as_string = evaluate_string_value();
        }

        return m_value_as_string;
    }

    template <>
    const std::string& get<const std::string&>() const {
        return get<std::string>();
    }


    template <typename TypeValue>
    void set(const TypeValue p_value) {
        std::memcpy((void*)m_buffer, (void*)p_value, sizeof(TypeValue));
        m_value_string_valid = false;
    }

    template <>
    void set<std::string>(std::string p_value) {
        switch (m_type) {
            case value::type::integral: {
                const std::uint64_t value = std::stoull(p_value);
                std::memcpy((void*)m_buffer, (void*)&value, m_size);
                break;
            }

            case value::type::floating: {
                const float value = std::stof(p_value);
                std::memcpy((void*)m_buffer, (void*)&value, m_size);
                break;
            }

            case value::type::doubling: {
                const double value = std::stod(p_value);
                std::memcpy((void*)m_buffer, (void*)&value, m_size);
                break;
            }
        }

        m_value_string_valid = false;
    }

private:
    std::string evaluate_string_value() const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const value& p_info);

public:
    static type string_to_type(const std::string& p_type);

    static std::string type_to_string(const type p_type);

    template <typename TypeValue>
    static type get_type() {
        if constexpr (std::is_integral<std::remove_reference<TypeValue>::type>::value) {
            return type::integral;
        }
        else if constexpr (std::is_same<double, std::remove_reference<TypeValue>::type>::value) {
            return type::doubling;
        }
        else if constexpr (std::is_same<float, std::remove_reference<TypeValue>::type>::value) {
            return type::floating;
        }
        else {
            static_assert(false);
        }
    }

    template <typename TypeValue>
    static value create(TypeValue p_value) {
        if constexpr (std::is_signed<TypeValue>::value) {
            return value(get_type<TypeValue>(), sizeof(TypeValue), true, &p_value);
        }
        else if constexpr (std::is_unsigned<TypeValue>::value) {
            return value(get_type<TypeValue>(), sizeof(TypeValue), false, &p_value);
        }
        else {
            static_assert(false);
        }
    }
};
