#pragma once
#include "common.hpp"
#include "memory/memory.hpp"

namespace Client {
	class SignatureStore {
		using Res = Memory::ScannedResult<void>;
		
		class Signature {
		public:
			Signature(const std::string& name, void** ptr, const std::string& signature, std::function<Res(Res)> mod)
				: m_Name(name)
				, m_Signature(signature)
				, m_Pointer(ptr)
				, m_Mod(std::move(mod))
			{}

			void Scan(std::size_t* countPtr) const {
				Res scTemp = Memory::SigScan(this->m_Signature, g_GameModuleName, this->m_Name);
				if (scTemp.As<void*>() != nullptr) {
					scTemp = this->m_Mod(scTemp);
					*this->m_Pointer = scTemp.As<void*>();
				}
				
				if (countPtr != nullptr) {
					(*countPtr)++;
				}
			}

			std::string m_Name;
			std::string m_Signature;
			void** m_Pointer;
			std::function<Res(Res)> m_Mod;
		};
	public:
		void Add(const std::string& name, void** ptr, const std::string& signature, std::function<Res(Res)> mod) {
			this->m_Signatures.push_back(Signature(name, ptr, signature, std::move(mod)));
		}
		
		void Add(const std::string& name, void** ptr, const std::string& signature) {
			this->m_Signatures.push_back(Signature(name, ptr, signature,
				[](Res r) { return r; }));
		}

		void ScanAll() {
			std::size_t targetCount = this->m_Signatures.size();
			std::size_t currentCount = 0;

			std::size_t* currentCountPtr = &currentCount;

			for (const auto& signature : this->m_Signatures) {
				std::thread t([=]() {
					signature.Scan(currentCountPtr);
				});
				t.detach();
			}

			while (currentCount < targetCount) {
				Sleep(100);
			}

			// is this a racist condition? yes.
			// do i care? no.
			Sleep(1000);
		}
	private:
		std::vector<Signature> m_Signatures{};
	};
}
