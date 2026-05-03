VermilionCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

VermilionCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VERMILION_CITY
	end

VermilionCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_Man::
	msgbox VermilionCity_PokemonCenter_1F_Text_PoisonedMonFaintedWhileWalking, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_Hiker::
	msgbox VermilionCity_PokemonCenter_1F_Text_TrainerMonsStrongerThanWild, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox VermilionCity_PokemonCenter_1F_Text_AllMonWeakToSpecificTypes, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_NatureChanger::
	lock
	faceplayer
	call EventScript_NatureChanger
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_LegalityChecker::
	lock
	faceplayer
	call EventScript_LegalityChecker
	release
	end

EventScript_LegalityChecker::
	message Text_WelcomeWantToCheckLegality
	waitmessage
	multichoice 19, 8, MULTICHOICE_YES_NO, FALSE | (TRUE << 1)
	switch VAR_RESULT
	case 0, EventScript_SelectMonForLegalityCheck
	case 1, EventScript_RefusedLegalityCheck
	case SCR_MENU_CANCEL, EventScript_RefusedLegalityCheck
	end

EventScript_SelectMonForLegalityCheck::
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	goto_if_ge VAR_0x8004, PARTY_SIZE, EventScript_RefusedLegalityCheck
	callnative BufferSelectedMonDataForNatureChange
	message Text_CheckingLegality
	waitmessage
	callnative CheckSelectedMonLegality
	goto_if_eq VAR_RESULT, FALSE, EventScript_FailedLegalityCheck
	msgbox Text_CheckingLegalityOK, MSGBOX_NPC
	end

EventScript_FailedLegalityCheck::
	msgbox Text_CheckingLegalityFailed, MSGBOX_NPC
	end

EventScript_RefusedLegalityCheck::
	msgbox Text_RefusedToCheckLegality, MSGBOX_NPC
	end

Text_WelcomeWantToCheckLegality::
    .string "Wanna check a Pokémon's legality?$"

Text_CheckingLegality::
    .string "Checking {STR_VAR_1}'s legality\n"
    .string "nature is {STR_VAR_2}$"

Text_CheckingLegalityOK::
    .string "Your {STR_VAR_1} is legal\n"
    .string "nature is {STR_VAR_2}$"

Text_CheckingLegalityFailed::
    .string "Your {STR_VAR_1} is not legal\n"
    .string "error is {STR_VAR_2}$"

Text_RefusedToCheckLegality::
    .string "OK, feel free to ask again\n"
    .string "if you change your mind.$"
