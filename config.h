#pragma once
#include <Windows.h>
#include "libs/utils/memory.h"
#include "SDK.hpp"
#include "database.h"
#include "ItemList.hpp"

typedef bool(*Tick)(SDK::APalPlayerCharacter* m_this, float DeltaSecond);
typedef void(*SendDamage)(SDK::APalPlayerState* d_this, SDK::APalCharacter* Target, SDK::FPalDamageInfo* info);


//Creadit:Evoc
using FName_Ctor = void(__fastcall*)(SDK::FName*, const char*, int);
using n_time = std::chrono::high_resolution_clock;


class config
{
public:
	//offsets
	DWORD64 ClientBase = 0;
	DWORD64 offset_Tick = 0x2AB1DC0;//APalPlayerCharacter::Tick
	DWORD64 offset_senddmg = 0x2A57920; //APalPlayerState::SendDamage_ToServer
	//check
	bool spec = false;
	bool IsESP = false;
	bool IsAimbot = false;
	bool IsSpeedHack = false;
	bool IsAttackModiler = false;
	bool IsDefuseModiler = false;
	bool IsInfStamina = false;
	bool IsSafe = true;
	bool IsInfinAmmo = false;
	bool IsToggledFly = false;
	bool IsMuteki = false;
	bool IsMonster = false;
	bool IsQuick = false;
	bool matchDbItems = true;
	bool isDebugESP = false;
	bool bisOpenManager = false;
	bool filterPlayer = false;
	bool bisRandomName = false;
	bool bisTeleporter = false;
	bool filterCharacters = true;
	bool filterGuilds = false;
	bool filterMapParts = false;
	float SpeedModiflers = 1.0f;
	//def and value
	float mDebugESPDistance = 5.0f;
	int DamageUp = 0;
	int DefuseUp = 0;
	int EXP = 0;
	int Item = 0;
	float Pos[3] = { 0,0,0 };
	char ItemName[255];
	char inputTextBuffer[255] = "";
	SDK::UWorld* gWorld = nullptr;
	SDK::APalPlayerCharacter* localPlayer = NULL;
	SDK::TArray<SDK::APalPlayerCharacter*> AllPlayers = {};
	SDK::UPalCharacterImportanceManager* UCIM = NULL;
	SDK::UObject* WorldContextObject = NULL;
	std::vector<SDK::FVector> ToReveal = std::vector<SDK::FVector>{};
	int AddItemSlot = 0;
	int AddItemCount = 2;

	enum QuickItemSet
	{
		
		basic_items_stackable,
		basic_items_single,
		pal_unlock_skills,
		spheres,
		tools,
		starter

	};
	//Filtered Items
	std::vector<std::string> db_filteredItems;

	//static function
	static SDK::UWorld* GetUWorld();
	static SDK::UPalCharacterImportanceManager* GetCharacterImpManager();
	static SDK::APalPlayerCharacter* GetPalPlayerCharacter(); 
	static SDK::APalPlayerController* GetPalPlayerController();
	static SDK::APalPlayerState* GetPalPlayerState();
	static SDK::UPalPlayerInventoryData* GetInventoryComponent();
	static SDK::APalWeaponBase* GetPlayerEquippedWeapon();
	static bool	GetTAllPlayers(SDK::TArray<class SDK::APalCharacter*>* outResult);
	static bool	GetTAllImpNPC(SDK::TArray<class SDK::APalCharacter*>* outResult);
	static bool	GetTAllNPC(SDK::TArray<class SDK::APalCharacter*>* outResult);
	static bool	GetTAllPals(SDK::TArray<class SDK::APalCharacter*>* outResult);
	static bool GetAllActorsofType(SDK::UClass* mType, std::vector<SDK::AActor*>* outArray, bool bLoopAllLevels = false, bool bSkipLocalPlayer = false);
	static void Init();
	static void Update(const char* filterText);
	static const std::vector<std::string>& GetFilteredItems();
	static auto timeSince(n_time::time_point start) {
		n_time::time_point now = n_time::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
	}
	
};
extern config Config;
