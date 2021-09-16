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

// layers
#define L_BA	0	//base
#define L_SH	1	//shift
#define L_Y1	2	//symbols 1
#define L_Y2	3	//symbols 2
#define L_CF	15	//keyboard config
#define L_NV	14	//navigation

#define _______   KC_TRNS
#define KM_CUT    LSFT(KC_DEL)
#define KM_COPY   LCTL(KC_INS)
#define KM_PAST   LSFT(KC_INS)

/*
// Shift + ? = !
// Does not require a shift layer.
// But now that we use a shift layer (LM(L_SH, MOD_LSFT)) to get to the double-shift layer (L_Y2),
// a key_override is no longer necessary.
const key_override_t **key_overrides = (const key_override_t *[]) {
	&ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, DE_QUES, DE_EXLM, 1, ~MOD_MASK_SHIFT)
};
*/

layer_state_t layer_state_set_user(layer_state_t state) {
	// Similar to [https://github.com/qmk/qmk_firmware/issues/2782]:
	// Layer L_Y2 is selected via LM(.., MOD_LSFT). To get unshifted symbols, we need to clear shift.
	// (Replacing MO(L_Y2) with LM(L_Y2, 0) did not help.)
	if (IS_LAYER_ON_STATE(state, L_Y2)) {
		del_mods(MOD_LSFT);
	}
	return state;
}

/* template
[L_] = LAYOUT(
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
),
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[L_BA] = LAYOUT( //base
 KC_PAUS      ,KC_1         ,KC_2         ,KC_3         ,KC_4         ,KC_5         ,KC_6         ,KC_7       ,KC_8         ,KC_9         ,KC_0         ,KC_PSCR      ,KM_CUT       ,(       KM_COPY     )      ,(      KM_PAST      )
,KC_F1        ,KC_F2        ,KC_F3        ,KC_F4        ,KC_F5        ,KC_F6                                  ,KC_F7        ,KC_F8        ,KC_F9        ,KC_F10       ,KC_F11       ,KC_F12       ,KC_DEL       ,(      KC_INS       )
,KC_GRV       ,KC_X         ,KC_V         ,KC_SPC       ,KC_C         ,KC_W                                   ,KC_K         ,KC_H         ,KC_G         ,KC_F         ,DE_Y         ,KC_CAPS      ,KC_HOME      ,KC_END       ,KC_PGUP
,TT(L_Y1)     ,LGUI_T(KC_U) ,LALT_T(KC_I) ,LCTL_T(KC_A) ,LT(L_NV,KC_E),KC_O               ,DE_QUES            ,KC_S         ,LT(L_NV,KC_N),RCTL_T(KC_R) ,LALT_T(KC_T) ,RGUI_T(KC_D) ,TT(L_Y1)     ,(      KC_SPC       )      ,KC_PGDN
,KC_ESC       ,KC_RO        ,KC_SLSH      ,KC_L         ,KC_P         ,DE_Z               ,KC_LEAD            ,KC_B         ,KC_M         ,KC_COMM      ,KC_DOT       ,KC_J         ,KC_Q                       ,KC_UP
,KC_APP       ,KC_LWIN      ,(      KC_BSPC      )      ,( LM(L_SH,MOD_LSFT) )      ,KC_TAB       ,KC_ENTER   ,( LM(L_SH,MOD_LSFT) )      ,(      TT(L_NV)      )     ,KC_LALT      ,KC_LCTL      ,KC_LEFT      ,KC_DOWN      ,KC_RGHT
),
[L_SH] = LAYOUT( //shift
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,DE_EXLM            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(     MO(L_Y2)      )      ,_______      ,_______    ,(     MO(L_Y2)      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
),
[L_Y1] = LAYOUT( //symbols 1
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,DE_LCBR      ,DE_RCBR      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,DE_LPRN      ,DE_RPRN      ,_______      ,_______      ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,DE_LBRC      ,DE_RBRC      ,_______      ,_______      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
),
[L_Y2] = LAYOUT( //symbols 2
 _______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,KC_7         ,KC_8         ,KC_9         ,_______      ,_______      ,_______      ,_______      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,DE_ASTR      ,KC_4         ,KC_5         ,KC_6         ,_______      ,_______      ,(      _______      )      ,_______
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,KC_0         ,KC_1         ,KC_2         ,KC_3         ,_______      ,_______                    ,_______
,_______      ,_______      ,(      _______      )      ,(       KC_NO       )      ,_______      ,_______    ,(       KC_NO       )      ,(      _______      )      ,_______      ,_______      ,_______      ,_______      ,_______
),
[L_NV] = LAYOUT( //navigation
 DM_RSTP      ,DM_REC1      ,DM_REC2      ,RGB_TOG      ,RGB_MOD      ,RGB_HUI      ,RGB_VAD      ,RGB_VAI    ,RGB_SPD      ,RGB_SPI      ,_______      ,KC_SLCK      ,RESET        ,(      NK_TOGG      )      ,(      EEP_RST      )
,LWIN(KC_1)   ,LWIN(KC_2)   ,LWIN(KC_3)   ,LWIN(KC_4)   ,LWIN(KC_5)   ,LWIN(KC_6)                             ,LWIN(KC_7)   ,LWIN(KC_8)   ,LWIN(KC_9)   ,LWIN(KC_0)   ,_______      ,_______      ,_______      ,(      _______      )
,_______      ,C(KC_6)      ,C(KC_7)      ,C(KC_8)      ,C(KC_9)      ,C(KC_0)                                ,C(KC_HOME)   ,KC_HOME      ,KC_UP        ,KC_END       ,C(KC_END)    ,_______      ,KC_WWW_HOME  ,KC_MSEL      ,KC_VOLU
,_______      ,KC_LWIN      ,KC_LALT      ,KC_LCTL      ,_______      ,_______            ,DM_PLY1            ,S(KC_INS)    ,KC_LEFT      ,KC_DOWN      ,KC_RGHT      ,KC_PGUP      ,_______      ,(      KC_MUTE      )      ,KC_VOLD
,C(KC_PLUS)   ,C(KC_1)      ,C(KC_2)      ,C(KC_3)      ,C(KC_4)      ,C(KC_5)            ,DM_PLY2            ,KC_INS       ,C(KC_LEFT)   ,KC_DEL       ,C(KC_RGHT)   ,KC_PGDN      ,_______                    ,KC_MSTP
,C(KC_MINUS)  ,KC_RWIN      ,(      _______      )      ,(      _______      )      ,KC_CAPS      ,KC_APP     ,(      _______      )      ,(      _______      )      ,_______      ,_______      ,KC_MPRV      ,KC_MPLY      ,KC_MNXT
),
[L_CF] = LAYOUT( //keyboard settings
 RESET        ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______    ,_______      ,KC_CALC      ,KC_MYCM      ,KC_MSEL      ,KC_MAIL      ,(      NK_TOGG       )     ,(      EEP_RST      )
,_______      ,_______      ,_______      ,_______      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      KC_NLCK      )
,RGB_TOG      ,RGB_MOD      ,RGB_VAI      ,RGB_HUI      ,_______      ,_______                                ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,KC_HOME
,_______      ,RGB_SPD      ,RGB_VAD      ,RGB_SPI      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,_______      ,(      _______      )      ,KC_END
,_______      ,_______      ,_______      ,_______      ,_______      ,_______            ,_______            ,_______      ,_______      ,_______      ,_______      ,_______      ,KC_MUTE                    ,KC_VOLU
,_______      ,_______      ,(      _______      )      ,(      _______      )      ,_______      ,_______    ,(      _______      )      ,(      _______      )      ,_______      ,KC_MPLY      ,KC_MPRV      ,KC_VOLD      ,KC_MNXT
)
};
