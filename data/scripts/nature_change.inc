EventScript_NatureChanger::
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
	goto_if_ge VAR_0x8004, PARTY_SIZE, EventScript_RefusedNatureChange
	callnative BufferSelectedMonDataForNatureChange
	message Text_ChangeMonNatureToWhat
	waitmessage
	callnative NatureSelectMenu
	waitstate
	goto_if_eq VAR_RESULT, NUM_NATURES, EventScript_RefusedNatureChange
	copyvar VAR_0x8004, VAR_RESULT
	callnative PerformNatureChange
	msgbox Text_MonChangedNature, MSGBOX_NPC
	end

EventScript_RefusedNatureChange::
	msgbox Text_RefusedToChangeNature, MSGBOX_NPC
	end

Text_WelcomeWantToChangeNature::
    .string "Wanna change a Pokémon's nature?$"

Text_ChangeMonNatureToWhat::
    .string "Change {STR_VAR_1}'s nature\n"
    .string "from {STR_VAR_2} to what?$"

Text_MonChangedNature::
    .string "Your {STR_VAR_1}'s nature\n"
    .string "changed to {STR_VAR_2}$"

Text_RefusedToChangeNature::
    .string "OK, feel free to ask again\n"
    .string "if you change your mind.$"
