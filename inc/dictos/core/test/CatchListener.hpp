#pragma once

/**
 * This class renders state changes in catch to the log output. It must
 * be in the global namespace to work with catch.
 */
struct CatchListener : ::Catch::TestEventListenerBase
{
	using TestEventListenerBase::TestEventListenerBase;

	void testRunStarting(::Catch::TestRunInfo const& info) override
	{
		LOG(test, "RUN START:", info.name);
	}

	void testRunEnded(::Catch::TestRunStats const& info) override
	{
		LOG(test, "RUN STOP:", info.runInfo.name);
	}

	void testCaseStarting(::Catch::TestCaseInfo const& info) override
	{
		LOG(test, "CASE START:", info.name);
	}

	void testCaseEnded(::Catch::TestCaseStats const& info) override
	{
		LOG(test, "CASE STOP:", info.testInfo.name);
	}

	// Assertions before/ after
	void assertionStarting(::Catch::AssertionInfo const& info) override
	{
//		LOG(test, "ASSERT STOP -", info.capturedExpression, "\n", info.lineInfo, "\n", info.macroName, info.resultDisposition);
	}

	bool assertionEnded(::Catch::AssertionStats const& info) override
	{
		return false;
	}

	void sectionStarting(::Catch::SectionInfo const& info) override
	{
	}

	void sectionEnded(::Catch::SectionStats const& info) override
	{
	}

};
