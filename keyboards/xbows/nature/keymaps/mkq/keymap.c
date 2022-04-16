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
	_NV	//navigation
};
#define _DS   _L3	//double shift = _L3
#define _S_L4 _L5	//shift + layer 4 = layer 5

enum custom_keycodes {
	CK_NEQ = SAFE_RANGE,
	CK_SB,
	CK_QX,
	CK_LMRES
};

layer_state_t layer_state_set_user(layer_state_t state) {
	// shift + layer _L4 => layer _S_L4
	// TODO This does not work.
	// TODO When fixed, remove the two MO(_L5) from layer _L4. They are only a
	// workaround to activate _L5 with the drawback that it requires TT(_L4)
	// to be pressed before shift, while the layer_state_set_user impl should
	// support any order.
	layer_state_t l4_mask = (layer_state_t) 1 << _L4;
	layer_state_t sh_l4_mask = (layer_state_t) 1 << _S_L4;
//	bool shift = shiftCount > 0;
	bool shift = get_mods() & MOD_MASK_SHIFT;
//	bool l4 = IS_LAYER_ON_STATE(state, _L4);
//	bool l4 = get_highest_layer(state) == _L4;
	bool l4 = (state & l4_mask) == l4_mask;
/*
	if (l4 && shift) {
//		layer_off(_L4);
//		layer_on(_S_L4);
		return state | sh_l4_mask;
	} else if (!shift && IS_LAYER_ON_STATE(state, _S_L4)) {
		return state & ~((layer_state_t) 1 << _S_L4);
	}
	return state;
*/
	if (l4 && shift) {
		del_mods(MOD_MASK_SHIFT);
		return state | sh_l4_mask;
	} else {
//		if (l4) { add_mods(MOD_MASK_SHIFT); }
		return state & ~sh_l4_mask;
//		return state;
	}
}

/*
void handle_shift_layer(bool shift) {
	if (shift && IS_LAYER_ON(_L4)) {
		layer_off(_L4);
		layer_on(_L5);
	} else if (!shift && IS_LAYER_ON(_L5)) {
		layer_off(_L5);
		layer_on(_L4);
	}
}
*/

static uint8_t shiftCount;
bool process_record_user_impl(uint16_t keycode, keyrecord_t *record) {
	bool pressed = record->event.pressed;
	switch (keycode) {
	case CK_LMRES:	// layer and modifier reset
		clear_keyboard();
		return false;
	case CK_SB:	// DE slash; with shift: DE backslash (but without shift (for layouts where that would give a capital sharp s))
		if (pressed) {
			uint16_t mods = get_mods();
			if (!(mods & MOD_MASK_SHIFT)) {	// without Shift
				tap_code16(DE_SLSH);
			} else {	// with Shift
				del_mods(MOD_MASK_SHIFT);
				tap_code16(RALT(DE_BSLS));
				set_mods(mods);
			}
		}
		return false;
	case CK_QX:	// DE question mark; with shift: DE exclamation mark
		if (pressed) {
			tap_code16(!(get_mods() & MOD_MASK_SHIFT) ? S(DE_QUES) : DE_EXLM);
		}
		return false;
	case CK_NEQ:
		if (pressed) { SEND_STRING("!="); }
		return false;
	case KC_LSFT:	// double shift => release shift, activate layer _DS
	case KC_RSFT:
		shiftCount += pressed ? 1 : -1;
//		handle_shift_layer(shiftCount == 1);
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
/* shift + layer _L4 => layer _S_L4 (attempt 3, TODO):
	case LT(_L4,KC_SPC):
		if (shiftCount > 0) { tap_code16(LT(_L5,KC_SPC)); return false; }
		break;
	case LT(_L4,KC_G):
		if (shiftCount > 0) { tap_code16(LT(_L5,KC_G)); return false; }
		break;
*/
	}
	return true;
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	bool result = process_record_user_impl(keycode, record);

/* shift + layer _L4 => layer _S_L4 (attempt 2, TODO):
	if (biton32(layer_state) == _L4 && shiftCount > 0) {
		layer_off(_L4);
		layer_on(_S_L4);
	} else if (biton32(layer_state) == _S_L4 && shiftCount <= 0) {
		layer_off(_S_L4);
		layer_on(_L4);
	}
*/
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

// abbreviations
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

// temporary key codes for testing
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
/* */,DE_PLUS      ,DE_MINS      ,CK_QX        ,KC_L         ,KC_P         ,DE_Z               ,DE_SECT            ,KC_B         ,KC_M         ,DE_COMM      ,DE_DOT       ,KC_J         ,KC_Q                       ,KC_UP
/*T*/,KC_LWIN      ,KC_DEL       ,(      KC_BSPC      )      ,(      KC_LSFT      )      ,LT(_BT,ESC)  ,KC_ENTER   ,(     KC_RSFT      )       ,(      MO(_NV)       )     ,KC_LALT      ,KC_LCTL      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT
), [_BT] = LAYOUT( //base, tap only ***********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,RESET        ,_______      ,(       _______     )      ,(      _______      )
/*F*/,_______      ,KC_F2        ,KC_F3        ,KC_F4        ,KC_F5        ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,KC_H         ,_______      ,_______      ,_______      ,KC_CAP       ,_______      ,_______      ,_______
/*H*/,KC_TAB       ,KC_U         ,KC_I         ,KC_A         ,KC_E         ,_______            ,KC_APP             ,_______      ,KC_N         ,KC_R         ,KC_T         ,KC_D         ,DE_MI        ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      KC_LSFT      )      ,KC_ESC       ,_______    ,(     KC_RSFT      )       ,(      _______       )     ,_______      ,_______      ,_______      ,_______      ,_______

), [_L3] = LAYOUT( //layer 3 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,RESET        ,_______      ,(      _______      )      ,(      _______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,_______      ,(      _______      )
/* */,_______      ,DE_PERC      ,DE_AMPR      ,DE_DQUO      ,DE_QUOT      ,DE_GRV                                 ,DE_HASH      ,DE_LCBR      ,DE_RCBR      ,DE_PIPE      ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,DE_AT        ,DE_EQL       ,DE_LABK      ,DE_RABK      ,_______            ,_______            ,DE_ASTR      ,DE_LPRN      ,DE_RPRN      ,DE_TILD      ,DE_DLR       ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,DE_PLUS      ,_______            ,_______            ,_______      ,DE_LBRC      ,DE_RBRC      ,_______      ,DE_SECT      ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_L4] = LAYOUT( //layer 4 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,X(SUP1)      ,X(SUP2)      ,X(SUP3)      ,X(SUP4)      ,X(SUP5)      ,X(SUP6)      ,X(SUP7)    ,X(SUP8)      ,X(SUP9)      ,X(SUP0)      ,RESET        ,_______      ,(      _______      )      ,(      _______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,DE_ACUT      ,_______                                ,KC_PSLS      ,KC_7         ,KC_8         ,KC_9         ,DE_COLN      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,DE_UDIA      ,CK_NEQ       ,DE_ADIA      ,_______      ,DE_ODIA            ,_______            ,KC_PAST      ,KC_4         ,KC_5         ,KC_6         ,DE_DOT       ,_______      ,(      _______      )      ,_______
/* */,DE_PERC      ,KC_PMNS      ,KC_PPLS      ,DE_EQL       ,TEST_1       ,DE_SS              ,_______            ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,DE_COMM      ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      MO(_L5)      )      ,_______      ,_______    ,(      _______      )      ,(      MO(_L5)      )      ,_______      ,_______      ,_______      ,_______      ,_______
///*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_L5] = LAYOUT( //layer 5 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,X(SUB1)      ,X(SUB2)      ,X(SUB3)      ,X(SUB4)      ,X(SUB5)      ,X(SUB6)      ,X(SUB7)    ,X(SUB8)      ,X(SUB9)      ,X(SUB0)      ,RESET        ,_______      ,(      _______      )      ,(      _______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,_______      ,(      _______      )
/* */,X(DCIRC)     ,X(DCEDI)     ,X(DTILD)     ,X(DDIA)      ,X(DACUT)     ,X(DGRV)                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,X(DDEGR)     ,S(DE_UDIA)   ,TEST_3       ,S(DE_ADIA)   ,DE_EURO      ,S(DE_ODIA)        ,_______             ,_______      ,X(NOT)       ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,TEST_2       ,RSA(DE_SS)        ,_______             ,_______      ,DE_MICR      ,_______      ,_______      ,_______      ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______

), [_NV] = LAYOUT( //navigation & keyboard settings *********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ DM_RSTP      ,DM_REC1      ,DM_REC2      ,RGB_TOG      ,RGB_MOD      ,RGB_HUI      ,RGB_VAD      ,RGB_VAI    ,RGB_SPD      ,RGB_SPI      ,_______      ,RESET        ,_______      ,(      NK_TOGG      )      ,(      EEP_RST      )
/*F*/,LWIN(KC_1)   ,LWIN(KC_2)   ,LWIN(KC_3)   ,LWIN(KC_4)   ,LWIN(KC_5)   ,LWIN(KC_6)                             ,LWIN(KC_7)   ,LWIN(KC_8)   ,LWIN(KC_9)   ,LWIN(KC_0)   ,DM_PLY1      ,DM_PLY2      ,CK_LMRES     ,(      UC_MOD       )
/* */,_______      ,C(KC_6)      ,C(KC_7)      ,C(KC_8)      ,C(KC_9)      ,C(KC_0)                                ,C(KC_HOME)   ,KC_HOME      ,KC_UP        ,KC_END       ,C(KC_END)    ,DT_UP        ,KC_WWW_HOME  ,KC_MSEL      ,KC_VOLU
/*H*/,C(KC_PLUS)   ,_______      ,_______      ,_______      ,_______      ,_______            ,KM_COPY            ,KM_PAST      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT      ,KC_PGUP      ,DT_DOWN      ,(      KC_MUTE      )      ,KC_VOLD
/* */,C(KC_MINUS)  ,C(KC_1)      ,C(KC_2)      ,C(KC_3)      ,C(KC_4)      ,C(KC_5)            ,KM_CUT             ,KC_INS       ,C(KC_LEFT)   ,KC_DEL       ,C(KC_RGHT)   ,KC_PGDN      ,DT_PRNT                    ,KC_MSTP
/*T*/,KC_PAUS      ,KC_SLCK      ,(      _______      )      ,(      _______      )      ,_______      ,KC_APP     ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,KC_MPRV      ,KC_MPLY      ,KC_MNXT

), [_AS] = LAYOUT( //arrows *****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,RESET        ,_______      ,(      _______      )      ,(      _______      )
/*F*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(ARR_NW)    ,X(ARR_N)     ,X(ARR_NE)    ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_W)     ,X(ARR_S)     ,X(ARR_E)     ,X(ARR_NS)    ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_SW)    ,X(ARR_S)     ,X(ARR_SE)    ,X(ARR_WE)    ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_AD] = LAYOUT( //double arrows ************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,RESET        ,_______      ,(      _______      )      ,(      _______      )
/*F*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(ARR_NW2)   ,X(ARR_N2)    ,X(ARR_NE2)   ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_W2)    ,X(ARR_S2)    ,X(ARR_E2)    ,X(ARR_NS2)   ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(ARR_SW2)   ,X(ARR_S2)    ,X(ARR_SE2)   ,X(ARR_WE2)   ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______

), [_FS] = LAYOUT( //frames *****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,RESET        ,_______      ,(      _______      )      ,(      _______      )
/*F*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(FRM_NW)    ,X(FRM_N)     ,X(FRM_NE)    ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_W)     ,X(FRM_CR)    ,X(FRM_E)     ,X(FRM_VL)    ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_SW)    ,X(FRM_S)     ,X(FRM_SE)    ,X(FRM_HL)    ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
), [_FD] = LAYOUT( //double frames ************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,RESET        ,_______      ,(      _______      )      ,(      _______      )
/*F*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,X(FRM_NW2)   ,X(FRM_N2)    ,X(FRM_NE2)   ,_______      ,_______      ,_______      ,_______      ,_______
/*H*/,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_W2)    ,X(FRM_CR2)   ,X(FRM_E2)    ,X(FRM_VL2)   ,_______      ,(      _______      )      ,_______
/* */,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,X(FRM_SW2)   ,X(FRM_S2)    ,X(FRM_SE2)   ,X(FRM_HL2)   ,_______                    ,_______
/*T*/,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
)};

/* template (replace backslashes with slashes when uncommenting)
) ,[_XY] = LAYOUT( // xxxx *******| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
\*N*\ _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,RESET        ,_______      ,(      _______      )      ,(      _______      )
\*F*\,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
\* *\,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
\*H*\,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,_______
\* *\,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______                    ,_______
\*T*\,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
*/
