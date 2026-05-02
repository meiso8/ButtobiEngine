#モジュールのインポート
import bpy

class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_object"
    bl_label = "ICO級生成"
    bl_description = "ICO球を生成します"
    bl_options = {'REGISTER','UNDO'}
    
    def execute(self,context):
        bpy.ops.mesh.primitive_ico_sphere_add()
        print("ICO球を生成しました。")
        return {'FINISHED'}
    