#pragma once
#include "common.hpp"
#include "game/function_types.hpp"
#include "memory/scanned_result.hpp"

namespace Client {
	namespace Game {
		class Pointers {
		public:
			class SignatureCalculator {
			public:
				using Res = Memory::ScannedResult<void>;

				SignatureCalculator(const std::string& signature, std::function<Res(Res)> mod)
					: m_Signature(signature)
					, m_Mod(std::move(mod))
				{}

				SignatureCalculator(const std::string& signature)
					: SignatureCalculator(signature, [](Res r) { return r; })
				{}

				std::string m_Signature;
				std::function<Res(Res)> m_Mod;
			};

			class PointerCalculator {
			public:
				PointerCalculator(SignatureCalculator calculator, std::string name, void** pointer)
					: m_Calculator(calculator)
					, m_Name(name)
					, m_Pointer(pointer)
				{}

				SignatureCalculator m_Calculator;
				std::string m_Name;
				void** m_Pointer;
			};

			class PointerList {
			public:
				PointerList(std::string _module, std::vector<PointerCalculator> pointers) {
					this->m_Module = _module;
					this->m_Pointers = pointers;
				}

				std::string m_Module{};
				std::vector<PointerCalculator> m_Pointers{};

				void Apply();
			};

			explicit Pointers();
			PointerList GetPointerList();

			std::uintptr_t* m_unk_AuthManager{};

			Functions::LoadImageAT* m_LoadImageA{};
			Functions::SetCursorPosT* m_SetCursorPos{};
			Functions::WS_CloseSocketT* m_WS_CloseSocket{};
			Functions::WS_ConnectT* m_WS_Connect{};
			Functions::WS_GetHostByNameT* m_WS_GetHostByName{};
			Functions::WS_IoctlSocketT* m_WS_IoctlSocket{};
			Functions::WS_RecvT* m_WS_Recv{};
			Functions::WS_RecvFromT* m_WS_RecvFrom{};
			Functions::WS_SelectT* m_WS_Select{};
			Functions::WS_SendT* m_WS_Send{};
			Functions::WS_SendToT* m_WS_SendTo{};
		};
	}
	inline std::unique_ptr<Game::Pointers> g_Pointers{};
}

#define MATERIAL_WHITE Client::g_Pointers->m_DB_FindXAssetHeader(Client::T9::XAssetType::ASSET_TYPE_MATERIAL, "white", true, -1).material
