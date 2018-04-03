#pragma once

namespace dictos::error {

/**
 * Not found is raised when stuff just can't be found.
 */
class NotFound : public Exception
{
public:
	using Exception::Exception;
};

}
