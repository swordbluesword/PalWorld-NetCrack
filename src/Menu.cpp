#include "../pch.h"
#include "../include/Menu.hpp"
#include "SDK.hpp"
#include "config.h"
#include <algorithm>
auto LastReveal = n_time::now();
static auto fname_ctor = reinterpret_cast<FName_Ctor>(signature("48 89 5C 24 ? 57 48 83 EC ? 41 8B F8 48 8B D9 48 85 D2").GetPointer());


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

namespace DX11_Base 
{
    // helper variables
    char inputBuffer_getFnAddr[100];

    namespace Styles 
    {
        void InitStyle()
        {
            ImGuiStyle& style = ImGui::GetStyle();
            ImVec4* colors = ImGui::GetStyle().Colors;

            //	STYLE PROPERTIES
            style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
            ImGui::StyleColorsClassic();

            if (g_Menu->dbg_RAINBOW_THEME) 
            {
                //  RGB MODE STLYE PROPERTIES
                colors[ImGuiCol_Separator] = ImVec4(g_Menu->dbg_RAINBOW);
                colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 1.0f);
            }
            else 
            {
                /// YOUR DEFAULT STYLE PROPERTIES HERE
            }
        }
    }


    namespace Tabs 
    {
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
            ImGui::InputText("Item Name", Config.ItemName, sizeof(Config.ItemName));
            ImGui::InputInt("Item Num", &Config.Item);
            if (ImGui::Button("Give item", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                SDK::APalPlayerState* p_apps = Config.GetPalPlayerState();
                if (p_appc && p_apps)
                {
                    SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
                    if (InventoryData && (Config.ItemName != NULL))
                    {
                        g_Console->printdbg("\n\n[+] ItemName: %s [+]\n\n", Console::Colors::green, Config.ItemName);
                        AddItemToInventoryByName(InventoryData, Config.ItemName, Config.Item);
                    }
                }
            }

            ImGui::InputInt("Slot to modify (start 0):", &Config.AddItemSlot);
            ImGui::InputInt("Multiple of how much:", &Config.AddItemCount);

            if (ImGui::Button("Give items from slot", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                IncrementInventoryItemCountByIndex(Config.AddItemCount, Config.AddItemSlot);

            // this does not work lol
            // std::stringstream AddItemsString;
            // AddItemsString << "Give " << Config.AddItemCount << " items from slot" << Config.AddItemSlot;
            if (ImGui::Button("Unlock All Effigies", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                UnlockAllEffigies();
            if (ImGui::Button("Unlock Fast Travel", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                std::vector<std::string> keyStrings = {
                    "6E03F8464BAD9E458B843AA30BE1CC8F","DDBBFFAF43D9219AE68DF98744DF0831","603ED0CD4CFB9AFDC9E11F805594CCE5","6282FE1E4029EDCDB14135AA4C171E4C","9FBB93D84811BE424A37C391DBFBB476","979BF2044C8E8FE559B598A95A83EDE3","923B781345D2AB7ECED6BABD6E97ECE8",
                    "16C7164C43E2B96BEDCC6696E2E592F7","D27AFCAD472164F000D33A8D8B61FE8B","41727100495D21DC905D309C53989914","6DB6B7B2431CA2EFFFC88EB45805BA6A","74270C2F45B8DCA66B6A1FAAA911D024","DF9FB9CB41B43052A9C74FA79A826A50","8CA5E9774FF1BBC1156ABCA09E616480",
                    "15314BE94E2FB8D018D5749BE9A318F0","79C561B747335A7A0A8FBF9FAE280E62","23B9E99C4A454B99220AF7B4A58FD8DE","A1BC65AA445619EF338E0388BC010648","BF8B123244ADB794A06EA8A10503FBDD","F8DF603B4C750B37D943C9AF6A911946","596996B948716D3FD2283C8B5C6E829C",
                    "28D514E74B51FD9EB078A891DB0787C2","ACAE5FB04D48DE4197443E9C0993086B","4D2F204549AB656CA1EA4B8E39C484F3","1BDEABA240B1699541C17F83E59E61DF","2BC5E46049E69D3549CFB58948BE3288","91DAC6F34D2A9FD7F01471B5166C6C02","41E36D9A4B2BA79A3AD1B7B83B16F77D",
                    "76B000914943BADDC56BCFBAE2BF051E","DC0ECF9241B4410C59EE619F56D1106A","71C4B2B2407F2BBBD77572A20C7FF0F5","EC94023A4CA571FF0FD19E90944F4231","2A2B744B41AC79964DAE6B89CAC51FC3","E0819EFB41581AEAC3A029B0EE2FE195","22095BFA48A46172F8D154B2EBEB7493",
                    "7C5E91514F6E84B0C1DEFFB52C7C4DBA","AECFED0D41AFEE11F30B4F9687BC3243","2EC07ACC4505CB726DE38A84246CA999","F8E5CB8143F4FA2F6213E6B454569F87","5F426B49469368B0C131D3A6DB8F7831","A277AE6341EF40D84D711EA52303353F","6231802D40C81C00445379AE238D9F90",
                    "F6C005A14B38FE0B57F1C7869FD899CB","7170881D44249E90902F728E240493AF","3E8E504B4A3975FD3862E1BC85A5D4F6","B001852C491FF5E70C4747BFF9972924","2DE1702048A1D4A82126168C49BE51A9","E88379634CB5B6117DA2E7B8810BFE0A","3697999C458BF8A3C7973382969FBDF9",
                    "65B10BB14ABDA9C2109167B21901D195","4669582D4081BF550AFB66A05D043A3D","FE90632845114C7FBFA4669D071E285F","5970E8E648D2A83AFDFF7C9151D9BEF5","B639B7ED4EE18A7AA09BA189EA703032","099440764403D1508D9BADADF4848697","B44AA24445864494E7569597ACCAEFC6",
                    "3A0F123947BE045BC415C6B061A5285A","F382ADAE4259150BF994FF873ECF242B"
                };

                SDK::APalPlayerCharacter* pPalCharacter = Config.GetPalPlayerCharacter();

                for (const std::string& keyString : keyStrings)
                {
                    SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();
                    //Convert FNAME
                    wchar_t  ws[255];
                    swprintf(ws, 255, L"%hs", keyString.c_str());
                    SDK::FName FNameKS = lib->Conv_StringToName(SDK::FString(ws));

                    ((SDK::APalPlayerState*)pPalCharacter->PlayerState)->RequestUnlockFastTravelPoint_ToServer(FNameKS);
                }

            }
            if (ImGui::Button("All Teleporters", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                SDK::APalPlayerCharacter* pPalCharacter = Config.GetPalPlayerCharacter();
                if (!pPalCharacter)
                    return;

                SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();

                for (int i = 0; i < database::ft_node_names.size(); i++) {
                    SDK::FName name{};
                    fname_ctor(&name, database::ft_node_names[i].data(), 0);
                    ((SDK::APalPlayerState*)pPalCharacter->PlayerState)->RequestUnlockFastTravelPoint_ToServer(name);
                }

                SDK::UWorld* world = Config.GetUWorld();
                if (!world)
                    return;

                SDK::TUObjectArray* objects = world->GObjects;

                for (int i = 0; i < objects->NumElements; ++i) {
                    SDK::UObject* object = objects->GetByIndex(i);

                    if (!object) {
                        continue;
                    }

                    if (!object->IsA(SDK::APalLevelObjectUnlockableFastTravelPoint::StaticClass())) {
                        continue;
                    }

                    SDK::APalLevelObjectUnlockableFastTravelPoint* tp = (SDK::APalLevelObjectUnlockableFastTravelPoint*)object;

                    if (!tp) continue;

                    Config.ToReveal.push_back(tp->K2_GetActorLocation());
                }
            }
            if (ImGui::Button("Tech Books", ImVec2(130, 20))) {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
                            if (InventoryData != NULL)
                                AddItemToInventoryByName(InventoryData, (char*)"TechnologyBook_G3", 50);
                        }
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Use Books (1st Slot)", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
                            if (InventoryData != NULL) {
                                SDK::UPalItemContainerMultiHelper* InventoryMultiHelper = InventoryData->InventoryMultiHelper;
                                if (InventoryMultiHelper != NULL) {
                                    SDK::TArray<class SDK::UPalItemContainer*> Containers = InventoryMultiHelper->Containers;
                                    if (Containers.Count() == 0) {
                                        return;
                                    }

                                    SDK::UPalItemSlot* FirstSlot = Containers[0]->Get(0);

                                    if (FirstSlot != NULL)
                                    {
                                        SDK::FPalItemSlotId ID = FirstSlot->GetSlotId();
                                        for (int i = 0; i < FirstSlot->GetStackCount(); i++)
                                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->TechnologyData->RequestAddTechnologyPointByItem(ID);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (ImGui::Button("9999 Essentials")) {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
                            if (InventoryData != NULL)
                                for (int i = 0; i < database::essential_items.size(); i++)
                                    AddItemToInventoryByName(InventoryData, (char*)database::essential_items[i].c_str(), 9999);
                        }
                    }
                }
            }
            if (ImGui::Button("Starter Pack", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
                            if (InventoryData != NULL) {
                                SpawnMultiple_ItemsToInventory(config::starter);
                               
                            }
                        }
                    }
                }
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
                ReviveLocalPlayer();

            //Creadit WoodgamerHD
            if (ImGui::Button("Give exp", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                GiveExperiencePoints(Config.EXP);
        }
        void TABThroll()
        {

            if (ImGui::Button("KillAura", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {

                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState())
                        {
                            SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;
                            for (int i = 0; i < T.Count(); i++)
                            {

                                if (T[i] != NULL)
                                {
                                    if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                                    {
                                        SDK::APalCharacter* monster = (SDK::APalCharacter*)T[i];
                                        if (monster->IsLocallyControlled())
                                        {
                                            continue;
                                        }
                                        Damage(monster, 9999999999999);

                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (ImGui::Button("Crash Server", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))//
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        Config.GetPalPlayerCharacter()->GetPalPlayerController()->RequestLiftup_ToServer(NULL);
                    }
                }
            }
            if (ImGui::Button("BossBatt Aura", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    auto localplayer = Config.GetPalPlayerCharacter();
                    if (localplayer->GetPalPlayerController() != NULL)
                    {
                        auto localcontroller = localplayer->GetPalPlayerController();
                        //localcontroller->Transmitter->BossBattle->RequestBossBattleEntry_ToServer(SDK::EPalBossType::GrassBoss, Config.GetPalPlayerCharacter());
                        if (Config.GetUWorld() != NULL)
                        {
                            SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;
                            if (T.IsValid())
                            {
                                for (int i = 0; i < T.Count(); i++)
                                {
                                    if (T[i] != NULL)
                                    {
                                        if (T[i]->IsA(SDK::APalPlayerCharacter::StaticClass()))
                                        {
                                            auto other = (SDK::APalPlayerCharacter*)T[i];
                                            if (other->GetPalPlayerController() != NULL)
                                            {
                                                if (other->GetPalPlayerController()->IsLocalPlayerController())
                                                {
                                                    continue;
                                                }
                                            }
                                            localcontroller->Transmitter->BossBattle->RequestBossBattleEntry_ToServer(SDK::EPalBossType::ElectricBoss, other);
                                        }
                                    }
                                }
                            }
                            localcontroller->Transmitter->BossBattle->RequestBossBattleStart_ToServer(SDK::EPalBossType::ElectricBoss, localplayer);
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
                g_Console->printdbg("\n\n[+] UNHOOK INITIALIZED [+]\n\n", Console::Colors::red);

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
                RespawnLocalPlayer(Config.IsSafe);

            ImGui::InputFloat3("Pos",Config.Pos);
            ImGui::SameLine();
            if (ImGui::Button("TP", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::FVector vector = { Config.Pos[0],Config.Pos[1],Config.Pos[2] };
                AnyWhereTP(vector, Config.IsSafe);
            }

            for (const auto& pair : database::locationMap) 
            {
                const std::string& locationName = pair.first;
                if (ImGui::Button(locationName.c_str())) 
                {
                    SDK::FVector location = SDK::FVector(pair.second[0], pair.second[1], pair.second[2]);
                    AnyWhereTP(location, Config.IsSafe);
                }
            }
        }

        void TABItemSpawner()
        {
            static int num_to_add = 1;
            static int category = 0;

            ImGui::InputInt("Num To Add", &num_to_add);

            ImGui::Combo("Item Category", &category, "Accessories\0Ammo\0Armor\0Crafting Materials\0Eggs\0Food\0Hats\0\Medicine\0Money\0Other\0Pal Spheres\0Seeds\0Tools\0Weapons\0");

            std::initializer_list list = itemlist::accessories;

            switch (category)
            {
                case 1:
                    list = itemlist::ammo;
                    break;
                case 2:
                    list = itemlist::armor;
                    break;
                case 3:
                    list = itemlist::craftingmaterials;
                    break;
                case 4:
                    list = itemlist::eggs;
                    break;
                case 5:
                    list = itemlist::food;
                    break;
                case 6:
                    list = itemlist::hats;
                    break;
                case 7:
                    list = itemlist::medicine;
                    break;
                case 8:
                    list = itemlist::money;
                    break;
                case 9:
                    list = itemlist::other;
                    break;
                case 10:
                    list = itemlist::palspheres;
                    break;
                case 11:
                    list = itemlist::seeds;
                    break;
                case 12:
                    list = itemlist::tools;
                    break;
                case 13:
                    list = itemlist::weapons;
                    break;
                default:
                    list = itemlist::accessories;
            }

            int cur_size = 0;

            static char item_search[100];

            ImGui::InputText("Search", item_search, IM_ARRAYSIZE(item_search));

            for (const auto& item : list) {
                std::istringstream ss(item);
                std::string left_text, right_text;

                std::getline(ss, left_text, '|');
                std::getline(ss, right_text);

                auto right_to_lower = right_text;
                std::string item_search_to_lower = item_search;

                std::transform(right_to_lower.begin(), right_to_lower.end(), right_to_lower.begin(), ::tolower);
                std::transform(item_search_to_lower.begin(), item_search_to_lower.end(), item_search_to_lower.begin(), ::tolower);

                if (item_search[0] != '\0' && (right_to_lower.find(item_search_to_lower) == std::string::npos))
                    continue;

                if (cur_size != 0 && cur_size < 20)
                {
                    ImGui::SameLine();
                }
                else if (cur_size != 0)
                {
                    cur_size = 0;
                }

                cur_size += right_text.length();

                ImGui::PushID(item);
                if (ImGui::Button(right_text.c_str()))
                {
                    SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
                    AddItemToInventoryByName(InventoryData, (char*)left_text.c_str(), num_to_add);
                }
                ImGui::PopID();
            }
        }

        void TABQuick()
        {
            if (ImGui::Button("Basic Items stack", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::basic_items_stackable);

            if (ImGui::Button("Basic Items single", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::basic_items_single);

            if (ImGui::Button("Unlock Pal skills", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::pal_unlock_skills);

            if (ImGui::Button("Spheres", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::spheres);

            if (ImGui::Button("Tools", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                    SpawnMultiple_ItemsToInventory(config::QuickItemSet::tools);
        }
        
        void TABDebug()
        {
            ImGui::Checkbox("DEBUG ESP", &Config.isDebugESP);
            if (Config.isDebugESP)
            {
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::SliderFloat("##DISTANCE", &Config.mDebugESPDistance, 1.0f, 100.f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            }

            //  @TODO: print additional debug information
            if (ImGui::Button("PrintPlayerAddr", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc)
                    g_Console->printdbg("\n\n[+] APalPlayerCharacter: 0x%llX\n", Console::Colors::green, p_appc);
                
            }

            ImGui::InputTextWithHint("##INPUT", "INPUT GOBJECT fn NAME", inputBuffer_getFnAddr, 100);
            ImGui::SameLine();
            if (ImGui::Button("GET fn", ImVec2(ImGui::GetContentRegionAvail().x, 20)))
            {
                std::string input = inputBuffer_getFnAddr;
                SDK::UFunction* object = SDK::UObject::FindObject<SDK::UFunction>(input);
                if (object)
                {
                    static __int64 dwHandle = reinterpret_cast<__int64>(GetModuleHandle(0));
                    void* fnAddr = object->ExecFunction;
                    unsigned __int64 fnOffset = (reinterpret_cast<__int64>(fnAddr) - dwHandle);
                    g_Console->printdbg("[+] Found [%s] -> 0x%llX\n", Console::Colors::yellow, input.c_str(), fnOffset);
                }
                else
                    g_Console->printdbg("[!] OBJECT [%s] NOT FOUND!\n", Console::Colors::red, input.c_str());
                memset(inputBuffer_getFnAddr, 0, 100);
            }

        }
	}

	void Menu::Draw()
	{

		if (g_GameVariables->m_ShowMenu)
			MainMenu();

		if (g_GameVariables->m_ShowHud)
			HUD(&g_GameVariables->m_ShowHud);

		if (g_GameVariables->m_ShowDemo)
			ImGui::ShowDemoWindow();
	}

    void Menu::ManagerMenu()
    {
        if (ImGui::Begin("Manager", &g_GameVariables->m_ShowMenu, 96))
        {
            if (Config.GetUWorld() != NULL)
            {
                ImGui::Checkbox("Filter Characters", &Config.filterCharacters);
                ImGui::SameLine();
                ImGui::Checkbox("Players", &Config.filterPlayer);
                ImGui::SameLine();
                ImGui::Checkbox("Guilds", &Config.filterGuilds);
                ImGui::SameLine();
                ImGui::Checkbox("Map Objects", &Config.filterMapParts);
                SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;

                for (int i = 0; i < T.Count(); i++)
                {
                    SDK::AActor* Actor = T[i];

                    if (Actor == nullptr) continue;

                    SDK::APalCharacter* Character = (SDK::APalCharacter*)Actor;
                    std::string name;

                    bool Skip = Config.filterCharacters || Config.filterPlayer || Config.filterGuilds || Config.filterMapParts;

                    if (Config.filterCharacters && Actor->IsA(SDK::APalCharacter::StaticClass())) Skip = false;
                    else if (Config.filterPlayer && Actor->IsA(SDK::APalPlayerCharacter::StaticClass())) Skip = false;
                    if (Config.filterGuilds && Actor->IsA(SDK::APalGuildInfo::StaticClass())) Skip = false;
                    if (Config.filterMapParts && Actor->IsA(SDK::APalMapObject::StaticClass())) Skip = false;
                    if (Actor->IsA(SDK::APalCaptureJudgeObject::StaticClass())) Skip = false;

                    if (Skip) continue;

                    if (Actor->IsA(SDK::APalPlayerCharacter::StaticClass()))
                    {
                        if (!Character) { continue; }
                        SDK::FString fn;
                        Character->CharacterParameterComponent->GetNickname(&fn);
                        if (fn.IsValid())
                            name = fn.ToString();
                        else
                            name = "unknown";
                    }
                    else if (Actor->IsA(SDK::APalGuildInfo::StaticClass()))
                        name = Character->Name.GetRawString() + "[]" + ((SDK::APalGuildInfo*)Actor)->Guild->GuildName.ToString();
                    else
                        name = Character->Name.GetRawString();
                    ImGui::PushID(i);
                    if (ImGui::Button("TP"))
                    {
                        if (Config.GetPalPlayerCharacter() != NULL)
                        {
                            if (!Character) { continue; }
                            SDK::FVector vector = Character->K2_GetActorLocation();
                            AnyWhereTP(vector, Config.IsSafe);
                        }
                    }

                    if (Actor->IsA(SDK::APalCharacter::StaticClass())) {
                        ImGui::SameLine();
                        if (ImGui::Button(GetAsyncKeyState(VK_LSHIFT) ? "-99%" : "Kill"))
                            Damage(Character, GetAsyncKeyState(VK_LSHIFT) ? (Character->CharacterParameterComponent->GetHP().Value - 1) / 1000 : 99999999999);
                    }
                    if (Actor->IsA(SDK::APalPlayerCharacter::StaticClass()))
                    {
                        ImGui::SameLine();
                        if (ImGui::Button("Boss"))
                        {
                            if (Config.GetPalPlayerController)
                            {
                                
                                Config.GetPalPlayerController()->Transmitter->BossBattle->RequestBossBattleEntry_ToServer(SDK::EPalBossType::ElectricBoss,(SDK::APalPlayerCharacter*)Character);
                                Config.GetPalPlayerController()->Transmitter->BossBattle->RequestBossBattleStart_ToServer(SDK::EPalBossType::ElectricBoss, (SDK::APalPlayerCharacter*)Character);
                            }
                        }
                    }
                    if (Actor->IsA(SDK::APalPlayerCharacter::StaticClass())) {
                        ImGui::SameLine();
                        if (ImGui::Button("Join Guild")) {
                            SDK::FGuid guid1 = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPlayerUId();
                            SDK::FGuid guid2;

                            guid2 = SDK::UPalUtility::GetDefaultObj()->GetPlayerUIDByActor(Actor);

                            g_Console->printdbg("\n\n[+] Player 1 GUID: %x\nPlayer 2 GUID: %x [+]\n\n", g_Console->green, guid1, guid2);

                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->Group->RequestJoinGuildForPlayer_ToServer(guid1, guid2);
                        }
                    }
                    if (Actor->IsA(SDK::APalMapLevelObject::StaticClass())) {
                        ImGui::SameLine();
                        if (ImGui::Button("Destroy")) {
                            SDK::APalMapLevelObject* Object = (SDK::APalMapLevelObject*)Actor;

                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->MapObject->RequestDismantleObject_ToServer(Object->LevelObjectInstanceId);
                        }
                    }
                    if (Character->IsA(SDK::APalPlayerCharacter::StaticClass()))
                    {
                        ImGui::SameLine();
                        if (ImGui::Button("Steal Name"))
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
                    ImGui::SameLine();
                    ImGui::Text(name.c_str());

                    ImGui::PopID();
                }
            }
        }
        ImGui::End();
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
        
        ImGuiContext* pImGui = GImGui;

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
            if (ImGui::BeginTabItem("Thrall"))
            {
                Tabs::TABThroll();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Database"))
            {
                Tabs::TABDatabase();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Item Spawner"))
            {
                Tabs::TABItemSpawner();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("CONFIG"))
            {
                Tabs::TABConfig();
                ImGui::EndTabItem();
            }
#if DEBUG
            if (ImGui::BeginTabItem("DEBUG"))
            {
                Tabs::TABDebug();
                ImGui::EndTabItem();
            }
#endif
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



        if (Config.bisOpenManager)
            ManagerMenu();
	}

	void Menu::HUD(bool* p_open)
	{
        
        ImGui::SetNextWindowPos(g_D3D11Window->pViewport->WorkPos);
        ImGui::SetNextWindowSize(g_D3D11Window->pViewport->WorkSize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, NULL);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
        if (!ImGui::Begin("##HUDWINDOW", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
        {
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();
            return;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        auto ImDraw = ImGui::GetWindowDrawList();
        auto draw_size = g_D3D11Window->pViewport->WorkSize;
        auto center = ImVec2({ draw_size.x * .5f, draw_size.y * .5f });
        auto top_center = ImVec2({ draw_size.x * .5f, draw_size.y * 0.0f });
        
        //  Watermark
        ImDraw->AddText(top_center, g_Menu->dbg_RAINBOW, "PalWorld-NetCrack");

        if (Config.IsESP)
            ESP();

        if (Config.isDebugESP)
            ESP_DEBUG(Config.mDebugESPDistance, ImVec4(0, 1, 0, 1));

        ImGui::End();
	}

    void Menu::Loops()
    {
        if ((GetAsyncKeyState(VK_OEM_3) && GetAsyncKeyState(0x52) & 1))
        {
            if (Config.GetUWorld() != NULL)
            {
                SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;

                for (int i = 0; i < T.Count(); i++)
                {
                    SDK::AActor* Actor = T[i];

                    if (Actor == nullptr) continue;
                    if (!Actor->IsA(SDK::APalCharacter::StaticClass())) continue;
                    if (Actor->IsA(SDK::APalPlayerCharacter::StaticClass())) continue;

                    SDK::APalCharacter* Character = (SDK::APalCharacter*)Actor;

                    if (Character->IsLocallyControlled()) continue;

                    if (Character->CharacterParameterComponent->GetHP().Value < Character->CharacterParameterComponent->GetMaxHP().Value * 0.015) continue;

                    Damage(Character, (Character->CharacterParameterComponent->GetHP().Value - 1) / 1000);
                }
            }
        }

        //  Respawn
        if ((GetAsyncKeyState(VK_F5) & 1))
            RespawnLocalPlayer(Config.IsSafe);

        //  Revive Player
        if ((GetAsyncKeyState(VK_F6) & 1))
            ReviveLocalPlayer();

        if ((GetAsyncKeyState(VK_F3) & 1))
        {
            SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
            if (p_appc != NULL)
            {
                if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                {
                    Config.spec = !Config.spec;
                    Config.GetPalPlayerCharacter()->SetSpectatorMode(Config.spec);
                }
            }
        }
        if (Config.IsSpeedHack)
            SpeedHack(Config.SpeedModiflers);
        
        //  
        if (Config.IsAttackModiler)
            SetPlayerDefenseParam(Config.DamageUp);

        //  
        if (Config.IsDefuseModiler)
            SetPlayerDefenseParam(Config.DefuseUp);

        //  
        if (Config.IsInfStamina)
            ResetStamina();
        if (!Config.ToReveal.empty() && Config.timeSince(LastReveal) > 100) {
            LastReveal = n_time::now();

            SDK::FVector pos = Config.ToReveal.back();
            SDK::FHitResult _;
            Config.ToReveal.pop_back();
            Config.GetPalPlayerCharacter()->K2_SetActorLocation(pos, false, &_, true);
        }
        //  
        //  SetDemiGodMode(Config.IsMuteki);
    }
}