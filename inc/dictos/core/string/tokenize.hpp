#pragma once

namespace dictos::string {

/**
 * A tokenizer is a boost concept that we typedef here for simplicity.
 * It allows some fairly advanced forms of tokenization, example:
 *
 * 	Tokenizer tokenizer("mystring1=myval1,mystring2=myval2", Delim(",="))
 * 	std::map<std::string, std::string> tokens;
 * 	for (auto it = token.begin(); it != token.end() it++) {
 *		tokens.insert(tokens::value_type(*it++, *it));
 * 	}
 *
 * 	tokens == {{"mystring1, "myval1"}, {"mystring2", "myval2"} }
 */

using Delimiter = boost::char_separator<char>;
using Tokenizer = boost::tokenizer<Delimiter>;

template<class DelimType = char>
inline std::map<std::string, std::string> tokenizeMap(
		const std::string &string,
	   	const DelimType &first_delim,
	   	const DelimType &second_delim)
{
	auto tokenizer = string::Tokenizer(
		string,
		string::Delimiter("=,")
	);

	std::map<std::string, std::string> result;
	for (auto it = tokenizer.begin(); it != tokenizer.end(); it++) {
		std::string key = *it++;
		std::string val = *it;
		result.insert(std::make_pair(std::move(key), std::move(val)));
	}

	return result;
}

template<class DelimType = char, class ListType = std::vector<std::string>>
inline ListType tokenizeList(
		const std::string &string,
	   	const DelimType &delim)
{
	auto tokenizer = string::Tokenizer(
		string,
		string::Delimiter("=,")
	);

	ListType result;
	for (auto it = tokenizer.begin(); it != tokenizer.end(); it++) {
		std::string entry = *it;
		result.emplace_back(std::move(entry));
	}

	return result;
}

}
