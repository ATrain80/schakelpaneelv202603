#include "lvgl.h"

#define NUM_WISSELS 5
#define NUM_TT_POS 8

static lv_obj_t *wissel_btns[NUM_WISSELS];
static lv_obj_t *tt_btns[NUM_TT_POS];
static lv_obj_t *tt_center;

static bool wissel_state[NUM_WISSELS] = {0};
static int tt_current = 0;

/* ========================= */
/* EVENT CALLBACKS */
/* ========================= */

static void wissel_event_cb(lv_event_t *e) {
    int id = (int)lv_event_get_user_data(e);
    wissel_state[id] = !wissel_state[id];
    lv_obj_set_style_bg_color(wissel_btns[id],
        wissel_state[id] ? lv_color_hex(0xFF0000) : lv_color_hex(0x00FF00),0);
}

static void tt_event_cb(lv_event_t *e) {
    int id = (int)lv_event_get_user_data(e);
    tt_current = id;
    for(int i=0;i<NUM_TT_POS;i++) {
        lv_obj_set_style_bg_color(tt_btns[i],
            (i==tt_current)?lv_color_hex(0x00FF00):lv_color_hex(0x555555),0);
    }
}

static void tt_center_event_cb(lv_event_t *e) {
    tt_current = (tt_current + 4) % NUM_TT_POS;
    tt_event_cb((lv_event_t*)tt_btns[tt_current]);
}

/* ========================= */
/* UI CREATE FUNCTION */
/* ========================= */

void ui_create(void) {
    /* Bovenste paneel: sporenplan + draaischijf */
    lv_obj_t* sporenplan = lv_obj_create(lv_scr_act());
    lv_obj_set_size(sporenplan, 716, 420);
    lv_obj_t* lbl = lv_label_create(sporenplan);
    lv_label_set_text(lbl, "SPORENPLAN (placeholder)");
    lv_obj_center(lbl);

    lv_obj_t* draaischijf = lv_obj_create(lv_scr_act());
    lv_obj_set_size(draaischijf, 308, 420);
    /* Draaischijf knoppen */
    for(int i=0;i<NUM_TT_POS;i++) {
        tt_btns[i] = lv_btn_create(draaischijf);
        lv_obj_set_size(tt_btns[i],42,42);
        lv_obj_set_pos(tt_btns[i],50+i*20,50+i*10);
        lv_obj_add_event_cb(tt_btns[i], tt_event_cb, LV_EVENT_CLICKED,(void*)i);
    }
    tt_center = lv_btn_create(draaischijf);
    lv_obj_set_size(tt_center,60,60);
    lv_obj_set_pos(tt_center,150,150);
    lv_obj_add_event_cb(tt_center, tt_center_event_cb, LV_EVENT_CLICKED,NULL);

    /* Wisselknoppen onder */
    for(int i=0;i<NUM_WISSELS;i++) {
        wissel_btns[i] = lv_btn_create(lv_scr_act());
        lv_obj_set_size(wissel_btns[i],110,60);
        lv_obj_set_pos(wissel_btns[i],10+i*120,430);
        lv_obj_add_event_cb(wissel_btns[i], wissel_event_cb, LV_EVENT_CLICKED,(void*)i);
    }
}

/* ========================= */
/* MAIN */
int main(void)
{
    lv_init();
    ui_create();
    while(1) {
        lv_timer_handler();
    }
    return 0;
}
