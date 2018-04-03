#pragma once

namespace dictos::error {

/**
 * A bad alloc occurs when a memory allocation operation fails.
 */
class BadAllocation : public Exception
{
public:
	using Exception::Exception;
};

}
