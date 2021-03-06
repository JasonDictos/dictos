#include <tests.hpp>
#include <catch/catch.hpp>

using namespace dictos;
using namespace dictos;

TEST_CASE("string::append multi")
{
	REQUIRE(string::append<'|'>(1, 2, 3, 4) == "1|2|3|4|");
	REQUIRE(string::append<' '>("1", "2", "3", "4") == "1 2 3 4 ");
}

TEST_CASE("string::enclose")
{
	REQUIRE(string::enclose<'|'>(1) == "|1|");
}

TEST_CASE("string::append")
{
	REQUIRE(string::append<'|'>(1) == "1|");
}

TEST_CASE("string::prepend")
{
	REQUIRE(string::prepend<'|'>(1) == "|1");
}

TEST_CASE("string::toString")
{
	REQUIRE(string::toString(1) == "1");
	REQUIRE(string::toString(1.1) == "1.1");
	REQUIRE(string::toString("hello") == "hello");
}

TEST_CASE("string::split")
{
	REQUIRE(string::split("c", "abcdefg").second == "defg");
	REQUIRE(string::split("c", "abcdefg").first== "ab");
}

TEST_CASE("string::tokenizeList")
{
	auto tokens = string::tokenizeList(
		"a=b=c=d=e=f",
		"="
	);

	REQUIRE(
		tokens == std::vector<std::string>({
			{"a"}, {"b"}, {"c"}, {"d"}, {"e"}, {"f"}})
	);
}

TEST_CASE("string::tokenizeMap")
{
	auto tokens = string::tokenizeMap(
		"a=b,c=d,e=f",
		"=",
		","
	);

	REQUIRE(
		tokens == std::map<std::string, std::string>({
			{"a", "b"},
			{"c", "d"},
			{"e", "f"}})
	);
}

TEST_CASE("string::tokenize")
{
	auto tokenizer = string::Tokenizer(
		"a=b,c=d,e=f"s,
		string::Delimiter("=,")
	);

	std::map<std::string, std::string> tokens;
	for (auto it = tokenizer.begin(); it != tokenizer.end(); it++) {
		std::string key = *it++;
		std::string val = *it;
		tokens.insert(std::make_pair(std::move(key), std::move(val)));
	}

	for (auto &entry : tokens) {
		std::cout << entry.first << "=>" << entry.second << std::endl;
	}

	REQUIRE(
		tokens == std::map<std::string, std::string>({
			{"a", "b"},
			{"c", "d"},
			{"e", "f"}})
	);
}

TEST_CASE("string::endsWith")
{
	REQUIRE(!string::endsWith("1234", "123"));
	REQUIRE(!string::endsWith("1234", "12"));
	REQUIRE(!string::endsWith("1234", "a"));
	REQUIRE(!string::endsWith("1234", ""));
	REQUIRE(string::endsWith("1234", "4"));
	REQUIRE(string::endsWith("1234", "34"));
	REQUIRE(string::endsWith("1234", "234"));
	REQUIRE(string::endsWith("1234", "1234"));
}

TEST_CASE("string::startsWith")
{
	REQUIRE(string::startsWith("1234", "1234"));
	REQUIRE(string::startsWith("1234", "123"));
	REQUIRE(string::startsWith("1234", "12"));
	REQUIRE(string::startsWith("1234", "1"));
	REQUIRE(!string::startsWith("1234", ""));
	REQUIRE(!string::startsWith("1234", "a"));
}

TEST_CASE("string::toNumber")
{
	REQUIRE(string::toNumber<double>("1234") == 1234);
	REQUIRE(string::toNumber<int>("0x1234") == 0x1234);
	REQUIRE(string::toNumber<float>("1.5") == 1.5);
}
