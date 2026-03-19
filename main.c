#include "lvgl.h"
#include <math.h>

#define NUM_WISSELS 5
#define NUM_TT_POS 12

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

    if(wissel_state[id])
        lv_obj_set_style_bg_color(wissel_btns[id], lv_color_hex(0xFF0000), 0);
    else
        lv_obj_set_style_bg_color(wissel_btns[id], lv_color_hex(0x00FF00), 0);
}

static void tt_event_cb(lv_event_t *e) {
    int pos = (int)lv_event_get_user_data(e);
    tt_current = pos;

    for(int i=0;i<NUM_TT_POS;i++) {
        if(i==tt_current)
            lv_obj_set_style_bg_color(tt_btns[i], lv_color_hex(0x00FF00),0);
        else
            lv_obj_set_style_bg_color(tt_btns[i], lv_color_hex(0x555555),0);
    }
}

static void tt_center_event_cb(lv_event_t *e) {
    tt_current = (tt_current + 6) % NUM_TT_POS;
    tt_event_cb((lv_event_t*)tt_btns[tt_current]); // update UI
}

/* ========================= */
/* UI CREATE FUNCTION */
/* ========================= */

void ui_create(void) {
    /* ROOT CONTAINER */
    lv_obj_t* root = lv_obj_create(lv_scr_act());
    lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);

    /* ===== TOP PANEL: SPORENPLAN + DRAAISCHIJF ===== */
    lv_obj_t* top = lv_obj_create(root);
    lv_obj_set_size(top, LV_PCT(100), LV_PCT(70));
    lv_obj_set_flex_flow(top, LV_FLEX_FLOW_ROW);

    /* -- SPORENPLAN -- */
    lv_obj_t* plan = lv_obj_create(top);
    lv_obj_set_size(plan, LV_PCT(70), LV_PCT(100));
    lv_obj_t* label = lv_label_create(plan);
    lv_label_set_text(label,"SPORENPLAN (placeholder)");
    lv_obj_center(label);

    /* -- DRAAISCHIJF -- */
    lv_obj_t* tt = lv_obj_create(top);
    lv_obj_set_size(tt, LV_PCT(30), LV_PCT(100));

    int cx = 150;
    int cy = 150;
    int radius = 100;

    for(int i=0;i<NUM_TT_POS;i++) {
        float angle = i * 30.0f * (M_PI / 180.0f);
        int x = cx + (int)(cos(angle) * radius);
        int y = cy + (int)(sin(angle) * radius);

        tt_btns[i] = lv_btn_create(tt);
        lv_obj_set_size(tt_btns[i], 42,42);
        lv_obj_set_pos(tt_btns[i], x, y);
        lv_obj_add_event_cb(tt_btns[i], tt_event_cb, LV_EVENT_CLICKED,(void*)i);

        lv_obj_t* lbl = lv_label_create(tt_btns[i]);
        lv_label_set_text_fmt(lbl,"%d",i+1);
        lv_obj_center(lbl);
    }

    /* Middenknop 180° */
    tt_center = lv_btn_create(tt);
    lv_obj_set_size(tt_center,60,60);
    lv_obj_set_pos(tt_center,cx-30,cy-30);
    lv_obj_add_event_cb(tt_center, tt_center_event_cb, LV_EVENT_CLICKED,NULL);
    lv_obj_t* center_lbl = lv_label_create(tt_center);
    lv_label_set_text(center_lbl,"180");
    lv_obj_center(center_lbl);

    /* ===== BOTTOM PANEL: WISSELS ===== */
    lv_obj_t* bottom = lv_obj_create(root);
    lv_obj_set_size(bottom, LV_PCT(100), LV_PCT(30));
    lv_obj_set_flex_flow(bottom, LV_FLEX_FLOW_ROW_WRAP);

    for(int i=0;i<NUM_WISSELS;i++) {
        wissel_btns[i] = lv_btn_create(bottom);
        lv_obj_set_size(wissel_btns[i],110,60);
        lv_obj_add_event_cb(wissel_btns[i], wissel_event_cb, LV_EVENT_CLICKED,(void*)i);

        lv_obj_t* lbl = lv_label_create(wissel_btns[i]);
        lv_label_set_text_fmt(lbl,"W%d",i+1);
        lv_obj_center(lbl);
        lv_obj_set_style_bg_color(wissel_btns[i], lv_color_hex(0x00FF00),0);
    }
}

/* ========================= */
/* MAIN (viewer compatible) */
/* ========================= */

int main(void)
{
    lv_init();

    /* viewer Lvgl.io init */
    lv_port_disp_init();  // viewer doet dit automatisch
    lv_port_indev_init();

    ui_create();

    while(1) {
        lv_timer_handler(); // viewer loopt automatisch
    }

    return 0;
}
