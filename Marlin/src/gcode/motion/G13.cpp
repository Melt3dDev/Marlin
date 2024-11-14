/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "../../inc/MarlinConfigPre.h"

#include "../../feature/z_stepper_align.h"

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../module/stepper/trinamic.h"
#include "../../lcd/marlinui.h" // for LCD_MESSAGE
#include "../../../src/core/macros.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if HAS_Z_STEPPER_ALIGN_STEPPER_XY
  #include "../../libs/least_squares_fit.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../../feature/bltouch.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

// #if NUM_Z_STEPPERS >= 3
//   #define TRIPLE_Z 1
//   #if NUM_Z_STEPPERS >= 4
//     #define QUAD_Z 1
//   #endif
// #endif

void GcodeSuite::G13() {
  planner.synchronize();

  // stepper.set_separate_multi_axis(true);

  // stepper.set_z1_lock(true);
  // stepper.set_z2_lock(true);

  // SERIAL_ECHOLNPGM("test move only axis 3");
  // do_blocking_move_to_z(10 + current_position.z);
  
  // planner.synchronize();

  // stepper.set_z1_lock(false);
  // stepper.set_z2_lock(false);

  // stepper.set_z3_lock(true);

  // SERIAL_ECHOLNPGM("test move only axis 1 and 2");  
  // do_blocking_move_to_z(-10 + current_position.z);

  // stepper.set_z3_lock(false);

  // stepper.set_separate_multi_axis(false); 

  // WRITE(Z_STEP_PIN,10);

  // stepper.set_samostatny_pohyb(true);
  // do_blocking_move_to_z(1 + current_position.z);
  // SERIAL_ECHOLNPGM("tri"); 
  // stepper.set_samostatny_pohyb(false);
  
  // planner.synchronize();

  // current_position[Z_AXIS] -= 10;
  // planner.buffer_line(current_position, feedrate_mm_s, active_extruder);
  // SERIAL_ECHOLNPGM("st");

  if (parser.seenval('R') && parser.seenval('Z') && parser.seenval('V') && parser.seenval('W')) {
    const float z_val = parser.floatval('Z'),
                v_val = parser.floatval('V'),
                w_val = parser.floatval('W');
    set_relative_mode(true);
    stepper.set_samostatny_pohyb(true);
    current_position[Z_AXIS] += z_val;
    current_position[I_AXIS] += v_val;
    current_position[J_AXIS] += w_val;
    planner.buffer_line(current_position, feedrate_mm_s, active_extruder);
    stepper.set_samostatny_pohyb(false);
    set_relative_mode(false);
  }
  else if (parser.seenval('S') && parser.seenval('Z') && parser.seenval('V') && parser.seenval('W')) {
    const float z_val = parser.floatval('Z'),
                w_val = parser.floatval('V'),
                v_val = parser.floatval('W');
    current_position[Z_AXIS] = z_val;
    current_position[I_AXIS] = v_val;
    current_position[J_AXIS] = w_val;
    stepper.set_samostatny_pohyb(true);
    planner.buffer_line(current_position, feedrate_mm_s, active_extruder);
    stepper.set_samostatny_pohyb(false);
  }
}
