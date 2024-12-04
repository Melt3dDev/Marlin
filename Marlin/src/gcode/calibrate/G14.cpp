#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/endstops.h"
#include "../../module/planner.h"
#include "../../module/stepper.h" // for various
#include "../../feature/tmc_util.h"

void GcodeSuite::G14() {
  planner.synchronize();
  set_axis_is_at_home(I_AXIS);
  set_axis_is_at_home(J_AXIS);
  sync_plan_position();
  destination[I_AXIS] = current_position[I_AXIS];
  destination[J_AXIS] = current_position[J_AXIS];
}