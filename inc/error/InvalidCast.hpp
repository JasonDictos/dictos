#pragma once

namespace dictos::error {

/**
 * Invalid cast is thrown when some kind of cast operation failed due to the
 * types being un-castable to each other.
 */
class InvalidCast : public RuntimeError
{
public:
	using RuntimeError::RuntimeError;
};

}
