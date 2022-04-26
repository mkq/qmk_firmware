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
const uint8_t layer_leds[] = {
	// These values (1 = blue, 2 = green, 4 = red) are chosen here for readability in binary literals.
	// Function layer_state_set_user_led maps them back to ergodox_right_led_..._on calls.
	//... = ..rgb
	[_L3] = 0b001,
	[_L4] = 0b100,
	[_L5] = 0b110,
	[_AS] = 0b101,
	[_AD] = 0b101,
	[_FS] = 0b111,
	[_FD] = 0b111,
	[_NV] = 0b010,
};
const uint8_t layer_leds_length = sizeof(layer_leds) / sizeof(layer_leds[0]);

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
	CKC_ARR_N1, CKC_ARR_S1, CKC_ARR_W1, CKC_ARR_E1, CKC_ARR_NW1, CKC_ARR_NE1, CKC_ARR_SE1, CKC_ARR_SW1, CKC_ARR_WE1, CKC_ARR_NS1,
	CKC_ARR_N2, CKC_ARR_S2, CKC_ARR_W2, CKC_ARR_E2, CKC_ARR_NW2, CKC_ARR_NE2, CKC_ARR_SE2, CKC_ARR_SW2, CKC_ARR_WE2, CKC_ARR_NS2,
	// - frames and double frames
	CKC_FRM_N1, CKC_FRM_S1, CKC_FRM_W1, CKC_FRM_E1, CKC_FRM_NW1, CKC_FRM_NE1, CKC_FRM_SE1, CKC_FRM_SW1, CKC_FRM_HL1, CKC_FRM_VL1, CKC_FRM_CR1,
	CKC_FRM_N2, CKC_FRM_S2, CKC_FRM_W2, CKC_FRM_E2, CKC_FRM_NW2, CKC_FRM_NE2, CKC_FRM_SE2, CKC_FRM_SW2, CKC_FRM_HL2, CKC_FRM_VL2, CKC_FRM_CR2
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

void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
    rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

layer_state_t layer_state_set_user_led(layer_state_t state) {
	ergodox_right_led_1_off();
	ergodox_right_led_2_off();
	ergodox_right_led_3_off();

	uint8_t layer = get_highest_layer(state);
	uint8_t led_state = 0 <= layer && layer < layer_leds_length ? layer_leds[layer] : 0;
	if (led_state & 0b100) { ergodox_right_led_1_on(); }
	if (led_state & 0b010) { ergodox_right_led_2_on(); }
	if (led_state & 0b001) { ergodox_right_led_3_on(); }

	return state;
};

layer_state_t layer_state_set_user(layer_state_t state) {
	layer_state_t new_layer_state = layer_state_set_user_impl(state);
	new_layer_state = layer_state_set_user_led(new_layer_state);
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

// process_record_user implementation for (custom AutoHotkey) compose sequences
bool pru_compose(keyrecord_t *record, const char *s) {
	if (record->event.pressed) {
		SEND_STRING(SS_LSFT("3"));
		send_string(s);
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

	// custom keys using (my custom, not UC_WINC) AutoHotkey compose:
	// - misc
	case CKC_NOT    : return pru_compose(record, "bn");
	case CKC_POO    : return pru_compose(record, "xp");
	// - dead accents
	case CKC_DGRV   : return pru_compose(record, ";`");
	case CKC_DACUT  : return pru_compose(record, ";'");
	case CKC_DCIRC  : return pru_compose(record, ";^");
	case CKC_DDEGR  : return pru_compose(record, ";°");
	case CKC_DDIA   : return pru_compose(record, ";\"");
	case CKC_DTILD  : return pru_compose(record, ";~");
	case CKC_DCEDI  : return pru_compose(record, ";,");
	// - superscript and subscript digits (TODO: find a place in the keymap for them)
	case CKC_SUP0   : return pru_compose(record, "^0");
	case CKC_SUP1   : return pru_compose(record, "^1");
	case CKC_SUP2   : return pru_compose(record, "^2");
	case CKC_SUP3   : return pru_compose(record, "^3");
	case CKC_SUP4   : return pru_compose(record, "^4");
	case CKC_SUP5   : return pru_compose(record, "^5");
	case CKC_SUP6   : return pru_compose(record, "^6");
	case CKC_SUP7   : return pru_compose(record, "^7");
	case CKC_SUP8   : return pru_compose(record, "^8");
	case CKC_SUP9   : return pru_compose(record, "^9");
	case CKC_SUB0   : return pru_compose(record, "_0");
	case CKC_SUB1   : return pru_compose(record, "_1");
	case CKC_SUB2   : return pru_compose(record, "_2");
	case CKC_SUB3   : return pru_compose(record, "_3");
	case CKC_SUB4   : return pru_compose(record, "_4");
	case CKC_SUB5   : return pru_compose(record, "_5");
	case CKC_SUB6   : return pru_compose(record, "_6");
	case CKC_SUB7   : return pru_compose(record, "_7");
	case CKC_SUB8   : return pru_compose(record, "_8");
	case CKC_SUB9   : return pru_compose(record, "_9");
	// - arrows and double arrows
	case CKC_ARR_N1 : return pru_compose(record, "ag");
	case CKC_ARR_S1 : return pru_compose(record, "ar");
	case CKC_ARR_W1 : return pru_compose(record, "an");
	case CKC_ARR_E1 : return pru_compose(record, "at");
	case CKC_ARR_NW1: return pru_compose(record, "ah");
	case CKC_ARR_NE1: return pru_compose(record, "af");
	case CKC_ARR_SE1: return pru_compose(record, "a.");
	case CKC_ARR_SW1: return pru_compose(record, "am");
	case CKC_ARR_WE1: return pru_compose(record, "ad");
	case CKC_ARR_NS1: return pru_compose(record, "a,");
	case CKC_ARR_N2 : return pru_compose(record, "aag");
	case CKC_ARR_S2 : return pru_compose(record, "aar");
	case CKC_ARR_W2 : return pru_compose(record, "aan");
	case CKC_ARR_E2 : return pru_compose(record, "aat");
	case CKC_ARR_NW2: return pru_compose(record, "aah");
	case CKC_ARR_NE2: return pru_compose(record, "aaf");
	case CKC_ARR_SE2: return pru_compose(record, "aa.");
	case CKC_ARR_SW2: return pru_compose(record, "aam");
	case CKC_ARR_WE2: return pru_compose(record, "aad");
	case CKC_ARR_NS2: return pru_compose(record, "aa,");
	// - frames and double frames
	case CKC_FRM_N1 : return pru_compose(record, "fg");
	case CKC_FRM_S1 : return pru_compose(record, "f,");
	case CKC_FRM_W1 : return pru_compose(record, "fn");
	case CKC_FRM_E1 : return pru_compose(record, "ft");
	case CKC_FRM_NW1: return pru_compose(record, "fh");
	case CKC_FRM_NE1: return pru_compose(record, "ff");
	case CKC_FRM_SE1: return pru_compose(record, "f.");
	case CKC_FRM_SW1: return pru_compose(record, "fm");
	case CKC_FRM_HL1: return pru_compose(record, "fd");
	case CKC_FRM_VL1: return pru_compose(record, "fs");
	case CKC_FRM_CR1: return pru_compose(record, "fr");
	case CKC_FRM_N2 : return pru_compose(record, "Fg");
	case CKC_FRM_S2 : return pru_compose(record, "F,");
	case CKC_FRM_W2 : return pru_compose(record, "Fn");
	case CKC_FRM_E2 : return pru_compose(record, "Ft");
	case CKC_FRM_NW2: return pru_compose(record, "Fh");
	case CKC_FRM_NE2: return pru_compose(record, "Ff");
	case CKC_FRM_SE2: return pru_compose(record, "F.");
	case CKC_FRM_SW2: return pru_compose(record, "Fm");
	case CKC_FRM_HL2: return pru_compose(record, "Fd");
	case CKC_FRM_VL2: return pru_compose(record, "Fs");
	case CKC_FRM_CR2: return pru_compose(record, "Fr");
	}

	return true;
}

// keycode aliases
#define SPC       KC_SPC
#define ESC       KC_ESC
#define KC_BS     KC_BSPC
#define KC_AP     KC_APP
#define DE_MI     DE_MINS
#define KM_CUT    LSFT(KC_DEL)
#define KM_COPY   LCTL(KC_INS)
#define KM_PAST   LSFT(KC_INS)
// - not as readabe as _______, but editor navigation-friendlier (word boundary)
#define x______   _______
// - temporary key codes for testing
#define TEST_1    KC_A
#define TEST_2    KC_B
#define TEST_3    KC_C
#define TEST_4    KC_D

// ____________________ layers ____________________
// Row label comments: F = function keys; H = home; T = thumb
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BA] = LAYOUT_ergodox_pretty(
// [base]          | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ KC_F1        ,LT(_FS,KC_F2),LT(_FD,KC_F3),LT(_AS,KC_F4),LT(_AD,KC_F5),KC_F6        ,KC_LWIN        ,KC_PSCR      ,KC_F7        ,KC_F8        ,KC_F9        ,KC_F10       ,KC_F11       ,KC_F12
/* */,DE_CIRC      ,KC_X         ,KC_V         ,LT(_L4,SPC)  ,KC_C         ,KC_W         ,KC_LALT        ,LCAG_T(KC_AP),KC_K         ,KC_H         ,LT(_L4,KC_G) ,KC_F         ,DE_Y         ,SFT_T(KC_CAPS)
/*H*/,KC_TAB       ,LWIN_T(KC_U) ,LALT_T(KC_I) ,LCTL_T(KC_A) ,LT(_NV,KC_E) ,KC_O                                       ,KC_S         ,LT(_NV,KC_N) ,RCTL_T(KC_R) ,LALT_T(KC_T) ,RWIN_T(KC_D) ,CK_SB
/* */,DE_PLUS      ,LT(_DW,DE_MI),CK_QX        ,LT(_DC,KC_L) ,KC_P         ,DE_Z         ,KC_LCTL        ,x______      ,KC_B         ,KC_M         ,DE_COMM      ,DE_DOT       ,KC_J         ,KC_Q
/* */,KC_HOME      ,KC_END       ,KC_PGUP      ,KC_PGDN      ,KC_ESC                                                                 ,MO(_NV)      ,KC_UP        ,KC_DOWN      ,KC_LEFT      ,KC_RGHT
/*T*/                                                                      ,KM_COPY      ,KM_PAST        ,x______      ,TT(_BT)
/*T*/                                                                                    ,KM_CUT         ,x______
/*T*/                                                        ,KC_LSFT      ,KC_BS        ,KC_DEL         ,KC_SPC       ,KC_ENTER     ,KC_RSFT
), [_BT] = LAYOUT_ergodox_pretty(
// [base, tap only]| U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ x______      ,KC_F2        ,KC_F3        ,KC_F4        ,KC_F5        ,x______      ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,SPC          ,x______      ,x______      ,KC_LWIN        ,x______      ,x______      ,x______      ,KC_G         ,x______      ,x______      ,KC_CAPS
/*H*/,x______      ,KC_U         ,KC_I         ,KC_A         ,KC_E         ,x______                                    ,x______      ,KC_N         ,KC_R         ,KC_T         ,KC_D         ,x______
/* */,x______      ,DE_MI        ,x______      ,KC_L         ,x______      ,x______      ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,KC_BS        ,x______        ,x______      ,x______      ,x______

), [_L3] = LAYOUT_ergodox_pretty(
// [layer 3]       | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX        ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX
/* */,x______      ,DE_PERC      ,DE_AMPR      ,DE_DQUO      ,DE_QUOT      ,DE_GRV       ,x______        ,x______      ,DE_HASH      ,DE_LCBR      ,DE_RCBR      ,DE_PIPE      ,x______      ,x______
/*H*/,x______      ,DE_AT        ,DE_EQL       ,DE_LABK      ,DE_RABK      ,x______                                    ,DE_ASTR      ,DE_LPRN      ,DE_RPRN      ,DE_TILD      ,DE_DLR       ,x______
/* */,x______      ,x______      ,x______      ,x______      ,DE_PLUS      ,x______      ,x______        ,x______      ,x______      ,DE_LBRC      ,DE_RBRC      ,x______      ,DE_SECT      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,x______
), [_L4] = LAYOUT_ergodox_pretty(
// [layer 4]       | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX        ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX
/* */,x______      ,x______      ,x______      ,x______      ,DE_ACUT      ,x______      ,x______        ,x______      ,KC_PSLS      ,KC_7         ,KC_8         ,KC_9         ,DE_COLN      ,x______
/*H*/,x______      ,DE_UDIA      ,CK_NEQ       ,DE_ADIA      ,x______      ,DE_ODIA                                    ,KC_PAST      ,KC_4         ,KC_5         ,KC_6         ,DE_DOT       ,x______
/* */,DE_PERC      ,KC_PMNS      ,KC_PPLS      ,DE_EQL       ,TEST_1       ,DE_SS        ,x______        ,x______      ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,DE_COMM      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,MO(_L5)      ,x______      ,x______        ,x______      ,x______      ,MO(_L5)
), [_L5] = LAYOUT_ergodox_pretty(
// [layer 5]       | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX        ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX      ,XXXXXXX
/* */,CKC_DCIRC    ,CKC_DCEDI    ,CKC_DTILD    ,CKC_DDIA     ,CKC_DACUT    ,CKC_DGRV     ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/*H*/,CKC_DDEGR    ,S(DE_UDIA)   ,TEST_3       ,S(DE_ADIA)   ,DE_EURO      ,S(DE_ODIA)                                 ,x______      ,CKC_NOT      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,CKC_POO      ,RSA(DE_SS)   ,x______        ,x______      ,DE_MICR      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,x______

), [_NV] = LAYOUT_ergodox_pretty(
// [navigation & settings] ******| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ DM_RSTP      ,DM_REC1      ,DM_REC2      ,DM_PLY1      ,DM_PLY2      ,x______      ,KC_SLCK        ,KC_PAUS      ,RGB_TOG      ,x______      ,RESET        ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,KM_COPY      ,C(KC_HOME)   ,KC_HOME      ,KC_UP        ,KC_END       ,C(KC_END)    ,KC_VOLU
/*H*/,C(KC_PLUS)   ,x______      ,x______      ,x______      ,x______      ,x______                                    ,KM_PAST      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT      ,KC_PGUP      ,KC_VOLD
/* */,C(KC_MINUS)  ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,KM_CUT       ,KC_INS       ,C(KC_LEFT)   ,KC_DEL       ,C(KC_RGHT)   ,KC_PGDN      ,KC_MUTE
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,XXXXXXX      ,KC_MSEL      ,KC_MPLY      ,KC_MPRV      ,KC_MNXT
/*T*/                                                                      ,x______      ,x______        ,KC_WBAK      ,KC_WFWD
/*T*/                                                                                    ,x______        ,KC_WHOM
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,KC_RSFT

) ,[_DC] = LAYOUT_ergodox_pretty(
// [ctrl+digits]   | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,KC_7         ,KC_8         ,KC_9         ,x______      ,x______
/*H*/,x______      ,x______      ,x______      ,x______      ,x______      ,x______                                    ,x______      ,KC_4         ,KC_5         ,KC_6         ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,x______
), [_DW] = LAYOUT_ergodox_pretty(
// [win+digits]    | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,KC_7         ,KC_8         ,KC_9         ,x______      ,x______
/*H*/,x______      ,x______      ,x______      ,x______      ,x______      ,x______                                    ,x______      ,KC_4         ,KC_5         ,KC_6         ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,x______

), [_AS] = LAYOUT_ergodox_pretty(
// [arrows]        | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,CKC_ARR_NW1  ,CKC_ARR_N1   ,CKC_ARR_NE1  ,x______      ,x______
/*H*/,x______      ,x______      ,x______      ,x______      ,x______      ,x______                                    ,x______      ,CKC_ARR_W1   ,CKC_ARR_S1   ,CKC_ARR_E1   ,CKC_ARR_WE1  ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,CKC_ARR_SW1  ,CKC_ARR_NS1  ,CKC_ARR_SE1  ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,x______
), [_AD] = LAYOUT_ergodox_pretty(
// [double arrows] | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,CKC_ARR_NW2  ,CKC_ARR_N2   ,CKC_ARR_NE2  ,x______      ,x______
/*H*/,x______      ,x______      ,x______      ,x______      ,x______      ,x______                                    ,x______      ,CKC_ARR_W2   ,CKC_ARR_S2   ,CKC_ARR_E2   ,CKC_ARR_WE2  ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,CKC_ARR_SW2  ,CKC_ARR_NS2  ,CKC_ARR_SE2  ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,x______

), [_FS] = LAYOUT_ergodox_pretty(
// [frames]        | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,CKC_FRM_NW1  ,CKC_FRM_N1   ,CKC_FRM_NE1  ,x______      ,x______
/*H*/,x______      ,x______      ,x______      ,x______      ,x______      ,x______                                    ,CKC_FRM_VL1  ,CKC_FRM_W1   ,CKC_FRM_CR1  ,CKC_FRM_E1   ,CKC_FRM_HL1  ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,CKC_FRM_SW1  ,CKC_FRM_S1   ,CKC_FRM_SE1  ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,x______
), [_FD] = LAYOUT_ergodox_pretty(
// [double frames] | U **********| I **********| A **********| E **********| O **********|                             | S **********| N **********| R **********| T **********| D **********|*************
/*F*/ x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,CKC_FRM_NW2  ,CKC_FRM_N2   ,CKC_FRM_NE2  ,x______      ,x______
/*H*/,x______      ,x______      ,x______      ,x______      ,x______      ,x______                                    ,CKC_FRM_VL2  ,CKC_FRM_W2   ,CKC_FRM_CR2  ,CKC_FRM_E2   ,CKC_FRM_HL2  ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______      ,x______      ,x______        ,x______      ,x______      ,CKC_FRM_SW2  ,CKC_FRM_S2   ,CKC_FRM_SE1  ,x______      ,x______
/* */,x______      ,x______      ,x______      ,x______      ,x______                                                                ,x______      ,x______      ,x______      ,x______      ,x______
/*T*/                                                                      ,x______      ,x______        ,x______      ,x______
/*T*/                                                                                    ,x______        ,x______
/*T*/                                                        ,x______      ,x______      ,x______        ,x______      ,x______      ,x______
)};
