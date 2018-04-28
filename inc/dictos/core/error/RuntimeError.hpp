#pragma once

namespace dictos::error {

/**
 * Runtime error means there was some kind of unexpected result caused by
 * some environmental or parameter changes, such as an overflow condition on an add etc.
 */
class RuntimeError : public Exception
{
public:
	using Exception::Exception;
};

}
