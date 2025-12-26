# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "arm_pkg: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iarm_pkg:/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg;-Istd_msgs:/usr/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(arm_pkg_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg/handle_info.msg" NAME_WE)
add_custom_target(_arm_pkg_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "arm_pkg" "/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg/handle_info.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(arm_pkg
  "/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg/handle_info.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/arm_pkg
)

### Generating Services

### Generating Module File
_generate_module_cpp(arm_pkg
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/arm_pkg
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(arm_pkg_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(arm_pkg_generate_messages arm_pkg_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg/handle_info.msg" NAME_WE)
add_dependencies(arm_pkg_generate_messages_cpp _arm_pkg_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(arm_pkg_gencpp)
add_dependencies(arm_pkg_gencpp arm_pkg_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS arm_pkg_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(arm_pkg
  "/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg/handle_info.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/arm_pkg
)

### Generating Services

### Generating Module File
_generate_module_lisp(arm_pkg
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/arm_pkg
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(arm_pkg_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(arm_pkg_generate_messages arm_pkg_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg/handle_info.msg" NAME_WE)
add_dependencies(arm_pkg_generate_messages_lisp _arm_pkg_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(arm_pkg_genlisp)
add_dependencies(arm_pkg_genlisp arm_pkg_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS arm_pkg_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(arm_pkg
  "/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg/handle_info.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/arm_pkg
)

### Generating Services

### Generating Module File
_generate_module_py(arm_pkg
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/arm_pkg
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(arm_pkg_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(arm_pkg_generate_messages arm_pkg_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/arm_ctrl_ws_trac_ik/src/arm_pkg/msg/handle_info.msg" NAME_WE)
add_dependencies(arm_pkg_generate_messages_py _arm_pkg_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(arm_pkg_genpy)
add_dependencies(arm_pkg_genpy arm_pkg_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS arm_pkg_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/arm_pkg)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/arm_pkg
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(arm_pkg_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/arm_pkg)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/arm_pkg
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(arm_pkg_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/arm_pkg)
  install(CODE "execute_process(COMMAND \"/usr/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/arm_pkg\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/arm_pkg
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(arm_pkg_generate_messages_py std_msgs_generate_messages_py)
endif()
