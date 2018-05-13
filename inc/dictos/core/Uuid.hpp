#pragma once

namespace dictos {

/**
 * The UUID object represents a 128 bit uuid.
 */
class Uuid : public boost::uuids::uuid
{
public:
	Uuid() = default;
	virtual ~Uuid() = default;

    explicit Uuid(boost::uuids::uuid const& u) : boost::uuids::uuid(u) {}

    operator boost::uuids::uuid()
	{
        return static_cast<boost::uuids::uuid&>(*this);
    }

    operator boost::uuids::uuid() const
	{
        return static_cast<boost::uuids::uuid const&>(*this);
    }

	static auto __fromString(const std::string_view &str)
	{
		boost::uuids::string_generator gen;
		return Uuid(gen(str.data()));
	}

	auto __toString() const
	{
		return boost::uuids::to_string(*this);
	}

	static auto nill() { return Uuid(boost::uuids::nil_uuid()); }
    static auto create() { return Uuid(boost::uuids::uuid(boost::uuids::random_generator()())); }
	explicit operator bool () const { return nill() == *this; }

};

namespace string {

template<>
inline Uuid fromString(const std::string_view &string)
{
	return Uuid(Uuid::__fromString(string));
}

}

}
