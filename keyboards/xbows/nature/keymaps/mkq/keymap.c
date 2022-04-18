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
	CK_LMRES,

	// custom keys using (my custom, not UC_WINC) AutoHotkey compose:
	// - misc
	CKC_NOT, CKC_POO,
	// - dead accents
	CKC_DGRV, CKC_DACUT, CKC_DCIRC, CKC_DDEGR, CKC_DDIA, CKC_DTILD, CKC_DCEDI,
	// - superscript and subscript digits
	CKC_SUP0, CKC_SUP1, CKC_SUP2, CKC_SUP3, CKC_SUP4, CKC_SUP5, CKC_SUP6, CKC_SUP7, CKC_SUP8, CKC_SUP9,
	CKC_SUB0, CKC_SUB1, CKC_SUB2, CKC_SUB3, CKC_SUB4, CKC_SUB5, CKC_SUB6, CKC_SUB7, CKC_SUB8, CKC_SUB9,
	// - arrows and double arrows
	CKC_ARR_N,  CKC_ARR_S,  CKC_ARR_W,  CKC_ARR_E,  CKC_ARR_NW,  CKC_ARR_NE,  CKC_ARR_SE,  CKC_ARR_SW,  CKC_ARR_WE,  CKC_ARR_NS,
	CKC_ARR_N2, CKC_ARR_S2, CKC_ARR_W2, CKC_ARR_E2, CKC_ARR_NW2, CKC_ARR_NE2, CKC_ARR_SE2, CKC_ARR_SW2, CKC_ARR_WE2, CKC_ARR_NS2,
	// - frames and double frames
	CKC_FRM_N,  CKC_FRM_S,  CKC_FRM_W,  CKC_FRM_E,  CKC_FRM_NW,  CKC_FRM_NE,  CKC_FRM_SE,  CKC_FRM_SW,  CKC_FRM_HL,  CKC_FRM_VL,  CKC_FRM_CR,
	CKC_FRM_N2, CKC_FRM_S2, CKC_FRM_W2, CKC_FRM_E2, CKC_FRM_NW2, CKC_FRM_NE2, CKC_FRM_SE2, CKC_FRM_SW2, CKC_FRM_HL2, CKC_FRM_VL2, CKC_FRM_CR2
};
const uint16_t PROGMEM COMPOSE_KEYCODE_START = CKC_NOT;
const char* COMPOSE_STRINGS[] = {
	// - misc
	"bn", "xp",
	// - dead accents
	",`", ",'", ",^", ",°", ",\"", ",~", ",,",
	// - superscript and subscript digits
	"^0", "^1", "^2", "^3", "^4", "^5", "^6", "^7", "^8", "^9",
	"_0", "_1", "_2", "_3", "_4", "_5", "_6", "_7", "_8", "_9",
	// - arrows and double arrows
	"an",  "ag",  "at",  "ar",  "ad",  "a,",  "ah",  "af",  "a.",  "am",
	"aan", "aag", "aat", "aar", "aad", "aa,", "aah", "aaf", "aa.", "aam",
	// - frames and double frames
	"fg", "f,", "fn", "ft", "fh", "ff", "f.", "fm", "fs", "fd", "fr",
	"Fg", "F,", "Fn", "Ft", "Fh", "Ff", "F.", "Fm", "Fs", "Fd", "Fr"
};
const char *COMPOSE_LEADER = SS_LSFT("3"); // DE "§";

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

static uint8_t shift_count;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
		shift_count += pressed ? 1 : -1;
		switch (shift_count) {
		case 0:
			del_mods(MOD_MASK_SHIFT);
			return true;
		case 1:
			if (pressed) {	// previous shift_count was 0 => normal shift behavior
				return true;
			} else {	// previous shift_count was 2 => switch back from layer _DS to shift
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

	// COMPOSE_STRINGS
/* TODO
	if (keycode >= COMPOSE_KEYCODE_START) {
		uint16_t compose_index = keycode - COMPOSE_KEYCODE_START;
		if (compose_index < sizeof(COMPOSE_STRINGS) / sizeof(COMPOSE_STRINGS[0])) {
			if (pressed) {
				send_string(COMPOSE_LEADER);
				send_string(COMPOSE_STRINGS[compose_index]);
			}
			return false;
		}
	}
*/
	return true;
}

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
/*N*/ x______      ,CKC_SUP1     ,CKC_SUP2     ,CKC_SUP3     ,CKC_SUP4     ,CKC_SUP5     ,CKC_SUP6     ,CKC_SUP7   ,CKC_SUP8     ,CKC_SUP9     ,CKC_SUP0     ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,DE_ACUT      ,x                                      ,KC_PSLS      ,KC_7         ,KC_8         ,KC_9         ,DE_COLN      ,x            ,x            ,x            ,x______
/*H*/,x            ,DE_UDIA      ,CK_NEQ       ,DE_ADIA      ,x            ,DE_ODIA            ,x                  ,KC_PAST      ,KC_4         ,KC_5         ,KC_6         ,DE_DOT       ,x            ,(      x______      )      ,x______
/* */,DE_PERC      ,KC_PMNS      ,KC_PPLS      ,DE_EQL       ,TEST_1       ,DE_SS              ,x                  ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,DE_COMM      ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      MO(_L5)      )      ,x            ,x          ,(      MO(_L5)      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
), [_L5] = LAYOUT( //layer 5 ****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,CKC_SUB1     ,CKC_SUB2     ,CKC_SUB3     ,CKC_SUB4     ,CKC_SUB5     ,CKC_SUB6     ,CKC_SUB7   ,CKC_SUB8     ,CKC_SUB9     ,CKC_SUB0     ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX                                ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,x            ,(      x______      )
/* */,CKC_DCIRC    ,CKC_DCEDI    ,CKC_DTILD    ,CKC_DDIA     ,CKC_DACUT    ,CKC_DGRV                               ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,x______
/*H*/,CKC_DDEGR    ,S(DE_UDIA)   ,TEST_3       ,S(DE_ADIA)   ,DE_EURO      ,S(DE_ODIA)        ,x                   ,x            ,CKC_NOT      ,x            ,x            ,x            ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,CKC_POO      ,RSA(DE_SS)        ,x                   ,x            ,DE_MICR      ,x            ,x            ,x            ,x                          ,x______
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
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,CKC_ARR_NW   ,CKC_ARR_N    ,CKC_ARR_NE   ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,CKC_ARR_W    ,CKC_ARR_S    ,CKC_ARR_E    ,CKC_ARR_WE   ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,CKC_ARR_SW   ,CKC_ARR_NS   ,CKC_ARR_SE   ,x            ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
), [_AD] = LAYOUT( //double arrows ************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,CKC_ARR_NW2  ,CKC_ARR_N2   ,CKC_ARR_NE2  ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,CKC_ARR_W2   ,CKC_ARR_S2   ,CKC_ARR_E2   ,CKC_ARR_WE2  ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,CKC_ARR_SW2  ,CKC_ARR_NS2  ,CKC_ARR_SE2  ,x            ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______

), [_FS] = LAYOUT( //frames *****| I **********| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,CKC_FRM_NW   ,CKC_FRM_N    ,CKC_FRM_NE   ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,CKC_FRM_HL   ,CKC_FRM_W    ,CKC_FRM_CR   ,CKC_FRM_E    ,CKC_FRM_VL   ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,CKC_FRM_SW   ,CKC_FRM_S    ,CKC_FRM_SE   ,x            ,x                          ,x______
/*T*/,x            ,x            ,(      x______      )      ,(      x______      )      ,x            ,x          ,(      x______      )      ,(      x______      )      ,x            ,x            ,x______      ,x______      ,x______
), [_FD] = LAYOUT( //double frames ************| A **********| E **********| O **********|*********|***************| S **********| N **********| R **********| T **********| D **********|**********|****************|********************|
/*N*/ x______      ,x            ,x            ,x            ,x            ,x            ,x            ,x          ,x            ,x            ,x            ,RESET        ,x            ,(      x______      )      ,(      x______      )
/*F*/,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,x            ,x            ,x            ,x            ,x            ,x            ,(      x______      )
/* */,x            ,x            ,x            ,x            ,x            ,x                                      ,x            ,CKC_FRM_NW2  ,CKC_FRM_N2   ,CKC_FRM_NE2  ,x            ,x            ,x            ,x            ,x______
/*H*/,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,CKC_FRM_HL2  ,CKC_FRM_W2   ,CKC_FRM_CR2  ,CKC_FRM_E2   ,CKC_FRM_VL2  ,x            ,(      x______      )      ,x______
/* */,x            ,x            ,x            ,x            ,x            ,x                  ,x                  ,x            ,CKC_FRM_SW2  ,CKC_FRM_S2   ,CKC_FRM_SE2  ,x            ,x                          ,x______
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
