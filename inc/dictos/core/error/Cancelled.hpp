#pragma once

namespace dictos::error {

/**
 * Cancelled means some operation was aborted early due to a
 * cancellation request, say when a thread or fiber's cancel flag
 * is set to true and the thread/fiber exits early.
 */
class Cancelled : public Exception
{
public:
	using Exception::Exception;
};

}
