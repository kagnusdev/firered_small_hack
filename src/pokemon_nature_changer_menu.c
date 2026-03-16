#include "global.h"
#include "event_data.h"
#include "menu.h"
#include "list_menu.h"
#include "new_menu_helpers.h"
#include "script.h"
#include "string_util.h"
#include "sound.h"
#include "constants/songs.h"
#include "constants/sound.h"

static const u8 sHardyNatureChoice[] = _("Hardy (neutral atk)");
static const u8 sLonelyNatureChoice[] = _("Lonely (+atk -def)");
static const u8 sBraveNatureChoice[] = _("Brave (+atk -speed)");
static const u8 sAdamantNatureChoice[] = _("Adamant (+atk -sp.atk)");
static const u8 sNaughtyNatureChoice[] = _("Naughty (+atk -sp.def)");
static const u8 sBoldNatureChoice[] = _("Bold (+def -atk)");
static const u8 sDocileNatureChoice[] = _("Docile (neutral def)");
static const u8 sRelaxedNatureChoice[] = _("Relaxed (+def -speed)");
static const u8 sImpishNatureChoice[] = _("Impish (+def -sp.atk)");
static const u8 sLaxNatureChoice[] = _("Lax (+def -sp.def)");
static const u8 sTimidNatureChoice[] = _("Timid (+speed -atk)");
static const u8 sHastyNatureChoice[] = _("Hasty (+speed -def)");
static const u8 sSeriousNatureChoice[] = _("Serious (neutral speed)");
static const u8 sJollyNatureChoice[] = _("Jolly (+speed -sp.atk)");
static const u8 sNaiveNatureChoice[] = _("Naive (+speed -sp.def)");
static const u8 sModestNatureChoice[] = _("Modest (+sp.atk -atk)");
static const u8 sMildNatureChoice[] = _("Mild (+sp.atk -def)");
static const u8 sQuietNatureChoice[] = _("Quiet (+sp.atk -speed)");
static const u8 sBashfulNatureChoice[] = _("Bashful (neutral sp.atk)");
static const u8 sRashNatureChoice[] = _("Rash (+sp.atk -sp.def)");
static const u8 sCalmNatureChoice[] = _("Calm (+sp.def -atk)");
static const u8 sGentleNatureChoice[] = _("Gentle (+sp.def -def)");
static const u8 sSassyNatureChoice[] = _("Sassy (+sp.def -speed)");
static const u8 sCarefulNatureChoice[] = _("Careful (+sp.def -sp.atk)");
static const u8 sQuirkyNatureChoice[] = _("Quirky (neutral sp.def)");
static const u8 sCancelNatureChoice[] = _("Cancel");

static const struct ListMenuItem sNaturesListMenuItems[] =
{
    {
        .label = sHardyNatureChoice,
        .index = NATURE_HARDY,
    },
    {
        .label = sLonelyNatureChoice,
        .index = NATURE_LONELY
    },
    {
        .label = sBraveNatureChoice,
        .index = NATURE_BRAVE
    },
    {
        .label = sAdamantNatureChoice,
        .index = NATURE_ADAMANT
    },
    {
        .label = sNaughtyNatureChoice,
        .index = NATURE_NAUGHTY
    },
    {
        .label = sBoldNatureChoice,
        .index = NATURE_BOLD
    },
    {
        .label = sDocileNatureChoice,
        .index = NATURE_DOCILE
    },
    {
        .label = sRelaxedNatureChoice,
        .index = NATURE_RELAXED
    },
    {
        .label = sImpishNatureChoice,
        .index = NATURE_IMPISH
    },
    {
        .label = sLaxNatureChoice,
        .index = NATURE_LAX
    },
    {
        .label = sTimidNatureChoice,
        .index = NATURE_TIMID
    },
    {
        .label = sHastyNatureChoice,
        .index = NATURE_HASTY
    },
    {
        .label = sSeriousNatureChoice,
        .index = NATURE_SERIOUS
    },
    {
        .label = sJollyNatureChoice,
        .index = NATURE_JOLLY
    },
    {
        .label = sNaiveNatureChoice,
        .index = NATURE_NAIVE
    },
    {
        .label = sModestNatureChoice,
        .index = NATURE_MODEST
    },
    {
        .label = sMildNatureChoice,
        .index = NATURE_MILD
    },
    {
        .label = sQuietNatureChoice,
        .index = NATURE_QUIET
    },
    {
        .label = sBashfulNatureChoice,
        .index = NATURE_BASHFUL
    },
    {
        .label = sRashNatureChoice,
        .index = NATURE_RASH
    },
    {
        .label = sCalmNatureChoice,
        .index = NATURE_CALM
    },
    {
        .label = sGentleNatureChoice,
        .index = NATURE_GENTLE
    },
    {
        .label = sSassyNatureChoice,
        .index = NATURE_SASSY
    },
    {
        .label = sCarefulNatureChoice,
        .index = NATURE_CAREFUL
    },
    {
        .label = sQuirkyNatureChoice,
        .index = NATURE_QUIRKY
    },
    {
        .label = sCancelNatureChoice,
        .index = NUM_NATURES
    }
};

static const struct ListMenuTemplate sNaturesListMenuTemplate = 
{
    .items = sNaturesListMenuItems,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = NUM_NATURES + 1,
    .maxShowed = 6,
    .windowId = 0,
    .header_X = 2,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = FONT_NORMAL_COPY_2,
    .cursorKind = 0
};

static const struct WindowTemplate sNaturesMenuWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 12,
    .tilemapTop = 1,
    .width = 17,
    .height = 11,
    .paletteNum = 15,
    .baseBlock = 8
};

#define tMenuListTaskId     data[0]
#define tWindowId           data[1]

static void Task_HandleNatureSelectMenuInput(u8 taskId)
{
    u32 input = ListMenu_ProcessInput(gTasks[taskId].tMenuListTaskId);
    u32 closeMenu = FALSE;

    if (gMain.newKeys & A_BUTTON)
    {
        gSpecialVar_Result = input;
        closeMenu = TRUE;
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        gSpecialVar_Result = NUM_NATURES;
        closeMenu = TRUE;
    }

    if (closeMenu)
    {
        DestroyListMenuTask(gTasks[taskId].tMenuListTaskId, NULL, NULL);
        ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
        RemoveWindow(gTasks[taskId].tWindowId);
        DestroyTask(taskId);
        ScriptContext_Enable();
    }
}

void NatureSelectMenu(void)
{
    u32 windowId;
    u32 listMenuTaskId;
    u32 naturesMenuTaskId;
    s16 *data;

    windowId = AddWindow(&sNaturesMenuWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    gMultiuseListMenuTemplate = sNaturesListMenuTemplate;
    gMultiuseListMenuTemplate.windowId = windowId;
    listMenuTaskId = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    naturesMenuTaskId = CreateTask(Task_HandleNatureSelectMenuInput, 3);
    data = gTasks[naturesMenuTaskId].data;
    tMenuListTaskId = listMenuTaskId;
    tWindowId = windowId;
}

#undef tMenuListTaskId
#undef tWindowId

extern const u8 *const gNatureNamePointers[];

void BufferSelectedMonDataForNatureChange(void)
{
    struct Pokemon *mon = &gPlayerParty[gSpecialVar_0x800A];
    u32 nature = GetNature(mon);

    GetMonData(mon, MON_DATA_NICKNAME, gStringVar1);
    StringGet_Nickname(gStringVar1);
    StringCopy(gStringVar2, gNatureNamePointers[nature]);
}

void PerformNatureChange(void)
{
    struct Pokemon *mon = &gPlayerParty[gSpecialVar_0x800A];
    u32 newNature = gSpecialVar_0x8004;

    gSpecialVar_Result = SetNature(mon, newNature);
    StringCopy(gStringVar2, gNatureNamePointers[newNature]);
    PlayFanfare(FANFARE_LEVEL_UP);
}
