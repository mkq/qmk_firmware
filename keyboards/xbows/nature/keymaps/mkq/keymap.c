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
enum custom_layers {
	_BA = 0, //base
	_BT,	//like base, but tap only (no tap-hold) => hold can be used to repeat, when
	    	//TAPPING_FORCE_HOLD disables (tap, hold) repetition
	_L3,	//layer 3 (counting base as 1 and shift as 2; activation: both shift keys)
	_L4,	//layer 4
	_L5,	//layer 5 (activation: shift + layer 4)
	_FS,	//frames
	_FD,	//double frames
	_AS,	//arrows
	_AD,	//double arrows
	_NV,	//navigation
	_DC,	//LCTL + digits
	_DW,	//LWIN + digits
};
#define _DS   _L3	//double shift = _L3
#define _S_L4 _L5	//shift + layer 4 = layer 5

enum custom_keycodes {
	CK_NEQ = SAFE_RANGE,
	CK_SB,
	CK_QX,
	CK_LMRES
};

static layer_state_t prev_layer_state;
layer_state_t layer_state_set_user_impl(layer_state_t state) {
	// add LCTL modifier when layer _DC is active
	layer_state_t dc_mask = (layer_state_t) 1 << _DC;
	if ((state & dc_mask) == dc_mask) {
		add_mods(MOD_LCTL);
	} else if ((prev_layer_state & dc_mask) == dc_mask) {
		del_mods(MOD_LCTL);
	}

	// add LWIN modifier when layer _DW is active
	layer_state_t dw_mask = (layer_state_t) 1 << _DW;
	if ((state & dw_mask) == dw_mask) {
		add_mods(MOD_LGUI);
	} else if ((prev_layer_state & dw_mask) == dw_mask) {
		del_mods(MOD_LGUI);
	}

	// shift + layer _L4 => layer _S_L4
	// TODO Only works if shift is held before LT(_L4,..). When fixed, remove the two MO(_L5) from layer _L4.
	// Inspired by the update_tri_layer_state implementation.
	layer_state_t l4_mask = (layer_state_t) 1 << _L4;
	layer_state_t sh_l4_mask = (layer_state_t) 1 << _S_L4;
	bool shift = get_mods() & MOD_MASK_SHIFT;
	bool l4 = (state & l4_mask) == l4_mask;
	if (l4 && shift) {
		del_mods(MOD_MASK_SHIFT);
		return state | sh_l4_mask;
	} else {
		return state & ~sh_l4_mask;
	}
}
layer_state_t layer_state_set_user(layer_state_t state) {
	layer_state_t new_layer_state = layer_state_set_user_impl(state);
	prev_layer_state = new_layer_state;
	return new_layer_state;
}

// process_record_user implementation for a mod-sensitive custom key
bool pru_mod_sensitive_key(keyrecord_t *record, uint16_t mod_mask, uint16_t keycode, uint16_t shift_keycode) {
	if (record->event.pressed) {
		uint16_t mods = get_mods();
		if (!(mods & mod_mask)) {	// without modifier
			tap_code16(keycode);
		} else {	// with modifier
			del_mods(mod_mask);
			tap_code16(shift_keycode);
			set_mods(mods);
		}
	}
	return false;
}

static uint8_t shiftCount;
bool process_record_user_impl(uint16_t keycode, keyrecord_t *record) {
	bool pressed = record->event.pressed;
	switch (keycode) {
	case CK_LMRES:	// layer and modifier reset
		clear_keyboard();
		return false;
	case CK_SB:	// DE slash; with shift: DE backslash (but without shift (for layouts where that would give a capital sharp s))
		return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, DE_SLSH, RALT(DE_BSLS));
	case CK_QX:	// DE question mark; with shift: DE exclamation mark
		return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, S(DE_QUES), DE_EXLM);
	case CK_NEQ:
		if (pressed) { SEND_STRING("!="); }
		return false;
	case KC_LSFT:	// double shift => release shift, activate layer _DS
	case KC_RSFT:
		shiftCount += pressed ? 1 : -1;
		switch (shiftCount) {
		case 0:
			del_mods(MOD_MASK_SHIFT);
			return true;
		case 1:
			if (pressed) {	// previous shiftCount was 0 => normal shift behavior
				return true;
			} else {	// previous shiftCount was 2 => switch back from layer _DS to shift
				add_mods(MOD_MASK_SHIFT);
				layer_off(_DS);
				return false;
			}
		default:	// double shift => release shift, activate layer _DS
			del_mods(MOD_MASK_SHIFT);
			layer_on(_DS);
			return false;
		}
	}
	return true;
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
/*
	layer_state_t dw_mask = (layer_state_t) 1 << _DW;
	if ((state & dw_mask) == dw_mask) {
		add_mods(MOD_LCTL);
	} else if ((prev_layer_state & dw_mask) == dw_mask) {
		del_mods(MOD_LCTL);
	}
*/
	bool result = process_record_user_impl(keycode, record);
	return result;
}

enum unicode_names {
	  NOT
	, DGRV, DACUT, DCIRC, DDEGR, DDIA, DTILD, DCEDI
	, SUP0, SUP1, SUP2, SUP3, SUP4, SUP5, SUP6, SUP7, SUP8, SUP9, SUPN
	, SUB0, SUB1, SUB2, SUB3, SUB4, SUB5, SUB6, SUB7, SUB8, SUB9, SUBN
	, ARR_W, ARR_N, ARR_E, ARR_S, ARR_WE, ARR_NS, ARR_NW, ARR_NE, ARR_SE, ARR_SW
	, ARR_W2, ARR_N2, ARR_E2, ARR_S2, ARR_WE2, ARR_NS2, ARR_NW2, ARR_NE2, ARR_SE2, ARR_SW2
	, FRM_W, FRM_N, FRM_E, FRM_S, FRM_WE, FRM_NS, FRM_NW, FRM_NE, FRM_SE, FRM_SW, FRM_CR, FRM_HL, FRM_VL
	, FRM_W2, FRM_N2, FRM_E2, FRM_S2, FRM_WE2, FRM_NS2, FRM_NW2, FRM_NE2, FRM_SE2, FRM_SW2, FRM_CR2, FRM_HL2, FRM_VL2
};
const uint32_t PROGMEM unicode_map[] = {
	 [NOT]	= 0x00ac	// negation
//	,[BSLS]	= 0x005c	// backslash
//	,[GRV]	= 0x0060	// `
//	,[ACUT]	= 0x00b4	// ´
//	,[CIRC]	= 0x005e	// ^
//	,[DEGR]	= 0x00b0	// °
	,[DGRV]	= 0x0300	// dead `
	,[DACUT]	= 0x0301	// dead ´
	,[DCIRC]	= 0x0302	// dead ^
	,[DDEGR]	= 0x030a	// dead °
	,[DDIA]	= 0x0308	// dead diaresis
	,[DTILD]	= 0x0303	// dead ~
	,[DCEDI]	= 0x0327	// dead ,
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

// keycode aliases
#define SPC       KC_SPC
#define ESC       KC_ESC
#define DE_PL     DE_PLUS
#define DE_MI     DE_MINS
#define DE_CC     DE_CIRC
#define KC_CAP    KC_CAPS
#define LW_T(x)   LWIN_T(x)
#define RW_T(x)   RWIN_T(x)
#define KM_CUT    LSFT(KC_DEL)
#define KM_COPY   LCTL(KC_INS)
#define KM_PAST   LSFT(KC_INS)
// - not as readabe as _______, but editor navigation-friendlier
#define x         _______
#define x______   _______
// - temporary key codes for testing
#define TEST_1    KC_A
#define TEST_2    KC_B
#define TEST_3    KC_C
#define TEST_4    KC_D

// ____________________ layers ____________________
// Row label comments: N = numbers; F = function keys; H = home; T = thumb
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BA] = LAYOUT( //base **********| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ KC_PSCR      ,KC_1         ,KC_2         ,KC_3         ,KC_4         ,KC_5         ,KC_6         ,KC_7       ,KC_8         ,KC_9         ,KC_0         ,RESET        ,KM_CUT       ,(       KM_COPY     )      ,(      KM_PAST      )
/*F*/,KC_F1        ,LT(_FS,KC_F2),LT(_FD,KC_F3),LT(_AS,KC_F4),LT(_AD,KC_F5),KC_F6                                  ,KC_F7        ,KC_F8        ,KC_F9        ,KC_F10       ,KC_F11       ,KC_F12       ,KC_DEL       ,(      KC_INS       )
/* */,DE_CC        ,KC_X         ,KC_V         ,LT(_L4,SPC)  ,KC_C         ,KC_W                                   ,KC_K         ,KC_H         ,LT(_L4,KC_G) ,KC_F         ,DE_Y         ,SFT_T(KC_CAP),KC_HOME      ,KC_END       ,KC_PGUP
/*H*/,KC_TAB       ,LWIN_T(KC_U) ,LALT_T(KC_I) ,LCTL_T(KC_A) ,LT(_NV,KC_E) ,KC_O               ,LCAG_T(KC_APP)     ,KC_S         ,LT(_NV,KC_N) ,RCTL_T(KC_R) ,LALT_T(KC_T) ,RWIN_T(KC_D) ,CK_SB        ,(      KC_SPC       )      ,KC_PGDN
/* */,DE_PLUS      ,LT(_DW,DE_MI),CK_QX        ,LT(_DC,KC_L) ,KC_P         ,DE_Z               ,DE_SECT            ,KC_B         ,KC_M         ,DE_COMM      ,DE_DOT       ,KC_J         ,KC_Q                       ,KC_UP
/*T*/,KC_LWIN      ,KC_DEL       ,(      KC_BSPC      )      ,(      KC_LSFT      )      ,LT(_BT,ESC)  ,KC_ENTER   ,(     KC_RSFT      )       ,(      MO(_NV)       )     ,KC_LALT      ,KC_LCTL      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT
), [_BT] = LAYOUT( //base, tap only ***********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x            ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(       x______     )      ,(      x______      )
/*F*/,x            ,KC_F2        ,KC_F3        ,KC_F4        ,KC_F5        ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x            )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,KC_H         ,x            ,x            ,x            ,KC_CAP       ,x            ,x            ,x______
/*H*/,KC_TAB       ,KC_U         ,KC_I         ,KC_A         ,KC_E         ,x                  ,KC_APP             ,x            ,KC_N         ,KC_R         ,KC_T         ,KC_D         ,DE_MI        ,(      x            )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,x            ,x            ,x            ,x            ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      KC_LSFT      )      ,KC_ESC       ,x          ,(     KC_RSFT      )       ,(      x______       )     ,x            ,x            ,x______      ,x______      ,x______

), [_L3] = LAYOUT( //layer 3 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,x            ,(      x______      )
/* */,x            ,DE_PERC      ,DE_AMPR      ,DE_DQUO      ,DE_QUOT      ,DE_GRV                                 ,DE_HASH      ,DE_LCBR      ,DE_RCBR      ,DE_PIPE      ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,DE_AT        ,DE_EQL       ,DE_LABK      ,DE_RABK      ,x                  ,x                  ,DE_ASTR      ,DE_LPRN      ,DE_RPRN      ,DE_TILD      ,DE_DLR       ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,DE_PLUS      ,x                  ,x                  ,x            ,DE_LBRC      ,DE_RBRC      ,x            ,DE_SECT      ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
), [_L4] = LAYOUT( //layer 4 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,X(SUP1)      ,X(SUP2)      ,X(SUP3)      ,X(SUP4)      ,X(SUP5)      ,X(SUP6)      ,X(SUP7)    ,X(SUP8)      ,X(SUP9)      ,X(SUP0)      ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,DE_ACUT      ,x                                      ,KC_PSLS      ,KC_7         ,KC_8         ,KC_9         ,DE_COLN      ,x            ,x            ,x            ,x______
/*H*/,x            ,DE_UDIA      ,CK_NEQ       ,DE_ADIA      ,x            ,DE_ODIA            ,x                  ,KC_PAST      ,KC_4         ,KC_5         ,KC_6         ,DE_DOT       ,x            ,(      x______      )      ,x______
/* */,DE_PERC      ,KC_PMNS      ,KC_PPLS      ,DE_EQL       ,TEST_1       ,DE_SS              ,x                  ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,DE_COMM      ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      MO(_L5)      )      ,x            ,x          ,(      MO(_L5)      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
), [_L5] = LAYOUT( //layer 5 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,X(SUB1)      ,X(SUB2)      ,X(SUB3)      ,X(SUB4)      ,X(SUB5)      ,X(SUB6)      ,X(SUB7)    ,X(SUB8)      ,X(SUB9)      ,X(SUB0)      ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,x            ,(      x______      )
/* */,X(DCIRC)     ,X(DCEDI)     ,X(DTILD)     ,X(DDIA)      ,X(DACUT)     ,X(DGRV)                                ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,x______
/*H*/,X(DDEGR)     ,S(DE_UDIA)   ,TEST_3       ,S(DE_ADIA)   ,DE_EURO      ,S(DE_ODIA)        ,x                   ,x            ,X(NOT)       ,x            ,x            ,x            ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,TEST_2       ,RSA(DE_SS)        ,x                   ,x            ,DE_MICR      ,x            ,x            ,x            ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______

), [_NV] = LAYOUT( //navigation & keyboard settings *********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ DM_RSTP      ,DM_REC1      ,DM_REC2      ,RGB_TOG      ,RGB_MOD      ,RGB_HUI      ,RGB_VAD      ,RGB_VAI    ,RGB_SPD      ,RGB_SPI      ,x            ,RESET        ,x            ,(      NK_TOGG      )      ,(      EEP_RST      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,DM_PLY1      ,DM_PLY2      ,CK_LMRES     ,(      UC_MOD       )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,C(KC_HOME)   ,KC_HOME      ,KC_UP        ,KC_END       ,C(KC_END)    ,DT_UP        ,KC_WWW_HOME  ,KC_MSEL      ,KC_VOLU
/*H*/,C(KC_PLUS)   ,x            ,x            ,x            ,x            ,x                  ,KM_COPY            ,KM_PAST      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT      ,KC_PGUP      ,DT_DOWN      ,(      KC_MUTE      )      ,KC_VOLD
/* */,C(KC_MINUS)  ,x            ,x            ,x            ,x            ,x                  ,KM_CUT             ,KC_INS       ,C(KC_LEFT)   ,KC_DEL       ,C(KC_RGHT)   ,KC_PGDN      ,DT_PRNT                    ,KC_MSTP
/*T*/,KC_PAUS      ,KC_SLCK      ,(      x______      )      ,(      x______      )      ,x            ,KC_APP     ,(      x______      )      ,(      x______      )      ,x            ,x            ,KC_MPRV      ,KC_MPLY      ,KC_MNXT
) ,[_DC] = LAYOUT( //LCTL + digits *************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,C(KC_7)      ,C(KC_8)      ,C(KC_9)      ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,C(KC_4)      ,C(KC_5)      ,C(KC_6)      ,x            ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,C(KC_0)      ,C(KC_1)      ,C(KC_2)      ,C(KC_3)      ,x            ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
) ,[_DW] = LAYOUT( //LWIN + digits *************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,    (KC_7)   ,    (KC_8)   ,    (KC_9)   ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,    (KC_4)   ,    (KC_5)   ,    (KC_6)   ,x            ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,    (KC_0)   ,    (KC_1)   ,    (KC_2)   ,    (KC_3)   ,x            ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______

), [_AS] = LAYOUT( //arrows *****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,X(ARR_NW)    ,X(ARR_N)     ,X(ARR_NE)    ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,X(ARR_W)     ,X(ARR_S)     ,X(ARR_E)     ,X(ARR_NS)    ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,X(ARR_SW)    ,X(ARR_S)     ,X(ARR_SE)    ,X(ARR_WE)    ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
), [_AD] = LAYOUT( //double arrows ************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,X(ARR_NW2)   ,X(ARR_N2)    ,X(ARR_NE2)   ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,X(ARR_W2)    ,X(ARR_S2)    ,X(ARR_E2)    ,X(ARR_NS2)   ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,X(ARR_SW2)   ,X(ARR_S2)    ,X(ARR_SE2)   ,X(ARR_WE2)   ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______

), [_FS] = LAYOUT( //frames *****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,X(FRM_NW)    ,X(FRM_N)     ,X(FRM_NE)    ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,X(FRM_W)     ,X(FRM_CR)    ,X(FRM_E)     ,X(FRM_VL)    ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,X(FRM_SW)    ,X(FRM_S)     ,X(FRM_SE)    ,X(FRM_HL)    ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
), [_FD] = LAYOUT( //double frames ************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,X(FRM_NW2)   ,X(FRM_N2)    ,X(FRM_NE2)   ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,X(FRM_W2)    ,X(FRM_CR2)   ,X(FRM_E2)    ,X(FRM_VL2)   ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,X(FRM_SW2)   ,X(FRM_S2)    ,X(FRM_SE2)   ,X(FRM_HL2)   ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
)};

/* template (replace backslashes with slashes when uncommenting)
) ,[_XY] = LAYOUT( // xxxx *******| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
\*N*\ x            ,x            ,x            ,x            ,x            ,x            ,x______      ,x______    ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______       )
\*F*\,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______       )
\* *\,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,x______
\*H*\,x            ,x            ,x            ,x            ,x            ,x                  ,x______            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )      ,x______
\* *\,x            ,x            ,x            ,x            ,x            ,x                  ,x______            ,x            ,x            ,x            ,x            ,x            ,x                          ,x______
\*T*\,x            ,x            ,(      x______      )      ,(      x______      )      ,x______      ,x______    ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
*/
