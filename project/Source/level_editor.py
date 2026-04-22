import bpy
import math

bl_info = {
    "name": "レベルエディタ",
    "author" :"Taro Kamata",
    "version": (1,0),
    "blender": (3,3,1),
    "location" :"",
    "descriptioin": "レベルエディタ",
    "warning": "",
    "wiki_url" : "",
    "tracker_url": "",
    "category": "Object"
}
    
#メニュー項目描画
def draw_menu_manual(self,context):
    self.layout.operator("wm.url_open_preset",text ="Manual",icon ="HELP")

class MYADDON_OT_stretch_vertex(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_stretch_vertex"
    bl_label = "頂点を伸ばす"
    bl_description = "頂点座標を引っ張って伸ばします"
    bl_options = {'REGISTER','UNDO'}

    def execute(self,context):
        bpy.data.objects["Cube"].data.vertices[0].co.x += 1.0
        print("頂点を伸ばしました。")
        return {'FINISHED'}
 

class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_object"
    bl_label = "ICO級生成"
    bl_description = "ICO球を生成します"
    bl_options = {'REGISTER','UNDO'}
    
    def execute(self,context):
        bpy.ops.mesh.primitive_ico_sphere_add()
        print("ICO球を生成しました。")
        return {'FINISHED'}
    
class MYADDON_OT_export_scene(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をExportします"

    def execute(self,context):
        print("シーン情報をExportします")
        
        for object in bpy.context.scene.objects:
            print(object.type + " - " + object.name)
            trans,rot, scale = object.matrix_local.decompose()
            rot = rot.to_euler()
            rot.x = math.degrees(rot.x)
            rot.y = math.degrees(rot.y)
            rot.z = math.degrees(rot.z)
            print("Trans(%f,%f,%f)" % (trans.x,trans.y,trans.z))
            print("Rot(%f,%f,%f)"% (rot.x,rot.y,rot.z))
            print("Scale(%f,%f,%f)"% (scale.x,scale.y,scale.z))

            if object.parent:
                print("Parent:",object.parent.name)
            print()

        print("シーン情報をExportしました")
        self.report({'INFO'},"シーン情報をExportしました")

        return {'FINISHED'}



class TOPBAR_MT_my_menu(bpy.types.Menu):
    bl_idname = "TOPBAR_MT_my_menu"
    bl_label = "MyMenu"
    bl_description = "拡張メニュー by " + bl_info["author"]

    def draw(self,context):
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname,text = MYADDON_OT_stretch_vertex.bl_label)
        self.layout.separator()
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname,text = MYADDON_OT_create_ico_sphere.bl_label)
        self.layout.separator()
        self.layout.operator(MYADDON_OT_export_scene.bl_idname,text = MYADDON_OT_export_scene.bl_label)
        self.layout.separator()
        self.layout.operator("wm.url_open_preset",text ="Manual",icon ="HELP")


    def submenu(self,context):
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)

classes = (
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_export_scene,
    TOPBAR_MT_my_menu,
           )


    
def register():
    for cls in classes:
        bpy.utils.register_class(cls)

    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    print("レベルエディタが有効化されました。")

def unregister():
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    for cls in classes:
        bpy.utils.unregister_class(cls)
    print("レベルエディタが無効化されました。")

if __name__ == "__main__":
    register()