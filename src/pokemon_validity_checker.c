#include "global.h"
#include "event_data.h"
#include "random.h"
#include "pokemon_validity.h"
#include "string_util.h"

extern const u8 *const gNatureNamePointers[];

static const u8 sUnseedablePersonality[] = _("Unseedable personality");
static const u8 sWrongIVsForPersonality[] = _("Wrong IVs for personality");

static const u8 sMethod1Or2[] = _("method 1 or 2");
static const u8 sMethod4[] = _("method 4");

static void SearchPersonalitySeed(u32 personality, struct LegalityCheckCtx *ctx, SearchCB cb)
{
    u32 high = personality >> 16;
    u32 seedMask = personality << 16;
    u32 seeker;

    for (seeker = 0; seeker <= 0xFFFF; seeker++)
    {
        u32 test = seeker | seedMask;

        if (NextRandom(&test) == high)
        {
            cb(ctx, seeker | seedMask);
        }
    }
}

static void SearchMethod4Seed(u32 ivs, struct LegalityCheckCtx *ctx, SearchCB cb)
{
    u32 high = ivs >> 16;
    u32 seedMask = ivs << 16;
    u32 seeker;
    for (seeker = 0; seeker <= 0xFFFF; seeker++)
    {
        u32 test = seeker | seedMask;

        NextRandom(&test);
        if (NextRandom(&test) == high)
        {
            cb(ctx, seeker | seedMask);
        }
    }
}

static bool32 PersonalityMatchesIvs(struct LegalityCheckCtx *ctx, u32 seed)
{
    u32 *monIVsHalves;
    u32 seedIVs[3];
    s32 i;

    monIVsHalves = ctx->monIVsHalves;

    DebugPrintf("monIVsHalves[0] = %u (0x%x)", monIVsHalves[0], monIVsHalves[0]);
    DebugPrintf("monIVsHalves[1] = %u (0x%x)", monIVsHalves[1], monIVsHalves[1]);

    if (NextRandom(&seed) != (ctx->personality >> 16))
    {
        return FALSE;
    }

    for (i = 0; i < ARRAY_COUNT(seedIVs); i++)
    {
        seedIVs[i] = NextRandom(&seed) & (MAX_IV_MASK | (MAX_IV_MASK << 5) | (MAX_IV_MASK << 10));
        DebugPrintf("seedIVs[%u] = %u (0x%x)", i, seedIVs[i], seedIVs[i]);
    }

    if (monIVsHalves[0] == seedIVs[0] && monIVsHalves[1] == seedIVs[1]) // method 1
    {
        DebugPrintf("Validity check passed by method 1");
        return TRUE;
    }
    if (monIVsHalves[0] == seedIVs[1] && monIVsHalves[1] == seedIVs[2]) // method 2
    {
        DebugPrintf("Validity check passed by method 2");
        return TRUE;
    }
    if (monIVsHalves[0] == seedIVs[0] && monIVsHalves[1] == seedIVs[2]) // method 4
    {
        DebugPrintf("Validity check passed by method 4");
        return TRUE;
    }

    return FALSE;
}

static void LegalityCheckCtx_Init(struct LegalityCheckCtx *this, struct Pokemon *mon)
{
    this->mon = mon;
    this->personality = GetMonData3(mon, MON_DATA_PERSONALITY, NULL);
    this->monIVsHalves[0] = GetMonData3(mon, MON_DATA_HP_IV, NULL) | (GetMonData3(mon, MON_DATA_ATK_IV, NULL) << 5) | (GetMonData3(mon, MON_DATA_DEF_IV, NULL) << 10);
    this->monIVsHalves[1] = GetMonData3(mon, MON_DATA_SPEED_IV, NULL) | (GetMonData3(mon, MON_DATA_SPATK_IV, NULL) << 5) | (GetMonData3(mon, MON_DATA_SPDEF_IV, NULL) << 10);
    //  hatched eggs are considered valid
    this->valid = GetMonData3(mon, MON_DATA_MET_LEVEL, NULL) == 0;
    if (this->valid)
    {
        this->error = NULL;
    }
    else
    {
        this->error = sUnseedablePersonality;
    }
}

static void LC_CB_MatchFound(struct LegalityCheckCtx *ctx, u32 seed)
{
    DebugPrintf("Personality %u (0x%x) half seeded by %u (0x%x)", ctx->personality, ctx->personality, seed, seed);
    if (ctx->valid) return;

    if (PersonalityMatchesIvs(ctx, seed))
    {
        ctx->valid = TRUE;
        ctx->error = NULL;
    }
    else
    {
        ctx->error = sWrongIVsForPersonality;
    }
}

static void LC_CB_IVsFound(struct LegalityCheckCtx *ctx, u32 seed)
{
    u32 high = ctx->personality >> 16;
    u32 values[3];
    u32 ogPersonality;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(values); i++)
    {
        values[i] = PrevRandom(&seed);
        if (values[i] == high)
        {
            ogPersonality = (high << 16) | PrevRandom(&seed);
            DebugPrintf("Found seed (%u [0x%x]) and personality (%u [0x%x]) matching IVs (%S), nature is %S", seed, seed, ogPersonality, ogPersonality, ctx->error, gNatureNamePointers[ogPersonality % NUM_NATURES]);
        }
    }
}

static void FindPersonalityFromIvs(struct LegalityCheckCtx *ctx)
{
    u32 *monIVsHalves = ctx->monIVsHalves;
    u8 const *backup = ctx->error;
    s32 i;

    for (i = 0; i < 4; i++)
    {
        u32 seed;
        u32 personality = ((i & 1) << 31) | (monIVsHalves[1] << 16) | ((i & 2) << 14) | monIVsHalves[0];
        ctx->error = sMethod1Or2;
        SearchPersonalitySeed(personality, ctx, LC_CB_IVsFound);
        ctx->error = sMethod4;
        SearchMethod4Seed(personality, ctx, LC_CB_IVsFound);
    }
    ctx->error = backup;
}

void CheckSelectedMonLegality(void)
{
    struct LegalityCheckCtx ctx;

    LegalityCheckCtx_Init(&ctx, &gPlayerParty[gSpecialVar_0x800A]);
    gSpecialVar_Result = FALSE;

    if (!ctx.valid)
        SearchPersonalitySeed(ctx.personality, &ctx, LC_CB_MatchFound);

    if (ctx.error == NULL)
    {
        gSpecialVar_Result = TRUE;
    }
    else
    {
        StringCopy(gStringVar2, ctx.error);
        FindPersonalityFromIvs(&ctx);
    }
}
