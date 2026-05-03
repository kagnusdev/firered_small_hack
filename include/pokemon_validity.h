#ifndef     GUARD_POKEMON_VALIDITY_H
# define    GUARD_POKEMON_VALIDITY_H

struct LegalityCheckCtx
{
    struct Pokemon  *mon;
    u32             personality;
    u32             monIVsHalves[2];
    bool32          valid;
    u8 const *      error;
};

typedef void (*SearchCB)(struct LegalityCheckCtx *ctx, u32 seed);

void CheckSelectedMonLegality(void);

#endif /*  !GUARD_POKEMON_VALIDITY_H */
