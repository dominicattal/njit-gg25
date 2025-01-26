#include "components.h"
#include "../../game/game.h"

void comp_death_init(Component* comp)
{
    comp_set_is_text(comp, TRUE);
}

void comp_death_update(Component* comp, f64 dt)
{
    if (game_player_dead())
        comp_set_visible(comp, TRUE);
    else
        comp_set_visible(comp, FALSE);
}