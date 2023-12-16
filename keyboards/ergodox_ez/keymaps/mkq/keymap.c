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

// layers
enum custom_layers {
	_BA = 0, //base
	_BT,	//like base, but tap only (no tap-hold) => hold can be used to repeat, when
	    	//TAPPING_FORCE_HOLD disables (tap, hold) repetition
	_L3,	//layer 3 (counting base as 1 and shift as 2; activation: both shift keys)
	_L4,	//layer 4
	_L5,	//layer 5 (activation: shift + layer 4)
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
#define _DSS  _L3	//double shift (simultaneous) = _L3
#define _DSL  _L4	//double shift (left first)   = _L4
#define _DSR  _L5	//double shift (right first)  = _L5

#define _S_L4 _L5b	//shift + layer 4 = layer 5b

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

// maximum delay between key presses to be considered simultaneous
#define SIMULTANEOUS_TERM 133

enum custom_keycodes {
	CK_NEQ = SAFE_RANGE,
	CK_SB,	// slash / backslash
	CK_QX,
	CK_DQSQ,	// double quote / single quote
	CK_LMRES,
	CK_CYLAY,	// cycle some layers
	CK_DBG,	// toggle debug
	CK_DEADQ,	// dead Q

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

// dead key maps
// format: [0] fallback key, [1] number of mappings; [2],[3] mapping 1, [4],[5] mapping 2, ...
const uint16_t DEAD_MAP_Q[] = {
	KC_Q,	4,
	KC_A,	DE_ADIA,
	KC_O,	DE_ODIA,
	KC_U,	DE_UDIA,
	DE_Z,	DE_SS
};

static uint16_t previous_keycode;

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
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
	else if ((previous_keycode & 0xFF00) == (TO(0) & 0xFF00)) {
		dprintf("keycode %s is a TO(..).\n", previous_keycode);
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

// process_record_user implementation for a mod-sensitive custom key
bool pru_mod_sensitive_key(keyrecord_t *record, uint16_t mod_mask, uint16_t keycode, uint16_t mod_keycode) {
	if (record->event.pressed) {
		uint16_t mods = get_mods();
		if (!(mods & mod_mask)) {	// without modifier
			tap_code16(keycode);
		} else {	// with modifier
			del_mods(mod_mask);
			tap_code16(mod_keycode);
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

		// Workaround attempt against locked shift
		tap_code(KC_LSFT);
		del_mods(MOD_MASK_SHIFT);
	}
	return false;
}

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

static const uint16_t *active_dead_key_array;
bool pru_dead_key_start(const uint16_t *dead_key_array, keyrecord_t *record) {
	if (get_mods() != 0) {	// do not treat as dead key with mods
		return process_record_user_impl(dead_key_array[0], record);
	}
	active_dead_key_array = dead_key_array;
	return false;
}

bool pru_dead_key_continue(uint16_t keycode, keyrecord_t *record) {
	const uint16_t count = active_dead_key_array[1];
	for (uint16_t c = 0; c < count; c++) {
		uint16_t from_key = active_dead_key_array[3 + 2 * c];
		if (keycode == from_key) {
			uint16_t to_key = active_dead_key_array[2 + 2 * c];
			active_dead_key_array = NULL;
			return process_record_user_impl(to_key, record);
		}
	}
	// fallback
	active_dead_key_array = NULL;
	return process_record_user_impl(active_dead_key_array[0], record);
}

static uint16_t previous_key_timer;
static uint16_t time_since_previous_key;
static uint8_t shift_count;
bool process_record_user_impl(uint16_t keycode, keyrecord_t *record) {
//		// see function pru_lt_osl
//		if (keycode != LT(_LY, OSL(_L3)) && keycode != LT(_LY, OSL(_L4)) && keycode != LT(_LY, OSL(_L5))) {
//			osl_other_key_pressed = true;
//		}

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
	case LT(_LY, CK_SB):
		// LT with non-basic tap keycode: https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps
		if (pressed && record->tap.count) {
			return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, DE_SLSH, RALT(DE_BSLS));
		}
		break;
	case CK_SB:	// DE slash; with shift: DE backslash (but without shift (for layouts where that would give a capital sharp s))
		return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, DE_SLSH, RALT(DE_BSLS));
	case CK_DQSQ:	// DE double quote; with shift: DE single quote
		return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, DE_DQUO, DE_QUOT);
	case CK_QX:	// DE question mark; with shift: DE exclamation mark
		return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, S(DE_QUES), DE_EXLM);
	case LT(_DA, CK_QX):
		// LT with non-basic tap keycode: https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps
		if (pressed && record->tap.count) {
			return pru_mod_sensitive_key(record, MOD_MASK_SHIFT, S(DE_QUES), DE_EXLM);
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
	case CK_DEADQ:
		return pru_dead_key_start(DEAD_MAP_Q, record);
	case KC_LSFT:	// double shift => release shift, activate layer _DSS, _DSL, or _DSR,
	case KC_RSFT:	// depending on timing and order
		shift_count += pressed ? 1 : -1;
		switch (shift_count) {
		case 0:
			del_mods(MOD_MASK_SHIFT);
			return true;
		case 1:
			if (pressed) {	// previous shift_count was 0 => normal shift behavior
				return true;
			} else {	// previous shift_count was 2 => switch back from layer _DS to shift
				layer_off(_DSS);
				layer_off(_DSL);
				layer_off(_DSR);
				add_mods(MOD_MASK_SHIFT);
				return false;
			}
		default:	// double shift => release shift, activate layer _DS..
			del_mods(MOD_MASK_SHIFT);
			layer_on(
				(previous_keycode == KC_LSFT || previous_keycode == KC_RSFT)
				? (time_since_previous_key > SIMULTANEOUS_TERM
					? (keycode == KC_RSFT ? _DSL : _DSR)
					: _DSS)
				: _DSS);
			return false;
		}

	// custom keys using (my custom, not UC_WINC) AutoHotkey compose:
	// - misc
	case CKC_NOT    : return pru_compose(record, "bn");
	case CKC_NEQ    : return pru_compose(record, "=n");
	case CKC_AEQ    : return pru_compose(record, "=a");
	case CKC_POO    : return pru_compose(record, "xp");
	case CKC_BULLET : return pru_compose(record, ".o");
	case CKC_ELIP   : return pru_compose(record, "...");
	case CKC_POUND  : return pru_compose(record, "cp");
	case CKC_TM     : return pru_compose(record, "xM");
	case CKC_COPY   : return pru_compose(record, "xC");
	// - dead accents
	case CKC_DGRV   : return pru_compose(record, ",`");
	case CKC_DACUT  : return pru_compose(record, ",'");
	case CKC_DCIRC  : return pru_compose(record, ",^");
	case CKC_DDEGR  : return pru_compose(record, ",°");
	case CKC_DDIA   : return pru_compose(record, ",:");
	case CKC_DTILD  : return pru_compose(record, ",~");
	case CKC_DCEDI  : return pru_compose(record, ",,");
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
	case CKC_SUPN   : return pru_compose(record, "^n");
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
	case CKC_SUBN   : return pru_compose(record, "_n");
	// - triangles
	case CKC_TRIO_N : return pru_compose(record, "Tog");
	case CKC_TRIO_S : return pru_compose(record, "Tor");
	case CKC_TRIO_W : return pru_compose(record, "Ton");
	case CKC_TRIO_E : return pru_compose(record, "Tot");
	case CKC_TRIF_N : return pru_compose(record, "Tfg");
	case CKC_TRIF_S : return pru_compose(record, "Tfr");
	case CKC_TRIF_W : return pru_compose(record, "Tfn");
	case CKC_TRIF_E : return pru_compose(record, "Tft");
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	time_since_previous_key = timer_elapsed(previous_key_timer);
	previous_key_timer = timer_read();
	bool result = active_dead_key_array != NULL
		? pru_dead_key_continue(keycode, record)
		: process_record_user_impl(keycode, record);
	previous_keycode = keycode;
	return result;
}

// keycode aliases
// - compact (help to keep colums short when used inside tap-hold macros)
#define SPC       KC_SPC
#define ENTER     KC_ENTER
#define ESC       KC_ESC
#define TAB       KC_TAB
#define DEL       KC_DEL
#define BSPC      KC_BSPC
#define APP       KC_APP
#define HOME      KC_HOME
#define END       KC_END
#define DE_MI     DE_MINS
#define DE_PL     DE_PLUS
#define DE_CN     DE_COLN
#define PSLS      KC_PSLS
#define KM_CUT    LSFT(KC_DEL)
#define KM_COPY   LCTL(KC_INS)
#define KM_PAST   LSFT(KC_INS)
// - temporary key codes for testing
//#define TEST_A    KC_A
//#define TEST_B    KC_B

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
