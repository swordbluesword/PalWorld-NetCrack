#pragma once
#include <Windows.h>
#include "libs/utils/memory.h"
#include "SDK.hpp"
#include "database.h"

typedef bool(*Tick)(SDK::APalPlayerCharacter* m_this, float DeltaSecond);

class config
{
public:
	//offsets
	DWORD64 ClientBase = 0;
	DWORD64 offset_Tick = 0x2AB1DC0;//APalPlayerCharacter::Tick
	DWORD64 offset_AddStatus = 0x2B0E960; //UPalNetworkIndividualComponent::AddPlayerCharacterStatusPoint_ToServer
	//check
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
	int AddItemSlot = 0;
	int AddItemCount = 2;

	enum QuickItemSet
	{
		basic_items_stackable,
		basic_items_single,
		pal_unlock_skills,
		spheres,
		tools

	};
	//Filtered Items
	std::vector<std::string> db_filteredItems;

	//static function
	static SDK::UWorld* GetUWorld();
	static SDK::UPalCharacterImportanceManager* GetCharacterImpManager();
	static SDK::APalPlayerCharacter* GetPalPlayerCharacter();
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
};
extern config Config;
