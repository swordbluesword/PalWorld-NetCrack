#include "../pch.h"
#include "../include/Menu.hpp"
#include "SDK.hpp"
#include "config.h"
std::string rand_str(const int len)
{
    std::string str;
    char c;
    int idx;
    for (idx = 0; idx < len; idx++)
    {
        c = 'a' + rand() % 26;
        str.push_back(c);
    }
    return str;
}
void Damage(SDK::APalCharacter* character, int32 damage)
{
    SDK::FPalDamageInfo  info = SDK::FPalDamageInfo();
    info.AttackElementType = SDK::EPalElementType::Normal;
    info.Attacker = Config.GetPalPlayerCharacter();
    info.AttackerGroupID = Config.GetPalPlayerState()->IndividualHandleId.PlayerUId;
    info.AttackerLevel = 50;
    info.AttackType = SDK::EPalAttackType::Weapon;
    info.bApplyNativeDamageValue = true;
    info.bAttackableToFriend = true;
    info.IgnoreShield = true;
    info.NativeDamageValue = damage;
    Config.GetPalPlayerState()->SendDamage_ToServer(character, info);
}

int InputTextCallback(ImGuiInputTextCallbackData* data) {
    char inputChar = data->EventChar;

    Config.Update(Config.inputTextBuffer);

    return 0;
}
//SDK::FPalDebugOtomoPalInfo palinfo = SDK::FPalDebugOtomoPalInfo();
//SDK::TArray<SDK::EPalWazaID> EA = { 0U };
//void SpawnPal(char* PalName, bool IsMonster, int rank=1, int lvl = 1, int count=1)
//{
//    SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();
//
//    //Convert FNAME
//    wchar_t  ws[255];
//    swprintf(ws, 255, L"%hs", PalName);
//    SDK::FName Name = lib->Conv_StringToName(SDK::FString(ws));
//    //Call
//    if (Config.GetPalPlayerCharacter() != NULL)
//    {
//        if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
//        {
//            if (Config.GetPalPlayerCharacter()->GetPalPlayerController())
//            {
//                if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState())
//                {
//                    if (IsMonster)
//                    {
//                        Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestSpawnMonsterForPlayer(Name, count, lvl);
//                        return;
//                    }
//                    EA[0] = SDK::EPalWazaID::AirCanon;
//                    palinfo.Level = lvl;
//                    palinfo.Rank = rank;
//                    palinfo.PalName.Key = Name;
//                    palinfo.WazaList = EA;
//                    palinfo.PassiveSkill = NULL;
//                    Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->Debug_CaptureNewMonsterByDebugOtomoInfo_ToServer(palinfo);
//                }
//            }
//        }
//    }
//}
void AddItem(SDK::UPalPlayerInventoryData* data,char* itemName, int count)
{
    SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();

    //Convert FNAME
    wchar_t  ws[255];
    swprintf(ws, 255, L"%hs",itemName);
    SDK::FName Name = lib->Conv_StringToName(SDK::FString(ws));
    //Call
    data->RequestAddItem(Name, count, true);
}
void AnyWhereTP(SDK::FVector& vector,bool IsSafe)
{
        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
        {

            SDK::FGuid guid = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPlayerUId();
            vector = { vector.X,vector.Y + 100,vector.Z };
            Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->Player->RegisterRespawnLocation_ToServer(guid, vector);
            Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestRespawn();
        }
    return;
}
void ExploitFly(bool IsFly)
{
    SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
    if (p_appc != NULL)
    {
        if (IsFly)
        {
            if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
            {
                Config.GetPalPlayerCharacter()->GetPalPlayerController()->StartFlyToServer();
            }
        }
        else
        {
            if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
            {
                Config.GetPalPlayerCharacter()->GetPalPlayerController()->EndFlyToServer();
            }
        }

    }
    return;
}
void Spawn_Multiple(config::QuickItemSet Set)
{
        SDK::UPalPlayerInventoryData * InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
        switch (Set)
        {
        case 0:
            for (int i = 0; i < IM_ARRAYSIZE(database::basic_items_stackable); i++) {
                AddItem(InventoryData, _strdup(database::basic_items_stackable[i].c_str()), 100);
            }
        case 1:
            for (int i = 0; i < IM_ARRAYSIZE(database::basic_items_single); i++) 
             {
                    AddItem(InventoryData, _strdup(database::basic_items_single[i].c_str()), 1);
             }
        case 2:
            for (int i = 0; i < IM_ARRAYSIZE(database::pal_unlock_skills); i++) {
                AddItem(InventoryData, _strdup(database::pal_unlock_skills[i].c_str()), 1);
            }
        case 3:
            for (int i = 0; i < IM_ARRAYSIZE(database::spheres); i++) {
                AddItem(InventoryData, _strdup(database::spheres[i].c_str()), 100);
            }
        case 4:
            for (int i = 0; i < IM_ARRAYSIZE(database::tools); i++) {
                AddItem(InventoryData, _strdup(database::tools[i].c_str()), 1);
            }
        default:
            break;
        }
}//Creadit:asashi

namespace DX11_Base {

    namespace Styles {
        void InitStyle()
        {
            ImGuiStyle& style = ImGui::GetStyle();
            ImVec4* colors = ImGui::GetStyle().Colors;

            //	STYLE PROPERTIES
            style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

            //  Base ImGui Styling , Aplying a custyom style is left up to you.
            ImGui::StyleColorsClassic();

            /// EXAMPLE COLOR 
            //colors[ImGuiCol_FrameBg] = ImVec4(0, 0, 0, 0);

            //	COLORS
            if (g_Menu->dbg_RAINBOW_THEME) {
                //  RGB MODE STLYE PROPERTIES
                colors[ImGuiCol_Separator] = ImVec4(g_Menu->dbg_RAINBOW);
                colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 1.0f);
            }
            else {
                /// YOUR DEFAULT STYLE PROPERTIES HERE
            }
        }
    }


    namespace Tabs {
        void TABPlayer()
        {
            
            //�л�����һ��
            ImGui::Checkbox("SpeedHack", &Config.IsSpeedHack);

            ImGui::Checkbox("AttackHack", &Config.IsAttackModiler);

            ImGui::Checkbox("DefenseHack", &Config.IsDefuseModiler);

            ImGui::Checkbox("InfStamina", &Config.IsInfStamina);


            if (ImGui::Button("RandomName", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();
                        std::string s = rand_str(20);

                        wchar_t  ws[255];
                        swprintf(ws, 255, L"%hs", s.c_str());

                        Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->NetworkIndividualComponent->UpdateCharacterNickName_ToServer(Config.GetPalPlayerCharacter()->CharacterParameterComponent->IndividualHandle->ID, SDK::FString(ws));
                    }
                }
            }

            //Creadit Mokobake
            //ImGui::Checkbox("MuteKiGodmode", &Config.IsMuteki);

            if (ImGui::Button("ToggleInfAmmo",ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter()->ShooterComponent != NULL && Config.GetPalPlayerCharacter()->ShooterComponent->CanShoot())
                {
                    if (Config.GetPalPlayerCharacter()->ShooterComponent->GetHasWeapon() != NULL)
                    {
                        Config.GetPalPlayerCharacter()->ShooterComponent->GetHasWeapon()->IsRequiredBullet = !Config.GetPalPlayerCharacter()->ShooterComponent->GetHasWeapon()->IsRequiredBullet;
                    }
                }
            }
            
            ImGui::SliderFloat("SpeedModifilers", &Config.SpeedModiflers, 1, 10);
            ImGui::SliderInt("AttackModifilers", &Config.DamageUp, 0, 200000);
            ImGui::SliderInt("defenseModifilers", &Config.DefuseUp, 0, 200000);
            
            if (ImGui::Button("PrintPlayerAddr", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    g_Console->printdbg("\n\n[+] APalPlayerCharacter: %x [+]\n\n", g_Console->color.green, p_appc);
                }
            }
        }
        void TABExploit()
        {
            //Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestSpawnMonsterForPlayer(name, 5, 1);
            ImGui::Checkbox("Show Quick Tab", &Config.IsQuick);
            ImGui::Checkbox("Open Manager Menu", &Config.bisOpenManager);
            ImGui::Checkbox("Show Teleporter Tab", &Config.bisTeleporter);
            //creadit 
            //ImGui::Checkbox("PalIsMonster", &Config.IsMonster);
            ImGui::InputInt("EXP:", &Config.EXP);
            ImGui::InputText("Item Name", Config.ItemName,sizeof(Config.ItemName));
            ImGui::InputInt("Item Num", &Config.Item);
            if (ImGui::Button("Give item", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {

                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
                            if (InventoryData != NULL)
                            {
                                if (Config.ItemName != NULL)
                                {
                                    g_Console->printdbg("\n\n[+] ItemName: %s [+]\n\n", g_Console->color.green, Config.ItemName);
                                    AddItem(InventoryData, Config.ItemName, Config.Item);
                                }
                            }
                        }
                    }
                }
            }

            ImGui::InputInt("Slot to modify (start 0):", &Config.AddItemSlot);
            ImGui::InputInt("Multiple of how much:", &Config.AddItemCount);
            
            if (ImGui::Button("Give items from slot", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                AddToInventoryContainer(Config.AddItemCount, Config.AddItemSlot);
            }
            
            // this does not work lol
            // std::stringstream AddItemsString;
            // AddItemsString << "Give " << Config.AddItemCount << " items from slot" << Config.AddItemSlot;
            if (ImGui::Button("Unlock All Effigies", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                UnlockAllEffigies();
            }
            if (ImGui::Button("ToggleFly", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                Config.IsToggledFly = !Config.IsToggledFly;
                ExploitFly(Config.IsToggledFly);
            }
            /*if (ImGui::Button("DeleteSelf", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->Debug_RequestDeletePlayerSelf_ToServer();
                        }
                    }
                }
            }*/
            if (ImGui::Button("GodHealth", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            SDK::FFixedPoint fixpoint = SDK::FFixedPoint();
                            fixpoint.Value = 99999999;
                            Config.GetPalPlayerCharacter()->ReviveCharacter_ToServer(fixpoint);
                            
                        }
                    }
                }
            }
            //Creadit WoodgamerHD
            if(ImGui::Button("Give exp", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            if (Config.EXP >= 0)
                            {
                                Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GrantExpForParty(Config.EXP);
                            }
                        }
                    }
                }
            }
        }
        void TABConfig()
        {
            
            ImGui::Text("PalWorld Menu");
            ImGui::Text("VERSION: v1.2.3");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if (ImGui::Button("UNHOOK DLL", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
#if DEBUG
                g_Console->printdbg("\n\n[+] UNHOOK INITIALIZED [+]\n\n", g_Console->color.red);

#endif
                g_KillSwitch = TRUE;
            }
        }
        void TABDatabase()
        {
            //ImGui::Checkbox("IsItems", &Config.matchDbItems);

            ImGui::InputText("Filter", Config.inputTextBuffer, sizeof(Config.inputTextBuffer), ImGuiInputTextFlags_CallbackCharFilter, InputTextCallback);

            Config.Update(Config.inputTextBuffer);

            const auto& filteredItems = Config.GetFilteredItems();

            for (const auto& itemName : filteredItems) {
                if (ImGui::Button(itemName.c_str())) 
                {
                        strcpy_s(Config.ItemName, itemName.c_str());
                        continue;
                //if (Config.matchDbItems) {}
                //strcpy_s(Config.PalName, itemName.c_str());
                }
            }
        }
        void TABTeleporter()
        {
            ImGui::Checkbox("SafeTeleport", &Config.IsSafe);
            if (ImGui::Button("Home", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            if (Config.IsSafe)
                            {
                                Config.GetPalPlayerCharacter()->GetPalPlayerController()->TeleportToSafePoint_ToServer();
                            }
                            else
                            {
                                Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestRespawn();
                            }

                        }
                    }
                }
            }
            ImGui::InputFloat3("Pos",Config.Pos);
            ImGui::SameLine();
            if (ImGui::Button("TP", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::FVector vector = { Config.Pos[0],Config.Pos[1],Config.Pos[2] };
                AnyWhereTP(vector, Config.IsSafe);
            }
            for (const auto& pair : database::locationMap) {
                const std::string& locationName = pair.first;
                if (ImGui::Button(locationName.c_str())) {
                    SDK::FVector location = SDK::FVector(pair.second[0], pair.second[1], pair.second[2]);
                    AnyWhereTP(location, Config.IsSafe);
                }
            }
        }
        void TABQuick()//Creadit:asashi
        {
                if (ImGui::Button("Basic Items stack", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                    Spawn_Multiple(config::QuickItemSet::basic_items_stackable);
                }
                if (ImGui::Button("Basic Items single", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                    Spawn_Multiple(config::QuickItemSet::basic_items_single);
                }
                if (ImGui::Button("Unlock Pal skills", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                    Spawn_Multiple(config::QuickItemSet::pal_unlock_skills);
                }
                if (ImGui::Button("Spheres", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                    Spawn_Multiple(config::QuickItemSet::spheres);
                }
                if (ImGui::Button("Tools", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                    Spawn_Multiple(config::QuickItemSet::tools);
                }
        }
	}

	void Menu::Draw()
	{
        if (Config.IsESP)
        {
            ESP();
        }
		if (g_GameVariables->m_ShowMenu)
			MainMenu();
        if (Config.bisOpenManager && g_GameVariables->m_ShowMenu)
        {
            ManagerMenu();
        }

		if (g_GameVariables->m_ShowHud)
			HUD(&g_GameVariables->m_ShowHud);

		if (g_GameVariables->m_ShowDemo)
			ImGui::ShowDemoWindow();

        if (Config.isDebugESP)
            ESP_DEBUG(Config.mDebugESPDistance, ImVec4(0,1,0,1));
	}
    void Menu::ManagerMenu()
    {
        if (ImGui::Begin("Manager", &g_GameVariables->m_ShowMenu, 96))
        {
            if (Config.GetUWorld() != NULL)
            {
                ImGui::Checkbox("filterPlayer", &Config.filterPlayer);
                SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;
                for (int i = 0; i < T.Count(); i++)
                {
                    if (T[i] != NULL)
                    {
                        if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                        {
                            SDK::APalCharacter* Character = (SDK::APalCharacter*)T[i];
                            SDK::FString name;
                            if (Config.filterPlayer)
                            {
                                if (!T[i]->IsA(SDK::APalPlayerCharacter::StaticClass()))
                                {
                                    continue;
                                }
                            }
                            if (T[i]->IsA(SDK::APalPlayerCharacter::StaticClass()))
                            {
                                if (!Character) { continue; }
                                Character->CharacterParameterComponent->GetNickname(&name);
                            }
                            else
                            {
                                SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();
                                if (!Character) { continue; }
                                std::string s = Character->GetFullName();
                                size_t firstUnderscorePos = s.find('_');

                                if (firstUnderscorePos != std::string::npos) {
                                    std::string result = s.substr(firstUnderscorePos + 1);

                                    size_t secondUnderscorePos = result.find('_');

                                    if (secondUnderscorePos != std::string::npos) {
                                        result = result.substr(0, secondUnderscorePos);
                                    }
                                    wchar_t  ws[255];
                                    swprintf(ws, 255, L"%hs", result);
                                    name = SDK::FString(ws);
                                }
                            }
                            ImGui::Text(name.ToString().c_str());
                            ImGui::SameLine();
                            ImGui::PushID(i);
                            if (ImGui::Button("Kill"))
                            {
                                if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                                {
                                    Damage(Character, 99999999999);
                                }
                                continue;
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("TP"))
                            {
                                if (Config.GetPalPlayerCharacter() != NULL)
                                {
                                    if (!Character) { continue; }
                                    SDK::FVector vector = Character->K2_GetActorLocation();
                                    AnyWhereTP(vector, Config.IsSafe);
                                }
                            }

                            /*if (Character->IsA(SDK::APalPlayerCharacter::StaticClass()))
                            {
                                ImGui::SameLine();
                                if (ImGui::Button("Boss"))
                                {
                                    if (Config.GetPalPlayerCharacter() != NULL)
                                    {
                                        auto controller = Config.GetPalPlayerCharacter()->GetPalPlayerController();
                                        if (controller != NULL)
                                        {
                                            controller->Transmitter->BossBattle->RequestBossBattleEntry_ToServer(SDK::EPalBossType::ElectricBoss, (SDK::APalPlayerCharacter*)Character);
                                            controller->Transmitter->BossBattle->RequestBossBattleStart_ToServer(SDK::EPalBossType::ElectricBoss, (SDK::APalPlayerCharacter*)Character);
                                        }
                                    }
                                }
                            }*/
                            if (Character->IsA(SDK::APalPlayerCharacter::StaticClass()))
                            {
                                ImGui::SameLine();
                                if (ImGui::Button("MaskIt"))
                                {
                                    if (Config.GetPalPlayerCharacter() != NULL)
                                    {
                                        auto controller = Config.GetPalPlayerCharacter()->GetPalPlayerController();
                                        if (controller != NULL)
                                        {
                                            auto player = (SDK::APalPlayerCharacter*)Character;
                                            SDK::FString fakename;
                                            player->CharacterParameterComponent->GetNickname(&fakename);
                                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->NetworkIndividualComponent->UpdateCharacterNickName_ToServer(Config.GetPalPlayerCharacter()->CharacterParameterComponent->IndividualHandle->ID, fakename);
                                        }
                                    }
                                }
                            }
                            ImGui::PopID();
                        }
                    }
                }
            }
        }
    }
	void Menu::MainMenu()
	{
        if (!g_GameVariables->m_ShowDemo)
            Styles::InitStyle();

        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(g_Menu->dbg_RAINBOW));
        }
        if (!ImGui::Begin("PalWorld", &g_GameVariables->m_ShowMenu, 96))
        {
            ImGui::End();
            return;
        }
        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }
        
        //  Display Menu Content
        //Tabs::TABMain();

        ImGui::Text("Testing some case...");

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
          if (ImGui::BeginTabItem("Player"))
           {
               Tabs::TABPlayer();
               ImGui::EndTabItem();
           }
          if (ImGui::BeginTabItem("EXPLOIT"))
          {
              Tabs::TABExploit();
              ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Database"))
          {
              Tabs::TABDatabase();
              ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("CONFIG"))
          {
              Tabs::TABConfig();
              ImGui::EndTabItem();
          }
          if (Config.IsQuick && ImGui::BeginTabItem("Quick"))
          {
              Tabs::TABQuick();
              ImGui::EndTabItem();
          }
          if (Config.bisTeleporter && ImGui::BeginTabItem("Teleporter"))
          {
              Tabs::TABTeleporter();
              ImGui::EndTabItem();
          }
           ImGui::EndTabBar();
        }
        ImGui::End();

        
	}

	void Menu::HUD(bool* p_open)
	{
	}

    void Menu::Loops()
    {
        if ((GetAsyncKeyState(VK_F5) & 1))
        {
            SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
            if (p_appc != NULL)
            {
                if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                    {
                        if (Config.IsSafe)
                        {
                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->TeleportToSafePoint_ToServer();
                        }
                        else
                        {
                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestRespawn();

                        }

                    }
                }
            }
        }
        if ((GetAsyncKeyState(VK_F6) & 1))
        {
            SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
            if (p_appc != NULL)
            {
                if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                    {
                        SDK::FFixedPoint fixpoint = SDK::FFixedPoint();
                        fixpoint.Value = 99999999;
                        Config.GetPalPlayerCharacter()->ReviveCharacter_ToServer(fixpoint);

                    }
                }
            }
        }
        if (Config.IsSpeedHack)
        {
            if (Config.GetUWorld()
                || Config.GetUWorld()->PersistentLevel
                || Config.GetUWorld()->PersistentLevel->WorldSettings)
           {
                Config.GetUWorld()->PersistentLevel->WorldSettings->TimeDilation = Config.SpeedModiflers;
           }
        }
        if (Config.IsAttackModiler)
        {
            if (Config.GetPalPlayerCharacter() != NULL && Config.GetPalPlayerCharacter()->CharacterParameterComponent->AttackUp != Config.DamageUp)
            {
                if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
                {
                    Config.GetPalPlayerCharacter()->CharacterParameterComponent->AttackUp = Config.DamageUp;
                }
            }
        }
        //Creadit Mokobake
        /**
           if (Config.GetPalPlayerCharacter() != NULL)
            {
                if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
                {
                    Config.GetPalPlayerCharacter()->CharacterParameterComponent->bIsEnableMuteki = Config.IsMuteki;
                }
            }**/
        if (Config.IsDefuseModiler)
        {
            if (Config.GetPalPlayerCharacter() != NULL && Config.GetPalPlayerCharacter()->CharacterParameterComponent->DefenseUp != Config.DefuseUp)
            {
                if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
                {
                    Config.GetPalPlayerCharacter()->CharacterParameterComponent->DefenseUp = Config.DefuseUp;
                }
            }
        }
        if (Config.IsInfStamina)
        {
            if (Config.GetPalPlayerCharacter() != NULL)
            {
                if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
                {
                    Config.GetPalPlayerCharacter()->CharacterParameterComponent->ResetSP();
                }
            }
        }
 
    }
}