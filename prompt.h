// msvc is weird with __cplusplus, 
// you need the compile with /Zc:__cplusplus
// for the correct macro definition
// the below macro prevents this
#if defined(_MSVC_LANG)
#	define _PROMPT_LANG	_MSVC_LANG
#else
#	define _PROMPT_LANG __cplusplus
#endif

#if _PROMPT_LANG < 201703L
#	error C++17 is required for prompt.h
#endif

#include <iostream>
#include <string>
#include <string_view>
#include <optional>
#include <functional>
#include <charconv>

#pragma once


/// <summary>
/// get type name of `T`, supports gnu, clang, and msvc C++17
/// also very hacky
/// </summary>
/// <typeparam name="T">type to get name of</typeparam>
/// <returns>`T`'s type name</returns>
template<typename T>
constexpr std::string_view type_name() {
#if defined(_MSC_VER)
	std::string_view func_sig = __FUNCSIG__;
	std::string_view type_beg_find = "type_name<";
	std::string_view type_end_find = ">";
#elif defined(__clang__)
	std::string_view func_sig = __PRETTY_FUNCTION__;
	std::string_view type_beg_find = "= ";
	std::string_view type_end_find = "]";
#elif defined(__GNUC__)
	std::string_view func_sig = __PRETTY_FUNCTION__;
	std::string_view type_beg_find = "= ";
	std::string_view type_end_find = ";";
#else
#	error Unsupported Compiler
#endif

	size_t type_beg = func_sig.find(type_beg_find) + type_beg_find.size();
	size_t type_end = func_sig.find(type_end_find, type_beg);

	return func_sig.substr(type_beg, type_end - type_beg);
}


/// <summary>
/// a generic prompt validator for some type,
/// basically a `std::hash` clone
/// </summary>
/// <typeparam name="T">type to validate</typeparam>
template<typename T>
struct prompt_validator {

	/// <summary>
	/// validates and parses `in` to `out`
	/// </summary>
	/// <param name="in">string representation of T</param>
	/// <param name="out">output parsed</param>
	/// <returns>whether or not the text `in` could be parsed to `out`</returns>
	bool operator()(const std::string& in, T& out) {
		return false;
	}
};


/// <summary>
/// prompt user for input
/// </summary>
/// <typeparam name="T">input type</typeparam>
/// <typeparam name="Validator">input type validator</typeparam>
/// <param name="msg">prompt message to display</param>
/// <returns>the optional input as type T</returns>
template<typename T, typename Validator = prompt_validator<T>>
std::optional<T> prompt(const std::string& msg) {
	T value = { };
	std::string value_buf;

	std::cout << msg;
	std::getline(std::cin, value_buf);

	if (!Validator()(value_buf, value))
		return std::nullopt;

	return value;
}

/// <summary>
/// prompt user for input, with default value
/// </summary>
/// <typeparam name="T">input type</typeparam>
/// <param name="msg">prompt message to display</param>
/// <param name="default_value">return value if invalid input is provided</param>
/// <returns>parsed input of type T if input is valid `default_value` otherwise</returns>
template<typename T>
T prompt(const std::string& msg, const T& default_value) {
	auto value = prompt<T>(msg);
	return value.has_value() ? value.value() : default_value;
}


/// <summary>
/// prompt user for input and ensure the type is inputted
/// </summary>
/// <typeparam name="T">input type</typeparam>
/// <param name="msg">prompt message to display</param>
/// <param name="bound">optional function to check if the input is in it's bound</param>
/// <returns>input as type T</returns>
template<typename T>
T prompt_ensure(const std::string& msg, std::function<bool(T&)> bound = nullptr) {
	while (true) {
		auto res = prompt<T>(msg);

		if (!res.has_value()) {
			std::cout << "Invalid `" << type_name<T>() << "` Entered\n";
			continue;
		}

		if (bound && !bound(res.value())) {
			std::cout << "Value Entered not in Bound\n";
			continue;
		}

		return res.value();
	}
}

//
//	Provided specializations for `prompt_validator`
//

template<>
struct prompt_validator<std::string> {
	bool operator()(const std::string& in, std::string& out) {
		out = in;
		return true;
	}
};

template<>
struct prompt_validator<bool> {
	bool operator()(const std::string& in, bool& out) {
		const char* true_str = "true";
		const char* false_str = "false";

		if (in.empty())	return false;

		if (in.size() == 1) {
			switch (in[0])
			{
			case '1':
				out = true;
				return true;

			case '0':
				out = false;
				return true;

			default:
				return false;
			}
		}

		// cross-platform, iterator based, case-insensitive, string equals for "true"
		if (std::equal(
			in.begin(), in.end(), "true",
			[](uint8_t a, uint8_t b) { return tolower(a) == tolower(b); }
		)) {
			out = true;
			return true;
		}

		// cross-platform, iterator based, case-insensitive, string equals for "false"
		if (std::equal(
			in.begin(), in.end(), "false",
			[](uint8_t a, uint8_t b) { return tolower(a) == tolower(b); }
		)) {
			out = false;
			return true;
		}

		return false;
	}
};

template<>
struct prompt_validator<char> {
	bool operator()(const std::string& in, char& out) {
		if (in.empty())	return false;
		out = in[0];
		return true;
	}
};

//
//	generic declarations for types with defined behavior for `std::from_chars` minus `char`
//

#define DECL_PRIM_PROMPT_VALID(_type_)						\
template<> struct prompt_validator<_type_> {				\
	bool operator()(const std::string& in, _type_& out) {	\
		if (in.empty())	return false;						\
		auto res = std::from_chars(							\
			&in[0],											\
			&in[in.size()],									\
			out												\
		);													\
		return res.ptr == &in[in.size()];					\
	}														\
}

DECL_PRIM_PROMPT_VALID(int16_t);
DECL_PRIM_PROMPT_VALID(int32_t);
DECL_PRIM_PROMPT_VALID(int64_t);

DECL_PRIM_PROMPT_VALID(uint8_t);
DECL_PRIM_PROMPT_VALID(uint16_t);
DECL_PRIM_PROMPT_VALID(uint32_t);
DECL_PRIM_PROMPT_VALID(uint64_t);

// gnu has yet to include float and double
// as convertable types for `std::from_chars`
#ifdef _MSC_VER

DECL_PRIM_PROMPT_VALID(float);
DECL_PRIM_PROMPT_VALID(double);

#else

template<>
struct prompt_validator<float> {
	bool operator()(const std::string& in, float& out) {
		char* p_end = nullptr;
		out = strtof(in.c_str(), &p_end);
		return !(*p_end);
	}
};

template<>
struct prompt_validator<double> {
	bool operator()(const std::string& in, double& out) {
		char* p_end = nullptr;
		out = strtod(in.c_str(), &p_end);
		return !(*p_end);
	}
};

#endif