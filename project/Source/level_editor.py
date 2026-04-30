import bpy
import math
import bpy_extras

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
    
#多重継承　オペレータかつエクスポートヘルパー
class MYADDON_OT_export_scene(bpy.types.Operator,bpy_extras.io_utils.ExportHelper):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をExportします"

    #出力するファイルの拡張子
    filename_ext = ".scene"

    def write_and_print(self,file,str):
        print(str)
        file.write(str)
        file.write('\n')


    def parse_scene_recursive(self,file,object,level):
        """シーン解析用再帰関数"""

        indent = ''
        for i in range(level):
            indent += "\t"

        self.write_and_print(file,indent + object.type)
        trans,rot, scale = object.matrix_local.decompose()
        rot = rot.to_euler()
        rot.x = math.degrees(rot.x)
        rot.y = math.degrees(rot.y)
        rot.z = math.degrees(rot.z)
        self.write_and_print(file,indent + "T %f,%f,%f" % (trans.x,trans.y,trans.z))
        self.write_and_print(file,indent + "R %f,%f,%f"% (rot.x,rot.y,rot.z))
        self.write_and_print(file,indent + "S %f,%f,%f"% (scale.x,scale.y,scale.z))
        #カスタムプロパティ'file_name'
        if "file_name" in object:
            self.write_and_print(file, indent + "N %s" % object["file_name"])
        self.write_and_print(file, indent + 'END')
        self.write_and_print(file,'')
        
        #子ノードに進む
        for child in object.children:
            self.parse_scene_recursive(file,child,level + 1)

    def export(self):
        """ファイルに出力"""
        print("シーン情報出力開始... %r" % self.filepath);

        with open(self.filepath, "wt") as file:
            self.write_and_print(file,"SCENE")

            for object in bpy.context.scene.objects:
                     
                if (object.parent):
                    continue

                self.parse_scene_recursive(file,object,0);

    
    def execute(self,context):
        print("シーン情報をExportします")      
        self.export()
        self.report({'INFO'},"シーン情報をExportしました")
        print("シーン情報をExportしました")
        return {'FINISHED'}
    

class MYADDON_OT_add_filename(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_filename"
    bl_label = "FileName 追加"
    bl_description = "['file_name']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}

    def execute(self,context):  
        context.object["file_name"] = ""
        return {"FINISHED"}

class OBJECT_PT_file_name(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_idname = "OBJECT_PT_file_name"
    bl_label = "FileName"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    # サブメニューの描画
    def draw(self,context):
        #パネルに項目を追加
        if "file_name" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["file_name"]',text=self.bl_label)
        else:
            self.layout.operator(MYADDON_OT_add_filename.bl_idname)





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
    MYADDON_OT_add_filename,
    OBJECT_PT_file_name,

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