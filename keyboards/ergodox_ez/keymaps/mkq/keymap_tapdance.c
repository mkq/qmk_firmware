// adapted from https://docs.qmk.fm/features/tap_dance#example-6

// Define a type for as many tap dance states as you need
typedef enum {
	TD_NONE,
	TD_UNKNOWN,
	TD_SINGLE_TAP,
	TD_SINGLE_HOLD,
	TD_DOUBLE_TAP
} td_state_t;

typedef struct {
	bool is_press_action;
	td_state_t state;
} td_tap_t;

// ______________________________________________________________________

// Determine the current tap dance state
td_state_t cur_dance(qk_tap_dance_state_t *state) {
	if (state->count == 1) {
		if (!state->pressed) return TD_SINGLE_TAP;
		else return TD_SINGLE_HOLD;
	} else if (state->count == 2) return TD_DOUBLE_TAP;
	else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t sqdql4_tap_state = {
	.is_press_action = true,
	.state = TD_NONE
};

// Functions that control what our tap dance key does
void td_sqdql4_finished(qk_tap_dance_state_t *state, void *user_data) {
	sqdql4_tap_state.state = cur_dance(state);
	switch (sqdql4_tap_state.state) {
		case TD_SINGLE_TAP:
//TODO Bug: may lead to locked shift
//			mod_sensitive_tap_code16(MOD_MASK_SHIFT, DE_QUOT, DE_DQUO);
			tap_code16(DE_QUOT);
			break;
		case TD_DOUBLE_TAP:
			tap_code16(DE_DQUO);
			break;
		case TD_SINGLE_HOLD:
			layer_on(_L4);
			break;
		default:
			break;
	}
}

void td_sqdql4_reset(qk_tap_dance_state_t *state, void *user_data) {
	// If the key was held down and now is released then switch off the layer
	if (sqdql4_tap_state.state == TD_SINGLE_HOLD) {
		layer_off(_L4);
	}
	sqdql4_tap_state.state = TD_NONE;
}

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
	[SQ_DQ_L4] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_sqdql4_finished, td_sqdql4_reset)
};
