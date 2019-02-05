#pragma once

#include <memory>
#include "gamefw_exception.h"

namespace gamefw
{
	//header-onlyÇ»åpè≥ópsingleton class.
	template<class DerivedClass>
	class singleton_base
	{
	public:
		singleton_base()
		{
			auto& ptr = get();
			if (ptr) GAMEFW_THROW_EXCEPTION("you tried singleton class duplication.");

			ptr.reset(static_cast<DerivedClass*>(this));
		}

		virtual ~singleton_base()
		{
			auto& ptr = get();
			ptr.release();
		}

		inline static const std::unique_ptr<DerivedClass>& get_instance()
		{
			return get();
		}

	private:
		static std::unique_ptr<DerivedClass>& get()
		{
			static std::unique_ptr<DerivedClass> ptr;
			return ptr;
		}
	};
}