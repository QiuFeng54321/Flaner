#ifndef _FLANER_PARSER_VALUE_HH_
#define _FLANER_PARSER_VALUE_HH_

#include <variant>
#include <type_traits>

namespace flaner
{
namespace parser
{
	using V_none = nullptr_t;
	using V_boolean = bool;
	using V_integer = int64_t;
	using V_real = double;
	using V_string = struct {}; // TODO...

	class Value
	{
	public:
		using Storage = std::variant<
			V_none,
			V_boolean,
			V_integer,
			V_real,
			V_string
		>;

	private:
		Storage storage;

	public:
		Value(nullptr_t = nullptr)
		{

		}

		Value(bool val) : storage(V_boolean(val))
		{

		}

		Value(signed char val) : storage(V_integer(val))
		{

		}

		Value(signed short val) : storage(V_integer(val))
		{

		}

		Value(signed val) : storage(V_integer(val))
		{

		}

		Value(signed long val) : storage(V_integer(val))
		{

		}

		Value(signed long long val) : storage(V_integer(val))
		{

		}

		Value& operator=(nullptr_t)
		{
			this->storage = V_none();
			return *this;
		}
		
		Value& operator=(bool val)
		{
			this->storage = V_boolean(val);
			return *this;
		}

		Value& operator=(signed char val)
		{
			this->storage = V_integer(val);
			return *this;
		}

		Value& operator=(signed short val)
		{
			this->storage = V_integer(val);
			return *this;
		}

		Value& operator=(signed val)
		{
			this->storage = V_integer(val);
			return *this;
		}

		Value& operator=(signed long val)
		{
			this->storage = V_integer(val);
			return *this;
		}

		Value& operator=(signed long long val)
		{
			this->storage = V_integer(val);
			return *this;
		}

	public:
		
	};
}
}


#endif // !_FLANER_PARSER_VALUE_HH_
