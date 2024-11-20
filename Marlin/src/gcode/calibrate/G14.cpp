#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/endstops.h"
#include "../../module/planner.h"
#include "../../module/stepper.h" // for various
#include "../../feature/tmc_util.h"

void GcodeSuite::G14() {
  // Disallow Z homing if X or Y homing is needed
  if (homing_needed_error(_BV(X_AXIS) | _BV(Y_AXIS))) return;

  sync_plan_position();
   /**
   * Move the Z probe (or just the nozzle) to the safe homing point
   * (Z is already at the right height)
   */
  // constexpr xy_float_t safe_homing_xy = { Z_SAFE_HOMING_X_POINT, Z_SAFE_HOMING_Y_POINT };
  // destination.set(safe_homing_xy, current_position.z);

  TERN_(HOMING_Z_WITH_PROBE, destination -= probe.offset_xy);
  if (position_is_reachable(destination)) {
    // Free the active extruder for movement
    //endstops.enable(true);
    TERN_(DUAL_X_CARRIAGE, idex_set_parked(false));
    TERN_(SENSORLESS_HOMING, safe_delay(500)); // Short delay needed to settle3AXIS_SIMULTANEOUS_MOVE
    do_blocking_move_to_xy(destination);
    homeaxis(Z_AXIS);
    planner.synchronize();
    set_axis_is_at_home(I_AXIS);
    set_axis_is_at_home(J_AXIS);
    sync_plan_position();
    destination[I_AXIS] = current_position[I_AXIS];
    destination[J_AXIS] = current_position[J_AXIS];
  }
}