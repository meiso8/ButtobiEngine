import bpy

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
 

class TOPBAR_MT_my_menu(bpy.types.Menu):
    bl_idname = "TOPBAR_MT_my_menu"
    bl_label = "MyMenu"
    bl_description = "拡張メニュー by " + bl_info["author"]

    def draw(self,context):
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname,text = MYADDON_OT_stretch_vertex.bl_label)
        self.layout.separator()
        self.layout.operator("wm.url_open_preset",
            text ="Manual",icon ="HELP")


    def submenu(self,context):
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)


classes = (
    MYADDON_OT_stretch_vertex,
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