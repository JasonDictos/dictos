#pragma once

namespace dictos::async::lock::trait {

/**
 * This trait will be true if the type has try_lock_for method, used to detect differences in
 * system mutex implementations across various compiler and vendor versions.
 */
template<typename T>
struct HasTryLockForTrait
{
private:
	typedef std::true_type yes;
	typedef std::false_type no;

	template<typename U> static auto test(int) -> decltype(std::declval<U>().try_lock_for(), yes());

	template<typename> static no test(...);

public:
	static constexpr bool value = std::is_same<decltype(test<T>(0)),yes>::value;
};

}
