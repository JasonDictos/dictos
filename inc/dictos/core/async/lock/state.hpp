#pragma once

namespace dictos::async::lock {

	// Define our states which allow us to assert well known values of our
	// properties at each one
	enum class STATE
	{
		Init,
		Locked,
		Unlocked,
		Rewound,
		Destructed,
	};

}
