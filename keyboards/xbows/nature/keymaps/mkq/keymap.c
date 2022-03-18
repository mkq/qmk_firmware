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
#define _L3	 1	//layer 3 (counting base as 1 and shift as 2)
#define _L4	 2	//layer 4
#define _L5	 3	//layer 5
#define _FS	10	//frames
#define _FD	11	//double frames
#define _AS	12	//arrows
#define _AD	13	//double arrows
#define _NV	14	//navigation

//layer_state_t layer_state_set_user(layer_state_t state) {
//	// shift + layer _Y1 => layer _Y3
//	// TODO This does not work.
//	// TODO When fixed, remove the two MO(_Y3) from _Y1. They are only a
//	// workaround to activate _Y3 with the drawback that it requires TT(_Y1)
//	// to be pressed before shift, while the layer_state_set_user impl should
//	// support any order.
//	if (get_highest_layer(state) == _Y1 && (get_mods() & MOD_MASK_SHIFT)) {
//		layer_off(_Y1);
//		layer_on(_Y3);
//	}
//	return state;
//}

enum custom_keycodes {
	CK_NEQ = SAFE_RANGE,
	CK_SB,
	CK_QX,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
	case CK_SB:	// DE slash; with shift: DE backslash (but without shift (for layouts where that would give a capital sharp s))
		if (record->event.pressed) {
			if (!(get_mods() & MOD_MASK_SHIFT)) {	// without Shift
				tap_code16(DE_SLSH);
			} else {	// with Shift
				del_mods(MOD_MASK_SHIFT);
				tap_code16(RALT(DE_BSLS));
				add_mods(MOD_MASK_SHIFT);
			}
		}
		return false;
	case CK_QX:	// DE question mark; with shift: DE exclamation mark
		if (record->event.pressed) {
			tap_code16(!(get_mods() & MOD_MASK_SHIFT) ? S(DE_QUES) : DE_EXLM);
		}
		return false;
	case CK_NEQ:
		if (record->event.pressed) { SEND_STRING("!="); }
		return false;
//	case KC_LSFT:	// double shift => release shift, activate layer _Y2
//	case KC_RSFT:
//		shiftCount += record->event.pressed ? 1 : -1;
//		switch (shiftCount) {
//		case 0:
//			del_mods(MOD_MASK_SHIFT);
//			return true;
//		case 1:
//			if (record->event.pressed) {	// previous shiftCount was 0 => normal shift behavior
//				return true;
//			} else {	// previous shiftCount was 2 => switch back from layer _Y2 to shift
//				add_mods(MOD_MASK_SHIFT);
//				layer_off(_Y2);
//				return false;
//			}
//		default:	// double shift => release shift, activate layer _Y2
//			del_mods(MOD_MASK_SHIFT);
//			layer_on(_Y2);
//			return false;
//		}
	}
	return true;
}

// tap dance [https://docs.qmk.fm/#/feature_tap_dance?id=example-5] <<<
enum td_keycodes { TD_SB_SFT, TD_QX_SFT };
typedef enum { TD_NONE, TD_UNKNOWN, TD_SINGLE_TAP, TD_SINGLE_HOLD } td_state_t;
static td_state_t td_state;
td_state_t cur_dance(qk_tap_dance_state_t *state);
void td_sb_sft_finished(qk_tap_dance_state_t *state, void *user_data);
void td_sb_sft_reset(qk_tap_dance_state_t *state, void *user_data);
void td_qx_sft_finished(qk_tap_dance_state_t *state, void *user_data);
void td_qx_sft_reset(qk_tap_dance_state_t *state, void *user_data);

td_state_t cur_dance(qk_tap_dance_state_t *state) {
	if (state->count == 1) {
		return (state->interrupted || !state->pressed) ? TD_SINGLE_TAP : TD_SINGLE_HOLD;
	}
	return TD_UNKNOWN;
}

void td_sb_sft_finished(qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case TD_SINGLE_TAP:  register_code16(CK_QX); break;
		case TD_SINGLE_HOLD: register_mods(MOD_BIT(KC_LSFT)); break;
		default: break;
	}
}
void td_sb_sft_reset(qk_tap_dance_state_t *state, void *user_data) {
	switch (td_state) {
		case TD_SINGLE_TAP:  unregister_code16(CK_QX); break;
		case TD_SINGLE_HOLD: unregister_mods(MOD_BIT(KC_LSFT)); break;
		default: break;
	}
}

void td_qx_sft_finished(qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case TD_SINGLE_TAP:  register_code16(CK_QX); break;
		case TD_SINGLE_HOLD: register_mods(MOD_BIT(KC_RSFT)); break;
		default: break;
	}
}
void td_qx_sft_reset(qk_tap_dance_state_t *state, void *user_data) {
	switch (td_state) {
		case TD_SINGLE_TAP:  unregister_code16(CK_QX); break;
		case TD_SINGLE_HOLD: unregister_mods(MOD_BIT(KC_RSFT)); break;
		default: break;
	}
}

qk_tap_dance_action_t tap_dance_actions[] = {
	[TD_SB_SFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_sb_sft_finished, td_sb_sft_reset),
	[TD_QX_SFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_qx_sft_finished, td_qx_sft_reset)
};
// >>> tap dance

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

#define SPC       KC_SPC
#define DE_PL     DE_PLUS
#define KM_CUT    LSFT(KC_DEL)
#define KM_COPY   LCTL(KC_INS)
#define KM_PAST   LSFT(KC_INS)

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case TD(TD_SB_SFT):
		case TD(TD_QX_SFT):
			return (uint16_t)(TAPPING_TERM / 2.5);
		default:
			return TAPPING_TERM;
	}
}

// ____________________ layers ____________________
// Row label comments: N = numbers; F = function keys; H = home; T = thumb
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//---|-------------|-U-----------|-I-----------|-A-----------|-E-----------|-------------
[_BA] = LAYOUT( //base **********| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ KC_PAUS      ,KC_1         ,KC_2         ,KC_3         ,KC_4         ,KC_5         ,KC_6         ,KC_7       ,KC_8         ,KC_9         ,KC_0         ,KC_PSCR      ,KM_CUT       ,(       KM_COPY     )      ,(      KM_PAST      )
/*F*/,KC_F1        ,KC_F2        ,KC_F3        ,KC_F4        ,KC_F5        ,KC_F6                                  ,KC_F7        ,KC_F8        ,KC_F9        ,KC_F10       ,KC_F11       ,KC_F12       ,KC_DEL       ,(      KC_INS       )
/* */,DE_CIRC      ,KC_X         ,LT(_FD,KC_V) ,LT(_AD,SPC)  ,LT(_L5,KC_C) ,KC_W                                   ,KC_K         ,LT(_L5,KC_H) ,KC_G         ,KC_F         ,DE_Y        ,SFT_T(KC_CAPS),KC_HOME      ,KC_END       ,KC_PGUP
/*H*/,WIN_T(KC_ESC),LALT_T(KC_U) ,LCTL_T(KC_I) ,LT(_NV,KC_A) ,LT(_L3,KC_E) ,KC_O               ,LCAG_T(KC_APP)     ,KC_S         ,LT(_L3,KC_N) ,LT(_NV,KC_R) ,RCTL_T(KC_T) ,LALT_T(KC_D) ,RWIN_T(KC_BSPC),(    KC_SPC       )      ,KC_PGDN
/* */,KC_LEFT      ,DE_MINS      ,LT(_FS,DE_PL),LT(_AS,KC_L) ,LT(_L4,KC_P) ,DE_Z               ,DE_SECT            ,KC_B         ,LT(_L4,KC_M) ,DE_COMM      ,DE_DOT       ,KC_J         ,KC_Q                       ,KC_UP
/*T*/,KC_RGHT      ,KC_DEL       ,(      KC_BSPC      )      ,(   TD(TD_SB_SFT)   )      ,KC_TAB       ,KC_ENTER   ,(  TD(TD_QX_SFT)    )      ,(      MO(_NV)       )     ,KC_LALT      ,KC_LCTL      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT

), [_L3] = LAYOUT( //layer 3 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,X(SUP1)      ,X(SUP2)      ,X(SUP3)      ,X(SUP4)      ,X(SUP5)      ,X(SUP6)      ,X(SUP7)    ,X(SUP8)      ,X(SUP9)      ,X(SUP0)      ,X(SUPN)      ,_______      ,(      _______      )      ,(      _______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,_______      ,(      _______      )
/* */,_______      ,DE_PERC      ,DE_AMPR      ,DE_DQUO      ,DE_QUOT      ,DE_GRV                                 ,DE_HASH      ,DE_LCBR      ,DE_RCBR      ,DE_PIPE      ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,DE_AT        ,DE_EQL       ,DE_LABK      ,DE_RABK      ,_______            ,_______            ,_______      ,DE_LPRN      ,DE_RPRN      ,DE_TILD      ,DE_DLR       ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,DE_LBRC      ,DE_RBRC      ,_______      ,DE_SECT      ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_L4] = LAYOUT( //layer 4 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,X(SUB1)      ,X(SUB2)      ,X(SUB3)      ,X(SUB4)      ,X(SUB5)      ,X(SUB6)      ,X(SUB7)    ,X(SUB8)      ,X(SUB9)      ,X(SUB0)      ,X(SUBN)      ,_______      ,(      _______      )      ,(      _______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,DE_ACUT      ,_______                                ,KC_PSLS      ,KC_7         ,KC_8         ,KC_9         ,DE_COLN      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,DE_UDIA      ,CK_NEQ       ,DE_ADIA      ,_______      ,DE_ODIA            ,_______            ,KC_PAST      ,KC_4         ,KC_5         ,KC_6         ,DE_DOT       ,_______      ,(      _______      )      ,_______
/* */,DE_PERC      ,KC_PMNS      ,KC_PPLS      ,DE_EQL       ,_______      ,DE_SS              ,_______            ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,DE_COMM      ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_L5] = LAYOUT( //layer 5 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,_______      ,(      _______      )
/* */,X(DCIRC)     ,X(DCEDI)     ,X(DTILD)     ,X(DDIA)      ,X(DACUT)     ,X(DGRV)                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,X(DDEGR)     ,S(DE_UDIA)   ,_______      ,S(DE_ADIA)   ,DE_EURO      ,S(DE_ODIA)        ,_______             ,_______      ,(      _______      )      ,_______      ,_______      ,X(NOT)       ,_______      ,_______      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,RSA(DE_SS)        ,_______             ,_______      ,DE_MICR                    ,_______      ,_______      ,_______      ,_______      ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______

), [_NV] = LAYOUT( //navigation & keyboard settings *********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ DM_RSTP      ,DM_REC1      ,DM_REC2      ,RGB_TOG      ,RGB_MOD      ,RGB_HUI      ,RGB_VAD      ,RGB_VAI    ,RGB_SPD      ,RGB_SPI      ,_______      ,KC_SLCK      ,RESET        ,(      NK_TOGG      )      ,(      EEP_RST      )
/*F*/,LWIN(KC_1)   ,LWIN(KC_2)   ,LWIN(KC_3)   ,LWIN(KC_4)   ,LWIN(KC_5)   ,LWIN(KC_6)                             ,LWIN(KC_7)   ,LWIN(KC_8)   ,LWIN(KC_9)   ,LWIN(KC_0)   ,DM_PLY1      ,DM_PLY2      ,_______      ,(      UC_MOD       )
/* */,_______      ,C(KC_6)      ,C(KC_7)      ,C(KC_8)      ,C(KC_9)      ,C(KC_0)                                ,C(KC_HOME)   ,KC_HOME      ,KC_UP        ,KC_END       ,C(KC_END)    ,DT_UP        ,KC_WWW_HOME  ,KC_MSEL      ,KC_VOLU
/*H*/,KC_LWIN      ,_______      ,_______      ,_______      ,_______      ,_______            ,KM_COPY            ,KM_PAST      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT      ,KC_PGUP      ,DT_DOWN      ,(      KC_MUTE      )      ,KC_VOLD
/* */,C(KC_PLUS)   ,C(KC_1)      ,C(KC_2)      ,C(KC_3)      ,C(KC_4)      ,C(KC_5)            ,KM_CUT             ,KC_INS       ,C(KC_LEFT)   ,KC_DEL       ,C(KC_RGHT)   ,KC_PGDN      ,DT_PRNT                    ,KC_MSTP
/*T*/,C(KC_MINUS)  ,KC_RWIN      ,(      _______      )      ,(      _______      )      ,KC_CAPS      ,KC_APP     ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,KC_MPRV      ,KC_MPLY      ,KC_MNXT

), [_AS] = LAYOUT( //arrows *****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
/*F*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(ARR_NW)    ,X(ARR_N)     ,X(ARR_NE)    ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_W)     ,X(ARR_S)     ,X(ARR_E)     ,X(ARR_NS)    ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_SW)    ,X(ARR_S)     ,X(ARR_SE)    ,X(ARR_WE)    ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_AD] = LAYOUT( //double arrows ************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
/*F*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(ARR_NW2)   ,X(ARR_N2)    ,X(ARR_NE2)   ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_W2)    ,X(ARR_S2)    ,X(ARR_E2)    ,X(ARR_NS2)   ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_SW2)   ,X(ARR_S2)    ,X(ARR_SE2)   ,X(ARR_WE2)   ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______

), [_FS] = LAYOUT( //frames *****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
/*F*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(FRM_NW)    ,X(FRM_N)     ,X(FRM_NE)    ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_W)     ,X(FRM_CR)    ,X(FRM_E)     ,X(FRM_VL)    ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_SW)    ,X(FRM_S)     ,X(FRM_SE)    ,X(FRM_HL)    ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_FD] = LAYOUT( //double frames ************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
/*F*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(FRM_NW2)   ,X(FRM_N2)    ,X(FRM_NE2)   ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_W2)    ,X(FRM_CR2)   ,X(FRM_E2)    ,X(FRM_VL2)   ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_SW2)   ,X(FRM_S2)    ,X(FRM_SE2)   ,X(FRM_HL2)   ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
)};

/* template (replace backslashes with slashes when uncommenting)
) ,[_XY] = LAYOUT( // xxxx *******| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
\*N*\ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
\*F*\,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
\* *\,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
\*H*\,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,_______
\* *\,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______                    ,_______
\*T*\,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
*/
