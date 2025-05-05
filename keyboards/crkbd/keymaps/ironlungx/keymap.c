#include "action_layer.h"
#include "oled_driver.h"
#include "string.h"
#include "modifiers.h"
#include QMK_KEYBOARD_H

#define HMR_A LGUI_T(KC_A)
#define HMR_S LALT_T(KC_S)
#define HMR_D LSFT_T(KC_D)
#define HMR_F LCTL_T(KC_F)

#define HMR_J RCTL_T(KC_J)
#define HMR_K RSFT_T(KC_K)
#define HMR_L RALT_T(KC_L)
#define HMR_SCLN RGUI_T(KC_SCLN)

#define TTY_SWITCH(x) LALT(LCTL(x))
#define TTY TTY_SWITCH

enum layers { ALPHA_HMR, ALPHA, GAME, NUM, SYM, FUNC, EMOJI, MOUSE };

static const unsigned char PROGMEM image_GameMode_bits[] = {0x04,0x00,0x7f,0xc0,0xdf,0xe0,0x8e,0xa0,0xdf,0xe0,0xf1,0xe0,0xe0,0xe0,0xc0,0x60};

#ifdef OLED_ENABLE
void oled_draw_xbm(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap) {
  uint8_t byteWidth = (width + 7) / 8; // Width in bytes

  for (uint8_t j = 0; j < height; j++) {
    for (uint8_t i = 0; i < width; i++) {
      // Calculate byte position and bit position within that byte
      uint8_t byte_index = j * byteWidth + (i / 8);
      uint8_t bit_position = i % 8;

      // Check if the bit is set in the bitmap
      bool pixel = bitmap[byte_index] & (1 << bit_position);

      if (pixel) {
        oled_write_pixel(x + i, y + j, true);
      }
    }
  }
}

bool oled_task_user(void) {
    // Clear the display for a clean slate
    oled_clear();

    // Get current layer and display with appropriate icon/text
    oled_set_cursor(0, 0);
    switch (get_highest_layer(layer_state)) {
        case ALPHA_HMR:
            oled_write_P(PSTR("base#"), false);
            break;
        case ALPHA:
            oled_write_P(PSTR("base"), false);
            break;
        case GAME:
            oled_write_P(PSTR("game"), false);
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
        case EMOJI:
            oled_write_P(PSTR("emoji"), false);
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

    oled_set_cursor(oled_max_chars() - text_length, 0);
    oled_write(wpm, false);

    uint8_t mod_state = get_mods();

    char mods[5];
    int idx = 0;

    if (mod_state & MOD_MASK_GUI)   mods[idx++] = 'G';
    if (mod_state & MOD_MASK_ALT)   mods[idx++] = 'A';
    if (mod_state & MOD_MASK_CTRL)  mods[idx++] = 'C';
    if (mod_state & MOD_MASK_SHIFT) mods[idx++] = 'S';

    mods[idx++] = '\0';

    oled_set_cursor(0, 1);
    oled_write(mods, false);

    return false;
}
void oled_render_boot(bool bootloader) {
  oled_clear();

  if (bootloader) {
    oled_write_ln_P("Awaiting New Firmware", false);
  }
  oled_render_dirty(true);
}

// Show a message when in download mode
bool shutdown_user(bool jump_to_bootloader) {
  oled_render_boot(jump_to_bootloader);
  return true;
}

#endif // OLED_ENABLE

/* bool process_record_user(uint16_t keycode, keyrecord_t *record) {
   switch(keycode) {
   }
   return true;
   } */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[ALPHA_HMR] = LAYOUT_split_3x6_3(
//    ┌───────────┬───────┬───────┬───────┬─────────┬─────┐                   ┌──────┬─────────┬───────┬───────┬──────────┬────┐
//    │    tab    │   q   │   w   │   e   │    r    │  t  │                   │  y   │    u    │   i   │   o   │    p     │ no │
//    ├───────────┼───────┼───────┼───────┼─────────┼─────┤                   ├──────┼─────────┼───────┼───────┼──────────┼────┤
//    │ MO(MOUSE) │ win/a │ alt/s │ sft/d │  ctl/f  │  g  │                   │  h   │  ctl/j  │ sft/k │ alt/l │ alt/sCLN │ '  │
//    ├───────────┼───────┼───────┼───────┼─────────┼─────┤                   ├──────┼─────────┼───────┼───────┼──────────┼────┤
//    │ MO(EMOJI) │   z   │   x   │   c   │    v    │  b  │                   │  n   │    m    │   ,   │   .   │    /     │ no │
//    └───────────┴───────┴───────┴───────┼─────────┼─────┼─────┐       ┌─────┼──────┼─────────┼───────┴───────┴──────────┴────┘
//                                        │ MO(NUM) │ esc │ spc │       │ ent │ bspc │ MO(SYM) │
//                                        └─────────┴─────┴─────┘       └─────┴──────┴─────────┘
  KC_TAB    , KC_Q  , KC_W  , KC_E  , KC_R    , KC_T   ,                           KC_Y    , KC_U    , KC_I    , KC_O   , KC_P     , XXXXXXX,
  MO(MOUSE) , HMR_A , HMR_S , HMR_D , HMR_F   , KC_G   ,                           KC_H    , HMR_J   , HMR_K   , HMR_L  , HMR_SCLN , KC_QUOT,
  MO(EMOJI) , KC_Z  , KC_X  , KC_C  , KC_V    , KC_B   ,                           KC_N    , KC_M    , KC_COMM , KC_DOT , KC_SLSH  , XXXXXXX,
                                      MO(NUM) , KC_ESC , KC_SPC ,         KC_ENT , KC_BSPC , MO(SYM)
),

[ALPHA] = LAYOUT_split_3x6_3(
//    ┌──────┬───┬───┬───┬─────────┬──────┐                   ┌──────┬─────────┬───┬───┬───┬──────┐
//    │ tab  │ q │ w │ e │    r    │  t   │                   │  y   │    u    │ i │ o │ p │ bspc │
//    ├──────┼───┼───┼───┼─────────┼──────┤                   ├──────┼─────────┼───┼───┼───┼──────┤
//    │ lsft │ a │ s │ d │    f    │  g   │                   │  h   │    j    │ k │ l │ ; │  '   │
//    ├──────┼───┼───┼───┼─────────┼──────┤                   ├──────┼─────────┼───┼───┼───┼──────┤
//    │ lctl │ z │ x │ c │    v    │  b   │                   │  n   │    m    │ , │ . │ / │ esc  │
//    └──────┴───┴───┴───┼─────────┼──────┼─────┐       ┌─────┼──────┼─────────┼───┴───┴───┴──────┘
//                       │ MO(NUM) │ lgui │ spc │       │ ent │ lalt │ MO(SYM) │
//                       └─────────┴──────┴─────┘       └─────┴──────┴─────────┘
  KC_TAB  , KC_Q , KC_W , KC_E , KC_R    , KC_T    ,                           KC_Y    , KC_U    , KC_I    , KC_O   , KC_P    , KC_BSPC,
  KC_LSFT , KC_A , KC_S , KC_D , KC_F    , KC_G    ,                           KC_H    , KC_J    , KC_K    , KC_L   , KC_SCLN , KC_QUOT,
  KC_LCTL , KC_Z , KC_X , KC_C , KC_V    , KC_B    ,                           KC_N    , KC_M    , KC_COMM , KC_DOT , KC_SLSH , KC_ESC ,
                                 MO(NUM) , KC_LGUI , KC_SPC ,         KC_ENT , KC_LALT , MO(SYM)
),

[NUM] = LAYOUT_split_3x6_3(
//    ┌──────┬──────┬──────┬──────┬──────┬──────┐                   ┌──────┬──────────┬──────┬──────┬────┬──────┐
//    │ tab  │  1   │  2   │  3   │  4   │  5   │                   │  6   │    7     │  8   │  9   │ 0  │ bspc │
//    ├──────┼──────┼──────┼──────┼──────┼──────┤                   ├──────┼──────────┼──────┼──────┼────┼──────┤
//    │  no  │ mprv │ mply │ mnxt │ vold │ volu │                   │ left │   down   │  up  │ rght │ no │ del  │
//    ├──────┼──────┼──────┼──────┼──────┼──────┤                   ├──────┼──────────┼──────┼──────┼────┼──────┤
//    │ lctl │  no  │  no  │  no  │  no  │  no  │                   │ home │   pgdn   │ pgup │ end  │ no │  no  │
//    └──────┴──────┴──────┴──────┼──────┼──────┼─────┐       ┌─────┼──────┼──────────┼──────┴──────┴────┴──────┘
//                                │  no  │      │ spc │       │ ent │  no  │ MO(FUNC) │
//                                └──────┴──────┴─────┘       └─────┴──────┴──────────┘
  KC_TAB  , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    ,                           KC_6    , KC_7     , KC_8    , KC_9     , KC_0    , KC_BSPC,
  XXXXXXX , KC_MPRV , KC_MPLY , KC_MNXT , KC_VOLD , KC_VOLU ,                           KC_LEFT , KC_DOWN  , KC_UP   , KC_RIGHT , XXXXXXX , KC_DEL ,
  KC_LCTL , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                           KC_HOME , KC_PGDN  , KC_PGUP , KC_END   , XXXXXXX , XXXXXXX,
                                          XXXXXXX , _______ , KC_SPC ,         KC_ENT , XXXXXXX , MO(FUNC)
),

[SYM] = LAYOUT_split_3x6_3(
//    ┌──────┬────┬────┬────┬──────────┬────┐                   ┌─────┬────┬───┬───┬───┬──────┐
//    │ tab  │ !  │ @  │ #  │    $     │ %  │                   │  ^  │ &  │ * │ ( │ ) │ bspc │
//    ├──────┼────┼────┼────┼──────────┼────┤                   ├─────┼────┼───┼───┼───┼──────┤
//    │ lsft │ no │ no │ no │    no    │ no │                   │  -  │ =  │ [ │ ] │ \ │  `   │
//    ├──────┼────┼────┼────┼──────────┼────┤                   ├─────┼────┼───┼───┼───┼──────┤
//    │ lctl │ no │ no │ no │    no    │ no │                   │  _  │ +  │ { │ } │ | │  ~   │
//    └──────┴────┴────┴────┼──────────┼────┼─────┐       ┌─────┼─────┼────┼───┴───┴───┴──────┘
//                          │ MO(FUNC) │ no │ spc │       │ ent │     │ no │
//                          └──────────┴────┴─────┘       └─────┴─────┴────┘
  KC_TAB  , KC_EXLM , KC_AT   , KC_HASH , KC_DLR   , KC_PERC ,                           KC_CIRC , KC_AMPR , KC_ASTR , KC_LPRN , KC_RPRN , KC_BSPC,
  KC_LSFT , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX  , XXXXXXX ,                           KC_MINS , KC_EQL  , KC_LBRC , KC_RBRC , KC_BSLS , KC_GRV ,
  KC_LCTL , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX  , XXXXXXX ,                           KC_UNDS , KC_PLUS , KC_LCBR , KC_RCBR , KC_PIPE , KC_TILD,
                                          MO(FUNC) , XXXXXXX , KC_SPC ,         KC_ENT , _______ , XXXXXXX
),

[FUNC] = LAYOUT_split_3x6_3(
//    ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐                   ┌─────────┬─────────┬─────────┬──────────┬───────────┬──────────┐
//    │   f1    │   f2    │   f3    │   f4    │   f5    │   f6    │                   │   f7    │   f8    │   f9    │   f10    │    f11    │   f12    │
//    ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                   ├─────────┼─────────┼─────────┼──────────┼───────────┼──────────┤
//    │ RGB_TOG │ RGB_HUI │ RGB_SAI │ RGB_VAI │   no    │   no    │                   │   no    │   no    │   no    │ TG(GAME) │ TG(ALPHA) │ QK_BOOT  │
//    ├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┤                   ├─────────┼─────────┼─────────┼──────────┼───────────┼──────────┤
//    │ TTY(f1) │ TTY(f2) │ TTY(f3) │ TTY(f4) │ TTY(f5) │ TTY(f6) │                   │ TTY(f7) │ TTY(f8) │ TTY(f9) │ TTY(f10) │ TTY(f11)  │ TTY(f12) │
//    └─────────┴─────────┴─────────┴─────────┼─────────┼─────────┼─────┐       ┌─────┼─────────┼─────────┼─────────┴──────────┴───────────┴──────────┘
//                                            │         │   no    │ spc │       │ ent │   no    │         │
//                                            └─────────┴─────────┴─────┘       └─────┴─────────┴─────────┘
  KC_F1      , KC_F2      , KC_F3      , KC_F4      , KC_F5      , KC_F6      ,                           KC_F7      , KC_F8      , KC_F9      , KC_F10      , KC_F11      , KC_F12     ,
  RGB_TOG    , RGB_HUI    , RGB_SAI    , RGB_VAI    , XXXXXXX    , XXXXXXX    ,                           XXXXXXX    , XXXXXXX    , XXXXXXX    , TG(GAME)    , TG(ALPHA)   , QK_BOOT    ,
  TTY(KC_F1) , TTY(KC_F2) , TTY(KC_F3) , TTY(KC_F4) , TTY(KC_F5) , TTY(KC_F6) ,                           TTY(KC_F7) , TTY(KC_F8) , TTY(KC_F9) , TTY(KC_F10) , TTY(KC_F11) , TTY(KC_F12),
                                                      _______    , XXXXXXX    , KC_SPC ,         KC_ENT , XXXXXXX    , _______
),

[MOUSE] = LAYOUT_split_3x6_3(
//    ┌─────┬──────┬──────┬──────┬──────┬──────┐                     ┌──────┬─────────┬──────┬──────┬────┬────┐
//    │ no  │  no  │  no  │  no  │  no  │  no  │                     │  no  │   no    │  no  │  no  │ no │ no │
//    ├─────┼──────┼──────┼──────┼──────┼──────┤                     ├──────┼─────────┼──────┼──────┼────┼────┤
//    │     │ lgui │ lalt │ lsft │ lctl │  no  │                     │ ms_l │  ms_d   │ ms_u │ ms_r │ no │ no │
//    ├─────┼──────┼──────┼──────┼──────┼──────┤                     ├──────┼─────────┼──────┼──────┼────┼────┤
//    │ no  │  no  │  no  │  no  │  no  │  no  │                     │  no  │   no    │  no  │  no  │ no │ no │
//    └─────┴──────┴──────┴──────┼──────┼──────┼──────┐       ┌──────┼──────┼─────────┼──────┴──────┴────┴────┘
//                               │  no  │ wh_u │ btn2 │       │ btn1 │ wh_d │ QK_LLCK │
//                               └──────┴──────┴──────┘       └──────┴──────┴─────────┘
  XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX     ,                                   XXXXXXX       , XXXXXXX    , XXXXXXX  , XXXXXXX     , XXXXXXX , XXXXXXX,
  _______ , KC_LGUI , KC_LALT , KC_LSFT , KC_LCTL , XXXXXXX     ,                                   KC_MS_LEFT    , KC_MS_DOWN , KC_MS_UP , KC_MS_RIGHT , XXXXXXX , XXXXXXX,
  XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX     ,                                   XXXXXXX       , XXXXXXX    , XXXXXXX  , XXXXXXX     , XXXXXXX , XXXXXXX,
                                          XXXXXXX , KC_MS_WH_UP , KC_MS_BTN2 ,         KC_MS_BTN1 , KC_MS_WH_DOWN , QK_LLCK
),

[EMOJI] = LAYOUT_split_3x6_3(
//    ┌─────┬────┬────┬────┬────┬────┐                 ┌────┬────┬────┬────┬────┬────┐
//    │ no  │ no │ no │ no │ no │ no │                 │ no │ no │ no │ no │ no │ no │
//    ├─────┼────┼────┼────┼────┼────┤                 ├────┼────┼────┼────┼────┼────┤
//    │ no  │ no │ no │ no │ no │ no │                 │ no │ no │ no │ no │ no │ no │
//    ├─────┼────┼────┼────┼────┼────┤                 ├────┼────┼────┼────┼────┼────┤
//    │     │ no │ no │ no │ no │ no │                 │ no │ no │ no │ no │ no │ no │
//    └─────┴────┴────┴────┼────┼────┼────┐       ┌────┼────┼────┼────┴────┴────┴────┘
//                         │ no │ no │ no │       │ no │ no │ no │
//                         └────┴────┴────┘       └────┴────┴────┘
  XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                             XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX,
  XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                             XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX,
  _______ , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,                             XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX,
                                          XXXXXXX , XXXXXXX , XXXXXXX ,         XXXXXXX , XXXXXXX , XXXXXXX
)
};
