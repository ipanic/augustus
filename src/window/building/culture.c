#include "culture.h"

#include "building/building.h"
#include "building/monument.h"
#include "city/constants.h"
#include "city/resource.h"
#include "core/mods.h"
#include "graphics/generic_button.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"
#include "sound/speech.h"
#include "translation/translation.h"
#include "window/option_popup.h"

#define GOD_PANTHEON 5

static void add_module_prompt(void);

static int god_id;

static generic_button add_module_button[] = {
    {0, 0, 304, 20, add_module_prompt, button_none, 0, 0}
};

static struct {
    int focus_button_id;
    int building_id;
} data = { 0, 0 };

option_menu_item temple_module_options[12] = {
   {TR_BUILDING_GRAND_TEMPLE_CERES_DESC_MODULE_1, TR_BUILDING_GRAND_TEMPLE_CERES_MODULE_1_DESC, 0, "Areldir", "Grand_Temple_Banners", "Ceres M Icon"},
   {TR_BUILDING_GRAND_TEMPLE_CERES_DESC_MODULE_2, TR_BUILDING_GRAND_TEMPLE_CERES_MODULE_2_DESC, 0, "Areldir", "Grand_Temple_Banners", "Ceres M2 Icon"},
   {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC_MODULE_1, TR_BUILDING_GRAND_TEMPLE_NEPTUNE_MODULE_1_DESC, 0, "Areldir", "Grand_Temple_Banners", "Nept M Icon"},
   {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC_MODULE_2, TR_BUILDING_GRAND_TEMPLE_NEPTUNE_MODULE_2_DESC, 0, "Areldir", "Grand_Temple_Banners", "Nept M2 Icon"},
   {TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC_MODULE_1, TR_BUILDING_GRAND_TEMPLE_MERCURY_MODULE_1_DESC, 0, "Areldir", "Grand_Temple_Banners", "Merc M Icon"},
   {TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC_MODULE_2, TR_BUILDING_GRAND_TEMPLE_MERCURY_MODULE_2_DESC, 0, "Areldir", "Grand_Temple_Banners", "Merc M2 Icon"},
   {TR_BUILDING_GRAND_TEMPLE_MARS_DESC_MODULE_1, TR_BUILDING_GRAND_TEMPLE_MARS_MODULE_1_DESC, 0, "Areldir", "Grand_Temple_Banners", "Mars M Icon"},
   {TR_BUILDING_GRAND_TEMPLE_MARS_DESC_MODULE_2, TR_BUILDING_GRAND_TEMPLE_MARS_MODULE_2_DESC, 0, "Areldir", "Grand_Temple_Banners", "Mars M2 Icon"},
   {TR_BUILDING_GRAND_TEMPLE_VENUS_DESC_MODULE_1, TR_BUILDING_GRAND_TEMPLE_VENUS_MODULE_1_DESC, 0, "Areldir", "Grand_Temple_Banners", "Venus M Icon"},
   {TR_BUILDING_GRAND_TEMPLE_VENUS_DESC_MODULE_2, TR_BUILDING_GRAND_TEMPLE_VENUS_MODULE_2_DESC, 0, "Areldir", "Grand_Temple_Banners", "Venus M2 Icon"},
   {TR_BUILDING_PANTHEON_DESC_MODULE_1, TR_BUILDING_PANTHEON_MODULE_1_DESC, 0, "Areldir", "Grand_Temple_Banners", "Panth M Icon"},
   {TR_BUILDING_PANTHEON_DESC_MODULE_2, TR_BUILDING_PANTHEON_MODULE_2_DESC, 0, "Areldir", "Grand_Temple_Banners", "Panth M2 Icon"}
};

static void draw_culture_info(building_info_context *c, int help_id, const char *sound_file, int group_id)
{
    c->help_id = help_id;
    window_building_play_sound(c, sound_file);
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(group_id, 0, c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK);

    if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (building_get(c->building_id)->num_workers <= 0) {
        window_building_draw_description(c, group_id, 2);
    } else {
        window_building_draw_description(c, group_id, 3);
    }
    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 4);
    window_building_draw_employment(c, 142);
}

void window_building_draw_clinic(building_info_context *c)
{
    draw_culture_info(c, 65, "wavs/clinic.wav", 81);
}

void window_building_draw_hospital(building_info_context *c)
{
    draw_culture_info(c, 66, "wavs/hospital.wav", 82);
}

void window_building_draw_bathhouse(building_info_context *c)
{
    c->help_id = 64;
    window_building_play_sound(c, "wavs/baths.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(83, 0, c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK);

    building *b = building_get(c->building_id);
    if (!b->has_water_access) {
        window_building_draw_description(c, 83, 4);
    } else if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (b->num_workers <= 0) {
        window_building_draw_description(c, 83, 2);
    } else {
        window_building_draw_description(c, 83, 3);
    }
    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 4);
    window_building_draw_employment(c, 142);
}

void window_building_draw_barber(building_info_context *c)
{
    draw_culture_info(c, 63, "wavs/barber.wav", 84);
}

void window_building_draw_school(building_info_context *c)
{
    draw_culture_info(c, 68, "wavs/school.wav", 85);
}

void window_building_draw_academy(building_info_context *c)
{
    draw_culture_info(c, 69, "wavs/academy.wav", 86);
}

void window_building_draw_library(building_info_context *c)
{
    draw_culture_info(c, 70, "wavs/library.wav", 87);
}

static void draw_temple(building_info_context *c, const char *sound_file, int group_id, int image_offset)
{
    c->help_id = 67;
    window_building_play_sound(c, sound_file);
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(group_id, 0, c->x_offset, c->y_offset + 12, 16 * c->width_blocks, FONT_LARGE_BLACK);
    inner_panel_draw(c->x_offset + 16, c->y_offset + 56, c->width_blocks - 2, 4);
    window_building_draw_employment(c, 62);
    if (c->has_road_access) {
        image_draw(image_offset + image_group(GROUP_PANEL_WINDOWS),
            c->x_offset + 190, c->y_offset + 16 * c->height_blocks - 118);
    } else {
        window_building_draw_description_at(c, 16 * c->height_blocks - 128, 69, 25);
    }
}

void window_building_draw_temple_ceres(building_info_context *c)
{
    draw_temple(c, "wavs/temple_farm.wav", 92, 21);
}

void window_building_draw_temple_neptune(building_info_context *c)
{
    draw_temple(c, "wavs/temple_ship.wav", 93, 22);
}

void window_building_draw_temple_mercury(building_info_context *c)
{
    draw_temple(c, "wavs/temple_comm.wav", 94, 23);
}

void window_building_draw_temple_mars(building_info_context *c)
{
    draw_temple(c, "wavs/temple_war.wav", 95, 24);
}

void window_building_draw_temple_venus(building_info_context *c)
{
    draw_temple(c, "wavs/temple_love.wav", 96, 25);
}

void window_building_draw_oracle(building_info_context *c)
{
    c->help_id = 67;
    window_building_play_sound(c, "wavs/oracle.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(110, 0, c->x_offset, c->y_offset + 12, 16 * c->width_blocks, FONT_LARGE_BLACK);
    window_building_draw_description_at(c, 16 * c->height_blocks - 158, 110, 1);
}

void window_building_draw_theater(building_info_context *c)
{
    c->help_id = 71;
    window_building_play_sound(c, "wavs/theatre.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(72, 0, c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK);
    building *b = building_get(c->building_id);
    if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (b->num_workers <= 0) {
        window_building_draw_description(c, 72, 4);
    } else if (!b->data.entertainment.num_shows) {
        window_building_draw_description(c, 72, 2);
    } else if (b->data.entertainment.days1) {
        window_building_draw_description(c, 72, 3);
    }

    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 6);
    window_building_draw_employment(c, 138);
    if (b->data.entertainment.days1 > 0) {
        int width = lang_text_draw(72, 6, c->x_offset + 32, c->y_offset + 182, FONT_SMALL_BLACK);
        lang_text_draw_amount(8, 44, 2 * b->data.entertainment.days1,
            c->x_offset + width + 32, c->y_offset + 182, FONT_SMALL_BLACK);
        lang_text_draw(72, 7 + b->data.entertainment.play,
            c->x_offset + 32, c->y_offset + 202, FONT_SMALL_BLACK);
    } else {
        lang_text_draw(72, 5, c->x_offset + 32, c->y_offset + 182, FONT_SMALL_BLACK);
    }
}

void window_building_draw_amphitheater(building_info_context *c)
{
    c->help_id = 72;
    // The file name has a typo
    window_building_play_sound(c, "wavs/ampitheatre.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(71, 0, c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK);
    building *b = building_get(c->building_id);
    if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (b->num_workers <= 0) {
        window_building_draw_description(c, 71, 6);
    } else if (!b->data.entertainment.num_shows) {
        window_building_draw_description(c, 71, 2);
    } else if (b->data.entertainment.num_shows == 2) {
        window_building_draw_description(c, 71, 3);
    } else if (b->data.entertainment.days1) {
        window_building_draw_description(c, 71, 4);
    } else if (b->data.entertainment.days2) {
        window_building_draw_description(c, 71, 5);
    }

    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 7);
    window_building_draw_employment(c, 138);
    if (b->data.entertainment.days1 > 0) {
        int width = lang_text_draw(71, 8, c->x_offset + 32, c->y_offset + 182, FONT_SMALL_BLACK);
        lang_text_draw_amount(8, 44, 2 * b->data.entertainment.days1,
            c->x_offset + width + 32, c->y_offset + 182, FONT_SMALL_BLACK);
    } else {
        lang_text_draw(71, 7, c->x_offset + 32, c->y_offset + 182, FONT_SMALL_BLACK);
    }
    if (b->data.entertainment.days2 > 0) {
        int width = lang_text_draw(71, 10, c->x_offset + 32, c->y_offset + 202, FONT_SMALL_BLACK);
        lang_text_draw_amount(8, 44, 2 * b->data.entertainment.days2,
            c->x_offset + width + 32, c->y_offset + 202, FONT_SMALL_BLACK);
        lang_text_draw(72, 7 + b->data.entertainment.play,
            c->x_offset + 32, c->y_offset + 222, FONT_SMALL_BLACK);
    } else {
        lang_text_draw(71, 9, c->x_offset + 32, c->y_offset + 202, FONT_SMALL_BLACK);
    }
}

void window_building_draw_colosseum(building_info_context *c)
{
    c->help_id = 73;
    window_building_play_sound(c, "wavs/colloseum.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(74, 0, c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK);
    building *b = building_get(c->building_id);
    if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (b->num_workers <= 0) {
        window_building_draw_description(c, 74, 6);
    } else if (!b->data.entertainment.num_shows) {
        window_building_draw_description(c, 74, 2);
    } else if (b->data.entertainment.num_shows == 2) {
        window_building_draw_description(c, 74, 3);
    } else if (b->data.entertainment.days1) {
        window_building_draw_description(c, 74, 5);
    } else if (b->data.entertainment.days2) {
        window_building_draw_description(c, 74, 4);
    }

    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 6);
    window_building_draw_employment(c, 138);
    if (b->data.entertainment.days1 > 0) {
        int width = lang_text_draw(74, 8, c->x_offset + 32, c->y_offset + 182, FONT_SMALL_BLACK);
        lang_text_draw_amount(8, 44, 2 * b->data.entertainment.days1,
            c->x_offset + width + 32, c->y_offset + 182, FONT_SMALL_BLACK);
    } else {
        lang_text_draw(74, 7, c->x_offset + 32, c->y_offset + 182, FONT_SMALL_BLACK);
    }
    if (b->data.entertainment.days2 > 0) {
        int width = lang_text_draw(74, 10, c->x_offset + 32, c->y_offset + 202, FONT_SMALL_BLACK);
        lang_text_draw_amount(8, 44, 2 * b->data.entertainment.days2,
            c->x_offset + width + 32, c->y_offset + 202, FONT_SMALL_BLACK);
    } else {
        lang_text_draw(74, 9, c->x_offset + 32, c->y_offset + 202, FONT_SMALL_BLACK);
    }
}

void window_building_draw_hippodrome(building_info_context *c)
{
    c->help_id = 74;
    window_building_play_sound(c, "wavs/hippodrome.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(73, 0, c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK);
    building *b = building_get(c->building_id);
    if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (b->num_workers <= 0) {
        window_building_draw_description(c, 73, 4);
    } else if (!b->data.entertainment.num_shows) {
        window_building_draw_description(c, 73, 2);
    } else if (b->data.entertainment.days1) {
        window_building_draw_description(c, 73, 3);
    }

    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 6);
    window_building_draw_employment(c, 138);
    if (b->data.entertainment.days1 > 0) {
        int width = lang_text_draw(73, 6, c->x_offset + 32, c->y_offset + 202, FONT_SMALL_BLACK);
        lang_text_draw_amount(8, 44, 2 * b->data.entertainment.days1,
            c->x_offset + width + 32, c->y_offset + 202, FONT_SMALL_BLACK);
    } else {
        lang_text_draw(73, 5, c->x_offset + 32, c->y_offset + 202, FONT_SMALL_BLACK);
    }
}

static void draw_entertainment_school(building_info_context *c, const char *sound_file, int group_id)
{
    c->help_id = 75;
    window_building_play_sound(c, sound_file);

    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    lang_text_draw_centered(group_id, 0, c->x_offset, c->y_offset + 10, 16 * c->width_blocks, FONT_LARGE_BLACK);
    if (!c->has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else if (building_get(c->building_id)->num_workers <= 0) {
        window_building_draw_description(c, group_id, 7);
    } else if (c->worker_percentage >= 100) {
        window_building_draw_description(c, group_id, 2);
    } else if (c->worker_percentage >= 75) {
        window_building_draw_description(c, group_id, 3);
    } else if (c->worker_percentage >= 50) {
        window_building_draw_description(c, group_id, 4);
    } else if (c->worker_percentage >= 25) {
        window_building_draw_description(c, group_id, 5);
    } else {
        window_building_draw_description(c, group_id, 6);
    }
    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 4);
    window_building_draw_employment(c, 142);
}

void window_building_draw_actor_colony(building_info_context *c)
{
    draw_entertainment_school(c, "wavs/art_pit.wav", 77);
}

void window_building_draw_gladiator_school(building_info_context *c)
{
    draw_entertainment_school(c, "wavs/glad_pit.wav", 75);
}

void window_building_draw_lion_house(building_info_context *c)
{
    draw_entertainment_school(c, "wavs/lion_pit.wav", 76);
}

void window_building_draw_chariot_maker(building_info_context *c)
{
    draw_entertainment_school(c, "wavs/char_pit.wav", 78);
}

static void window_building_draw_monument_resources_needed(building_info_context* c)
{
    building* b = building_get(c->building_id);
    if (b->subtype.monument_phase != MONUMENT_FINISHED)
    {
        int width = text_draw(translation_for(TR_CONSTRUCTION_PHASE), c->x_offset + 22, c->y_offset + 50, FONT_NORMAL_BLACK, 0);
        width += text_draw_number(b->subtype.monument_phase, '@', "/5", c->x_offset + 22 + width, c->y_offset + 50, FONT_NORMAL_BLACK);
        text_draw(translation_for(TR_BUILDING_GRAND_TEMPLE_PHASE_1 + b->subtype.monument_phase - 1), c->x_offset + 32 + width, c->y_offset + 50, FONT_NORMAL_BLACK, 0);
        text_draw(translation_for(TR_REQUIRED_RESOURCES), c->x_offset + 22, c->y_offset + 70, FONT_NORMAL_BLACK, 0);
        for (int r = RESOURCE_TIMBER; r <= RESOURCE_MARBLE; r++) {
            int image_id = image_group(GROUP_RESOURCE_ICONS);
            image_draw(image_id + r, c->x_offset + 22, c->y_offset - 105 + r * 20);
            text_draw_number(b->data.monument.resources_needed[r], '@', " ", c->x_offset + 54, c->y_offset + 10 + r * 20 - 106, FONT_NORMAL_BLACK);
            text_draw_number(b->data.monument.resources_needed[r], '@', " ", c->x_offset + 54, c->y_offset + 10 + r * 20 - 106, FONT_NORMAL_BLACK);

        }
        int height = text_draw_multiline(translation_for(TR_BUILDING_GRAND_TEMPLE_PHASE_1_TEXT + b->subtype.monument_phase - 1), c->x_offset + 22, c->y_offset + 170, 16 * (c->width_blocks - 4), FONT_NORMAL_BLACK, 0);
        text_draw_multiline(translation_for(TR_BUILDING_GRAND_TEMPLE_CONSTRUCTION_DESC), c->x_offset + 22, c->y_offset + 180 + height, 16 * (c->width_blocks - 4), FONT_NORMAL_BLACK, 0);

    }
}

static void draw_grand_temple(building_info_context* c, const char* sound_file, int name, int bonus_desc,int banner_id, int quote, int temple_god_id)
{
    building* b = building_get(c->building_id);
    window_building_play_sound(c, sound_file);
    god_id = temple_god_id;
    if (b->subtype.monument_phase == MONUMENT_FINISHED) {
        outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    } 
    else {
        outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    }
    if (b->data.monument.upgrades) {
        int module_name = temple_module_options[god_id * 2 + (b->data.monument.upgrades - 1)].header;
        text_draw_centered(translation_for(module_name), c->x_offset, c->y_offset + 12, 16 * c->width_blocks, FONT_LARGE_BLACK, 0);
    }
    else {
        text_draw_centered(translation_for(name), c->x_offset, c->y_offset + 12, 16 * c->width_blocks, FONT_LARGE_BLACK, 0);
    }
    
    window_building_draw_monument_resources_needed(c);
    if (b->subtype.monument_phase == MONUMENT_FINISHED) {
        int height = text_draw_multiline(translation_for(bonus_desc), c->x_offset + 22, c->y_offset + 56, 15 * c->width_blocks, FONT_NORMAL_BLACK, 0);
        if (b->data.monument.upgrades) {
            int module_desc = temple_module_options[god_id * 2 + (b->data.monument.upgrades - 1)].desc;
            height += text_draw_multiline(translation_for(module_desc), c->x_offset + 22, c->y_offset + 66 + height, 15 * c->width_blocks, FONT_NORMAL_BLACK, 0);
        }
        inner_panel_draw(c->x_offset + 16, c->y_offset + 86 + height, c->width_blocks - 2, 4);
        window_building_draw_employment(c, 96 + height);
        image_draw(banner_id, c->x_offset + 32, c->y_offset + 166 + height);
        text_draw_centered_with_linebreaks(translation_for(quote), c->x_offset, c->y_offset + 386 + height, 16 * c->width_blocks, FONT_NORMAL_BLACK, 0);
    }
}

void window_building_draw_grand_temple_foreground(building_info_context* c)
{
    building* b = building_get(c->building_id);
    if (b->subtype.monument_phase == MONUMENT_FINISHED && !b->data.monument.upgrades) {
        button_border_draw(c->x_offset + 80, c->y_offset + 16 * c->height_blocks - 34,
            16 * (c->width_blocks - 10), 20, data.focus_button_id == 1 ? 1 : 0);
        text_draw_centered(translation_for(TR_BUILDING_GRAND_TEMPLE_ADD_MODULE), c->x_offset + 80, c->y_offset + 16 * c->height_blocks - 30,
            16 * (c->width_blocks - 10), FONT_NORMAL_BLACK, 0);
    }
    else if (b->subtype.monument_phase == MONUMENT_FINISHED) {
        text_draw_centered(translation_for(TR_BUILDING_TEMPLE_MODULE_CONSTRUCTED), c->x_offset + 80, c->y_offset + 16 * c->height_blocks - 30,
            16 * (c->width_blocks - 10), FONT_NORMAL_BLACK, 0);
    }
}

int window_building_handle_mouse_grand_temple(const mouse* m, building_info_context* c)
{
    building* b = building_get(c->building_id);
	data.building_id = c->building_id;
    if (b->subtype.monument_phase != MONUMENT_FINISHED || b->data.monument.upgrades) {
        return 0;
    }
	if (generic_buttons_handle_mouse(m, c->x_offset + 80, c->y_offset + 16 * c->height_blocks - 34,
		add_module_button, 1, &data.focus_button_id)) {
        return 1;
	}
	return 0;
}

void window_building_draw_grand_temple_ceres(building_info_context* c)
{
    draw_grand_temple(c, "wavs/temple_farm.wav", TR_BUILDING_GRAND_TEMPLE_CERES_DESC, TR_BUILDING_GRAND_TEMPLE_CERES_BONUS_DESC, mods_get_image_id(mods_get_group_id("Areldir", "Grand_Temple_Banners"), "Ceres L Banner"), TR_BUILDING_CERES_TEMPLE_QUOTE, GOD_CERES);
}

void window_building_draw_grand_temple_neptune(building_info_context* c)
{
    draw_grand_temple(c, "wavs/temple_ship.wav", TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC, TR_BUILDING_GRAND_TEMPLE_NEPTUNE_BONUS_DESC, mods_get_image_id(mods_get_group_id("Areldir", "Grand_Temple_Banners"), "Nept L Banner"), TR_BUILDING_NEPTUNE_TEMPLE_QUOTE, GOD_NEPTUNE);
}

void window_building_draw_grand_temple_mercury(building_info_context* c)
{
    draw_grand_temple(c, "wavs/temple_comm.wav", TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC, TR_BUILDING_GRAND_TEMPLE_MERCURY_BONUS_DESC, mods_get_image_id(mods_get_group_id("Areldir", "Grand_Temple_Banners"), "Merc L Banner"), TR_BUILDING_MERCURY_TEMPLE_QUOTE, GOD_MERCURY);
}

void window_building_draw_grand_temple_mars(building_info_context* c)
{
    draw_grand_temple(c, "wavs/temple_war.wav", TR_BUILDING_GRAND_TEMPLE_MARS_DESC, TR_BUILDING_GRAND_TEMPLE_MARS_BONUS_DESC, mods_get_image_id(mods_get_group_id("Areldir", "Grand_Temple_Banners"), "Mars L Banner"), TR_BUILDING_MARS_TEMPLE_QUOTE, GOD_MARS);
}

void window_building_draw_grand_temple_venus(building_info_context* c)
{
    draw_grand_temple(c, "wavs/temple_love.wav", TR_BUILDING_GRAND_TEMPLE_VENUS_DESC, TR_BUILDING_GRAND_TEMPLE_VENUS_BONUS_DESC, mods_get_image_id(mods_get_group_id("Areldir", "Grand_Temple_Banners"), "Venus L Banner"), TR_BUILDING_VENUS_TEMPLE_QUOTE, GOD_VENUS);
}

void window_building_draw_pantheon(building_info_context* c)
{
    draw_grand_temple(c, "wavs/oracle.wav", TR_BUILDING_PANTHEON_DESC, TR_BUILDING_PANTHEON_BONUS_DESC, mods_get_image_id(mods_get_group_id("Areldir", "Grand_Temple_Banners"), "Panth L Banner"), TR_BUILDING_PANTHEON_QUOTE, GOD_PANTHEON);
}


void window_building_draw_work_camp(building_info_context* c)
{
	window_building_play_sound(c, "wavs/eng_post.wav");
	outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
	inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 4);
    text_draw_centered(translation_for(TR_BUILDING_WORK_CAMP), c->x_offset, c->y_offset + 12, 16 * c->width_blocks, FONT_LARGE_BLACK, 0);
    16 * (c->width_blocks - 4),
    text_draw_multiline(translation_for(TR_BUILDING_WORK_CAMP_DESC), c->x_offset + 32, c->y_offset + 76, 16 * (c->width_blocks - 4), FONT_NORMAL_BLACK, 0);
    window_building_draw_employment(c, 138);
}

void window_building_draw_engineer_guild(building_info_context* c)
{
    window_building_play_sound(c, "wavs/eng_post.wav");
    outer_panel_draw(c->x_offset, c->y_offset, c->width_blocks, c->height_blocks);
    inner_panel_draw(c->x_offset + 16, c->y_offset + 136, c->width_blocks - 2, 4);
    text_draw_centered(translation_for(TR_BUILDING_ENGINEER_GUILD), c->x_offset, c->y_offset + 12, 16 * c->width_blocks, FONT_LARGE_BLACK, 0);
    16 * (c->width_blocks - 4),
        text_draw_multiline(translation_for(TR_BUILDING_ENGINEER_GUILD_DESC), c->x_offset + 32, c->y_offset + 76, 16 * (c->width_blocks - 4), FONT_NORMAL_BLACK, 0);
    window_building_draw_employment(c, 138);
}

static int add_module(int selection) {
    if (!selection) {
        return 0;
    }
    sound_speech_play_file("wavs/oracle.wav");
    building* b = building_get(data.building_id);
    building_monument_add_module(b, selection);
    return 1;
}

static void add_module_prompt(void)
{
    option_menu_item options[2] = { temple_module_options[god_id * 2], temple_module_options[god_id * 2 + 1] };
    window_show_option_popup(TR_SELECT_EPITHET_PROMPT_HEADER, TR_SELECT_EPITHET_PROMPT_TEXT, options, add_module);
}
