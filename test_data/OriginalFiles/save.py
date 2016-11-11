import bpy
import ctypes
import sys

load_file_name = "test_scene_" + str(bpy.app.version[0]) + str(bpy.app.version[1]-1)
load_file_name += "_x" + str(ctypes.sizeof(ctypes.c_voidp) * 8) + ".blend"
bpy.ops.wm.open_mainfile(filepath=load_file_name);

file_name = "test_scene_" + str(bpy.app.version[0]) + str(bpy.app.version[1]) + bpy.app.version_char
file_name += "_x" + str(ctypes.sizeof(ctypes.c_voidp) * 8) + ".blend"

bpy.ops.wm.save_as_mainfile(filepath=file_name)

sys.exit(1);