/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "keymap_german.h"
#include "sendstring_german.h"

// layers
#define _BA	 0	//base
#define _LS	 1	//left shift
#define _RS	 2	//right shift
#define _Y1	 3	//symbols 1
#define _Y2	 4	//symbols 2
#define _Y3	 5	//symbols 3
#define _FS	10	//frames
#define _FD	11	//double frames
#define _AS	12	//arrows
#define _AD	13	//double arrows
#define _NV	14	//navigation
#define _CF	15	//keyboard config

/*
// Shift + ? = !
// Does not require a shift layer.
// But now that we use a shift layer (LM(_SH, MOD_LSFT)) to get to the double-shift layer (_Y2),
// a key_override is no longer necessary.
const key_override_t **key_overrides = (const key_override_t *[]) {
	&ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, DE_QUES, DE_EXLM, 1, ~MOD_MASK_SHIFT)
};
*/

layer_state_t layer_state_set_user(layer_state_t state) {
	state = update_tri_layer_state(state, _LS, _RS, _Y2);

	// Similar to [https://github.com/qmk/qmk_firmware/issues/2782]:
	// Layer _Y2 is selected via LM(.., MOD_LSFT). To get unshifted symbols, we need to clear shift.
	// (Replacing MO(_LS) with LM(_LS, 0) in base layer did not help.)
	if (IS_LAYER_ON_STATE(state, _Y2)) { del_mods(MOD_LSFT); }

	return state;
}

enum custom_keycodes {
    CK_NEQ = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case CK_NEQ:
        if (record->event.pressed) { SEND_STRING("!="); }
        break;
    }
    return true;
};

#define SPC       KC_SPC
#define KM_CUT    LSFT(KC_DEL)
#define KM_COPY   LCTL(KC_INS)
#define KM_PAST   LSFT(KC_INS)

enum unicode_names {
	  BSLS, NOT
	, GRV, ACUT, CIRC, DEGR
	, DGRV, DACUT, DCIRC, DDEGR, DDIA, DTILD, DCEDI
	, SUP0, SUP1, SUP2, SUP3, SUP4, SUP5, SUP6, SUP7, SUP8, SUP9, SUPN
	, SUB0, SUB1, SUB2, SUB3, SUB4, SUB5, SUB6, SUB7, SUB8, SUB9, SUBN
	, ARR_W, ARR_N, ARR_E, ARR_S, ARR_WE, ARR_NS, ARR_NW, ARR_NE, ARR_SE, ARR_SW
	, ARR_W2, ARR_N2, ARR_E2, ARR_S2, ARR_WE2, ARR_NS2, ARR_NW2, ARR_NE2, ARR_SE2, ARR_SW2
	, FRM_W, FRM_N, FRM_E, FRM_S, FRM_WE, FRM_NS, FRM_NW, FRM_NE, FRM_SE, FRM_SW, FRM_CR, FRM_HL, FRM_VL
	, FRM_W2, FRM_N2, FRM_E2, FRM_S2, FRM_WE2, FRM_NS2, FRM_NW2, FRM_NE2, FRM_SE2, FRM_SW2, FRM_CR2, FRM_HL2, FRM_VL2
};
const uint32_t PROGMEM unicode_map[] = {
	 [BSLS]	= 0x005c	// backslash
	,[GRV]	= 0x0060	// `
	,[ACUT]	= 0x00b4	// ´
	,[CIRC]	= 0x005e	// ^
	,[DEGR]	= 0x00b0	// °
	,[DGRV]	= 0x0300	// dead `
	,[DACUT]	= 0x0301	// dead ´
	,[DCIRC]	= 0x0302	// dead ^
	,[DDEGR]	= 0x030a	// dead °
	,[DTILD]	= 0x0303	// dead ~
	,[NOT]	= 0x00ac	// negation
	//superscript digits and n
	,[SUP0]	= 0x2070 ,[SUP1] = 0x00b9 ,[SUP2] = 0x00b2 ,[SUP3] = 0x00b3 ,[SUP4] = 0x2074 ,[SUP5] = 0x2075 ,[SUP6] = 0x2076 ,[SUP7] = 0x2077 ,[SUP8] = 0x2078 ,[SUP9] = 0x2079, [SUPN] = 0x207f
	//subscript digits and n
	,[SUB0]	= 0x2080 ,[SUB1] = 0x2081 ,[SUB2] = 0x2082 ,[SUB3] = 0x2083 ,[SUB4] = 0x2084 ,[SUB5] = 0x2085 ,[SUB6] = 0x2086 ,[SUB7] = 0x2087 ,[SUB8] = 0x2088 ,[SUB9] = 0x2089, [SUBN] = 0x2099
	//arrows
	,[ARR_W]	= 0x2190, [ARR_N] = 0x2191, [ARR_E] = 0x2192, [ARR_S] = 0x2193, [ARR_WE] = 0x2194, [ARR_NS] = 0x2195, [ARR_NW] = 0x2196, [ARR_NE] = 0x2197, [ARR_SE] = 0x2198, [ARR_SW] = 0x2199
	//double arrows
	,[ARR_W2]	= 0x21d0, [ARR_N2] = 0x21d1, [ARR_E2] = 0x21d2, [ARR_S2] = 0x21d3, [ARR_WE2] = 0x21d4, [ARR_NS2] = 0x21d5, [ARR_NW2] = 0x21d6, [ARR_NE2] = 0x21d7, [ARR_SE2] = 0x21d8, [ARR_SW2] = 0x21d9
	//frames
	,[FRM_W]	= 0x251c, [FRM_N] = 0x252c, [FRM_E] = 0x2524, [FRM_S] = 0x2534, [FRM_NW] = 0x250c, [FRM_NE] = 0x2510, [FRM_SE] = 0x2518, [FRM_SW] = 0x2514, [FRM_CR] = 0x253c, [FRM_HL] = 0x2500, [FRM_VL] = 0x2502
	//double frames
	,[FRM_W2]	= 0x2560, [FRM_N2] = 0x2566, [FRM_E2] = 0x2563, [FRM_S2] = 0x2569, [FRM_NW2] = 0x2554, [FRM_NE2] = 0x2557, [FRM_SE2] = 0x255d, [FRM_SW2] = 0x255a, [FRM_CR2] = 0x256c, [FRM_HL2] = 0x2550, [FRM_VL2] = 0x2551
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BA] = LAYOUT( //base------------------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 KC_PAUS      ,KC_1         ,KC_2         ,KC_3         ,KC_4         ,KC_5         ,KC_6         ,KC_7       ,KC_8         ,KC_9         ,KC_0         ,KC_PSCR      ,KM_CUT       ,(       KM_COPY     )      ,(      KM_PAST      )
,KC_F1        ,KC_F2        ,KC_F3        ,KC_F4        ,KC_F5        ,KC_F6                                  ,KC_F7        ,KC_F8        ,KC_F9        ,KC_F10       ,KC_F11       ,KC_F12       ,KC_DEL       ,(      KC_INS       )
,DE_CIRC      ,LT(_FD,KC_X) ,LT(_FS,KC_V) ,LT(_AD,SPC)  ,LT(_AS,KC_C) ,KC_W                                   ,KC_K         ,KC_H         ,KC_G         ,KC_F         ,DE_Y         ,KC_CAPS      ,KC_HOME      ,KC_END       ,KC_PGUP
,TT(_Y1)      ,LGUI_T(KC_U) ,LALT_T(KC_I) ,LCTL_T(KC_A) ,LT(_NV,KC_E) ,KC_O               ,DE_QUES            ,KC_S         ,LT(_NV,KC_N) ,RCTL_T(KC_R) ,LALT_T(KC_T) ,RGUI_T(KC_D) ,TT(_Y1)     ,(      KC_SPC       )      ,KC_PGDN
,KC_ESC       ,DE_MINS      ,DE_SLSH      ,KC_L         ,KC_P         ,DE_Z               ,KC_LEAD            ,KC_B         ,KC_M         ,DE_COMM      ,DE_DOT       ,KC_J         ,KC_Q                       ,KC_UP
,KC_APP       ,KC_LWIN      ,(      KC_BSPC      )      ,(  LM(_LS,MOD_LSFT) )      ,KC_TAB       ,KC_ENTER   ,( LM(_RS,MOD_LSFT)  )      ,(      TT(_NV)       )     ,KC_LALT      ,KC_LCTL      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT
), [_Y1] = LAYOUT( //symbols 1----------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 _______      ,X(SUP1)      ,X(SUP2)      ,X(SUP3)      ,X(SUP4)      ,X(SUP5)      ,X(SUP6)      ,X(SUP7)    ,X(SUP8)      ,X(SUP9)      ,X(SUP0)      ,X(SUPN)      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,DE_PERC      ,DE_AMPR      ,DE_DQUO      ,DE_QUOT      ,DE_GRV                                 ,DE_HASH      ,DE_LCBR      ,DE_RCBR      ,DE_PIPE      ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,DE_UDIA      ,_______      ,DE_ADIA      ,DE_AT        ,DE_ODIA            ,_______            ,DE_ASTR      ,DE_LPRN      ,DE_RPRN      ,DE_TILD      ,DE_DLR       ,_______      ,(      _______      )      ,_______
,_______      ,DE_PLUS      ,DE_EQL       ,DE_LABK      ,DE_RABK      ,DE_SS              ,_______            ,_______      ,DE_LBRC      ,DE_RBRC      ,_______      ,DE_SECT      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      MO(_Y3)      )      ,_______      ,_______    ,(      MO(_Y3)      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_Y2] = LAYOUT( //symbols 2----------------------------------------------------------------|------------------------------------------------------------------------------------------------|---------------------------------------
 _______      ,X(SUB1)      ,X(SUB2)      ,X(SUB3)      ,X(SUB4)      ,X(SUB5)      ,X(SUB6)      ,X(SUB7)    ,X(SUB8)      ,X(SUB9)      ,X(SUB0)      ,X(SUBN)      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,DE_ACUT      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,DE_SLSH      ,KC_7         ,KC_8         ,KC_9         ,DE_COLN      ,_______      ,_______      ,_______      ,_______
,_______      ,S(DE_UDIA)   ,_______      ,S(DE_ADIA)   ,DE_EURO      ,S(DE_ODIA)         ,_______            ,DE_ASTR      ,KC_4         ,KC_5         ,KC_6         ,DE_DOT       ,_______      ,(      _______      )      ,_______
,_______      ,DE_MINS      ,CK_NEQ       ,_______      ,DE_PLUS      ,RSA(DE_SS)         ,_______            ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,DE_COMM      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      XXXXXXX      )      ,_______      ,_______    ,(      XXXXXXX      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_Y3] = LAYOUT( //symbols 3----------------------------------------------------------------|------------------------------------------------------------------------------------------------|---------------------------------------
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,X(DCIRC)     ,_______      ,_______      ,X(DDIA)      ,X(DACUT)     ,X(DGRV)                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,X(DDEGR)           ,_______            ,_______      ,X(NOT)       ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,X(DCEDI)     ,_______      ,_______      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_LS] = LAYOUT( //left shift---------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
,MO(_Y3)      ,_______      ,_______      ,_______      ,_______      ,_______            ,DE_EXLM            ,_______      ,_______      ,_______      ,_______      ,_______      ,MO(_Y3)      ,(      _______      )      ,_______
,_______      ,DE_UNDS      ,DE_BSLS      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      MO(_RS)      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_RS] = LAYOUT( //right shift--------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
,MO(_Y3)      ,_______      ,_______      ,_______      ,_______      ,_______            ,DE_EXLM            ,_______      ,_______      ,_______      ,_______      ,_______      ,MO(_Y3)      ,(      _______      )      ,_______
,_______      ,DE_UNDS      ,DE_BSLS      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      MO(_LS)      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_AS] = LAYOUT( //arrows-------------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(ARR_NW)    ,X(ARR_N)     ,X(ARR_NE)    ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_W)     ,X(ARR_S)     ,X(ARR_E)     ,X(ARR_NS)    ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_SW)    ,X(ARR_S)     ,X(ARR_SE)    ,X(ARR_WE)    ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_AD] = LAYOUT( //double arrows------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(ARR_NW2)   ,X(ARR_N2)    ,X(ARR_NE2)   ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_W2)    ,X(ARR_S2)    ,X(ARR_E2)    ,X(ARR_NS2)   ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_SW2)   ,X(ARR_S2)    ,X(ARR_SE2)   ,X(ARR_WE2)   ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_FS] = LAYOUT( //frames-------------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(FRM_NW)    ,X(FRM_N)     ,X(FRM_NE)    ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_W)     ,X(FRM_CR)    ,X(FRM_E)     ,X(FRM_VL)    ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_SW)    ,X(FRM_S)     ,X(FRM_SE)    ,X(FRM_HL)    ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_FD] = LAYOUT( //double frames------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(FRM_NW2)   ,X(FRM_N2)    ,X(FRM_NE2)   ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_W2)    ,X(FRM_CR2)   ,X(FRM_E2)    ,X(FRM_VL2)   ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_SW2)   ,X(FRM_S2)    ,X(FRM_SE2)   ,X(FRM_HL2)   ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_NV] = LAYOUT( //navigation & keyboard settings-------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 DM_RSTP      ,DM_REC1      ,DM_REC2      ,RGB_TOG      ,RGB_MOD      ,RGB_HUI      ,RGB_VAD      ,RGB_VAI    ,RGB_SPD      ,RGB_SPI      ,_______      ,KC_SLCK      ,RESET        ,(      NK_TOGG      )      ,(      EEP_RST      )
,LWIN(KC_1)   ,LWIN(KC_2)   ,LWIN(KC_3)   ,LWIN(KC_4)   ,LWIN(KC_5)   ,LWIN(KC_6)                             ,LWIN(KC_7)   ,LWIN(KC_8)   ,LWIN(KC_9)   ,LWIN(KC_0)   ,_______      ,_______      ,_______      ,(      UC_MOD       )
,_______      ,C(KC_6)      ,C(KC_7)      ,C(KC_8)      ,C(KC_9)      ,C(KC_0)                                ,C(KC_HOME)   ,KC_HOME      ,KC_UP        ,KC_END       ,C(KC_END)    ,_______      ,KC_WWW_HOME  ,KC_MSEL      ,KC_VOLU
,_______      ,KC_LWIN      ,KC_LALT      ,KC_LCTL      ,_______      ,_______            ,DM_PLY1            ,S(KC_INS)    ,KC_LEFT      ,KC_DOWN      ,KC_RGHT      ,KC_PGUP      ,_______      ,(      KC_MUTE      )      ,KC_VOLD
,C(KC_PLUS)   ,C(KC_1)      ,C(KC_2)      ,C(KC_3)      ,C(KC_4)      ,C(KC_5)            ,DM_PLY2            ,KC_INS       ,C(KC_LEFT)   ,KC_DEL       ,C(KC_RGHT)   ,KC_PGDN      ,_______                    ,KC_MSTP
,C(KC_MINUS)  ,KC_RWIN      ,(      _______      )      ,(      _______      )      ,KC_CAPS      ,KC_APP     ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,KC_MPRV      ,KC_MPLY      ,KC_MNXT
/*
) ,[_CF] = LAYOUT( //keyboard settings--------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 RESET        ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,KC_CALC      ,KC_MYCM      ,KC_MSEL      ,KC_MAIL      ,(      NK_TOGG       )     ,(      EEP_RST      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      KC_NLCK      )
,RGB_TOG      ,RGB_MOD      ,RGB_VAI      ,RGB_HUI      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,KC_HOME
,_______      ,RGB_SPD      ,RGB_VAD      ,RGB_SPI      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,KC_END
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,KC_MUTE                    ,KC_VOLU
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,KC_MPLY      ,KC_MPRV      ,KC_VOLD      ,KC_MNXT
*/
)};

/* template
) ,[_X] = LAYOUT( //--------------------------------------------------------------------------|------------------------------------------------------------------------------------------------|--------------------------------------
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
*/
