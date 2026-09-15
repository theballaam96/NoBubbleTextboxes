#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"

#define TEXTBOX_HALF_WIDTH 100
#define TEXTBOX_HALF_HEIGHT 20
#define TEXTBOX_WIDTH (TEXTBOX_HALF_WIDTH << 1)
#define TEXTBOX_HEIGHT (TEXTBOX_HALF_HEIGHT << 1)

Gfx* drawScreenRect(Gfx* dl, s32 x1, s32 y1, s32 x2, s32 y2, u8 red, u8 green, u8 blue, u8 alpha) {
    u16 color;

	gDPPipeSync(dl++);
	gDPSetCycleType(dl++, G_CYC_1CYCLE);
	gDPSetRenderMode(dl++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPSetCombineMode(dl++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(dl++, 0, 0, red, green, blue, alpha);
	gDPFillRectangle(dl++, x1, y1, x2, y2);
    gDPPipeSync(dl++);
	return dl;
}
typedef struct HUDDisplay {
	/* 0x000 */ u16* actual_count_pointer;
	/* 0x004 */	u16 hud_count;
	/* 0x006 */	u8 freeze_timer;
	/* 0x007 */	u8 counter_timer;
	/* 0x008 */	s32 screen_x;
	/* 0x00C */	s32 screen_y;
	/* 0x010 */ f32 unk_10;
    /* 0x014 */ f32 unk_14;
    /* 0x018 */ f32 unk_18;
    /* 0x01C */ u8 unk_1c;
    /* 0x01D */ u8 unk_1d;
    /* 0x01E */ u8 unk_1e;
    /* 0x01F */ u8 unk_1f;
	/* 0x020 */ u32 hud_state; // 0 = invisible, 1 = appearing, 2 = visible, 3 = disappearing
	/* 0x024 */ s32 unk_24;
	/* 0x028 */	void* counter_pointer;
	/* 0x02C */ u8 unk_2c; // Infinites?
    /* 0x02D */ u8 unk_2d; // Infinites?
    /* 0x02E */ u8 unk_2e;
    /* 0x02F */ u8 unk_2f;
} HUDDisplay;

typedef struct {
    // TODO: Union with friendly field names?
    // TODO: Enum with indexes?
    // 0 = Coloured Banana
    // 1 = Banana Coin
    // 2 = ???
    // 3 = ???
    // 4 = ???
    // 5 = Crystal Coconut
    // 6 = ???
    // 7 = ???
    // 8 = GB Count (Character)
    // 9 = ???
    // 10 = Banana Medal
    // 11 = ???
    // 12 = Blueprint
    // 13 = Coloured Banana?
    // 14 = Banana Coin?
    HUDDisplay hud_item[15];
} PlayerHUD;


void func_global_asm_806A4B84(f32, Actor*, AAD_global_asm_806A4DDC*, Struct806A57C0_2*, u8, u8*);
void func_global_asm_8070D9AC(Struct806A57C0_2 *arg0);
void func_global_asm_8070E808(Actor *arg0, Actor *arg1);
extern PlayerHUD *D_global_asm_80754280;

RECOMP_PATCH void func_global_asm_806A4DDC(Actor *arg0) {
    AAD_global_asm_806A4DDC *aaD;
    Struct806A57C0_2 *var_s1;
    s32 pad2;
    u8 sp103;
    f32 var_f22;
    f32 var_f0;
    f64 temp_f0;
    f64 temp_f2;
    s32 var_s0;
    u8 var_v0;
    void *temp_a0;
    void *temp_a3;
    u8 cond;
    s32 pad[0x1E];
    f32 temp_f2_5;

    aaD = arg0->additional_actor_data;
    arg0->x_position = 160;
    arg0->y_position = character_change_array->unk270[3] - 40;
    if (D_global_asm_80754280) {
        D_global_asm_80754280->hud_item[9].hud_state = 0;
    }

    var_s1 = aaD->unkC;
    sp103 = FALSE;
    var_f0 = (1.0f - aaD->unk20) * 1.47058823529411775;
    if (var_f0 > 1.0f) {
        var_f0 = 1.0f;
    }
    aaD->unk40 = var_f0;
    aaD->unk44 = arg0->x_position - TEXTBOX_HALF_WIDTH;
    aaD->unk48 = arg0->y_position - TEXTBOX_HALF_HEIGHT;
    aaD->unk4C = arg0->x_position + TEXTBOX_HALF_WIDTH;
    aaD->unk50 = arg0->y_position + TEXTBOX_HALF_HEIGHT;

    aaD->unk44 = MAX(aaD->unk44, character_change_array->unk270[0]);
    aaD->unk48 = MAX(aaD->unk48, character_change_array->unk270[1]);
    aaD->unk4C = MIN(aaD->unk4C, character_change_array->unk270[2]);
    aaD->unk50 = MIN(aaD->unk50, character_change_array->unk270[3]);

    var_f22 = aaD->unk14;
    for (var_s0 = 0; (var_s0 <= aaD->unk10) && (var_s1) && (!sp103); var_s0++) {
        temp_f2_5 = var_f22 + var_s1->unk18;
        cond = var_s0 == aaD->unk10;
        if ((TEXTBOX_HEIGHT < temp_f2_5) && (arg0->control_state == 1)) {
            arg0->control_state = 2;
            aaD->unk18 = temp_f2_5 - TEXTBOX_HEIGHT;
            sp103 = TRUE;
        } else if ((temp_f2_5 < 0.0f) && (arg0->control_state == 2)) {
            temp_a0 = var_s1;
            var_s0 -= 1;
            var_f22 = temp_f2_5;
            aaD->unk14 += var_s1->unk18;
            aaD->unkC = var_s1->next;
            aaD->unk10--;
            var_s1 = var_s1->next;
            func_global_asm_8070D9AC(temp_a0);
        } else {
            func_global_asm_806A4B84(var_f22, arg0, aaD, var_s1, cond, &sp103);
            if (sp103 == 0) {
                var_s1 = var_s1->next;
                var_f22 = temp_f2_5;
            }
        }
    }
    if (arg0->control_state == 3) {
        if (aaD->unk12 != 0) {
            aaD->unk12--;
        } else {
            arg0->control_state = 1;
        }
    }
    if ((arg0->control_state == 1) && (var_s1 == NULL)) {
        func_global_asm_8070E808(arg0, aaD->unk4);
    }
}

Gfx *displayImage(Gfx *dl, u16 textureIndex, s32 arg3, s32 codec, s32 width, s32 height, s16 x, s16 y, f32 xScale, f32 yScale, s32 arg11, f32 arg12);
Gfx *drawBubbleSides(Gfx *dl, u16 image, u8 opacity, f32 x, f32 y) {
    void *texture;

    gDPPipeSync(dl++);
    gDPSetPrimColor(dl++, 0, 0, 0x00, 0x00, 0x00, opacity);
    dl = displayImage(dl, image, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, x * 4, y * 4, 2.5f, 2.5f, 0, 0);
    gDPPipeSync(dl++);
    return dl;
}

void *_malloc(s32);
void func_global_asm_8061134C(void *);
void func_global_asm_806A3B78(Gfx **arg0, AAD_global_asm_806A4DDC *arg1, Struct806A57C0_2 *arg2, u8 arg3, u8 *arg4);
extern Gfx** D_1000118;
extern Mtx D_20000C0;
Gfx *printStyledText(Gfx *dl, s16 style, s16 x, s16 y, u8 *string, u32 extraBitfield);

typedef struct {
    s32 id;
    u8 images_per_frame_horizontal;
    u8 images_per_frame_vertical;
    u8 unk6;
    u8 codec;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 table;
    s16 width;
    s16 height;
    s16 image_count;
    s16 images[1]; // TODO: How many elements? m2c doesn't support VLAs
} SpriteData;

typedef struct TextboxHeadStruct {
    Actors actor_type;
    SpriteData *sprite;
} TextboxHeadStruct;

extern SpriteData D_global_asm_80721238;
extern SpriteData D_global_asm_8072121C;
extern SpriteData D_global_asm_80721250;
extern SpriteData D_global_asm_80721200;
extern SpriteData D_global_asm_807214A0;
extern SpriteData D_global_asm_807214B8;
extern SpriteData D_global_asm_807214D0;
extern SpriteData D_global_asm_807214E8;
extern SpriteData D_global_asm_80721500;
extern SpriteData D_global_asm_8072145C;
extern SpriteData D_global_asm_80721094;
extern SpriteData D_global_asm_80721530;
const TextboxHeadStruct textbox_heads[] = {
    { ACTOR_SNIDE, &D_global_asm_80721238 },
    { ACTOR_CRANKY, &D_global_asm_8072121C },
    { ACTOR_FUNKY, &D_global_asm_80721250 },
    { ACTOR_CANDY, &D_global_asm_80721200 },
    { ACTOR_CUTSCENE_DK, &D_global_asm_807214A0 },
    { ACTOR_CUTSCENE_DIDDY, &D_global_asm_807214B8 },
    { ACTOR_CUTSCENE_LANKY, &D_global_asm_807214D0 },
    { ACTOR_CUTSCENE_TINY, &D_global_asm_807214E8 },
    { ACTOR_CUTSCENE_CHUNKY, &D_global_asm_80721500 },
    { ACTOR_SCOFF, &D_global_asm_8072145C },
    { ACTOR_FAIRY, &D_global_asm_80721094 },
    { ACTOR_FAIRY_QUEEN, &D_global_asm_80721094 },
    // { ACTOR_WRINKLY, &D_global_asm_80721530 },
};

SpriteData *getSpriteForTextbox(Actor *ac) {
    Actors actor_type;
    u32 i;
    
    if (!ac) return NULL;
    
    actor_type = ac->unk58;
    for (i = 0; i < sizeof(textbox_heads)/sizeof(TextboxHeadStruct); i++) {
        if (textbox_heads[i].actor_type == actor_type) {
            return textbox_heads[i].sprite;
        }
    }
    return NULL;
}

void func_global_asm_8071495C(void);
// void func_global_asm_807149FC(s32 arg0);
void func_global_asm_807149B8(u8 arg0);
void func_global_asm_80714A28(u16 arg0);
void changeActorColor(u8 red, u8 green, u8 blue, u8 alpha);
Struct80717D84 *drawSpriteAtPosition(void *sprite, f32 scale, f32 x, f32 y, f32 z);

Struct80717D84 *drawTextboxSprite(SpriteData *arg0, s32 arg2, s32 arg3, f32 arg4, u8 opacity) {
    changeActorColor(0xFF, 0xFF, 0xFF, opacity);
    func_global_asm_8071495C();
    // func_global_asm_807149FC(-1);
    func_global_asm_807149B8(1);
    func_global_asm_80714A28(4);
    return drawSpriteAtPosition(arg0, arg4, arg2, arg3, -10.0f);
}



RECOMP_FORCE_PATCH Gfx* func_global_asm_806A4284(Gfx* dl, Actor* arg1) {
    u8 sp13B;
    u8 res;
    AAD_global_asm_806A4DDC* temp_s2;
    Struct806A57C0_2* var_s1;
    Gfx* dl_0;
    Gfx* dl_0_start;
    Gfx* text_dl_start;
    s32 i;
    u32 gfx_val;
    u16 left, right;
    SpriteData *sprite;
    u8 opacity, opacity_0;
    
    temp_s2 = arg1->AAD_as_array[0];
    sprite = getSpriteForTextbox(temp_s2->unk4);
    var_s1 = temp_s2->unkC;
    sp13B = 0;
    dl_0 = _malloc(0x5000); // @recomp: Boost to 0x5000 (from 0x4000) to account for mtx tagging
    func_global_asm_8061134C(dl_0);
    dl_0_start = dl_0;
    gSPDisplayList(dl_0++, &D_1000118);
    left = arg1->x_position - TEXTBOX_HALF_WIDTH;
    right = arg1->x_position + TEXTBOX_HALF_WIDTH;
    opacity = 0xC0 * temp_s2->unk20;
    opacity_0 = 0xFF * temp_s2->unk20;
    dl_0 = drawScreenRect(dl_0,
        left,
        MIN(arg1->y_position - TEXTBOX_HALF_HEIGHT, 240),
        right,
        MIN(arg1->y_position + TEXTBOX_HALF_HEIGHT, 240),
        0x00, 0x00, 0x00, opacity);
    gSPMatrix(dl_0++, &D_20000C0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gDPPipeSync(dl_0++);
    gDPSetRenderMode(dl_0++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCycleType(dl_0++, G_CYC_1CYCLE);
    gDPSetCombineMode(dl_0++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gSPLoadGeometryMode(dl_0++, 0);
    gSPSetGeometryMode(dl_0++, G_SHADE | G_SHADING_SMOOTH);
    gDPPipeSync(dl_0++);
    dl_0 = drawBubbleSides(dl_0, 0xB1, opacity, left - 9.25f, (arg1->y_position - TEXTBOX_HALF_HEIGHT) + 21);
    dl_0 = drawBubbleSides(dl_0, 0xB5, opacity, right + 10.75f, (arg1->y_position - TEXTBOX_HALF_HEIGHT) + 21);
    gDPSetScissor(dl_0++, G_SC_NON_INTERLACE,
        temp_s2->unk44,
        temp_s2->unk48,
        temp_s2->unk4C,
        temp_s2->unk50);
    text_dl_start = dl_0;
    for (i = 0; (i <= temp_s2->unk10) && (var_s1) && (!sp13B); i++) {
        gDPPipeSync(dl_0++);
        res = i == temp_s2->unk10;
        func_global_asm_806A3B78(&dl_0, temp_s2, var_s1, res, &sp13B);
        if (!sp13B) {
            var_s1 = var_s1->next;
        }
    }
    if (sprite) {
        // Display sprite
        drawTextboxSprite(sprite, left - 6, (arg1->y_position - TEXTBOX_HALF_HEIGHT) + 20, 0.34f, opacity_0);
    }
    // Overwrite text draw with color change
    while (text_dl_start < dl_0) {
        if (text_dl_start->words.w0 == 0xFA000000) {
            text_dl_start->words.w1 = 0xFFFFFF00 | (text_dl_start->words.w1 & 0xFF);
        }
        text_dl_start++;
    }
    gDPSetScissor(dl_0++, G_SC_NON_INTERLACE,
        character_change_array[0].unk270[0],
        character_change_array[0].unk270[1],
        character_change_array[0].unk270[2],
        character_change_array[0].unk270[3]);
    temp_s2->unk1C++;
    temp_s2->unk1C %= 16;
    gSPEndDisplayList(dl_0++);
    gSPDisplayList(dl++, dl_0_start);
    return dl;
}

RECOMP_CALLBACK("*", recomp_on_init)
void textboxInit(void)
{
    *(volatile f64*)(0x8075e4a0) = TEXTBOX_WIDTH;
    *(volatile f64*)(0x8075e4a8) = TEXTBOX_WIDTH;
    *(volatile f64*)(0x8075a790) = TEXTBOX_WIDTH;
}