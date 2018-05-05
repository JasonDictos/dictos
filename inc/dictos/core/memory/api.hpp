#pragma once

namespace dictos::memory {

template<class T = heap::SystemImpl>
inline GenericBuffer<T> generateRandom(Size size)
{
	// @@ TODO make random
	return GenericBuffer<T>(size);
}

}
