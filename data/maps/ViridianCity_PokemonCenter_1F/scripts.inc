ViridianCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

ViridianCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VIRIDIAN_CITY
	end

ViridianCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end

ViridianCity_PokemonCenter_1F_EventScript_Boy::
	msgbox ViridianCity_PokemonCenter_1F_Text_PokeCenterInEveryTown, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox ViridianCity_PokemonCenter_1F_Text_FeelFreeToUsePC, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox ViridianCity_PokemonCenter_1F_Text_PokeCentersHealMons, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_NatureChanger::
	lock
	faceplayer
	message Text_WelcomeWantToChangeNature
	waitmessage
	multichoice 19, 8, MULTICHOICE_YES_NO, FALSE | (TRUE << 1)
	switch VAR_RESULT
	case 0, EventScript_SelectMonForNatureChange
	case 1, EventScript_RefusedNatureChange
	case SCR_MENU_CANCEL, EventScript_RefusedNatureChange
	end

EventScript_SelectMonForNatureChange::
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	callnative BufferSelectedMonDataForNatureChange
	message Text_ChangeMonNatureToWhat
	waitmessage
	callnative NatureSelectMenu
	waitstate
	goto_if_eq VAR_RESULT, NUM_NATURES, EventScript_RefusedNatureChange
	copyvar VAR_0x8004, VAR_RESULT
	callnative PerformNatureChange
	msgbox Text_MonChangedNature, MSGBOX_NPC
	release
	end

EventScript_RefusedNatureChange::
	msgbox Text_RefusedToChangeNature, MSGBOX_NPC
	release
	end
