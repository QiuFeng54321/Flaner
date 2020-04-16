#ifndef _FLANER_PARSER_EXPRESSIONUNIT_HH_
#define _FLANER_PARSER_EXPRESSIONUNIT_HH_

#include <parser/value.hh>

namespace flaner
{
namespace parser
{
	class ExpressionUnit
	{
	public:
		struct Literal
		{
			Value value;
		};

		// TODO...

		enum class index : uint16_t
		{
			literal,
			// TODO...
		};

		using Storage = std::variant<
			Literal
			// , TODO...
		>;

	private:
		Storage storage;
	};
}
}

#endif // !_FLANER_PARSER_EXPRESSIONUNIT_HH_
