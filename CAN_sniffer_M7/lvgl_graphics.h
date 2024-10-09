#include "lvgl.h"

#include <vector>
using namespace std;

#include "can_data.h"

enum DisplayScreen { MAIN_SCREEN, ACC_SCREEN, VEL_SCREEN, ROT_SCREEN };

DisplayScreen displayScreen = ACC_SCREEN;

static lv_obj_t* btn;
static lv_obj_t* labelBtn;
static lv_obj_t* container;
static lv_obj_t* chart;
static lv_obj_t* chartTitle;
static lv_obj_t* chartLegend;

void drawContainter();
void drawButton();
void drawButtonLabel();
void drawChartTitle(const char* title);
void drawChartLegend(int value);
template <typename T>
void drawChart(const vector<T>& data, lv_color_t color, int min, int max);


void paintAcceleration() {
  lv_obj_clean(container);

  drawChartTitle("Acceleration");

  drawChart(MotorAcc, lv_palette_main(LV_PALETTE_RED), 0, 255);

  drawChartLegend(MotorAcc[MotorAcc.size()-1]);

  lv_chart_refresh(chart); //Required after direct set
}

void paintVelocity() {
  lv_obj_clean(container);

  drawChartTitle("Velocity");

  drawChart(PositionVx, lv_palette_main(LV_PALETTE_GREEN), 0, 255);

  drawChartLegend(PositionVx[PositionVx.size()-1]);

  lv_chart_refresh(chart); //Required after direct set
}


static void event_handler(lv_event_t * event)
{
    lv_event_code_t event_code = lv_event_get_code(event);

    if(event_code == LV_EVENT_CLICKED) {
      if (displayScreen == ACC_SCREEN) {
        displayScreen = VEL_SCREEN;
      }
      else {
        displayScreen = ACC_SCREEN;
      }
    }
}

void drawContainter() {
  container = lv_obj_create(lv_screen_active());
  lv_obj_set_size(container, 700, lv_pct(100));
  lv_obj_align(container, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_clear_flag(container, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(btn, lv_color_hex(0xE9E9E9), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void drawButton() {
  btn = lv_btn_create(lv_screen_active());
  lv_obj_set_size(btn, 100, lv_pct(100));
  lv_obj_add_event_cb(btn, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn, LV_ALIGN_RIGHT_MID, 0, 0);
  lv_obj_set_style_bg_color(btn, lv_color_hex(0xE9E9E9), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void drawButtonLabel() {
  labelBtn = lv_label_create(btn);
  lv_label_set_text(labelBtn, ">");
  lv_obj_center(labelBtn);
  lv_obj_set_style_text_color(labelBtn, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(labelBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void drawChartTitle(const char* title) {
  chartTitle = lv_label_create(container);
  lv_label_set_text(chartTitle, title);
  lv_obj_set_style_text_align(chartTitle, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(chartTitle, LV_ALIGN_TOP_MID, 0, 0);
}

void drawChartLegend(int value) {
  chartLegend = lv_label_create(container);
  lv_label_set_text_fmt(chartLegend, "Value: %d", value);
  lv_obj_set_style_text_align(chartLegend, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(chartLegend, LV_ALIGN_BOTTOM_MID, 0, 0);
}

template <typename T>
void drawChart(const vector<T>& data, lv_color_t color, int min, int max) {
  chart = lv_chart_create(container);
  lv_obj_set_size(chart, 580, 400);
  lv_obj_center(chart);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE); // Show lines and points
  lv_obj_set_style_size(chart, 0, 0, LV_PART_INDICATOR); // Do not show points on the data
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_chart_set_point_count(chart, data.size()); // Number of points
  lv_chart_set_div_line_count(chart, 10, 10); // Division lines
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max); // Vertical range primary axis
  lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, min, max); // Vertical range secondary axis

  lv_chart_series_t * ser = lv_chart_add_series(chart, color, LV_CHART_AXIS_PRIMARY_Y);
  //lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);

  for(uint32_t i = 0; i < data.size(); i++) {
      //Set the next points on the serie
      ser->y_points[i] = data[i];
  }
}
