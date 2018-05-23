#include "dictos/core/all.hpp"

using namespace dictos;
using namespace dictos::log;

Logger::Logger(config::Options options) :
	Context(getSection(), std::move(options)) {
}
