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
#include "print.h"
#include "keymap_german.h"
#include "sendstring_german.h"

// maximum delay between key presses to be considered simultaneous
#define SIMULTANEOUS_TERM 133

// keycode aliases
// - compact (help to keep colums short when used inside tap-hold macros)
#define ___       _______
#define ____      _______
#define SPC       KC_SPC
#define ENTER     KC_ENTER
#define ESC       KC_ESC
#define TAB       KC_TAB
#define DEL       KC_DEL
#define INS       KC_INS
#define BSPC      KC_BSPC
#define PGDN      KC_PGDN
#define PGUP      KC_PGUP
#define APP       KC_APP
#define HOME      KC_HOME
#define END       KC_END
#define DE_MI     DE_MINS
#define DE_PL     DE_PLUS
#define DE_CN     DE_COLN
#define DE_CI     DE_CIRC
#define DE_DQ     DE_DQUO
#define PSLS      KC_PSLS
//#define CK_DQ     CK_DQSQ
#define KM_CUT    LSFT(KC_DEL)
#define KM_COPY   LCTL(KC_INS)
#define KM_PAST   LSFT(KC_INS)
// - temporary key codes for testing
//#define TEST_A    KC_A
//#define TEST_B    KC_B

enum custom_keycodes {
	CK_NEQ = SAFE_RANGE,
	CK_SB,	// slash / backslash
	CK_QX,
//	CK_DQSQ,	// double quote / single quote
	CK_LMRES,
	CK_CYLAY,	// cycle some layers
	CK_DBG,	// toggle debug

	// custom keys using (my custom, not UC_WINC) AutoHotkey compose:
	// - misc
	CKC_NOT, CKC_POO, CKC_ELIP, CKC_BULLET, CKC_TM, CKC_COPY, CKC_POUND, CKC_NEQ, CKC_AEQ,
	// - dead accents
	CKC_DGRV, CKC_DACUT, CKC_DCIRC, CKC_DDEGR, CKC_DDIA, CKC_DTILD, CKC_DCEDI,
	// - superscript and subscript digits
	CKC_SUP0, CKC_SUP1, CKC_SUP2, CKC_SUP3, CKC_SUP4, CKC_SUP5, CKC_SUP6, CKC_SUP7, CKC_SUP8, CKC_SUP9, CKC_SUPN,
	CKC_SUB0, CKC_SUB1, CKC_SUB2, CKC_SUB3, CKC_SUB4, CKC_SUB5, CKC_SUB6, CKC_SUB7, CKC_SUB8, CKC_SUB9, CKC_SUBN,
	// - triangles (outline and filled)
	CKC_TRIO_N, CKC_TRIO_S, CKC_TRIO_W, CKC_TRIO_E, CKC_TRIF_N, CKC_TRIF_S, CKC_TRIF_W, CKC_TRIF_E,
	// - arrows and double arrows
	CKC_ARR_N1, CKC_ARR_S1, CKC_ARR_W1, CKC_ARR_E1, CKC_ARR_NW1, CKC_ARR_NE1, CKC_ARR_SE1, CKC_ARR_SW1, CKC_ARR_WE1, CKC_ARR_NS1,
	CKC_ARR_N2, CKC_ARR_S2, CKC_ARR_W2, CKC_ARR_E2, CKC_ARR_NW2, CKC_ARR_NE2, CKC_ARR_SE2, CKC_ARR_SW2, CKC_ARR_WE2, CKC_ARR_NS2,
	// - frames and double frames
	CKC_FRM_N1, CKC_FRM_S1, CKC_FRM_W1, CKC_FRM_E1, CKC_FRM_NW1, CKC_FRM_NE1, CKC_FRM_SE1, CKC_FRM_SW1, CKC_FRM_HL1, CKC_FRM_VL1, CKC_FRM_CR1,
	CKC_FRM_N2, CKC_FRM_S2, CKC_FRM_W2, CKC_FRM_E2, CKC_FRM_NW2, CKC_FRM_NE2, CKC_FRM_SE2, CKC_FRM_SW2, CKC_FRM_HL2, CKC_FRM_VL2, CKC_FRM_CR2
};

// layers
enum custom_layers {
	_BA = 0, //base
	_BT,	//like base, but tap only (no tap-hold) => hold can be used to repeat, when
	    	//TAPPING_FORCE_HOLD disables (tap, hold) repetition
	_L3,	//layer 3 (counting base as 1 and shift as 2; activation: "odd" or both shift keys)
		// "odd" shift means using the shift key on the same side of the keyboard as the to-be-combined (tapped) key.
		// E.g. left shift + A acts as layer 3 instead of shift.
		// As long as shift is held further, it keeps its role regardless of the side of the next keys;
		// So holding normal shift is still like CAPS lock.
	_L4,	//layer 4
	_L5,	//layer 5
	_L5b,	//= layer 5, but needed for OSL on _L4 to work (because of my special handling shift + _L4 = _L5?)
	_NU,	//numbers, navigation
	_FS,	//frames
	_FD,	//double frames
	_AS,	//arrows
	_AD,	//double arrows
	_NV,	//navigation
	_DC,	//LCTL + digits
	_DA,	//LALT + digits
	_DW,	//LWIN + digits
	_LY,	//layer selection

	LAYER_COUNT
};

// double shift layers
#define _OSH  _L3	//odd shift = _L3
#define _DSS  _L3	//double shift (simultaneous) = _L3
#define _DSL  _L4	//double shift (left first)   = _L4
#define _DSR  _L5	//double shift (right first)  = _L5

#define _S_L4 _L5b	//shift + layer 4 = layer 5b

// tap dance keys
enum {
    SQ_DQ_L4,
};

// layers
#include "./keymap_layers.c"
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_LY] = _LY_LAYOUT,
	[_BA] = _BA_LAYOUT,
	[_BT] = _BT_LAYOUT,
	[_L3] = _L3_LAYOUT,
	[_L4] = _L4_LAYOUT,
	[_L5] = _L5_LAYOUT,
	[_L5b] = TRANS_LAYOUT,
	[_NV] = _NV_LAYOUT,
	[_NU] = _NU_LAYOUT,
	[_DC] = _DI_LAYOUT,
	[_DA] = _DI_LAYOUT,
	[_DW] = _DI_LAYOUT,
	[_AS] = _AS_LAYOUT,
	[_AD] = _AD_LAYOUT,
	[_FS] = _FS_LAYOUT,
	[_FD] = _FD_LAYOUT
};

const uint8_t layer_leds[] = {
	// These values (1 = blue, 2 = green, 4 = red) are chosen here for readability in binary literals.
	// Function layer_state_set_user_led maps them to ergodox_right_led_on calls.
	//... = ..RGB
	[_BT] = 0b101,
	[_L3] = 0b010,
	[_L4] = 0b010,
	[_L5] = 0b010,
	[_L5b]= 0b010,
	[_NU] = 0b001,
	[_NV] = 0b100,
	[_AS] = 0b011,
	[_AD] = 0b011,
	[_FS] = 0b011,
	[_FD] = 0b011,
	[_LY] = 0b111,
};
const uint8_t layer_leds_length = sizeof(layer_leds) / sizeof(layer_leds[0]);

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
	case QK_TAP_DANCE ... QK_TAP_DANCE_MAX: // https://docs.qmk.fm/features/tap_dance#example-6
		return 250;
	case OSL(_L3):
	case OSL(_L4):
		return 1;
	default:
		return TAPPING_TERM;
	}
}

// set or reset modifiers if a given layer is entered or exited
static layer_state_t prev_layer_state;
void set_mods_for_layer(layer_state_t state, uint8_t layer, uint16_t mods) {
	layer_state_t layer_mask = (layer_state_t) 1 << layer;
	if ((state & layer_mask) == layer_mask) {
		add_mods(mods);
	} else if ((prev_layer_state & layer_mask) == layer_mask) {
		del_mods(mods);
	}
}

layer_state_t layer_state_set_user_impl(layer_state_t state) {
	set_mods_for_layer(state, _DC, MOD_LCTL);
	set_mods_for_layer(state, _DA, MOD_LALT);
	set_mods_for_layer(state, _DW, MOD_LGUI);

	// shift + layer _L4 => layer _S_L4
	// Inspired by the update_tri_layer_state implementation.
	// TODO Relies on MO(_L5) on shift keys in _L4; only works if LT(_L4,..) is held before shift.
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
	uprintf("\nkeyboard_post_init_user()\n");
	debug_enable = false;

	#ifdef RGBLIGHT_COLOR_LAYER_0
		rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
	#endif
}

static uint8_t leds_state = 0;
void set_leds_for_layer(uint8_t layer) {
	uint8_t new_leds_state = 0 <= layer && layer < layer_leds_length ? layer_leds[layer] : 0;
	uint8_t rgb_mask = 0b100; // start with my representation of R (as the following R, G, B for loop)
	for (uint8_t led_no = 1; led_no <= 3; led_no++) { // args needed for ergodox_right_led_on: R = 1, G = 2, B = 3
		bool led_state = leds_state & rgb_mask;
		bool new_led_state = new_leds_state & rgb_mask;
		if (led_state != new_led_state) {
			if (new_led_state) { ergodox_right_led_on(led_no); } else { ergodox_right_led_off(led_no); }
		}
		rgb_mask /= 2; // iterate: 100, 010, 001 (R, G, B)
	}
	leds_state = new_leds_state;
}

static uint16_t prev_keycode;
layer_state_t layer_state_set_user(layer_state_t state) {
	layer_state_t new_layer_state = layer_state_set_user_impl(state);

	// Update LEDs on every layer change (locked (as TO does) or temporary (as MO)).
	// I don't want that anymore, only on locked layers.
//	set_leds_for_layer(get_highest_layer(new_layer_state));
	// Instead, only for base layer ...:
	uint8_t layer = get_highest_layer(new_layer_state);
	if (layer == _BA) { set_leds_for_layer(_BA); }
	// ... or any TO(..) keycode (TODO: handle TG(..) if used):
	// quantum_keycodes.h: TO(layer) = (QK_TO | (ON_PRESS << 0x4) | ((layer)&0xFF))
	else if ((prev_keycode & 0xFF00) == (TO(0) & 0xFF00)) {
		dprintf("keycode %s is a TO(..).\n", prev_keycode);
		set_leds_for_layer(layer);
	}

	prev_layer_state = new_layer_state;
	return new_layer_state;
}

bool process_record_user_impl(uint16_t keycode, keyrecord_t *record);

// process_record_user implementation: cycle layer
bool pru_cycle_layer(keyrecord_t *record, layer_state_t cycle_layers_mask) {
	if (!record->event.pressed) { return false; }
	dprintf("pru_cycle_layer(.., 0x%04X)\n", cycle_layers_mask);

	uint8_t current_layer = get_highest_layer(layer_state);
	dprintf("current layer: %d\n", current_layer);
	for (uint8_t offset = 1; offset < LAYER_COUNT; offset++) {
		uint8_t layer = (current_layer + offset) % LAYER_COUNT;
		layer_state_t layer_mask = (layer_state_t) 1 << layer;
		dprintf("check layer %d\t(mask 0x%04X)\n", layer, layer_mask);
		if ((layer_mask & cycle_layers_mask) == layer_mask) {
			dprintf("=> new layer: %d\n", layer);
//			tap_code16(TO(layer));
			layer_clear();
			layer_on(layer);
			set_leds_for_layer(layer);
			return false;
		}
	}
	dprintf("no new layer found?!\n");
	return false;
}
// }

bool mod_sensitive_tap_code16(uint16_t mod_mask, uint16_t keycode, uint16_t mod_keycode) {
	uint16_t mods = get_mods();
	if (!(mods & mod_mask)) {	// without modifier
		tap_code16(keycode);
	} else {	// with modifier
		del_mods(mod_mask);
		tap_code16(mod_keycode);
		set_mods(mods);
	}
	return false;
}

// process_record_user implementation for a mod-sensitive custom key
bool pru_mod_sensitive_key(keyrecord_t *record, uint16_t mod_mask, uint16_t keycode, uint16_t mod_keycode) {
	return record->event.pressed && mod_sensitive_tap_code16(mod_mask, keycode, mod_keycode);
}

// process_record_user implementation for (custom AutoHotkey) compose sequences
bool pru_compose(bool pressed, const char *s) {
	if (pressed) {
		SEND_STRING(SS_LSFT("3"));
		send_string(s);

		// Workaround attempt against locked shift
		tap_code(KC_LSFT);
		del_mods(MOD_MASK_SHIFT);
	}
	return false;
}

bool pru_compose_k(bool pressed, uint16_t keycode);

//	// process_record_user implementation for LT(hold_layer, OSL(oneshot_layer)).
//	// Adapted from "QMK: Is combining One-Shot-Layers and Layer-Toggles possible?" [https://www.reddit.com/r/olkb/comments/v5zvo4/comment/ibw1wx6]
//	// Bug: With a chain like
//	// - base to _L3: pru_lt_osl(.., .., _L3) on base,
//	// - _L3 to _L4:  pru_lt_osl(.., .., _L4) on _L3,
//	// - _L4 to _L5:  pru_lt_osl(.., .., _L5) on _L4,
//	// going to _L5 and pressing a key with base and _L5 assignments, both assigned keycodes are sent (e.g. "aÄ" instead of just "Ä").
//	static bool osl_other_key_pressed = false;
//	bool pru_lt_osl(keyrecord_t *record, uint8_t hold_layer, uint8_t oneshot_layer) {
//		if (record->event.pressed) {
//			layer_on(hold_layer);
//			//TODO: set_leds_for_layer
//			osl_other_key_pressed = false;
//		} else {
//			layer_off(hold_layer);
//			if (!osl_other_key_pressed) {
//				set_oneshot_layer(oneshot_layer, ONESHOT_OTHER_KEY_PRESSED);
//			}
//		}
//		return true;
//	}

enum shift_state_t { OFF = 0, PENDING, NORMAL, ODD, DOUBLE };
static shift_state_t shift_state;
// process_record_user implementation: shift mode (normal, odd, ...)
bool pru_shift(uint16_t keycode, keyrecord_t *record) {
	if (shift_state != OFF && record->tap.count) {
	}
	return true;
}

static uint16_t time_since_prev_key;
static uint16_t prev_key_timer;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		time_since_prev_key = timer_elapsed(prev_key_timer);
	}
	prev_key_timer = timer_read();
	bool result = process_record_user_impl(keycode, record);
	prev_keycode = keycode;
	return result;
}

bool process_record_user_impl(uint16_t keycode, keyrecord_t *record) {
//	// see function pru_lt_osl
//	if (keycode != LT(_LY, OSL(_L3)) && keycode != LT(_LY, OSL(_L4)) && keycode != LT(_LY, OSL(_L5))) { osl_other_key_pressed = true; }

	if (!pru_shift(keycode, record) { return false; }

	bool pressed = record->event.pressed;
//	dprintf("process_record_user(%d, ..): pressed == %b\n", keycode, pressed);

	switch (keycode) {
	case CK_LMRES:	// layer and modifier reset
		layer_clear();
		clear_keyboard();
		return false;
	case CK_DBG:
		if (pressed) {
			debug_enable = !debug_enable;
		}
		dprintf("debug_enable == %b\n", debug_enable);
		return false;
	case CK_CYLAY:	// cycle through some layers (some more with ALT)
		return pru_cycle_layer(record,
			((get_mods() & MOD_MASK_ALT) == 0)
			? ((1<<_BA) | (1<<_BT) | (1<<_L3) | (1<<_NV))
			: ((1<<_BA) | (1<<_BT) | (1<<_L3) | (1<<_NV) | (1<<_NU) | (1<<_AS) | (1<<_AD) | (1<<_FS) | (1<<_FD)));
	case LT(_L5, CK_SB):
		// LT with non-basic tap keycode: https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps
		if (!(pressed && record->tap.count)) { break; }
		// else (tap): fall through
	case CK_SB:	// DE slash; with shift: DE backslash (but without shift (for layouts where that would give a capital sharp s))
		return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, DE_SLSH, RALT(DE_BSLS));
//	case LT(_L4,CK_DQSQ):
//		// LT with non-basic tap keycode: https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps
//		if (!(pressed && record->tap.count)) { break; }
//		// else (tap): fall through
//	case CK_DQSQ:	// DE double quote; with shift: DE single quote
//		return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, DE_DQUO, DE_QUOT);
	case LT(_DA, CK_QX):
		// LT with non-basic tap keycode: https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps
		if (!(pressed && record->tap.count)) { break; }
		// else (tap): fall through
	case CK_QX:	// DE question mark; with shift: DE exclamation mark
		return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, S(DE_QUES), DE_EXLM);
	case LT(_L5, DE_DQUO):
		// LT with non-basic tap keycode: https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps
		if (pressed && record->tap.count) {
			tap_code16(DE_DQUO);
			return false;
		}
		break;
	case LT(_LY, DE_COLN):
		// LT with non-basic tap keycode: https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps
		if (pressed && record->tap.count) {
			tap_code16(DE_COLN);
			return false;
		}
		break;
	case CK_NEQ:
		if (pressed) { SEND_STRING("!="); }
		return false;
	case KC_LSFT:	// double shift => release shift, activate layer _DSS, _DSL, or _DSR,
	case KC_RSFT:	// depending on timing and order
		/*
		shift_count += pressed ? 1 : -1;
		switch (shift_count) {
		case 0:
			del_mods(MOD_MASK_SHIFT);
			return true;
		case 1:
			if (pressed) {	// previous shift_state was 0 => normal shift behavior
				return true;
			} else {	// previous shift_state was 2 => switch back from layer _DS.. to shift
				layer_off(_DSS);
				layer_off(_DSL);
				layer_off(_DSR);
				add_mods(MOD_MASK_SHIFT);
				return false;
			}
		default:	// double shift => release shift, activate layer _DS..
			del_mods(MOD_MASK_SHIFT);
			layer_on(
				(prev_keycode == KC_LSFT || prev_keycode == KC_RSFT)
				? (time_since_prev_key > SIMULTANEOUS_TERM
					? (keycode == KC_RSFT ? _DSL : _DSR)
					: _DSS)
				: _DSS);
			return false;
		}
		*/
		switch (shift_state) {
		case OFF:
			if (pressed) {
				shift_state = PENDING;
			}
			return true;
		case PENDING:
			if (!pressed) { del_mods(MOD_MASK_SHIFT); return true; }
		case NORMAL:
			if (!pressed) { del_mods(MOD_MASK_SHIFT); return true; }
		case ODD:
		case DOUBLE:
		}
	}

	if (!pru_compose_k(pressed, keycode)) { return false; }

	return true;
}

// process_record_user cases for custom keys using (my custom, not UC_WINC) AutoHotkey compose
bool pru_compose_k(bool pressed, uint16_t keycode) {
	switch (keycode) {
	// - misc
	case CKC_NOT    : return pru_compose(pressed, "bn");
	case CKC_NEQ    : return pru_compose(pressed, "=n");
	case CKC_AEQ    : return pru_compose(pressed, "=a");
	case CKC_POO    : return pru_compose(pressed, "xp");
	case CKC_BULLET : return pru_compose(pressed, ".o");
	case CKC_ELIP   : return pru_compose(pressed, "...");
	case CKC_POUND  : return pru_compose(pressed, "cp");
	case CKC_TM     : return pru_compose(pressed, "xM");
	case CKC_COPY   : return pru_compose(pressed, "xC");
	// - dead accents
	case CKC_DGRV   : return pru_compose(pressed, ",`");
	case CKC_DACUT  : return pru_compose(pressed, ",'");
	case CKC_DCIRC  : return pru_compose(pressed, ",^");
	case CKC_DDEGR  : return pru_compose(pressed, ",°");
	case CKC_DDIA   : return pru_compose(pressed, ",\"");
	case CKC_DTILD  : return pru_compose(pressed, ",~");
	case CKC_DCEDI  : return pru_compose(pressed, ",,");
	// - superscript and subscript digits (TODO: find a place in the keymap for them)
	case CKC_SUP0   : return pru_compose(pressed, "^0");
	case CKC_SUP1   : return pru_compose(pressed, "^1");
	case CKC_SUP2   : return pru_compose(pressed, "^2");
	case CKC_SUP3   : return pru_compose(pressed, "^3");
	case CKC_SUP4   : return pru_compose(pressed, "^4");
	case CKC_SUP5   : return pru_compose(pressed, "^5");
	case CKC_SUP6   : return pru_compose(pressed, "^6");
	case CKC_SUP7   : return pru_compose(pressed, "^7");
	case CKC_SUP8   : return pru_compose(pressed, "^8");
	case CKC_SUP9   : return pru_compose(pressed, "^9");
	case CKC_SUPN   : return pru_compose(pressed, "^n");
	case CKC_SUB0   : return pru_compose(pressed, "_0");
	case CKC_SUB1   : return pru_compose(pressed, "_1");
	case CKC_SUB2   : return pru_compose(pressed, "_2");
	case CKC_SUB3   : return pru_compose(pressed, "_3");
	case CKC_SUB4   : return pru_compose(pressed, "_4");
	case CKC_SUB5   : return pru_compose(pressed, "_5");
	case CKC_SUB6   : return pru_compose(pressed, "_6");
	case CKC_SUB7   : return pru_compose(pressed, "_7");
	case CKC_SUB8   : return pru_compose(pressed, "_8");
	case CKC_SUB9   : return pru_compose(pressed, "_9");
	case CKC_SUBN   : return pru_compose(pressed, "_n");
	// - triangles
	case CKC_TRIO_N : return pru_compose(pressed, "Tog");
	case CKC_TRIO_S : return pru_compose(pressed, "Tor");
	case CKC_TRIO_W : return pru_compose(pressed, "Ton");
	case CKC_TRIO_E : return pru_compose(pressed, "Tot");
	case CKC_TRIF_N : return pru_compose(pressed, "Tfg");
	case CKC_TRIF_S : return pru_compose(pressed, "Tfr");
	case CKC_TRIF_W : return pru_compose(pressed, "Tfn");
	case CKC_TRIF_E : return pru_compose(pressed, "Tft");
	// - arrows and double arrows
	case CKC_ARR_N1 : return pru_compose(pressed, "ag");
	case CKC_ARR_S1 : return pru_compose(pressed, "ar");
	case CKC_ARR_W1 : return pru_compose(pressed, "an");
	case CKC_ARR_E1 : return pru_compose(pressed, "at");
	case CKC_ARR_NW1: return pru_compose(pressed, "ah");
	case CKC_ARR_NE1: return pru_compose(pressed, "af");
	case CKC_ARR_SE1: return pru_compose(pressed, "a.");
	case CKC_ARR_SW1: return pru_compose(pressed, "am");
	case CKC_ARR_WE1: return pru_compose(pressed, "ad");
	case CKC_ARR_NS1: return pru_compose(pressed, "a,");
	case CKC_ARR_N2 : return pru_compose(pressed, "aag");
	case CKC_ARR_S2 : return pru_compose(pressed, "aar");
	case CKC_ARR_W2 : return pru_compose(pressed, "aan");
	case CKC_ARR_E2 : return pru_compose(pressed, "aat");
	case CKC_ARR_NW2: return pru_compose(pressed, "aah");
	case CKC_ARR_NE2: return pru_compose(pressed, "aaf");
	case CKC_ARR_SE2: return pru_compose(pressed, "aa.");
	case CKC_ARR_SW2: return pru_compose(pressed, "aam");
	case CKC_ARR_WE2: return pru_compose(pressed, "aad");
	case CKC_ARR_NS2: return pru_compose(pressed, "aa,");
	// - frames and double frames
	case CKC_FRM_N1 : return pru_compose(pressed, "fg");
	case CKC_FRM_S1 : return pru_compose(pressed, "f,");
	case CKC_FRM_W1 : return pru_compose(pressed, "fn");
	case CKC_FRM_E1 : return pru_compose(pressed, "ft");
	case CKC_FRM_NW1: return pru_compose(pressed, "fh");
	case CKC_FRM_NE1: return pru_compose(pressed, "ff");
	case CKC_FRM_SE1: return pru_compose(pressed, "f.");
	case CKC_FRM_SW1: return pru_compose(pressed, "fm");
	case CKC_FRM_HL1: return pru_compose(pressed, "fd");
	case CKC_FRM_VL1: return pru_compose(pressed, "fs");
	case CKC_FRM_CR1: return pru_compose(pressed, "fr");
	case CKC_FRM_N2 : return pru_compose(pressed, "Fg");
	case CKC_FRM_S2 : return pru_compose(pressed, "F,");
	case CKC_FRM_W2 : return pru_compose(pressed, "Fn");
	case CKC_FRM_E2 : return pru_compose(pressed, "Ft");
	case CKC_FRM_NW2: return pru_compose(pressed, "Fh");
	case CKC_FRM_NE2: return pru_compose(pressed, "Ff");
	case CKC_FRM_SE2: return pru_compose(pressed, "F.");
	case CKC_FRM_SW2: return pru_compose(pressed, "Fm");
	case CKC_FRM_HL2: return pru_compose(pressed, "Fd");
	case CKC_FRM_VL2: return pru_compose(pressed, "Fs");
	case CKC_FRM_CR2: return pru_compose(pressed, "Fr");
	}

	return true;
}

#include "./keymap_tapdance.c"
