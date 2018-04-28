#pragma once

namespace dictos::error {

/**
 * Invalid argument is raised anytime some kind of input to a function
 * didn't make sense.
 */
class InvalidArgument : public Exception
{
public:
	using Exception::Exception;
};

}
