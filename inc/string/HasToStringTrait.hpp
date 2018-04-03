#pragma once

namespace dictos::string {

/**
 * This trait will be true if the object has a method __toString on it, works with
 * inherited classes too.
 */
template<typename T>
struct HasToStringTrait
{
private:
	typedef std::true_type yes;
	typedef std::false_type no;

	template<typename U>
	static auto test(int) -> decltype(std::declval<U>().__toString(), yes());

	template<typename>
	static no test(...);

public:

	static constexpr bool value = std::is_same<decltype(test<T>(0)),yes>::value;
};

}
