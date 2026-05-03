#include "keycodes.h"
#include QMK_KEYBOARD_H
#include "bongo/bongo_cat.h"

#define HMR_A LGUI_T(KC_A)
#define HMR_S LALT_T(KC_S)
#define HMR_D LSFT_T(KC_D)
#define HMR_F LCTL_T(KC_F)

#define HMR_J RCTL_T(KC_J)
#define HMR_K RSFT_T(KC_K)
#define HMR_L RALT_T(KC_L)
#define HMR_SCLN RGUI_T(KC_SCLN)

#define TTY(x) LALT(LCTL(x))

enum layers { ALPHA_HMR, ALPHA,  NUM, SYM, FUNC, TTY_SWITCH, MOUSE };

#ifdef OLED_ENABLE

bool oled_task_user(void) {
    render_bongo_cat();

    /* Get current layer and display with appropriate text */
    oled_set_cursor(0, 0);
    switch (get_highest_layer(layer_state)) {
        case ALPHA_HMR:
            oled_write_P(PSTR("base#"), false);
            break;
        case ALPHA:
            oled_write_P(PSTR("base"), false);
            break;
        case NUM:
            oled_write_P(PSTR("num"), false);
            break;
        case SYM:
            oled_write_P(PSTR("sym"), false);
            break;
        case FUNC:
            oled_write_P(PSTR("func"), false);
            break;
        case TTY_SWITCH:
            oled_write_P(PSTR("tty"), false);
            break;
        case MOUSE:
            oled_write_P(PSTR("mouse#"), false);
            break;
        default:
            oled_write_P(PSTR("UNDEF"), false);
            break;
    }

    const char* wpm = get_u8_str(get_current_wpm(), ' ');

    uint8_t text_length = strlen(wpm);

    oled_set_cursor(oled_max_chars() - text_length, oled_max_lines() - 1);
    oled_write(wpm, false);

    uint8_t mod_state = get_mods();

    char mods[5];
    int idx = 0;

    if (mod_state & MOD_MASK_GUI)   mods[idx++] = 'M'; // meta
    if (mod_state & MOD_MASK_ALT)   mods[idx++] = 'A';
    if (mod_state & MOD_MASK_CTRL)  mods[idx++] = 'C';
    if (mod_state & MOD_MASK_SHIFT) mods[idx++] = 'S';

    mods[idx++] = '\0';

    oled_set_cursor(0, 1);
    oled_write(mods, false);
    oled_set_cursor(0, 2);

    led_t led_state = host_keyboard_led_state();
    oled_write(led_state.caps_lock ? "CAPS" : "    ", false);

    return false;
}

void oled_render_boot(bool bootloader) {
  oled_clear();

  if (bootloader) {
    oled_write_ln_P("Awaiting New Firmware", false);
  }
  oled_render_dirty(true);
}

bool shutdown_user(bool jump_to_bootloader) {
  oled_render_boot(jump_to_bootloader);
  return true;
}

#endif // OLED_ENABLE

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[ALPHA_HMR] = LAYOUT_split_3x6_3(
//    ┌───────────┬───────┬───────┬───────┬────────┬─────────┐                   ┌─────────┬───────┬───────┬───────┬───────┬────┐
//    │    tab    │   q   │   w   │   e   │   r    │    t    │                   │    y    │   u   │   i   │   o   │   p   │ no │
//    ├───────────┼───────┼───────┼───────┼────────┼─────────┤                   ├─────────┼───────┼───────┼───────┼───────┼────┤
//    │    esc    │ win/a │ alt/s │ sft/d │ ctl/f  │    g    │                   │    h    │ ctl/j │ sft/k │ alt/l │ win/; │ '  │
//    ├───────────┼───────┼───────┼───────┼────────┼─────────┤                   ├─────────┼───────┼───────┼───────┼───────┼────┤
//    │ MO(MOUSE) │   z   │   x   │   c   │   v    │    b    │                   │    n    │   m   │   ,   │   .   │   /   │ no │
//    └───────────┴───────┴───────┴───────┼────────┼─────────┼─────┐       ┌─────┼─────────┼───────┼───────┴───────┴───────┴────┘
//                                        │ QK_REP │ MO(NUM) │ spc │       │ ent │ MO(SYM) │ bspc  │
//                                        └────────┴─────────┴─────┘       └─────┴─────────┴───────┘
  KC_TAB    , KC_Q  , KC_W  , KC_E  , KC_R   , KC_T    ,                           KC_Y    , KC_U    , KC_I    , KC_O   , KC_P     , XXXXXXX,
  KC_ESC    , HMR_A , HMR_S , HMR_D , HMR_F  , KC_G    ,                           KC_H    , HMR_J   , HMR_K   , HMR_L  , HMR_SCLN , KC_QUOT,
  MO(MOUSE) , KC_Z  , KC_X  , KC_C  , KC_V   , KC_B    ,                           KC_N    , KC_M    , KC_COMM , KC_DOT , KC_SLSH  , XXXXXXX,
                                      QK_REP , MO(NUM) , KC_SPC ,         KC_ENT , MO(SYM) , KC_BSPC
),

[ALPHA] = LAYOUT_split_3x6_3(
//    ┌──────┬───┬───┬───┬──────┬─────────┐                   ┌─────────┬──────┬───┬───┬───┬──────┐
//    │ tab  │ q │ w │ e │  r   │    t    │                   │    y    │  u   │ i │ o │ p │ lalt │
//    ├──────┼───┼───┼───┼──────┼─────────┤                   ├─────────┼──────┼───┼───┼───┼──────┤
//    │ lsft │ a │ s │ d │  f   │    g    │                   │    h    │  j   │ k │ l │ ; │  '   │
//    ├──────┼───┼───┼───┼──────┼─────────┤                   ├─────────┼──────┼───┼───┼───┼──────┤
//    │ lctl │ z │ x │ c │  v   │    b    │                   │    n    │  m   │ , │ . │ / │ esc  │
//    └──────┴───┴───┴───┼──────┼─────────┼─────┐       ┌─────┼─────────┼──────┼───┴───┴───┴──────┘
//                       │ lgui │ MO(NUM) │ spc │       │ ent │ MO(SYM) │ bspc │
//                       └──────┴─────────┴─────┘       └─────┴─────────┴──────┘
  KC_TAB  , KC_Q , KC_W , KC_E , KC_R    , KC_T    ,                           KC_Y    , KC_U    , KC_I    , KC_O   , KC_P    , KC_LALT,
  KC_LSFT , KC_A , KC_S , KC_D , KC_F    , KC_G    ,                           KC_H    , KC_J    , KC_K    , KC_L   , KC_SCLN , KC_QUOT,
  KC_LCTL , KC_Z , KC_X , KC_C , KC_V    , KC_B    ,                           KC_N    , KC_M    , KC_COMM , KC_DOT , KC_SLSH , KC_ESC ,
                                 KC_LGUI , MO(NUM) , KC_SPC ,         KC_ENT , MO(SYM) , KC_BSPC
),

[NUM] = LAYOUT_split_3x6_3(
//    ┌──────┬──────┬──────┬──────┬──────┬──────┐                   ┌──────────┬──────┬──────┬──────┬────┬───────────┐
//    │ tab  │  1   │  2   │  3   │  4   │  5   │                   │    6     │  7   │  8   │  9   │ 0  │    no     │
//    ├──────┼──────┼──────┼──────┼──────┼──────┤                   ├──────────┼──────┼──────┼──────┼────┼───────────┤
//    │  no  │ mprv │ mply │ mnxt │ vold │ volu │                   │   left   │ down │  up  │ rght │ no │    no     │
//    ├──────┼──────┼──────┼──────┼──────┼──────┤                   ├──────────┼──────┼──────┼──────┼────┼───────────┤
//    │ lctl │  no  │  no  │  no  │  no  │  no  │                   │   home   │ pgdn │ pgup │ end  │ no │ caps_LOCK │
//    └──────┴──────┴──────┴──────┼──────┼──────┼─────┐       ┌─────┼──────────┼──────┼──────┴──────┴────┴───────────┘
//                                │      │  no  │ spc │       │ ent │ MO(FUNC) │ del  │
//                                └──────┴──────┴─────┘       └─────┴──────────┴──────┘
  KC_TAB  , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    ,                           KC_6     , KC_7    , KC_8    , KC_9     , KC_0    , XXXXXXX     ,
  XXXXXXX , KC_MPRV , KC_MPLY , KC_MNXT , KC_VOLD , KC_VOLU ,                           KC_LEFT  , KC_DOWN , KC_UP   , KC_RIGHT , XXXXXXX , XXXXXXX     ,
  KC_LCTL , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                           KC_HOME  , KC_PGDN , KC_PGUP , KC_END   , XXXXXXX , KC_CAPS_LOCK,
                                          _______ , XXXXXXX , KC_SPC ,         KC_ENT , MO(FUNC) , KC_DEL
),

[SYM] = LAYOUT_split_3x6_3(
//    ┌──────┬────┬────┬────┬────┬──────────┐                   ┌─────┬────┬───┬───┬───┬──────┐
//    │ tab  │ !  │ @  │ #  │ $  │    %     │                   │  ^  │ &  │ * │ ( │ ) │ bspc │
//    ├──────┼────┼────┼────┼────┼──────────┤                   ├─────┼────┼───┼───┼───┼──────┤
//    │ lsft │ no │ no │ no │ no │    no    │                   │  -  │ =  │ [ │ ] │ \ │  `   │
//    ├──────┼────┼────┼────┼────┼──────────┤                   ├─────┼────┼───┼───┼───┼──────┤
//    │ lctl │ no │ no │ no │ no │    no    │                   │  _  │ +  │ { │ } │ | │  ~   │
//    └──────┴────┴────┴────┼────┼──────────┼─────┐       ┌─────┼─────┼────┼───┴───┴───┴──────┘
//                          │ no │ MO(FUNC) │ spc │       │ ent │     │ no │
//                          └────┴──────────┴─────┘       └─────┴─────┴────┘
  KC_TAB  , KC_EXLM , KC_AT   , KC_HASH , KC_DLR  , KC_PERC  ,                           KC_CIRC , KC_AMPR , KC_ASTR , KC_LPRN , KC_RPRN , KC_BSPC,
  KC_LSFT , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX  ,                           KC_MINS , KC_EQL  , KC_LBRC , KC_RBRC , KC_BSLS , KC_GRV ,
  KC_LCTL , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX  ,                           KC_UNDS , KC_PLUS , KC_LCBR , KC_RCBR , KC_PIPE , KC_TILD,
                                          XXXXXXX , MO(FUNC) , KC_SPC ,         KC_ENT , _______ , XXXXXXX
),

[FUNC] = LAYOUT_split_3x6_3(
//    ┌────┬────┬────┬────┬────┬─────┐                   ┌─────────────────┬────┬────┬────┬───────────┬───────────────┐
//    │ no │ f1 │ f2 │ f3 │ f4 │ f5  │                   │       f6        │ f7 │ f8 │ f9 │    f10    │      f11      │
//    ├────┼────┼────┼────┼────┼─────┤                   ├─────────────────┼────┼────┼────┼───────────┼───────────────┤
//    │ no │ no │ no │ no │ no │ no  │                   │ OSL(TTY_SWITCH) │ no │ no │ no │ TG(ALPHA) │      f12      │
//    ├────┼────┼────┼────┼────┼─────┤                   ├─────────────────┼────┼────┼────┼───────────┼───────────────┤
//    │ no │ no │ no │ no │ no │ no  │                   │       no        │ no │ no │ no │    no     │ QK_BOOTLOADER │
//    └────┴────┴────┴────┼────┼─────┼─────┐       ┌─────┼─────────────────┼────┼────┴────┴───────────┴───────────────┘
//                        │ no │     │ spc │       │ ent │                 │ no │
//                        └────┴─────┴─────┘       └─────┴─────────────────┴────┘
  XXXXXXX , KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5   ,                           KC_F6           , KC_F7   , KC_F8   , KC_F9   , KC_F10    , KC_F11       ,
  XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                           OSL(TTY_SWITCH) , XXXXXXX , XXXXXXX , XXXXXXX , TG(ALPHA) , KC_F12       ,
  XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                           XXXXXXX         , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX   , QK_BOOTLOADER,
                                          XXXXXXX , _______ , KC_SPC ,         KC_ENT , _______         , XXXXXXX
),

[MOUSE] = LAYOUT_split_3x6_3(
//    ┌─────┬──────┬──────┬──────┬──────┬─────────┐                           ┌─────────┬─────────┬───────┬─────────┬────┬────┐
//    │ no  │  no  │  no  │  no  │  no  │   no    │                           │   no    │   no    │  no   │   no    │ no │ no │
//    ├─────┼──────┼──────┼──────┼──────┼─────────┤                           ├─────────┼─────────┼───────┼─────────┼────┼────┤
//    │     │ lgui │ lalt │ lsft │ lctl │   no    │                           │ MS_LEFT │ MS_DOWN │ MS_UP │ MS_RGHT │ no │ no │
//    ├─────┼──────┼──────┼──────┼──────┼─────────┤                           ├─────────┼─────────┼───────┼─────────┼────┼────┤
//    │ no  │  no  │  no  │  no  │  no  │   no    │                           │   no    │   no    │  no   │   no    │ no │ no │
//    └─────┴──────┴──────┴──────┼──────┼─────────┼─────────┐       ┌─────────┼─────────┼─────────┼───────┴─────────┴────┴────┘
//                               │  no  │ MS_WHLU │ MS_BTN2 │       │ MS_BTN1 │ MS_WHLD │ QK_LLCK │
//                               └──────┴─────────┴─────────┘       └─────────┴─────────┴─────────┘
  XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                             XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX,
  _______ , KC_LGUI , KC_LALT , KC_LSFT , KC_LCTL , XXXXXXX ,                             MS_LEFT , MS_DOWN , MS_UP   , MS_RGHT , XXXXXXX , XXXXXXX,
  XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                             XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX,
                                          XXXXXXX , MS_WHLU , MS_BTN2 ,         MS_BTN1 , MS_WHLD , QK_LLCK
),

[TTY_SWITCH] = LAYOUT_split_3x6_3(
//    ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                 ┌─────────┬─────────┬─────────┬──────────┬──────────┬──────────┐
//    │ TTY(f1) │ TTY(f2) │ TTY(f3) │ TTY(f4) │ TTY(f5) │ TTY(f6) │                 │ TTY(f7) │ TTY(f8) │ TTY(f9) │ TTY(f10) │ TTY(f11) │ TTY(f12) │
//    ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                 ├─────────┼─────────┼─────────┼──────────┼──────────┼──────────┤
//    │   no    │   no    │   no    │   no    │   no    │   no    │                 │         │   no    │   no    │    no    │    no    │    no    │
//    ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                 ├─────────┼─────────┼─────────┼──────────┼──────────┼──────────┤
//    │         │   no    │   no    │   no    │   no    │   no    │                 │   no    │   no    │   no    │    no    │    no    │    no    │
//    └─────────┴─────────┴─────────┴─────────┼─────────┼─────────┼────┐       ┌────┼─────────┼─────────┼─────────┴──────────┴──────────┴──────────┘
//                                            │   no    │   no    │ no │       │ no │   no    │   no    │
//                                            └─────────┴─────────┴────┘       └────┴─────────┴─────────┘
  TTY(KC_F1) , TTY(KC_F2) , TTY(KC_F3) , TTY(KC_F4) , TTY(KC_F5) , TTY(KC_F6) ,                             TTY(KC_F7) , TTY(KC_F8) , TTY(KC_F9) , TTY(KC_F10) , TTY(KC_F11) , TTY(KC_F12),
  XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    ,                             _______    , XXXXXXX    , XXXXXXX    , XXXXXXX     , XXXXXXX     , XXXXXXX    ,
  _______    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    ,                             XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX     , XXXXXXX     , XXXXXXX    ,
                                                      XXXXXXX    , XXXXXXX    , XXXXXXX ,         XXXXXXX , XXXXXXX    , XXXXXXX
)
};
