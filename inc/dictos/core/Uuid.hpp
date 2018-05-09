#pragma once

namespace dictos {

/**
 * The UUID object represents a 128 bit uuid.
 */
class Uuid : public boost::uuids::uuid
{
public:
    Uuid() : boost::uuids::uuid(boost::uuids::random_generator()()) {}
    explicit Uuid(boost::uuids::uuid const& u) : boost::uuids::uuid(u) {}

    operator boost::uuids::uuid()
	{
        return static_cast<boost::uuids::uuid&>(*this);
    }

    operator boost::uuids::uuid() const
	{
        return static_cast<boost::uuids::uuid const&>(*this);
    }

	static Uuid __fromString(const std::string_view &str)
	{
		return Uuid(boost::lexical_cast<boost::uuids::uuid>(std::string(str.begin(), str.end())));
	}

	std::string __toString() const
	{
		return boost::lexical_cast<std::string>(static_cast<boost::uuids::uuid>(*this));
	}

	static Uuid nill() { return Uuid(boost::uuids::nil_uuid()); }
};

namespace string {

template<>
inline Uuid fromString(const std::string_view &string)
{
	return Uuid(Uuid::__fromString(string));
}

}

}
