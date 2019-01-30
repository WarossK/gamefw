#pragma once

#include <memory>
#include <stdexcept>

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
			if (ptr) throw std::runtime_error("you tried duplication singleton class.");

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