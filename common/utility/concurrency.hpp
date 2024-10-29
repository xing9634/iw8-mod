#pragma once
#include "common_core.hpp"

namespace Common::Utility::Concurrency {
	template <typename T, typename MutexType = std::mutex>
	class Container {
	public:
		template <typename R = void, typename F>
		R Access(F&& accessor) const {
			std::lock_guard<MutexType> _{ this->m_Mutex };
			return accessor(this->m_Object);
		}

		template <typename R = void, typename F>
		R Access(F&& accessor) {
			std::lock_guard<MutexType> _{ this->m_Mutex };
			return accessor(this->m_Object);
		}

		template <typename R = void, typename F>
		R AccessWithLock(F&& accessor) const {
			std::unique_lock<MutexType> lock{ this->m_Mutex };
			return accessor(this->m_Object, lock);
		}

		template <typename R = void, typename F>
		R AccessWithLock(F&& accessor) {
			std::unique_lock<MutexType> lock{ this->m_Mutex };
			return accessor(this->m_Object, lock);
		}

		T& GetRaw() {
			return this->m_Object;
		}

		const T& GetRaw() const {
			return this->m_Object;
		}
	private:
		mutable MutexType m_Mutex{};
		T m_Object{};
	};
}
